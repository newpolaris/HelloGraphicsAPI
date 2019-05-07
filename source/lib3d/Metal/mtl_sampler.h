#ifndef __MTL_SAMPLER_H__
#define __MTL_SAMPLER_H__

#include <el_predefine.h>

#if EL_BUILD_METAL

#include <graphics_types.h>
#include <graphics_sampler.h>
#include <mtlpp.hpp>

namespace el {

    class MTLSampler final : public GraphicsSampler
    {
    public:

        MTLSampler();
        ~MTLSampler();

        bool create(const GraphicsSamplerDesc& desc);
        void destory();

        const GraphicsSamplerDesc& getSamplerDesc() const override;

    private:

        mtlpp::SamplerState _sampler;
        GraphicsSamplerDesc _samplerDesc;
    };

} // namespace el {

#endif // EL_BUILD_METAL

#endif // __MTL_SAMPLER_H__
