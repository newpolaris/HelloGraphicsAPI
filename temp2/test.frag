#include <metal_stdlib>
using namespace metal;

struct RasterizerData {
    float4 gl_Position [[position]];
    float3 color;
};
    
fragment half4 main0(
   RasterizerData in [[stage_in]]
{
    return half4(in.color, 1.0);
}
