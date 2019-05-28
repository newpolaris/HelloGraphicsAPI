#include <memory>
#include <queue>

using namespace std;

struct DynAlloc
{
};

struct LinearAllocationPage
{
};

class LinearAllocator
{
    LinearAllocator();

    void release_retired(index)
    {
        std::unique_lock();

        while (index_last_freed != index) {
            if (_retired_pages.top().index == index) {

            }
            index_last_freed = (index_last_freed + 1) % index_max;
        }
    }

    uint32_t index_last_freed;
    std::queue<LinearAllocationPage> _free_pages;
    std::queue<LinearAllocationPage> _retired_pages;
};

// fence is limited support, so just sync with scene complete semaphore

int main()
{
    // https://developer.apple.com/library/archive/documentation/3DDrawing/Conceptual/MTLBestPracticesGuide/TripleBuffering.html
    // Wait until the inflight command buffer has completed its work
    // dispatch_semaphore_wait(_frameBoundarySemaphore, DISPATH_TIME_FOREVERT);

    // Update the per-frame dynamic bufffer data
    // ...
    // 
    uint32_t index = semaphore->aquired();

    release_retired(index);

    [commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> commandBuffer) {
        // GPU work is complete
        // Signal the semaphore to start the CPU work
        semaphore->signal();
    }];


    Driver driver;
    driver.kick(false, false);
    return 0;
}


#if 0

// https://github.com/bkaradzic/bgfx/blob/cdc1886994c89f23d4ce3facff06966b71c0b6d3/src/renderer_mtl.h

void processArguments(
			  PipelineStateMtl* ps

struct Driver {

    void addCopleteHandler(mtl_callback_t _cb, void* _data)
    {
        [commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> commandBuffer) {
            _cb(data);
        }];
    }

    void commandBufferFinishCallback(void* data)
    {
        CommandQueueMtl* queue = (CommandQueueMtl*)_data;

		if (queue)
		{
			queue->m_framesSemaphore.post();
		}
    }

    void kick(bool isEndFrame, bool isWaitForFinish)
    {
        if (commandBuffer)
        {
            if (isEndFrame)
            {
                commandBuffer.addCompleteHandler(commandBufferFinishedCallback, this);
            }
        }
    }
};
#endif