var factory = sappheiros.createFactory();
var devices = factory.enumDevices();
var device = devices[0];

app = createApp({
    title: 'Triangle',
    width: 1920,
    height: 1080
});

// create command queue
var commandQueue = device.createCommandQueue(sappheiros.COMMAND_QUEUE_TYPE_GRAPHICS);

// create swap chain
var swapchain = factory.createSwapChain({
    width: 1920,
    height: 1080,
    format: sappheiros.PF_RGBA8UNorm
}, commandQueue, app);

//var renderTarget = device.createRenderTarget();
//var renderPass = device.createRenderPass();
//var vertexBuffer = device.createBuffer(1024);

//var pipelineState = device.createPipelineState({});
//var pipelineLayout = device.createPipelineLayout();
//var bindingGroup = device.createBindingGroup();

app.run(() => {
    // keep drawing
    var commandBuffer = commandQueue.commandBuffer();
    commandBuffer.begin();
    //commandBuffer.beginRenderPass(renderTarget, renderPass);
    //commandBuffer.setPipelineState(pipelineState);
    //commandBuffer.setPipelineLayout(pipelineLayout);
    //commandBuffer.setBindingGroup(bindingGroup);
    //commandBuffer.setVertexBuffer(0, 0, vertexBuffer);
    //commandBuffer.draw(3, 1);
    //commandBuffer.endRenderPass();
    commandBuffer.end();
    commandBuffer.commit();
});

device.getDesc()
