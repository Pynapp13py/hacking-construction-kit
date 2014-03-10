/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "the_plugin.h"


extern "C" __declspec(dllexport) void GetPlugin(Plugin** ppOutput) {
    *ppOutput = new ThePlugin();
}


