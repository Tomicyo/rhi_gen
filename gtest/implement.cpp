#include "implement.h"
#include <vector>
#include <array>

using namespace std;

namespace vulkan 
{

nxtResult FactoryImpl::EnumDevice(uint32_t * count, INXTDevice ** ppDevice)
{
  vkEnumeratePhysicalDevices(handle, count, nullptr);
  vector<VkPhysicalDevice> Devices;
  Devices.resize(*count);
  vkEnumeratePhysicalDevices(handle, count, Devices.data());
  if (ppDevice)
  {
    ppDevice[0] = new DeviceImpl(this, Devices[0]);
  }
  return NXT_RESULT_OK;
}

nxtResult FactoryImpl::CreateSwapchain(nxtSwapChainDesc * desc, void * pWindow, INXTSwapChain ** pSwapchain)
{
  *pSwapchain = new SwapChainImpl(this);
  return NXT_RESULT_OK;
}

FactoryImpl::FactoryImpl() : FactoryImpl::This()
{
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "test";
  appInfo.pEngineName = "test";
  appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 1);
  appInfo.engineVersion = 1;
  appInfo.applicationVersion = 0;

  VkInstanceCreateInfo instanceCreateInfo = {};
  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pNext = NULL;
  instanceCreateInfo.pApplicationInfo = &appInfo;
  instanceCreateInfo.enabledExtensionCount = 0;
  instanceCreateInfo.ppEnabledExtensionNames = nullptr;
  instanceCreateInfo.enabledLayerCount = 0;
  instanceCreateInfo.ppEnabledLayerNames = nullptr;

  VkResult err = vkCreateInstance(&instanceCreateInfo, nullptr, &handle);
}

FactoryImpl::~FactoryImpl()
{
  vkDestroyInstance(handle, nullptr);
}

void SwapChainImpl::Present()
{
}

SwapChainImpl::SwapChainImpl(FactoryImpl* impl)
  : mImpl(impl)
{
  mImpl->AddRef();
}

SwapChainImpl::~SwapChainImpl()
{
  mImpl->Release();
}

void DeviceImpl::GetDesc(nxtDeviceDesc * pDesc)
{
  vkGetPhysicalDeviceProperties(mGpu, &props);
  pDesc->maxAllocation = props.limits.maxMemoryAllocationCount;
  pDesc->vendorName = props.deviceName;
}

nxtResult DeviceImpl::CreateCommandQueue(nxtCommandQueueType queueType, INXTCommandQueue ** pQueue)
{
  return NXT_RESULT_OK;
}

nxtResult DeviceImpl::CreatePipelineLayout(INXTPipelineLayout ** ppPipelineLayout)
{
  return NXT_RESULT_OK;
}

nxtResult DeviceImpl::CreatePipelineState(const nxtPipelineStateDesc * pPipelineDesc, INXTPipelineState ** pPipelineState)
{
  return NXT_RESULT_OK;
}

nxtResult DeviceImpl::CreateRenderPass()
{
  return NXT_RESULT_OK;
}

nxtResult DeviceImpl::CreateRenderTarget()
{
  return NXT_RESULT_OK;
}

nxtResult DeviceImpl::CreateSampler(const nxtSamplerDesc* desc, INXTSampler ** pSampler)
{
  return NXT_RESULT_OK;
}

nxtResult DeviceImpl::CreateBuffer(const nxtBufferDesc* desc, INXTBuffer ** pBuffer)
{
  return NXT_RESULT_OK;
}

nxtResult DeviceImpl::CreateBufferView(const nxtBufferViewDesc * desc, INXTBuffer * pBuffer, INXTBufferView ** pBufView)
{
  return NXT_RESULT_OK;
}

nxtResult DeviceImpl::CreateTexture(const nxtTextureDesc * desc, INXTTexture ** pTexture)
{
  return NXT_RESULT_OK;
}

nxtResult DeviceImpl::CreateTextureView(const nxtTextureViewDesc * desc, INXTTexture * pTexture, INXTTextureView ** pTexView)
{
  return NXT_RESULT_OK;
}

void DeviceImpl::WaitIdle()
{
}

DeviceImpl::DeviceImpl(FactoryImpl* impl, VkPhysicalDevice Device)
  : DeviceImpl::This()
  , mGpu(Device)
  , mImpl(impl)
{
  std::array<float, 1> queuePriorities = { 0.0f };
  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = 0;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = queuePriorities.data();

  std::vector<const char*> enabledExtensions = { 
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHX_DEVICE_GROUP_EXTENSION_NAME };
  VkDeviceCreateInfo deviceCreateInfo = {};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pNext = NULL;
  deviceCreateInfo.queueCreateInfoCount = 1;
  deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
  deviceCreateInfo.pEnabledFeatures = NULL;

  if (enabledExtensions.size() > 0)
  {
    deviceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
  }
  vkCreateDevice(Device, &deviceCreateInfo, nullptr, &handle);
  mImpl->AddRef();
}

DeviceImpl::~DeviceImpl()
{
  vkDestroyDevice(handle, nullptr);
  mImpl->Release();
}

INXTCommandBuffer * CommandQueueImpl::CommandBuffer()
{
  return nullptr;
}

CommandQueueImpl::CommandQueueImpl()
{
}

CommandQueueImpl::~CommandQueueImpl()
{
}

void CommandBufferImpl::Commit()
{
}

void CommandBufferImpl::Present(INXTSwapChain * pSwapChain)
{
}

void CommandBufferImpl::CopyTexture()
{
}

void CommandBufferImpl::CopyBuffer()
{
}

void CommandBufferImpl::ClearColorDepth()
{
}

void CommandBufferImpl::Begin()
{
}

void CommandBufferImpl::BeginRenderPass(const INXTRenderPass* pRenderPass)
{
}

void CommandBufferImpl::SetScissorRects()
{
}

void CommandBufferImpl::SetViewport(const nxtViewport * pViewport)
{
}

void CommandBufferImpl::SetPipelineState(INXTPipelineState* pPipelineState)
{
}

void CommandBufferImpl::SetPipelineLayout(INXTPipelineLayout * pPipelineLayout)
{
}

void CommandBufferImpl::SetBindingGroup(INXTBindingGroup * pBindingGroup)
{
}

void CommandBufferImpl::SetIndexBuffer()
{
}

void CommandBufferImpl::SetVertexBuffer(uint32_t slot, uint64_t offset, INXTBuffer * pVertexBuffer)
{
}

void CommandBufferImpl::SetPrimitiveType(nxtPrimitiveType primitive)
{
}

void CommandBufferImpl::DrawInstanced(const nxtDrawInstancedDesc * drawParam)
{
}

void CommandBufferImpl::DrawIndexedInstanced(const nxtDrawIndexedInstancedDesc * drawParam)
{
}

void CommandBufferImpl::EndRenderPass()
{
}

void CommandBufferImpl::End()
{
}

void CommandBufferImpl::Dispatch(uint32_t x, uint32_t y, uint32_t z)
{
}

void CommandBufferImpl::ExecuteBundle()
{
}

CommandBufferImpl::CommandBufferImpl()
{
}

CommandBufferImpl::~CommandBufferImpl()
{
}

nxtPipelineType PipelineStateImpl::Type()
{
  return NXT_PIPELINE_TYPE_GRAPHICS;
}

nxtResult PipelineStateImpl::GetDesc(nxtPipelineStateDesc * desc)
{
  return NXT_RESULT_OK;
}

PipelineStateImpl::PipelineStateImpl()
{
}

PipelineStateImpl::~PipelineStateImpl()
{
}

PipelineLayoutImpl::PipelineLayoutImpl()
{
}

PipelineLayoutImpl::~PipelineLayoutImpl()
{
}

void BindingGroupImpl::SetSampler(uint32_t index, nxtShaderType shaderVis, INXTSampler * pSampler)
{
}

void BindingGroupImpl::SetBuffer(uint32_t index, nxtShaderType shaderVis, INXTBufferView * bufferView)
{
}

void BindingGroupImpl::SetTexture(uint32_t index, nxtShaderType shaderVis, INXTTextureView * textureView)
{
}

BindingGroupImpl::BindingGroupImpl()
{
}

BindingGroupImpl::~BindingGroupImpl()
{
}

nxtResult BufferImpl::GetDesc(nxtBufferDesc * pDesc)
{
  return NXT_RESULT_OK;
}

BufferImpl::BufferImpl()
{
}

BufferImpl::~BufferImpl()
{
}

nxtResult TextureImpl::GetDesc(nxtTextureDesc * pDesc)
{
  return NXT_RESULT_OK;
}

TextureImpl::TextureImpl()
{
}

TextureImpl::~TextureImpl()
{
}

nxtResult SamplerImpl::GetDesc(nxtSamplerDesc * desc)
{
  return NXT_RESULT_OK;
}

SamplerImpl::SamplerImpl()
{
}

SamplerImpl::~SamplerImpl()
{
}

RenderPassImpl::RenderPassImpl()
{
}

RenderPassImpl::~RenderPassImpl()
{
}

} // end vulkan
