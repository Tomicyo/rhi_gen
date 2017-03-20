#include  "implement.h"
#include "gtest/gtest.h"

namespace vulkan
{
  INXTFactory * CreateFactory()
  {
    return new FactoryImpl;
  }
}

using namespace vulkan;

TEST(CreateFactory, VULKANFactory) 
{
	INXTFactory* factory = CreateFactory();
	ASSERT_TRUE(factory);
	ASSERT_EQ(factory->Release(), 0);
}

TEST(CreateDevice, VULKANFactory) 
{
	INXTFactory* pFactory = CreateFactory();
	// create device
	uint32_t deviceCount = 0;
	pFactory->EnumDevice(&deviceCount, nullptr);
	ASSERT_GT(deviceCount, 0);
	INXTDevice ** ppDevice = new INXTDevice*[deviceCount];
	pFactory->EnumDevice(&deviceCount, ppDevice);
	INXTDevice * pCurDev = ppDevice[0];
	ASSERT_TRUE(pCurDev);
	ASSERT_EQ(pCurDev->Release(), 0);  // exchange with next call ?
	ASSERT_EQ(pFactory->Release(), 0);
}

TEST(CreateSwapchain, VULKANFactory) 
{
	INXTFactory* pFactory = CreateFactory();
	// create swapchain
	INXTSwapChain * pSwapChain = nullptr;
	pFactory->CreateSwapchain(nullptr, nullptr, &pSwapChain);
	ASSERT_EQ(pFactory->Release(), 1);
	ASSERT_TRUE(pSwapChain);
	ASSERT_EQ(pSwapChain->Release(), 0);
}

TEST(GetDesc, VULKANDevice)
{
  INXTFactory* pFactory = CreateFactory();
  // create device
  uint32_t deviceCount = 0;
  pFactory->EnumDevice(&deviceCount, nullptr);
  ASSERT_GT(deviceCount, 0);
  INXTDevice ** ppDevice = new INXTDevice*[deviceCount];
  pFactory->EnumDevice(&deviceCount, ppDevice);
  INXTDevice * pCurDev = ppDevice[0];
  pFactory->Release();
  nxtDeviceDesc desc = {0, nullptr};
  pCurDev->GetDesc(&desc);
  printf("Device: %s\n", desc.vendorName);
  pCurDev->Release();
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	//INXTFactory * pFactory = CreateFactory();

	//// create swapchain
	//INXTSwapChain * pSwapChain = nullptr;
	//pFactory->CreateSwapchain(nullptr, nullptr, &pSwapChain);

	//// create device
	//uint32_t deviceCount = 0;
	//pFactory->EnumDevice(&deviceCount, nullptr);
	//INXTDevice ** ppDevice = new INXTDevice*[deviceCount];
	//pFactory->EnumDevice(&deviceCount, ppDevice);
	//INXTDevice * pCurDev = ppDevice[0];

	//// create vertex buffer
	//INXTBuffer * pVBO = nullptr;
	//pCurDev->CreateBuffer(nullptr, &pVBO);
	//INXTBufferView * pVBV = nullptr;
	//pCurDev->CreateBufferView(nullptr, pVBO, &pVBV);

	//// create resource bindings
	//INXTPipelineLayout * pPL = nullptr;
	//pCurDev->CreatePipelineLayout(&pPL);

	//INXTBindingGroup * pBindingGroup = nullptr;

	//INXTPipelineState * pPS = nullptr;
	//pCurDev->CreatePipelineState(nullptr, &pPS);

	//// create command queue
	//INXTCommandQueue * pCmdQueue = nullptr;
	//pCurDev->CreateCommandQueue(NXT_COMMAND_QUEUE_TYPE_GRAPHICS, &pCmdQueue);

	//// obtain command buffer
	//INXTCommandBuffer * pCmdBuf = pCmdQueue->CommandBuffer();
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