#include "implement_js_binding.h"
#include "implement_vk.h"
#include "v8.h"
#include "libplatform/libplatform.h"

using namespace v8;
using namespace vulkan;

static void vk_device_getDesc(const FunctionCallbackInfo<v8::Value>& args)
{
  HandleScope handle_scope(args.GetIsolate());
  Local<External> device = Local<External>::Cast(args.Holder()->GetInternalField(0));
  ISPHDevice* ptr = (ISPHDevice*)device->Value();
  sphDeviceDesc desc = {};
  ptr->GetDesc(&desc);
  args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), desc.vendorName));
}

static void vk_factory_enumDevices(const FunctionCallbackInfo<v8::Value>& args)
{
  HandleScope handle_scope(args.GetIsolate());
  Local<External> factory = Local<External>::Cast(args.Holder()->GetInternalField(0));
  ISPHFactory* ptr = (ISPHFactory*)factory->Value();
  uint32_t count = 0;
  ptr->EnumDevice(&count, nullptr);
  ISPHDevice ** ppDevice = new ISPHDevice*[count];
  ptr->EnumDevice(&count, ppDevice);

  auto deviceArray = Array::New(args.GetIsolate(), count);

  Local<ObjectTemplate> deviceTempl = ObjectTemplate::New(args.GetIsolate());
  deviceTempl->SetInternalFieldCount(1);
  deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "getDesc"),
    FunctionTemplate::New(args.GetIsolate(), vk_device_getDesc));

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
  HandleScope handle_scope(args.GetIsolate());

  Local<ObjectTemplate> factoryTempl = ObjectTemplate::New(args.GetIsolate());
  factoryTempl->SetInternalFieldCount(1);
  factoryTempl->Set(String::NewFromUtf8(args.GetIsolate(), "enumDevices"), 
    FunctionTemplate::New(args.GetIsolate(), vk_factory_enumDevices));

  Local<Object> result =
    factoryTempl->NewInstance(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();

  ISPHFactory* factory = CreateFactory();
  Local<External> factory_ptr = External::New(args.GetIsolate(), factory);
  result->SetInternalField(0, factory_ptr);

  args.GetReturnValue().Set(result);
}

int v8_js_binding_test(const char * pth)
{
  V8::InitializeICUDefaultLocation(pth);
  V8::InitializeExternalStartupData(pth);
  Platform* platform = platform::CreateDefaultPlatform();
  V8::InitializePlatform(platform);
  V8::Initialize();

  // Create a new Isolate and make it the current one.
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
    v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  Isolate* isolate = Isolate::New(create_params);
  {
    Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    //
    auto global_template_ = ObjectTemplate::New(isolate);
    global_template_->Set(String::NewFromUtf8(isolate, "createFactory"), FunctionTemplate::New(isolate, vk_createFactory));

    // Create a new context.
    Local<Context> context = Context::New(isolate, nullptr, global_template_);

    // Enter the context for compiling and running the hello world script.
    Context::Scope context_scope(context);

    // Create a string containing the JavaScript source code.
    Local<String> source =
      String::NewFromUtf8(isolate, "var factory = createFactory();\nvar devices = factory.enumDevices();\ndevices[0].getDesc();",
        NewStringType::kNormal).ToLocalChecked();

    // Compile the source code.
    Local<Script> script = Script::Compile(context, source).ToLocalChecked();

    // Run the script to get the result.
    Local<Value> result = script->Run(context).ToLocalChecked();

    // Convert the result to an UTF8 string and print it.
    String::Utf8Value utf8(result);
    printf("%s\n", *utf8);
  }

  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;
  delete create_params.array_buffer_allocator;
  return 0;
}