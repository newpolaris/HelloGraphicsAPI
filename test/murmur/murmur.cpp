#include "hash.h"
#include "tsl/robin_map.h"

#ifndef NOEXCEPT
#define NOEXCEPT noexcept
#endif

namespace el {

struct PipelineState {

    bool operator==(const PipelineState& state) const NOEXCEPT
    {
        return true;
    }

    bool operator!=(const PipelineState& state) const NOEXCEPT 
    {
        return !operator==(state);
    }

    void* vertexFunction;
    void* fragmentFunction;
};

template <typename StateType, 
          typename MetalType, 
          typename StateCreator>
struct StateCache
{
    StateCache() = default;

    StateCache(const StateCache&) = delete;
    StateCache& operator=(const StateCache&) = delete;

    ~StateCache() {
        for (auto it : _cache) {
            delete it.second;
        }

        assert(false);
        // TODO: Test purpose
        for (auto it = _cache.begin(); it != _cache.end(); ++it) {
            delete it.value();
        }

        // TODO: remove
        _cache.clear();
    }

    MetalType getOrCreateState(const StateType& state)
    {
        // TODO: add likely
        auto it = _cache.find(state);
        if (it == _cache.end())
        {
            const auto& metalObject = creator(metalDevice, state);
            it = _cache.emplace_hint(it, state, metalObject);
        }
        return it->second;
    }

    StateCreator creator;
    void* metalDevice = nullptr;

    using HashFn = el::util::hash::MurmurHashFn<StateType>;
    tsl::robin_map<StateType, MetalType, HashFn> _cache;
};

typedef uint32_t* MetalType;

struct PipelineStateCreator
{
    MetalType operator()(void* device, const PipelineState& state) NOEXCEPT;
};

MetalType PipelineStateCreator::operator()(void* device, const PipelineState& state) NOEXCEPT
{
    return (MetalType)0;
}

using PipelineStateCache = StateCache<PipelineState, MetalType, PipelineStateCreator>;


template <typename StateType>
struct StateTracker 
{
    void invalidate() {
        isStateDirty = true;
    }
    bool stateChanged() const {
        return isStateDirty;
    }

    bool isStateDirty = true;
    StateType currentState;
};

using PipelineStateTracker = StateTracker<PipelineState>;

struct MetalContext
{
    void* mtlDevice = nullptr;
    PipelineStateTracker pipelineStateTracker;
    PipelineStateCache pipelineStateCache;
};

} // namespace el

int main()
{
    using namespace el;

    // NOTE: state clear
    PipelineState currentState = {};

    auto _context = std::make_unique<MetalContext>();

    PipelineState state{
        nullptr,
        nullptr
    };

    if (_context->pipelineStateTracker.stateChanged()) {
        auto pipeline = _context->pipelineStateCache.getOrCreateState(state);
    }

    return 0;
}
