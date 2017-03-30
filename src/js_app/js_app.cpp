#include <app/app.h>
#include "js_app.h"

namespace v8
{
  void app_js_run(const FunctionCallbackInfo<v8::Value>& args);

  void app_js_create(const FunctionCallbackInfo<v8::Value>& args)
  {
    if (args.Length() == 0 || !args[0]->IsObject())
    {
      args.GetReturnValue().Set(Undefined(args.GetIsolate()));
      return;
    }

    auto obj = args[0]->ToObject();

    auto title = obj->Get(String::NewFromUtf8(args.GetIsolate(), "title"));
    auto width = obj->Get(String::NewFromUtf8(args.GetIsolate(), "width"));
    auto height = obj->Get(String::NewFromUtf8(args.GetIsolate(), "height"));

    App * ptr = new App(*String::Utf8Value(title), width->Uint32Value(), height->Uint32Value());

    Local<ObjectTemplate> appTempl = ObjectTemplate::New(args.GetIsolate());
    appTempl->SetInternalFieldCount(1);
    appTempl->Set(String::NewFromUtf8(args.GetIsolate(), "run"),
      FunctionTemplate::New(args.GetIsolate(), app_js_run));

    Local<Object> app = appTempl->NewInstance(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
    Local<External> app_ptr = External::New(args.GetIsolate(), ptr);
    app->SetInternalField(0, app_ptr);

    args.GetReturnValue().Set(app);
  }

  void app_js_run(const FunctionCallbackInfo<v8::Value>& args)
  {
    Local<External> app_ptr = Local<External>::Cast(args.Holder()->GetInternalField(0));
    App* ptr = (App*)app_ptr->Value();
    ptr->Run();
    args.GetReturnValue().Set(Undefined(args.GetIsolate()));
  }
}