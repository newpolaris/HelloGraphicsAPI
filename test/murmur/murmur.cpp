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
};

template <typename StateType, 
          typename MetalType, 
          typename StateCreator>
struct StateCache
{
    typedef el::util::hash::MurmurHashFn<StateType> HashFn;

     MetalType getOrCreateState(void* metalDevice, const StateType& state)
    {
        auto it = _cache.find(state);
        if (it == _cache.end()) 
        {
            StateCreator creator;
            const auto& metalObject = creator(nullptr, state);
            it = _cache.emplace_hint(it, state, metalObject);
        }
        return it->second;
    }

    tsl::robin_map<StateType, MetalType, HashFn> _cache;
};

typedef uint32_t MetalType;

struct PipelineStateCreator
{
    MetalType operator()(void* device, const PipelineState& state) NOEXCEPT;
};

typedef StateCache<PipelineState, MetalType, PipelineStateCreator> PipelineStateCache;

} // namespace el

int main()
{
    using namespace el;

    PipelineStateCache cache;

    auto pipeline = cache.getOrCreateState(nullptr, PipelineState());

    return 0;
}
