#ifndef __App_h__
#define __App_h__

#if ENABLE_V8
#include <libplatform/libplatform.h>
#include <v8.h>
#endif

#if BUILD_LIB
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

class App
{
public:
  App(const char * title, uint32_t width, uint32_t height);
  ~App();

  void Run();

  void* Handle() const { return hWnd; }

private:
  void * hWnd;
};

#if ENABLE_V8
namespace v8
{
  extern API void app_js_create(const FunctionCallbackInfo<v8::Value>& args);
  //extern void app_js_run(const FunctionCallbackInfo<v8::Value>& args);
}
#endif

#endif