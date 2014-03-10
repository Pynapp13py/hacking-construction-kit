//This software is based on Touchless, which is released under the Microsoft Public License (Ms-PL) 
#ifdef CINTERFACE
#undef CINTERFACE
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dshow.h>
#pragma comment(lib, "strmiids")

#include "macro.h"

#include "print.h"

#ifdef LOG
#define BREAK_WITH_ERROR(s, ...) do { \
	Print::myprintf(s, __VA_ARGS__); \
	return 1; \
} while(0);
#else
#define BREAK_WITH_ERROR(s, ...) do { \
	return 1; \
} while(0);
#endif

#define PRINTF(s, ...) printf(s, __VA_ARGS__);

#include "cwebcam.h"
extern "C" {
#include "bmp2jpeg.h"
}
#pragma comment(lib, "jpeg.lib")

#include "memory_debug.h"

//Required interface stuff - bad hack for qedit.h not being present/compatible with later windows versions
interface ISampleGrabberCB : public IUnknown {
	virtual STDMETHODIMP SampleCB( double SampleTime, IMediaSample *pSample ) = 0;
	virtual STDMETHODIMP BufferCB( double SampleTime, BYTE *pBuffer, long BufferLen ) = 0;
};
static const IID IID_ISampleGrabberCB = { 0x0579154A, 0x2B53, 0x4994, { 0xB0, 0xD0, 0xE7, 0x73, 0x14, 0x8E, 0xFF, 0x85 } };
interface ISampleGrabber : public IUnknown {
	virtual HRESULT STDMETHODCALLTYPE SetOneShot( BOOL OneShot ) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetMediaType( const AM_MEDIA_TYPE *pType ) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType( AM_MEDIA_TYPE *pType ) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples( BOOL BufferThem ) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer( long *pBufferSize, long *pBuffer ) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample( IMediaSample **ppSample ) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetCallback( ISampleGrabberCB *pCallback, long WhichMethodToCallback ) = 0;
};
static const IID IID_ISampleGrabber = { 0x6B652FFF, 0x11FE, 0x4fce, { 0x92, 0xAD, 0x02, 0x66, 0xB5, 0xD7, 0xC7, 0x8F } };
static const CLSID CLSID_SampleGrabber = { 0xC1F400A0, 0x3F08, 0x11d3, { 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37 } };
static const CLSID CLSID_NullRenderer = { 0xC1F400A4, 0x3F08, 0x11d3, { 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37 } };

//Handle used for synchronization. Main thread waits for capture event to be signalled to clean up
HANDLE writeEvent = 0;

//Store width/height of captured frame
int nWidth;
int nHeight;
bool running = false;

//Capture variables
#define MAX_CAMERAS		10
IGraphBuilder* g_pGraphBuilder = NULL;
IMediaControl* g_pMediaControl = NULL;
ICaptureGraphBuilder2* g_pCaptureGraphBuilder = NULL;
IBaseFilter* g_pIBaseFilterCam = NULL;
IBaseFilter* g_pIBaseFilterSampleGrabber = NULL;
IBaseFilter* g_pIBaseFilterNullRenderer = NULL;

PBYTE imgdata = NULL;
long imgsize = 0;
UINT bmpsize = 0;
PBYTE imgdata_save = NULL;
PBYTE bmpdata = NULL;
//DWORD jpgsize = 0;
//PBYTE jpgarray = NULL; //shouldn't be bigger, right?

// SampleGrabber callback interface
class MySampleGrabberCB : public ISampleGrabberCB{
public:
	MySampleGrabberCB(){
		m_nRefCount = 0;
	}
	virtual HRESULT STDMETHODCALLTYPE SampleCB( 
		double SampleTime,
		IMediaSample *pSample){
			MYPRINTF("SampleCB\n");
			return E_FAIL;
	}
	virtual HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen) {
		//MYPRINTF("BufferCB: %d\n", BufferLen);
		//MYPRINTF("BufferCB: %u\n", BufferLen);
		if (imgdata == NULL || imgsize < BufferLen){
			imgsize = BufferLen;
			if(imgdata != NULL) {
				free(imgdata);
			}
			imgdata = (PBYTE)malloc(imgsize);
			if (imgdata == 0){
				MYPRINTF("Unable malloc\n");
			}
		}
		memcpy(imgdata,pBuffer,imgsize);
		if (!SetEvent(writeEvent)){ //Notify of new frame
			MYPRINTF("Unable to set event\n");
		}
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		REFIID riid,
		void **ppvObject) {
			//MYPRINTF("QueryInterface\n");
			return E_FAIL;  // Not a very accurate implementation
	}
	virtual ULONG STDMETHODCALLTYPE AddRef(){
		//MYPRINTF("AddRef\n");
		return ++m_nRefCount;
	}
	virtual ULONG STDMETHODCALLTYPE Release(){
		//MYPRINTF("Release\n");
		int n = --m_nRefCount;
		if (n <= 0)
			delete this;
		return n;
	}
private:
	int m_nRefCount;
};


// lists webcams
DWORD request_webcam_list(){

	DWORD dwResult = ERROR_SUCCESS;

	do{
		IEnumMoniker* pclassEnum = NULL;
		ICreateDevEnum* pdevEnum = NULL;

		CoInitialize(NULL);
		HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, 
			NULL, 
			CLSCTX_INPROC, 
			IID_ICreateDevEnum, 
			(LPVOID*)&pdevEnum);

		if (SUCCEEDED(hr)){
			hr = pdevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pclassEnum, 0);
		}

		if (pdevEnum != NULL){
			pdevEnum->Release();
			pdevEnum = NULL;
		}
		int nCount = 0;
		IUnknown* pUnk = NULL;
		if (pclassEnum == NULL){
			break;// Error!
		}

		IMoniker* apIMoniker[1];
		ULONG ulCount = 0;
		while (SUCCEEDED(hr) && nCount < MAX_CAMERAS && pclassEnum->Next(1, apIMoniker, &ulCount) == S_OK){
			IPropertyBag *pPropBag;
			hr = apIMoniker[0]->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
			if (SUCCEEDED(hr)) {
				// To retrieve the filter's friendly name, do the following:
				VARIANT varName;
				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				//get chars from wchars
				size_t converted;
				char charbuf[512];
				wcstombs_s(&converted, charbuf, sizeof(charbuf), varName.bstrVal, sizeof(charbuf));
				if (SUCCEEDED(hr) && varName.vt == VT_BSTR){
					PRINTF("%s\n", charbuf);

				}
				VariantClear(&varName);
				pPropBag->Release();
			}
			nCount++;
		}
		pclassEnum->Release();
		if(pUnk == NULL){
			break;// No webcam!
		}
	} while (0);

	dwResult = GetLastError();

	return dwResult;
}

DWORD request_webcam_count(int &count){

	DWORD dwResult = ERROR_SUCCESS;
	count = 0;
	do{
		IEnumMoniker* pclassEnum = NULL;
		ICreateDevEnum* pdevEnum = NULL;

		CoInitialize(NULL);
		HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, 
			NULL, 
			CLSCTX_INPROC, 
			IID_ICreateDevEnum, 
			(LPVOID*)&pdevEnum);

		if (SUCCEEDED(hr)) {
			hr = pdevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pclassEnum, 0);
		}

		if (pdevEnum != NULL){
			pdevEnum->Release();
			pdevEnum = NULL;
		}
		int nCount = 0;
		IUnknown* pUnk = NULL;
		if (pclassEnum == NULL){
			break;// Error!
		}

		IMoniker* apIMoniker[1];
		ULONG ulCount = 0;
		while (SUCCEEDED(hr) && nCount < MAX_CAMERAS && pclassEnum->Next(1, apIMoniker, &ulCount) == S_OK){
			IPropertyBag *pPropBag;
			hr = apIMoniker[0]->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
			if (SUCCEEDED(hr)) {
				count++;
				pPropBag->Release();
			}
			nCount++;
		}
		pclassEnum->Release();
		if(pUnk == NULL){
			break;// No webcam!
		}
	} while (0);

	dwResult = GetLastError();

	return dwResult;
}

HRESULT SetCameraResolution(ICaptureGraphBuilder2* g_pCaptureGraphBuilder, 
	IBaseFilter* g_pIBaseFilterCam, LONG desiredWidth, LONG desiredHeight) {
		HRESULT hr = S_OK;
		IAMStreamConfig *config;
		DWORD dwId = 0;

		hr = g_pCaptureGraphBuilder->FindInterface(
			&PIN_CATEGORY_CAPTURE,
			&MEDIATYPE_Video,
			g_pIBaseFilterCam,
			IID_IAMStreamConfig,
			(void**)&config);

		int iCount = 0;
		int iSize = 0;
		// get the number of different resolutions possible
		hr = config->GetNumberOfCapabilities(&iCount, &iSize);

		if (SUCCEEDED(hr) &&
			iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS)) {

				for (int i = 0 ; i < iCount; i++) {
					VIDEO_STREAM_CONFIG_CAPS scc;
					AM_MEDIA_TYPE *pmtConfig;
					// make sure we can set the capture format to the resolution we want
					hr = config->GetStreamCaps(i, &pmtConfig, (BYTE*)&scc);


					if (SUCCEEDED(hr)) {

						VIDEOINFOHEADER* videoInfoHeader = NULL;

						ULONG formatSize = pmtConfig->cbFormat;

						if (pmtConfig->formattype == FORMAT_VideoInfo) {
							if(formatSize >= sizeof(VIDEOINFOHEADER) &&	(pmtConfig->pbFormat != NULL)) {
								videoInfoHeader = (VIDEOINFOHEADER*)pmtConfig->pbFormat;
								LONG width = videoInfoHeader->bmiHeader.biWidth;  // Supported width
								LONG height = videoInfoHeader->bmiHeader.biHeight; // Supported height
								if (width >= desiredWidth && height >= desiredHeight){
									// That resolution is available, now we set the capture format to the resolution we want.
									hr = config->SetFormat(pmtConfig);
									config->Release();
									return hr;
								}
							}
						}
					}		
					// no width found, set the max
					hr = config->GetStreamCaps(iCount - 1, &pmtConfig, (BYTE*)&scc);
					if (SUCCEEDED(hr)) {
						hr = config->SetFormat(pmtConfig);
					}
				}

		}
		else {
			hr = E_FAIL;
		}
		config->Release();

		return hr;
}

// Starts webcam
// return 0 for success, > 0 for error
DWORD request_webcam_start(LONG width, LONG height, UINT index){

	DWORD dwResult = ERROR_SUCCESS;

	if(running) {
		BREAK_WITH_ERROR("Already running!\n", ERROR_SERVICE_ALREADY_RUNNING)
	}

	IEnumMoniker* pclassEnum = NULL;
	ICreateDevEnum* pdevEnum = NULL;
	if(index < 1){
		BREAK_WITH_ERROR("No webcams found\n", ERROR_FILE_NOT_FOUND)
	}

	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, 
		NULL, 
		CLSCTX_INPROC, 
		IID_ICreateDevEnum, 
		(LPVOID*)&pdevEnum);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("No webcams found\n", hr)
	}
	//MYPRINTF("CoCreateInstance ok\n");

	hr = pdevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pclassEnum, 0);

	if (pdevEnum != NULL){
		pdevEnum->Release();
		pdevEnum = NULL;
	}
	UINT nCount = 0;
	IUnknown* pUnk = NULL;
	if (pclassEnum == NULL){
		BREAK_WITH_ERROR("No webcams found\n", ERROR_FILE_NOT_FOUND) // Error!
	}
	//MYPRINTF("CreateClassEnumerator ok\n");

	IMoniker* apIMoniker[1];
	ULONG ulCount = 0;
	while (SUCCEEDED(hr) && nCount < index && pclassEnum->Next(1, apIMoniker, &ulCount) == S_OK){
		pUnk = apIMoniker[0];
		nCount++;
	}
	pclassEnum->Release();
	if(pUnk == NULL){
		BREAK_WITH_ERROR("No webcams found\n", ERROR_FILE_NOT_FOUND)
	}
	//MYPRINTF("pclassEnum->Release ok\n");

	IMoniker *pMoniker = NULL;

	// Grab the moniker interface
	hr = pUnk->QueryInterface(IID_IMoniker, (LPVOID*)&pMoniker);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Query interface failed\n", hr)
	}
	//MYPRINTF("pUnk->QueryInterface ok\n");

	// Build all the necessary interfaces to start the capture
	hr = CoCreateInstance(CLSID_FilterGraph, 
		NULL, 
		CLSCTX_INPROC, 
		IID_IGraphBuilder, 
		(LPVOID*)&g_pGraphBuilder);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Filter graph creation failed\n", hr)
	}
	//MYPRINTF("CoCreateInstance ok\n");

	hr = g_pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&g_pMediaControl);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Query interface failed\n", hr)
	}
	//MYPRINTF("g_pGraphBuilder->QueryInterface ok\n");

	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, 
		NULL, 
		CLSCTX_INPROC, 
		IID_ICaptureGraphBuilder2, 
		(LPVOID*)&g_pCaptureGraphBuilder);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Capture Graph Builder failed\n", hr)
	}
	//MYPRINTF("CoCreateInstance ok\n");

	// Setup the filter graph
	hr = g_pCaptureGraphBuilder->SetFiltergraph(g_pGraphBuilder);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Set filter graph failed\n", hr)
	}
	//MYPRINTF("g_pCaptureGraphBuilder->SetFiltergraph ok\n");

	// Build the camera from the moniker
	hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (LPVOID*)&g_pIBaseFilterCam);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Bind to object failed\n", hr)
	}
	//MYPRINTF("pMoniker->BindToObject ok\n");

	// Add the camera to the filter graph
	hr = g_pGraphBuilder->AddFilter(g_pIBaseFilterCam, L"WebCam");
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Add filter failed\n", hr)
	}
	//MYPRINTF("g_pGraphBuilder->AddFilter ok\n");

	// Create a SampleGrabber
	hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&g_pIBaseFilterSampleGrabber);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Create sample grabber failed\n", hr)
	}
	//MYPRINTF("CoCreateInstance ok\n");

	// Configure the Sample Grabber
	ISampleGrabber *pGrabber = NULL;
	hr = g_pIBaseFilterSampleGrabber->QueryInterface(IID_ISampleGrabber, (void**)&pGrabber);
	if (SUCCEEDED(hr)){
		//MYPRINTF("g_pIBaseFilterSampleGrabber->QueryInterface ok\n");
		AM_MEDIA_TYPE mt;
		ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
		mt.majortype = MEDIATYPE_Video;
		mt.subtype = MEDIASUBTYPE_RGB24;
		mt.formattype = FORMAT_VideoInfo;
		hr = pGrabber->SetMediaType(&mt);
		//MYPRINTF("SetMediaType ok\n");
	}
	

	if (SUCCEEDED(hr)){
		MySampleGrabberCB* msg = new MySampleGrabberCB();
		hr = pGrabber->SetCallback(msg, 1);
		//MYPRINTF("pGrabber->SetCallback ok\n");
	}
	

	if (pGrabber != NULL){
		pGrabber->Release();
		pGrabber = NULL;
	}
	//MYPRINTF("pGrabber->Release ok\n");

	if (FAILED(hr)){
		BREAK_WITH_ERROR("Sample grabber instantiation failed\n", hr)
	}

	SetCameraResolution(g_pCaptureGraphBuilder, g_pIBaseFilterCam, width, height);
	//MYPRINTF("SetCameraResolution ok\n");

	// Add Sample Grabber to the filter graph
	hr = g_pGraphBuilder->AddFilter(g_pIBaseFilterSampleGrabber, L"SampleGrabber");
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Add Sample Grabber to the filter graph failed\n", hr)
	}
	//MYPRINTF("g_pGraphBuilder->AddFilter ok\n");

	// Create the NullRender
	hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&g_pIBaseFilterNullRenderer);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Create the NullRender failed\n", hr)
	}
	//MYPRINTF("CoCreateInstance ok\n");

	// Add the Null Render to the filter graph
	hr = g_pGraphBuilder->AddFilter(g_pIBaseFilterNullRenderer, L"NullRenderer");
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Add the Null Render to the filter graph failed\n", hr)
	}
	//MYPRINTF("g_pGraphBuilder->AddFilter ok\n");

	// Configure the render stream
	hr = g_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, g_pIBaseFilterCam,
		g_pIBaseFilterSampleGrabber, g_pIBaseFilterNullRenderer);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Configure the render stream failed\n", hr)
	}
	//MYPRINTF("g_pCaptureGraphBuilder->RenderStream ok\n");

	// Grab the capture width and height
	hr = g_pIBaseFilterSampleGrabber->QueryInterface(IID_ISampleGrabber, (LPVOID*)&pGrabber);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Querying interface failed\n", hr)
	}
	//MYPRINTF("g_pIBaseFilterSampleGrabber->QueryInterface ok\n");

	AM_MEDIA_TYPE mt;
	hr = pGrabber->GetConnectedMediaType(&mt);
	if (FAILED(hr)){
		BREAK_WITH_ERROR("GetConnectedMediaType failed\n", hr)
	}
	//MYPRINTF("pGrabber->GetConnectedMediaType ok\n");

	VIDEOINFOHEADER *pVih;
	if ((mt.formattype == FORMAT_VideoInfo) && 
		(mt.cbFormat >= sizeof(VIDEOINFOHEADER)) &&
		(mt.pbFormat != NULL) ) {
			pVih = (VIDEOINFOHEADER*)mt.pbFormat;
			nWidth = pVih->bmiHeader.biWidth;
			nHeight = pVih->bmiHeader.biHeight;
	} else {
		BREAK_WITH_ERROR("Wrong format type\n", hr) // Wrong format
	}
	if (pGrabber != NULL){
		pGrabber->Release();
		pGrabber = NULL;
	}
	//MYPRINTF("pGrabber->Release ok\n");

	//Sync: set up semaphore
	writeEvent = CreateEvent( 
		NULL,               // default security attributes
		FALSE,               // auto-reset event
		FALSE,              // initial state is nonsignaled
		NULL);  // no object name
	if (writeEvent == 0){
		BREAK_WITH_ERROR("CreateEvent failed: %d\n", GetLastError())
	}
	//MYPRINTF("CreateEvent ok\n");

	// Start the capture
	if (FAILED(hr)){
		BREAK_WITH_ERROR("CreateEvent failed\n", hr)
	}

	hr = g_pMediaControl->Run();
	if (FAILED(hr)){
		BREAK_WITH_ERROR("Running capture failed\n", hr)
	}
	//MYPRINTF("g_pMediaControl->Run ok\n");

	// Cleanup
	if (pMoniker != NULL){
		pMoniker->Release();
		pMoniker = NULL;
	}
	//MYPRINTF("pMoniker->Release ok\n");

	//Now we wait for first frame
	if(WaitForSingleObject (writeEvent, 30000) == WAIT_TIMEOUT){
		BREAK_WITH_ERROR("timeout!\n", WAIT_TIMEOUT);
	}

	running = true;

	return dwResult;
}

// Gets image from running webcam
DWORD request_webcam_get_frame(const TCHAR *name, int quality){
	DWORD dwResult = ERROR_SUCCESS;


	if(!running) {
		BREAK_WITH_ERROR("Not running!\n", ERROR_INVALID_ACCESS)
	}
	//Make bmp
	BITMAPFILEHEADER	bfh;
	bfh.bfType = 0x4d42;	// always "BM"
	bfh.bfSize = sizeof( BITMAPFILEHEADER );
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = (DWORD) (sizeof( bfh ) + sizeof(BITMAPINFOHEADER));

	BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = nWidth;
	bih.biHeight = nHeight;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = imgsize;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	UINT mybmpsize = imgsize + sizeof(bfh) + sizeof(bih);
	if(bmpsize < mybmpsize){
		bmpsize = mybmpsize;
		if(bmpdata != NULL)
			delete [] bmpdata;
		bmpdata = new BYTE[bmpsize];
	}

	// put headers together to make a .bmp in memory
	memcpy(bmpdata, &bfh, sizeof(bfh));
	memcpy(bmpdata + sizeof(bfh), &bih, sizeof(bih));
	memcpy(bmpdata + sizeof(bfh) + sizeof(bih), imgdata, imgsize);

	// Now convert to JPEG
	if (bmp2jpegtofile(bmpdata, quality, name ) == 0){
		dwResult = 1;
	}

	return dwResult;
}

// Gets image from running webcam
DWORD request_webcam_get_frame_to_buffer(char **buffer, DWORD &size, int quality){
	DWORD dwResult = ERROR_SUCCESS;

	if(!running) {
		BREAK_WITH_ERROR("Not running!\n", ERROR_INVALID_ACCESS)
	}
	//Make bmp
	//PBYTE tmpImg = new BYTE[imgsize];
	//memcpy(tmpImg, imgdata, imgsize);
	//createBmp(tmpImg, imgsize);
	createBmp(imgdata, imgsize);

	// Now convert to JPEG
	if (bmp2jpegtomemory(bmpdata, quality, (BYTE **)buffer, &size) == 0){
		dwResult = 1;
	}
	//delete [] tmpImg;
	return dwResult;
}

DWORD createBmp(PBYTE img, DWORD size){
	//Make bmp
	BITMAPFILEHEADER	bfh;
	bfh.bfType = 0x4d42;	// always "BM"
	bfh.bfSize = sizeof( BITMAPFILEHEADER );
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = (DWORD) (sizeof( bfh ) + sizeof(BITMAPINFOHEADER));

	BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = nWidth;
	bih.biHeight = nHeight;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = size;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	UINT mybmpsize = size + sizeof(bfh) + sizeof(bih);

	if(bmpsize < mybmpsize){
		bmpsize = mybmpsize;
		if(bmpdata != NULL)
			delete [] bmpdata;
		bmpdata = new BYTE[bmpsize];
	}
	// put headers together to make a .bmp in memory
	memcpy(bmpdata, &bfh, sizeof(bfh));
	memcpy(bmpdata + sizeof(bfh), &bih, sizeof(bih));
	memcpy(bmpdata + sizeof(bfh) + sizeof(bih), img, size);
	return 0;
}

DWORD request_webcam_motion_detection(const TCHAR *name, int quality, int threshold, int diffInPercent, int pixels, char *savediff){
	DWORD dwResult = ERROR_SUCCESS;

	if(!running) {
		BREAK_WITH_ERROR("Not running!\n", ERROR_INVALID_ACCESS)
	}

	// create a temporary image to avoid modification by directx thread
	PBYTE tmpImg = new BYTE[imgsize];
	memcpy(tmpImg, imgdata, imgsize);

	int diff = 0;
	// We have a previous img, try do calculate difference
	if (imgdata_save != 0){
		diff = frame_difference(imgdata_save, tmpImg, imgsize, threshold, pixels);

		if (savediff != 0){
			// save difference img
			PBYTE frame_diff = (PBYTE)malloc(imgsize);
			show_frame_difference(imgdata_save, tmpImg, frame_diff, imgsize, threshold, pixels, true);

			createBmp(frame_diff, imgsize);

			bmp2jpegtofile(bmpdata, quality, savediff);

			free(frame_diff);
		}
	}
	if (diff > diffInPercent){
		MYPRINTF("move detected: %d\n", diff);
	} else if (imgdata_save != 0){
		MYPRINTF("difference between images: %d\n", diff);
	}
	// save only first img and if we up the threshold
	if (diff > diffInPercent || imgdata_save == 0){
		createBmp(tmpImg, imgsize);

		if (bmp2jpegtofile(bmpdata, quality, name) == 0){
			dwResult = 1;
		}

	} else {
		dwResult = -1;
	}
	// copy current img to save
	if (imgdata_save != 0){
		delete[] imgdata_save;
	}
	imgdata_save = new BYTE[imgsize];
	imgdata_save = (PBYTE)memcpy(imgdata_save, tmpImg, imgsize);

	delete[] tmpImg;

	return dwResult;
}

DWORD request_webcam_motion_detection_to_buffer(char **buffer, DWORD &size, int quality, int threshold, int diffInPercent, int pixels, char *savediff){
		DWORD dwResult = ERROR_SUCCESS;

	if(!running) {
		BREAK_WITH_ERROR("Not running!\n", ERROR_INVALID_ACCESS)
	}

	// create a temporary image to avoid modification by directx thread
	PBYTE tmpImg = new BYTE[imgsize];
	memcpy(tmpImg, imgdata, imgsize);

	int diff = 0;
	// We have a previous img, try do calculate difference
	if (imgdata_save != 0){
		diff = frame_difference(imgdata_save, tmpImg, imgsize, threshold, pixels);

		if (savediff != 0){
			// save difference img
			PBYTE frame_diff = (PBYTE)malloc(imgsize);
			show_frame_difference(imgdata_save, tmpImg, frame_diff, imgsize, threshold, pixels, true);

			createBmp(frame_diff, imgsize);

			bmp2jpegtofile(bmpdata, quality, savediff);

			free(frame_diff);
		}
	}
	if (diff > diffInPercent){
		MYPRINTF("move detected: %d\n", diff);
	} else if (imgdata_save != 0){
		MYPRINTF("difference between images: %d\n", diff);
	}
	// save only first img and if we up the threshold
	if (diff > diffInPercent || imgdata_save == 0){
		createBmp(tmpImg, imgsize);

		if (bmp2jpegtomemory(bmpdata, quality, (BYTE **)buffer, &size) == 0){
			dwResult = 1;
		}

	} else {
		dwResult = -1;
	}
	// copy current img to save
	if (imgdata_save != 0){
		delete[] imgdata_save;
	}
	imgdata_save = new BYTE[imgsize];
	imgdata_save = (PBYTE)memcpy(imgdata_save, tmpImg, imgsize);

	delete[] tmpImg;

	return dwResult;
}

int show_frame_difference(PBYTE previous, PBYTE current, PBYTE difference, long imgsize, int threshold, int nbPixel, bool deleteMove){
	if (previous == NULL) return 0;
	if (current == NULL) return 0;

	int next = nbPixel * 3;
	for (int i = 0; i < imgsize - next; i = i + next){
		int prevAgv = avgAdjacentPixel(previous, i, nbPixel);

		int curAgv = avgAdjacentPixel(current, i, nbPixel);

		int diff = prevAgv - curAgv;
		if (diff < 0){
			diff = -diff;
		}
		if (diff > threshold){
			for (int j = 0; j < nbPixel; j++){
				int dec = j * 3;
				if (deleteMove){
					difference[i + dec] = 255;
					difference[i + dec + 1] = 255;
					difference[i + dec + 2] = 255;
				} else {
					difference[i + dec] = current[i + dec];
					difference[i + dec + 1] = current[i + dec + 1];
					difference[i + dec + 2] = current[i + dec + 2];
				}

			}
		} else {
			for (int j = 0; j < nbPixel; j++){
				int dec = j * 3;
				difference[i + dec] = 0;
				difference[i + dec + 1] = 0;
				difference[i + dec + 2] = 0;
			}
		}
	}
	return 0;
}

int frame_difference(PBYTE previous, PBYTE current, long imgsize, int threshold, int nbPixel){
	if (previous == NULL) return 0;
	if (current == NULL) return 0;

	//long avg = 0;
	int nbDiff = 0;
	int next = nbPixel * 3;
	for (int i = 0; i < imgsize - next; i = i + next){
		int prevAgv = avgAdjacentPixel(previous, i, nbPixel);

		int curAgv = avgAdjacentPixel(current, i, nbPixel);

		int diff = prevAgv - curAgv;
		if (diff < 0){
			diff = -diff;
		}
		//avg += diff;
		if (diff > threshold){
			nbDiff++;
		}
	}
	//avg = (avg * next) / imgsize;
	//MYPRINTF("avg: %d\n", avg);
	long tmp = 100 * (nbDiff * next);
	return tmp / imgsize;
}

int avgPixel(PBYTE pixel, int i){
	return (pixel[i] + pixel[i + 1] + pixel[i + 2]) / 3;
}

int avgAdjacentPixel(PBYTE pixel, int i, int nb){
	int adj = 0;
	for (int j = 0; j < nb; j++){
		adj += avgPixel(pixel, i + (j * 3));
	}
	return adj / nb;
}



int print_frame_stats(PBYTE previous, PBYTE current, long imgsize){
	if (previous == NULL) return 0;
	if (current == NULL) return 0;
	long avgdiff = 0;

	long avgColor = 0;

	long avgred = 0;
	long avgreen = 0;
	long avgblue = 0;
	for (int i = 0; i < imgsize - 2; i = i + 3){
		int prevAgv = avgPixel(previous, i);

		int curAgv = avgPixel(current, i);

		BYTE curRed = current[i + 2];
		BYTE curGreen = current[i + 1];
		BYTE curBlue = current[i];

		int diff = prevAgv - curAgv;
		if (diff < 0){
			diff = -diff;
		}

		avgred += curRed;
		avgreen += curGreen;
		avgblue += curBlue;
		avgColor += curAgv;
		avgdiff += diff;
	}
	avgdiff = avgdiff / imgsize;
	avgColor = avgColor / imgsize;
#ifdef LOG
	MYPRINTF("average differences: %d\n", avgdiff);
	MYPRINTF("average color: %d\n", avgColor);
	MYPRINTF("average red: %d\n", avgred / imgsize);
	MYPRINTF("average green: %d\n", avgreen / imgsize);
	MYPRINTF("average blue: %d\n", avgblue / imgsize);
#endif
	return 0;
}

// Stops running webcam
DWORD request_webcam_stop(){
	DWORD dwResult = ERROR_SUCCESS;

	running = false;
	if (g_pMediaControl != NULL){
		g_pMediaControl->Stop();
		g_pMediaControl->Release();
		g_pMediaControl = NULL;
	}
	if (g_pIBaseFilterNullRenderer != NULL){
		g_pIBaseFilterNullRenderer->Release();
		g_pIBaseFilterNullRenderer = NULL;
	}
	if (g_pIBaseFilterSampleGrabber != NULL){
		g_pIBaseFilterSampleGrabber->Release();
		g_pIBaseFilterSampleGrabber = NULL;
	}
	if (g_pIBaseFilterCam != NULL){
		g_pIBaseFilterCam->Release();
		g_pIBaseFilterCam = NULL;
	}
	if (g_pGraphBuilder != NULL){
		g_pGraphBuilder->Release();
		g_pGraphBuilder = NULL;
	}
	if (g_pCaptureGraphBuilder != NULL){
		g_pCaptureGraphBuilder->Release();
		g_pCaptureGraphBuilder = NULL;
	}
	CloseHandle(writeEvent);
	return dwResult;
}

void freeGlobals(){
	bmpsize = 0;
	if (imgdata_save != 0){
		delete[] imgdata_save;
		imgdata_save = 0;
	}
	if (bmpdata != 0){
		delete[] bmpdata;
		bmpdata = 0;
	}
	if (imgdata != 0){
		free(imgdata);
		imgdata = 0;
	}
}

