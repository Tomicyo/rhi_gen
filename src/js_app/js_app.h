#pragma once

#if BUILD_JS_APP_LIB
#define JSAPP_API __declspec(dllexport)
#else
#define JSAPP_API __declspec(dllimport)
#endif

#include <libplatform/libplatform.h>
#include <v8.h>

namespace v8
{
  extern JSAPP_API void app_js_create(const FunctionCallbackInfo<v8::Value>& args);
  //extern void app_js_run(const FunctionCallbackInfo<v8::Value>& args);
}