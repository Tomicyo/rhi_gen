#include "implement_js_binding.h"
#include "implement_vk.h"
#include "v8.h"
#include "libplatform/libplatform.h"
#include <fstream>
#include <sstream>

#include <app.h>

using namespace v8;
using namespace std;
using namespace vulkan;

Global<ObjectTemplate> gDeviceTempl;
Global<ObjectTemplate> gFactoryTempl;

static void vk_device_getDesc(const FunctionCallbackInfo<v8::Value>& args)
{
  Local<External> device = Local<External>::Cast(args.Holder()->GetInternalField(0));
  ISPHDevice* ptr = (ISPHDevice*)device->Value();
  sphDeviceDesc desc = {};
  ptr->GetDesc(&desc);
  args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), desc.vendorName));
}

static void vk_factory_createSwapchain(const FunctionCallbackInfo<v8::Value>& args)
{

}

static void vk_factory_enumDevices(const FunctionCallbackInfo<v8::Value>& args)
{
  Local<External> factory = Local<External>::Cast(args.Holder()->GetInternalField(0));
  ISPHFactory* ptr = (ISPHFactory*)factory->Value();
  uint32_t count = 0;
  ptr->EnumDevice(&count, nullptr);
  ISPHDevice ** ppDevice = new ISPHDevice*[count];
  ptr->EnumDevice(&count, ppDevice);

  auto deviceArray = Array::New(args.GetIsolate(), count);

  if (gDeviceTempl.IsEmpty())
  {
    Local<ObjectTemplate> deviceTempl = ObjectTemplate::New(args.GetIsolate());
    deviceTempl->SetInternalFieldCount(1);
    deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "getDesc"),
      FunctionTemplate::New(args.GetIsolate(), vk_device_getDesc));
    gDeviceTempl.Reset(args.GetIsolate(), deviceTempl);
  }
  Local<ObjectTemplate> deviceTempl = Local<ObjectTemplate>::New(args.GetIsolate(), gDeviceTempl);

  for (uint32_t i = 0; i < count; i++)
  {
    Local<Object> device =
      deviceTempl->NewInstance(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
    Local<External> devicePtr = External::New(args.GetIsolate(), ppDevice[i]);
    device->SetInternalField(0, devicePtr);
    deviceArray->Set(i, device);
  }

  args.GetReturnValue().Set(deviceArray);
}

static void vk_createFactory(const FunctionCallbackInfo<v8::Value>& args) 
{
  if (gFactoryTempl.IsEmpty()) 
  {
    Local<ObjectTemplate> factoryTempl = ObjectTemplate::New(args.GetIsolate());
    factoryTempl->SetInternalFieldCount(1);
    factoryTempl->Set(String::NewFromUtf8(args.GetIsolate(), "enumDevices"),
      FunctionTemplate::New(args.GetIsolate(), vk_factory_enumDevices));
    factoryTempl->Set(String::NewFromUtf8(args.GetIsolate(), "createSwapChain"),
      FunctionTemplate::New(args.GetIsolate(), vk_factory_createSwapchain));
    gFactoryTempl.Reset(args.GetIsolate(), factoryTempl);
  }

  Local<ObjectTemplate> factoryTempl = Local<ObjectTemplate>::New(args.GetIsolate(), gFactoryTempl);
  Local<Object> result = factoryTempl->NewInstance(args.GetIsolate()->GetCurrentContext())
    .ToLocalChecked();

  ISPHFactory* factory = CreateFactory();
  Local<External> factory_ptr = External::New(args.GetIsolate(), factory);
  result->SetInternalField(0, factory_ptr);

  args.GetReturnValue().Set(result);
}

int v8_js_binding_test(int argc, char ** argv)
{
  V8::InitializeICUDefaultLocation(argv[0]);
  V8::InitializeExternalStartupData(argv[0]);
  Platform* platform = platform::CreateDefaultPlatform();
  V8::InitializePlatform(platform);
  V8::Initialize();

  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
    v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  Isolate* isolate = Isolate::New(create_params);
  {
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);

    auto global_template_ = ObjectTemplate::New(isolate);
    auto sap_template = ObjectTemplate::New(isolate);
    sap_template->Set(String::NewFromUtf8(isolate, "createFactory"), FunctionTemplate::New(isolate, vk_createFactory));
    global_template_->Set(String::NewFromUtf8(isolate, "sappheiros"), sap_template);
    global_template_->Set(String::NewFromUtf8(isolate, "createApp"), FunctionTemplate::New(isolate, v8::app_js_create));
    
    Local<Context> context = Context::New(isolate, nullptr, global_template_);
    Context::Scope context_scope(context);

    ifstream script_file(argv[1]);
    std::stringstream buffer;
    buffer << script_file.rdbuf();
    std::string contents(buffer.str());
    Local<String> source =
      String::NewFromUtf8(isolate, contents.c_str(),
        NewStringType::kNormal).ToLocalChecked();
    Local<String> source_origin =
      String::NewFromUtf8(isolate, argv[1],
        NewStringType::kNormal).ToLocalChecked();

    Local<Script> script = Script::Compile(context, source, new ScriptOrigin(source_origin))
      .ToLocalChecked();

    Local<Value> result = script->Run(context).ToLocalChecked();

    if (result->IsString())
    {
      String::Utf8Value utf8(result);
      printf("%s\n", *utf8);
    }
  }
  gDeviceTempl.Reset();
  gFactoryTempl.Reset();

  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;
  delete create_params.array_buffer_allocator;
  return 0;
}