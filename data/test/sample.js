var factory = sappheiros.createFactory();
var devices = factory.enumDevices();
var device = devices[0];

app = createApp({
    title: 'V8 Test',
    width: 1920,
    height: 1080
});

// create swap chain
var swapchain = factory.createSwapChain({
    width: 1920,
    height: 1080,
    format: sappheiros.PF_RGBA8UNorm
}, app);

// create command queue
var commandQueue = device.createCommandQueue(sappheiros.COMMAND_QUEUE_TYPE_GRAPHICS);

app.run(() => {
    // keep drawing
    var commandBuffer = commandQueue.commandBuffer();
    commandBuffer.begin();
    commandBuffer.end();
    commandBuffer.commit();
});

device.getDesc()
