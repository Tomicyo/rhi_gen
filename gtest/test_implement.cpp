#include "implement/implement_vk.h"
#include "js_implement/implement_js_binding.h"
#include "gtest/gtest.h"
#include "app/app.h"

using namespace vulkan;

TEST(CreateFactory, VULKANFactory) 
{
	ISPHFactory* factory = CreateFactory();
	ASSERT_TRUE(factory);
	ASSERT_EQ(factory->Release(), 0);
}

TEST(CreateDevice, VULKANFactory) 
{
	ISPHFactory* pFactory = CreateFactory();
	// create device
	uint32_t deviceCount = 0;
	pFactory->EnumDevice(&deviceCount, nullptr);
	ASSERT_GT(deviceCount, 0);
	ISPHDevice ** ppDevice = new ISPHDevice*[deviceCount];
	pFactory->EnumDevice(&deviceCount, ppDevice);
	ISPHDevice * pCurDev = ppDevice[0];
	ASSERT_TRUE(pCurDev);
	ASSERT_EQ(pCurDev->Release(), 0);  // exchange with next call ?
	ASSERT_EQ(pFactory->Release(), 0);
}

TEST(GetDesc, VULKANDevice)
{
  ISPHFactory* pFactory = CreateFactory();
  // create device
  uint32_t deviceCount = 0;
  pFactory->EnumDevice(&deviceCount, nullptr);
  ASSERT_GT(deviceCount, 0);
  ISPHDevice ** ppDevice = new ISPHDevice*[deviceCount];
  pFactory->EnumDevice(&deviceCount, ppDevice);
  ISPHDevice * pCurDev = ppDevice[0];
  pFactory->Release();
  sphDeviceDesc desc = { 0, nullptr };
  pCurDev->GetDesc(&desc);
  printf("Device: %s\n", desc.vendorName);
  pCurDev->Release();
}

TEST(CreateCommandQueue, VULKANDevice)
{
  ISPHFactory* pFactory = CreateFactory();
  // create device
  uint32_t deviceCount = 0;
  pFactory->EnumDevice(&deviceCount, nullptr);
  ASSERT_GT(deviceCount, 0);
  ISPHDevice ** ppDevice = new ISPHDevice*[deviceCount];
  pFactory->EnumDevice(&deviceCount, ppDevice);
  ISPHDevice * pCurDev = ppDevice[0];
  ISPHCommandQueue * pCmdQueue = nullptr;
  pCurDev->CreateCommandQueue(SPH_COMMAND_QUEUE_TYPE_GRAPHICS, &pCmdQueue);
  ASSERT_TRUE(pCmdQueue != nullptr);
}

TEST(CreateAppWindow, App)
{
  App* pApp = new App("CreateAppWindow", 800, 600);
  ASSERT_TRUE(pApp->Handle() != nullptr);
  delete pApp;
}

TEST(CreateSwapchain, VULKANFactory) 
{
	ISPHFactory* pFactory = CreateFactory();
  // create device
  uint32_t deviceCount = 0;
  pFactory->EnumDevice(&deviceCount, nullptr);
  ASSERT_GT(deviceCount, 0);
  ISPHDevice ** ppDevice = new ISPHDevice*[deviceCount];
  pFactory->EnumDevice(&deviceCount, ppDevice);
  ISPHDevice * pCurDev = ppDevice[0];
  // create command queue
  ISPHCommandQueue * pCmdQueue = nullptr;
  pCurDev->CreateCommandQueue(SPH_COMMAND_QUEUE_TYPE_GRAPHICS, &pCmdQueue);
	// create swapchain
  App* pApp = new App("Swapchain", 800, 600);
	ISPHSwapChain * pSwapChain = nullptr;
  sphSwapChainDesc swapchainDesc = { SPH_PIXEL_FORMAT_RGBA8UNORM, 800, 600 };
	pFactory->CreateSwapchain(&swapchainDesc, pCmdQueue, pApp->Handle(), &pSwapChain);

  ISPHTexture * pSwapchainTexture = nullptr;
  pSwapChain->GetTexture(&pSwapchainTexture, 0);
  ASSERT_TRUE(pSwapchainTexture);

	ASSERT_EQ(pFactory->Release(), 2);
	ASSERT_TRUE(pSwapChain);
	ASSERT_EQ(pSwapChain->Release(), 0);
  ASSERT_EQ(pCmdQueue->Release(), 0);
  ASSERT_EQ(pCurDev->Release(), 0);
  delete pApp;
}

TEST(CreateRenderTarget, VULKAN)
{
  ISPHFactory* pFactory = CreateFactory();
  // create device
  uint32_t deviceCount = 0;
  pFactory->EnumDevice(&deviceCount, nullptr);
  ASSERT_GT(deviceCount, 0);
  ISPHDevice ** ppDevice = new ISPHDevice*[deviceCount];
  pFactory->EnumDevice(&deviceCount, ppDevice);
  ISPHDevice * pCurDev = ppDevice[0];
  // create command queue
  ISPHCommandQueue * pCmdQueue = nullptr;
  pCurDev->CreateCommandQueue(SPH_COMMAND_QUEUE_TYPE_GRAPHICS, &pCmdQueue);
  // create swapchain
  App* pApp = new App("Swapchain", 800, 600);
  ISPHSwapChain * pSwapChain = nullptr;
  sphSwapChainDesc swapchainDesc = { SPH_PIXEL_FORMAT_RGBA8UNORM, 800, 600 };
  pFactory->CreateSwapchain(&swapchainDesc, pCmdQueue, pApp->Handle(), &pSwapChain);
  // create render target
  ISPHRenderTarget * renderTarget = nullptr;
  sphRenderTargetDesc desc = {};
  pCurDev->CreateRenderTarget(&desc, &renderTarget);

  ASSERT_EQ(pFactory->Release(), 2);
  ASSERT_TRUE(pSwapChain);
  ASSERT_EQ(pSwapChain->Release(), 0);
  ASSERT_EQ(pCmdQueue->Release(), 0);
  ASSERT_EQ(pCurDev->Release(), 0);
  delete pApp;
}

TEST(CreateTexture, VULKAN)
{
  ISPHFactory* pFactory = CreateFactory();
  // create device
  uint32_t deviceCount = 0;
  pFactory->EnumDevice(&deviceCount, nullptr);
  ASSERT_GT(deviceCount, 0);
  ISPHDevice ** ppDevice = new ISPHDevice*[deviceCount];
  pFactory->EnumDevice(&deviceCount, ppDevice);
  ISPHDevice * pCurDev = ppDevice[0];
  // create texture
  sphTextureDesc Desc = { SPH_PIXEL_FORMAT_RGBA8UNORM, 1024, 1024, 1, 1, 1 };
  ISPHTexture* pTexture = nullptr;
  auto ret = pCurDev->CreateTexture(&Desc, &pTexture);
  ASSERT_EQ(pTexture->Release(), 0);
  ASSERT_EQ(pCurDev->Release(), 0);
  ASSERT_EQ(pFactory->Release(), 0);
}

TEST(CreateTextureView, VULKAN)
{
  ISPHFactory* pFactory = CreateFactory();
  // create device
  uint32_t deviceCount = 0;
  pFactory->EnumDevice(&deviceCount, nullptr);
  ASSERT_GT(deviceCount, 0);
  ISPHDevice ** ppDevice = new ISPHDevice*[deviceCount];
  pFactory->EnumDevice(&deviceCount, ppDevice);
  ISPHDevice * pCurDev = ppDevice[0];
  // create texture
  sphTextureDesc Desc = { SPH_PIXEL_FORMAT_RGBA8UNORM, 1024, 1024, 1, 1, 1 };
  ISPHTexture* pTexture = nullptr;
  auto ret = pCurDev->CreateTexture(&Desc, &pTexture);

  sphTextureViewDesc ViewDesc = {};
  ISPHTextureView * pTextureView = nullptr;
  auto vRet = pCurDev->CreateTextureView(&ViewDesc, pTexture, &pTextureView);
  pTextureView->Release();

  ASSERT_EQ(pTexture->Release(), 0);
  ASSERT_EQ(pCurDev->Release(), 0);
  ASSERT_EQ(pFactory->Release(), 0);
}

TEST(CreateRenderPass, VULKAN)
{
  ISPHFactory* pFactory = CreateFactory();
  // create device
  uint32_t deviceCount = 0;
  pFactory->EnumDevice(&deviceCount, nullptr);
  ASSERT_GT(deviceCount, 0);
  ISPHDevice ** ppDevice = new ISPHDevice*[deviceCount];
  pFactory->EnumDevice(&deviceCount, ppDevice);
  ISPHDevice * pCurDev = ppDevice[0];
  // create texture
  sphTextureDesc TextureDesc = { SPH_PIXEL_FORMAT_RGBA8UNORM, 1024, 1024, 1, 1, 1 };
  ISPHTexture* pTexture = nullptr;
  auto ret = pCurDev->CreateTexture(&TextureDesc, &pTexture);

  sphTextureViewDesc ViewDesc = {};
  ISPHTextureView * pTextureView = nullptr;
  auto vRet = pCurDev->CreateTextureView(&ViewDesc, pTexture, &pTextureView);
  pTextureView->Release();

  sphRenderPassDesc RenderPassDesc = {};
  ISPHRenderPass* pRenderPass = nullptr;
  pCurDev->CreateRenderPass(&RenderPassDesc, &pRenderPass);


  ASSERT_EQ(pTexture->Release(), 0);
  ASSERT_EQ(pCurDev->Release(), 0);
  ASSERT_EQ(pFactory->Release(), 0);
}

TEST(CreateRenderTarget, VULKAN)
{

}

TEST(CreatePipelineState, VULKAN)
{

}

TEST(CreateBindingGroup, VULAKN)
{

}

TEST(CommandBufferOperation, VULKAN)
{

}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}