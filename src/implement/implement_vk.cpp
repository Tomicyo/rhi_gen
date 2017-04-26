#include "implement_vk.h"
#include <vector>
#include <array>
#include <cassert>

#define check(ret) assert(ret==VK_SUCCESS)

using namespace std;

namespace vulkan 
{

  VkFormat ConvertFormat(sphPixelFormat const& fmt)
  {
    switch (fmt)
    {
    case SPH_PIXEL_FORMAT_RGBA8UNORM:
      return VK_FORMAT_R8G8B8A8_UNORM;
    }
    return VK_FORMAT_R8G8B8A8_UNORM;
  }

sphResult FactoryImpl::EnumDevice(uint32_t * count, ISPHDevice ** ppDevice)
{
  vkEnumeratePhysicalDevices(Handle, count, nullptr);
  vector<VkPhysicalDevice> Devices;
  Devices.resize(*count);
  vkEnumeratePhysicalDevices(Handle, count, Devices.data());
  if (ppDevice)
  {
    ppDevice[0] = new DeviceImpl(this, Devices[0]);
  }
  return SPH_RESULT_OK;
}

sphResult FactoryImpl::CreateSwapchain(const sphSwapChainDesc * desc, ISPHCommandQueue * pCommandQueue, void * pWindow, ISPHSwapChain ** pSwapchain)
{
  if (desc == nullptr || pCommandQueue == nullptr || pWindow == nullptr)
    return SPH_RESULT_PARAM_ERROR;
  if (!pSwapchain)
  {
    pSwapchain = new ISPHSwapChain*;
  }
  *pSwapchain = new SwapChainImpl(this, desc, static_cast<CommandQueueImpl*>(pCommandQueue), pWindow);
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
  std::vector<const char*> enabledExtensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
  };
  instanceCreateInfo.enabledExtensionCount = enabledExtensions.size();
  instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
  instanceCreateInfo.enabledLayerCount = 0;
  instanceCreateInfo.ppEnabledLayerNames = nullptr;

  VkResult err = vkCreateInstance(&instanceCreateInfo, nullptr, &Handle);
}

FactoryImpl::~FactoryImpl()
{
  vkDestroyInstance(Handle, nullptr);
}

sphResult SwapChainImpl::GetTexture(ISPHTexture ** ppTexture, uint32_t index)
{
  if(!ppTexture || index >= ColorImages.size())
    return SPH_RESULT_PARAM_ERROR;
  *ppTexture = new TextureImpl(ColorImages[index], Extent.width, Extent.height, SPH_PIXEL_FORMAT_RGBA8UNORM);
  return SPH_RESULT_OK;
}

void SwapChainImpl::Present()
{
}

SwapChainImpl::SwapChainImpl(FactoryImpl* impl, const sphSwapChainDesc* desc, class CommandQueueImpl* pQueue, void* hWnd)
  : Impl(impl)
  , QueueImpl(pQueue)
{
  VkWin32SurfaceCreateInfoKHR SurfaceCreateInfo = {};
  SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  SurfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
  SurfaceCreateInfo.hwnd = (HWND)hWnd;
  vkCreateWin32SurfaceKHR(Impl->Handle, &SurfaceCreateInfo, nullptr, &Surface);

  Extent = { desc->width, desc->height };
  VkSwapchainCreateInfoKHR swapchainCI = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
  swapchainCI.surface = Surface;
  swapchainCI.minImageCount = 2;
  swapchainCI.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
  ColorAttachFmt = VK_FORMAT_R8G8B8A8_UNORM;
  swapchainCI.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  swapchainCI.imageExtent = Extent;
  swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainCI.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  swapchainCI.imageArrayLayers = 1;
  swapchainCI.queueFamilyIndexCount = VK_SHARING_MODE_EXCLUSIVE;
  swapchainCI.queueFamilyIndexCount = 0;
  swapchainCI.pQueueFamilyIndices = NULL;
  swapchainCI.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  swapchainCI.oldSwapchain = VK_NULL_HANDLE;
  swapchainCI.clipped = VK_TRUE;
  swapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  VkResult err = vkCreateSwapchainKHR(QueueImpl->Device->Handle, &swapchainCI, nullptr, &Handle);

  uint32_t backBufferCount = 0;
  vkGetSwapchainImagesKHR(QueueImpl->Device->Handle, Handle, &backBufferCount, nullptr);
  ColorImages.resize(backBufferCount);
  vkGetSwapchainImagesKHR(QueueImpl->Device->Handle, Handle, &backBufferCount, ColorImages.data());

  QueueImpl->AddRef();
  Impl->AddRef();
}

SwapChainImpl::~SwapChainImpl()
{
  vkDestroySurfaceKHR(Impl->Handle, Surface, nullptr);
  vkDestroySwapchainKHR(QueueImpl->Device->Handle, Handle, nullptr);
  Impl->Release();
  QueueImpl->Release();
}

void DeviceImpl::GetDesc(sphDeviceDesc * pDesc)
{
  vkGetPhysicalDeviceProperties(PhysicalDevice, &DeviceProps);
  pDesc->maxAllocation = DeviceProps.limits.maxMemoryAllocationCount;
  pDesc->vendorName = DeviceProps.deviceName;
}

sphResult DeviceImpl::CreateCommandQueue(sphCommandQueueType queueType, ISPHCommandQueue ** pQueue)
{
  *pQueue = new CommandQueueImpl(this);
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreatePipelineLayout(const sphPipelineLayoutDesc * pPipelineLayoutDesc, ISPHPipelineLayout ** ppPipelineLayout)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateBindingTable(ISPHPipelineLayout * pPipelineLayout, ISPHBindingTable ** ppBindingTable)
{
  return sphResult();
}

sphResult DeviceImpl::CreatePipelineState(const sphPipelineStateDesc * pPipelineDesc, ISPHPipelineLayout * pPipelineLayout, ISPHPipelineState ** pPipelineState)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateRenderPass(const sphRenderPassDesc * desc, ISPHRenderPass ** ppRenderpass)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateRenderTarget(const sphRenderTargetDesc * desc, ISPHRenderTarget ** ppRenderTarget)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateSampler(const sphSamplerDesc* desc, ISPHSampler ** pSampler)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateBuffer(const sphBufferDesc* desc, ISPHBuffer ** pBuffer)
{
  if (!desc || !pBuffer)
    return SPH_RESULT_PARAM_ERROR;
  *pBuffer = new BufferImpl(this, desc);
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateBufferView(const sphBufferViewDesc * desc, ISPHBuffer * pBuffer, ISPHBufferView ** pBufView)
{
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateTexture(const sphTextureDesc * desc, ISPHTexture ** pTexture)
{
  if (!pTexture || !desc)
    return SPH_RESULT_PARAM_ERROR;
  *pTexture = new TextureImpl(this, desc);
  return SPH_RESULT_OK;
}

sphResult DeviceImpl::CreateTextureView(const sphTextureViewDesc * desc, ISPHTexture * pTexture, ISPHTextureView ** pTexView)
{
  if (!pTexture || !desc || !pTexView)
    return SPH_RESULT_PARAM_ERROR;
  *pTexView = new TextureViewImpl(this, desc, pTexture);
  return SPH_RESULT_OK;
}

void DeviceImpl::WaitIdle()
{
}

GpuAllocation DeviceImpl::Allocate(VkMemoryAllocateInfo const& AllocInfo, bool Transient)
{
  return MemoryAllocator->Allocate(AllocInfo, Transient);
}

void DeviceImpl::Free(GpuAllocation const & Alloca)
{
  MemoryAllocator->Free(Alloca);
}

bool DeviceImpl::FindMemoryHeap(uint32_t typeBits, VkFlags requirementsMask, uint32_t * typeIndex)
{
#ifdef max
#undef max
  *typeIndex = std::numeric_limits<std::remove_pointer<decltype(typeIndex)>::type>::max();
#endif
  auto memProp = DeviceMemoryProps;
  for (uint32_t i = 0; i < memProp.memoryTypeCount; ++i) {
    if (typeBits & 0x00000001) {
      if (requirementsMask == (memProp.memoryTypes[i].propertyFlags & requirementsMask)) {
        *typeIndex = i;
        return true;
      }
    }
    typeBits >>= 1;
  }
  return false;
}

DeviceImpl::DeviceImpl(FactoryImpl* impl, VkPhysicalDevice Device)
  : DeviceImpl::This()
  , PhysicalDevice(Device)
  , Impl(impl)
{
  std::array<float, 1> queuePriorities = { 0.0f };
  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = 0;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = queuePriorities.data();

  std::vector<const char*> enabledExtensions = { 
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };
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
  VkResult err = vkCreateDevice(Device, &deviceCreateInfo, nullptr, &Handle);

  vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &DeviceMemoryProps);

  MemoryAllocator = new LinearMemoryAllocator(this);

  Impl->AddRef();
}

DeviceImpl::~DeviceImpl()
{
  vkDestroyDevice(Handle, nullptr);
  Impl->Release();
}

ISPHCommandBuffer * CommandQueueImpl::CommandBuffer()
{
  return nullptr;
}

CommandQueueImpl::CommandQueueImpl(DeviceImpl* dev) 
  : CommandQueueImpl::This()
  , Device(dev)
{
  vkGetDeviceQueue(dev->Handle, 0, 0, &Handle);
  Device->AddRef();
}

CommandQueueImpl::~CommandQueueImpl()
{
  Device->Release();
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

void CommandBufferImpl::SetBindingTable(ISPHBindingTable * pBindingGroup)
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

PipelineStateImpl::PipelineStateImpl(DeviceImpl* pDevice, const sphPipelineStateDesc* pDesc, ISPHRenderPass* pRenderPass)
  : Device(pDevice)
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

VkBufferCreateInfo ConvertBufferCreateInfo(sphBufferDesc const* pDesc)
{
  VkBufferCreateInfo Info = {};
  return Info;
}

BufferImpl::BufferImpl(DeviceImpl* Device, const sphBufferDesc * pDesc)
  : BufferImpl::This(Device)
{
  VkBufferCreateInfo Info = ConvertBufferCreateInfo(pDesc);
  VkResult ret = vkCreateBuffer(Device->Handle, &Info, nullptr, &Handle);
}

BufferImpl::~BufferImpl()
{
  vkDestroyBuffer(Device->Handle, Handle, nullptr);
}

sphResult TextureImpl::GetDesc(sphTextureDesc * pDesc)
{
  return SPH_RESULT_OK;
}

VkImageCreateInfo ConvertImageCreateInfo(sphTextureDesc const* pDesc)
{
  VkImageCreateInfo Info = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
  Info.format = ConvertFormat(pDesc->format);
  Info.extent = { pDesc->width, pDesc->height, pDesc->depth };
  Info.mipLevels = pDesc->mipLevels;
  Info.arrayLayers = pDesc->layers;
  if (pDesc->depth > 1)
  {
    Info.imageType = VK_IMAGE_TYPE_3D;
  }
  else if (pDesc->height > 1)
  {
    Info.imageType = VK_IMAGE_TYPE_2D;
  }
  else
  {
    Info.imageType = VK_IMAGE_TYPE_1D;
  }
  return Info;
}

TextureImpl::TextureImpl(VkImage SwapChainImg, uint32_t Width, uint32_t Height, sphPixelFormat Format)
  : TextureImpl::This(nullptr)
{
  Handle = SwapChainImg;
  Desc.format = Format;
  Desc.width = Width;
  Desc.height = Height;
  Desc.depth = 1;
  Desc.layers = 1;
  Desc.mipLevels = 1;
}

TextureImpl::TextureImpl(DeviceImpl* Device, const sphTextureDesc * pDesc)
  : TextureImpl::This(Device)
{
  // Create Image
  CachedCreateInfo = ConvertImageCreateInfo(pDesc);

  memcpy(&Desc, pDesc, sizeof(Desc));

  CachedCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  CachedCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
  CachedCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
  CachedCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // Or Concurrent
  CachedCreateInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;

  VkResult result = vkCreateImage(Device->Handle, &CachedCreateInfo, nullptr, &Handle);

  // Query Image Memory Requirements
  VkMemoryRequirements memReq = {};
  vkGetImageMemoryRequirements(Device->Handle, Handle, &memReq);
  uint32_t tyId;
  Device->FindMemoryHeap(memReq.memoryTypeBits, 0, &tyId);

  // Allocate Memory
  VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, memReq.size, tyId };
  Allocation = Device->Allocate(allocInfo, false);

  // Bind Memory
  vkBindImageMemory(Device->Handle, Handle, Allocation.Memory, Allocation.OffSet);

  Device->AddRef();
}

TextureImpl::~TextureImpl()
{
  if (Device)
  {
    vkDestroyImage(Device->Handle, Handle, nullptr);
    Device->Release();
  }
}

VkImageViewType ConvertImageViewType(sphTextureDimension const& Dim)
{
  switch (Dim)
  {
  case SPH_TEXTURE_DIMENSION_1D:
    return VK_IMAGE_VIEW_TYPE_1D;
  case SPH_TEXTURE_DIMENSION_2D:
    return VK_IMAGE_VIEW_TYPE_2D;
  case SPH_TEXTURE_DIMENSION_2D_ARRAY:
    return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
  case SPH_TEXTURE_DIMENSION_3D:
    return VK_IMAGE_VIEW_TYPE_3D;
  case SPH_TEXTURE_DIMENSION_CUBE:
    return VK_IMAGE_VIEW_TYPE_CUBE;
  default:
    break;
  }
  return VK_IMAGE_VIEW_TYPE_2D;
}

VkImageViewCreateInfo TextureViewImpl::ConvertImageViewCreateInfo(const sphTextureViewDesc* pDesc, ISPHTexture * pTexture)
{
  TextureImpl * pTextureImpl = reinterpret_cast<TextureImpl*>(pTexture);
  VkImageViewCreateInfo Info = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
  Info.viewType = ConvertImageViewType(pDesc->dimension);
  Info.subresourceRange = {}; // TODO
  Info.components = {};       // TODO
  Info.flags = 0;             // TODO
  Info.format = pTextureImpl->CachedCreateInfo.format;
  Info.image = pTextureImpl->GetHandle();
  return Info;
}

TextureViewImpl::TextureViewImpl(DeviceImpl * pDevice, const sphTextureViewDesc* pDesc, ISPHTexture * pTexture)
  : TextureViewImpl::This()
  , Device(pDevice)
{
  CachedCreateInfo = ConvertImageViewCreateInfo(pDesc, pTexture);
  VkResult ret = vkCreateImageView(Device->Handle, &CachedCreateInfo, nullptr, &Handle);
  check(ret);
  Device->AddRef();
}

TextureViewImpl::~TextureViewImpl()
{
  vkDestroyImageView(Device->Handle, Handle, nullptr);
  Device->Release();
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

RenderPassImpl::RenderPassImpl(DeviceImpl* pDevice, const sphRenderPassDesc* pDesc)
  : Device(pDevice)
{
}

RenderPassImpl::~RenderPassImpl()
{
}

RenderTargetImpl::RenderTargetImpl(DeviceImpl* pDevice)
  : RenderTargetImpl::This()
  , Device(pDevice)
{
  VkFramebufferCreateInfo Info = {};
  vkCreateFramebuffer(Device->Handle, &Info, nullptr, &Handle);
  Device->AddRef();
}

RenderTargetImpl::~RenderTargetImpl()
{
  vkDestroyFramebuffer(Device->Handle, Handle, nullptr);
  Device->Release();
}

ISPHFactory * CreateFactory()
{
  return new FactoryImpl;
}

GpuAllocation LinearMemoryAllocator::Allocate(VkMemoryAllocateInfo const & AllocaInfo, bool transient)
{
  VkDeviceMemory Memory;
  vkAllocateMemory(Device->Handle, &AllocaInfo, nullptr, &Memory);
  return {0, AllocaInfo.allocationSize, false, Memory };
}

void LinearMemoryAllocator::Free(GpuAllocation const & Allocation)
{
  vkFreeMemory(Device->Handle, Allocation.Memory, nullptr);
}

} // end vulkan
