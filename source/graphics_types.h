#pragma once

#include "predefine.h"
#include <memory>
#include <vector>
#include <cstdint>

namespace el {

	class GraphicsDeviceDesc;
	class GraphicsProgramDesc;
	class GraphicsShaderDesc;
	class GraphicsTextureDesc;
	class GraphicsBufferDesc;

	typedef std::shared_ptr<class GraphicsDevice> GraphicsDevicePtr;
	typedef std::shared_ptr<class GraphicsContext> GraphicsContextPtr;
	typedef std::shared_ptr<class GraphicsShader> GraphicsShaderPtr;
	typedef std::shared_ptr<class GraphicsProgram> GraphicsProgramPtr;
	typedef std::shared_ptr<class GraphicsTexture> GraphicsTexturePtr;
	typedef std::shared_ptr<class GraphicsBuffer> GraphicsBufferPtr;
	
	typedef std::vector<GraphicsShaderPtr> GraphicsShaders;

	enum GraphicsDeviceType
	{
		GraphicsDeviceTypeD3D9 = 0,
		GraphicsDeviceTypeD3D11 = 1,
		GraphicsDeviceTypeD3D12 = 2,
		GraphicsDeviceTypeOpenGL = 3,
		GraphicsDeviceTypeOpenGLCore = 4,
		GraphicsDeviceTypeOpenGLES2 = 5,
		GraphicsDeviceTypeOpenGLES3 = 6,
		GraphicsDeviceTypeOpenGLES31 = 7,
		GraphicsDeviceTypeOpenGLES32 = 8,
		GraphicsDeviceTypeVulkan = 9,
		GraphicsDeviceTypeMetal= 10,
		GraphicsDeviceTypeFirst = GraphicsDeviceTypeD3D9,
		GraphicsDeviceTypeLast = GraphicsDeviceTypeMetal,
	};

	enum GraphicsTextureDim {
		GraphicsTextureDim1D = 0,
		GraphicsTextureDim1DArray,
		GraphicsTextureDim2D,
		GraphicsTextureDim2DArray,
		GraphicsTextureDim3D,
		GraphicsTextureDimRect,
		GraphicsTextureDimRectArray,
		GraphicsTextureDimCube,
		GraphicsTextureDimCubeArray,
		GraphicsTextureDimFirst = GraphicsTextureDim1D,
		GraphicsTextureDimLast = GraphicsTextureDimCubeArray
	};

	enum GraphicsDataType
	{
		GraphicsDataTypeNone = 0,
		GraphicsDataTypeTransferSrc = 1,
		GraphicsDataTypeTransferDst = 2,
		GraphicsDataTypeUniformTexelBuffer = 3,
		GraphicsDataTypeUniformBuffer = 4,
		GraphicsDataTypeStorageTexelBuffer = 5,
		GraphicsDataTypeStorageBuffer = 6,
		GraphicsDataTypeStorageVertexBuffer = 7,
		GraphicsDataTypeStorageIndexBuffer = 8,
	};

	// https://www.khronos.org/opengl/wiki/Buffer_Object
	enum GraphicsUsageFlagBits
	{
		GraphicsUsageFlagReadBit = 0x00000001,
		GraphicsUsageFlagWriteBit = 0x00000002,
		GraphicsUsageFlagPersistentBit = 0x00000004,
		GraphicsUsageFlagCoherentBit = 0x00000008,
		GraphicsUsageFlagFlushExplicitBit = 0x00000010,
		GraphicsUsageFlagDynamicStorageBit = 0x00000020,
		GraphicsUsageFlagClientStorageBit = 0x00000040
	};

	typedef std::uint32_t GraphicsUsageFlags;

	// From vulkan spec.
	enum GraphicsShaderStageFlagBits
	{
		GraphicsShaderStageNoneBit = 0x00000000,
		GraphicsShaderStageVertexBit = 0x00000001,
		GraphicsShaderStageTessellationControlBit = 0x00000002,
		GraphicsShaderStageTessellationEvaluationBit = 0x00000004,
		GraphicsShaderStageGeometryBit = 0x00000008,
		GraphicsShaderStageFragmentBit = 0x00000010,
		GraphicsShaderStageComputeBit = 0x00000020,
		GraphicsShaderStageAllGraphics = 0x0000001F,
		GraphicsShaderStageAll = 0x7FFFFFFF,
		GraphicsShaderStageRaygenBit = 0x00000100,
		GraphicsShaderStageAnyHitBit = 0x00000200,
		GraphicsShaderStageClosestHitBit = 0x00000400,
		GraphicsShaderStageMissBit = 0x00000800,
		GraphicsShaderStageIntersectionBit = 0x00001000,
		GraphicsShaderStageCallableBit = 0x00002000,
		GraphicsShaderStageTaskBit = 0x00000040,
		GraphicsShaderStageMeshBit = 0x00000080,
		GraphicsShaderStageFlagBitsMaxEnum = 0x7FFFFFFF
	};
} // namespace el
