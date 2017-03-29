#include "implement_vk.h"
#include "implement_js_binding.h"
#include "gtest/gtest.h"

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

TEST(CreateSwapchain, VULKANFactory) 
{
	ISPHFactory* pFactory = CreateFactory();
	// create swapchain
	ISPHSwapChain * pSwapChain = nullptr;
	pFactory->CreateSwapchain(nullptr, nullptr, nullptr, &pSwapChain);
	ASSERT_EQ(pFactory->Release(), 1);
	ASSERT_TRUE(pSwapChain);
	ASSERT_EQ(pSwapChain->Release(), 0);
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
  sphDeviceDesc desc = {0, nullptr};
  pCurDev->GetDesc(&desc);
  printf("Device: %s\n", desc.vendorName);
  pCurDev->Release();
}

int myArgc;
char ** myArgv;

TEST(JS_Binding, V8)
{
  ASSERT_EQ(v8_js_binding_test(myArgc, myArgv), 0);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
  myArgc = argc;
  myArgv = argv;
	//ISPHFactory * pFactory = CreateFactory();

	//// create swapchain
	//ISPHSwapChain * pSwapChain = nullptr;
	//pFactory->CreateSwapchain(nullptr, nullptr, &pSwapChain);

	//// create device
	//uint32_t deviceCount = 0;
	//pFactory->EnumDevice(&deviceCount, nullptr);
	//ISPHDevice ** ppDevice = new ISPHDevice*[deviceCount];
	//pFactory->EnumDevice(&deviceCount, ppDevice);
	//ISPHDevice * pCurDev = ppDevice[0];

	//// create vertex buffer
	//ISPHBuffer * pVBO = nullptr;
	//pCurDev->CreateBuffer(nullptr, &pVBO);
	//ISPHBufferView * pVBV = nullptr;
	//pCurDev->CreateBufferView(nullptr, pVBO, &pVBV);

	//// create resource bindings
	//ISPHPipelineLayout * pPL = nullptr;
	//pCurDev->CreatePipelineLayout(&pPL);

	//ISPHBindingGroup * pBindingGroup = nullptr;

	//ISPHPipelineState * pPS = nullptr;
	//pCurDev->CreatePipelineState(nullptr, &pPS);

	//// create command queue
	//ISPHCommandQueue * pCmdQueue = nullptr;
	//pCurDev->CreateCommandQueue(SPH_COMMAND_QUEUE_TYPE_GRAPHICS, &pCmdQueue);

	//// obtain command buffer
	//ISPHCommandBuffer * pCmdBuf = pCmdQueue->CommandBuffer();
	//pCmdBuf->Begin();
	//pCmdBuf->BeginRenderPass(nullptr);
	//pCmdBuf->SetPipelineLayout(pPL);
	//pCmdBuf->SetPipelineState(pPS);
	//pCmdBuf->SetBindingGroup(pBindingGroup);
	//pCmdBuf->SetVertexBuffer(0, 0, pVBO);
	//pCmdBuf->DrawInstanced(nullptr);
	//pCmdBuf->EndRenderPass();
	//pCmdBuf->End();
	//
	//// command buffer submit
	//pCmdBuf->Commit();
	//pSwapChain->Present();

	//pCmdBuf->Release();
	//pCmdQueue->Release();

	//pCurDev->Release();
	//pFactory->Release();
	return RUN_ALL_TESTS();
}