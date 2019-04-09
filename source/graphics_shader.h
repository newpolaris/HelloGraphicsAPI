#pragma once

#include "graphics_types.h"

namespace el {

    class GraphicsShaderDesc final
    {
    public:

        GraphicsShaderDesc();

        void setStageFlag(GraphicsShaderStageFlagBits stage);
        GraphicsShaderStageFlagBits getStageFlag() const;

        void setShaderCode(const char* code);
        void setShaderCodeSize(size_t length);

        const char* getShaderCode() const;
        size_t getShaderCodeSize() const;

    private:

        GraphicsShaderStageFlagBits _stage;
        const char* _shaderCode;
        size_t _shaderCodeSize;
    };

    class GraphicsShader
    {
    public:

        GraphicsShader();
        virtual ~GraphicsShader();

        virtual const GraphicsShaderDesc& getDesc() const = 0;

    private:

        GraphicsShader(const GraphicsShader&) = delete;
        GraphicsShader& operator=(const GraphicsShader&) = delete;
    };

} // namespace el {
