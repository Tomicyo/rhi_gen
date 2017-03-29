#include "implement_vk.h"
#include <vector>
#include <array>

using namespace std;

namespace vulkan 
{

sphResult FactoryImpl::EnumDevice(uint32_t * count, ISPHDevice ** ppDevice)
{
  vkEnumeratePhysicalDevices(handle, count, nullptr);
  vector<VkPhysicalDevice> Devices;
  Devices.resize(*count);
  vkEnumeratePhysicalDevices(handle, count, Devices.data());
  if (ppDevice)
  {
    ppDevice[0] = new DeviceImpl(this, Devices[0]);
  }
  return SPH_RESULT_OK;
}

sphResult FactoryImpl::CreateSwapchain(const sphSwapChainDesc * desc, ISPHCommandQueue * pCommandQueue, void * pWindow, ISPHSwapChain ** pSwapchain)
{
  if (!pSwapchain)
  {
    pSwapchain = new ISPHSwapChain*;
  }
  *pSwapchain = new SwapChainImpl(this);
  return SPH_RESULT_OK;
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

void DeviceImpl::GetDesc(sphDeviceDesc * pDesc)
{
  vkGetPhysicalDeviceProperties(mGpu, &props);
  pDesc->maxAllocation = props.limits.maxMemoryAllocationCount;
  pDesc->vendorName = props.deviceName;
}

sphResult DeviceImpl::CreateCommandQueue(sphCommandQueueType queueType, ISPHCommandQueue ** pQueue)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreatePipelineLayout(ISPHPipelineLayout ** ppPipelineLayout)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreatePipelineState(const sphPipelineStateDesc * pPipelineDesc, ISPHPipelineState ** pPipelineState)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateRenderPass()
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateRenderTarget()
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateSampler(const sphSamplerDesc* desc, ISPHSampler ** pSampler)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateBuffer(const sphBufferDesc* desc, ISPHBuffer ** pBuffer)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateBufferView(const sphBufferViewDesc * desc, ISPHBuffer * pBuffer, ISPHBufferView ** pBufView)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateTexture(const sphTextureDesc * desc, ISPHTexture ** pTexture)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateTextureView(const sphTextureViewDesc * desc, ISPHTexture * pTexture, ISPHTextureView ** pTexView)
{
  return SPH_RESULT_OK;
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

ISPHCommandBuffer * CommandQueueImpl::CommandBuffer()
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

void CommandBufferImpl::Present(ISPHSwapChain * pSwapChain)
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

void CommandBufferImpl::BeginRenderPass(const ISPHRenderPass* pRenderPass)
{
}

void CommandBufferImpl::SetScissorRects()
{
}

void CommandBufferImpl::SetViewport(const sphViewport * pViewport)
{
}

void CommandBufferImpl::SetPipelineState(ISPHPipelineState* pPipelineState)
{
}

void CommandBufferImpl::SetPipelineLayout(ISPHPipelineLayout * pPipelineLayout)
{
}

void CommandBufferImpl::SetBindingGroup(ISPHBindingGroup * pBindingGroup)
{
}

void CommandBufferImpl::SetIndexBuffer()
{
}

void CommandBufferImpl::SetVertexBuffer(uint32_t slot, uint64_t offset, ISPHBuffer * pVertexBuffer)
{
}

void CommandBufferImpl::SetPrimitiveType(sphPrimitiveType primitive)
{
}

void CommandBufferImpl::DrawInstanced(const sphDrawInstancedDesc * drawParam)
{
}

void CommandBufferImpl::DrawIndexedInstanced(const sphDrawIndexedInstancedDesc * drawParam)
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

sphPipelineType PipelineStateImpl::Type()
{
  return SPH_PIPELINE_TYPE_GRAPHICS;
}

sphResult PipelineStateImpl::GetDesc(sphPipelineStateDesc * desc)
{
  return SPH_RESULT_OK;
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

void BindingGroupImpl::SetSampler(uint32_t index, sphShaderType shaderVis, ISPHSampler * pSampler)
{
}

void BindingGroupImpl::SetBuffer(uint32_t index, sphShaderType shaderVis, ISPHBufferView * bufferView)
{
}

void BindingGroupImpl::SetTexture(uint32_t index, sphShaderType shaderVis, ISPHTextureView * textureView)
{
}

BindingGroupImpl::BindingGroupImpl()
{
}

BindingGroupImpl::~BindingGroupImpl()
{
}

sphResult BufferImpl::GetDesc(sphBufferDesc * pDesc)
{
  return SPH_RESULT_OK;
}

BufferImpl::BufferImpl()
{
}

BufferImpl::~BufferImpl()
{
}

sphResult TextureImpl::GetDesc(sphTextureDesc * pDesc)
{
  return SPH_RESULT_OK;
}

TextureImpl::TextureImpl()
{
}

TextureImpl::~TextureImpl()
{
}

sphResult SamplerImpl::GetDesc(sphSamplerDesc * desc)
{
  return SPH_RESULT_OK;
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

ISPHFactory * CreateFactory()
{
  return new FactoryImpl;
}

} // end vulkan
