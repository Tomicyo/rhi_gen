let factory = sappheiros.createFactory();
var devices = factory.enumDevices();
var device = devices[0];

var swapchain = factory.createSwapChain({
    width: 1920,
    height: 1080,
    format: sappheiros.PF_RGBA8UNorm
});

var commandQueue = device.createCommandQueue(sappheiros.COMMAND_QUEUE_TYPE_GRAPHICS);
var commandBuffer = commandQueue.commandBuffer();

commandBuffer.begin();
commandBuffer.end();
commandBuffer.commit();

