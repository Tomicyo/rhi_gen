#pragma once

#include "sappheiros.h"

#if BUILD_JS_IMPL_LIB
#define JSIMPL_API __declspec(dllexport)
#else
#define JSIMPL_API __declspec(dllimport)
#endif

extern JSIMPL_API int v8_js_binding_test(int argc, char ** argv);