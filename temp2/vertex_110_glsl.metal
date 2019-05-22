#include <metal_stdlib>
using namespace metal;
struct xlatMtlShaderInput {
  float3 vPosition [[attribute(0)]];
  float3 vNormal [[attribute(1)]];
};
struct xlatMtlShaderOutput {
  float4 gl_Position [[position]];
  float3 color;
};
struct xlatMtlShaderUniform {
  float uScale;
  float3 uTranslate;
  float4 uOrientation;
  float4x4 uProject;
};
vertex xlatMtlShaderOutput vertex_110_glsl (xlatMtlShaderInput _mtl_i [[stage_in]], constant xlatMtlShaderUniform& _mtl_u [[buffer(0)]])
{
  xlatMtlShaderOutput _mtl_o;
  _mtl_o.color = ((_mtl_i.vNormal * 0.5) + float3(0.5, 0.5, 0.5));
  float3 b_1;
  b_1 = (((_mtl_u.uOrientation.yzx * _mtl_i.vPosition.zxy) - (_mtl_u.uOrientation.zxy * _mtl_i.vPosition.yzx)) + (_mtl_u.uOrientation.w * _mtl_i.vPosition));
  float4 tmpvar_2;
  tmpvar_2.w = 1.0;
  tmpvar_2.xyz = (((_mtl_i.vPosition + 
    (2.0 * ((_mtl_u.uOrientation.yzx * b_1.zxy) - (_mtl_u.uOrientation.zxy * b_1.yzx)))
  ) * _mtl_u.uScale) + _mtl_u.uTranslate);
  _mtl_o.gl_Position = (_mtl_u.uProject * tmpvar_2);
  _mtl_o.gl_Position.z = (_mtl_o.gl_Position.z + _mtl_o.gl_Position.w) / 2.0f;
  return _mtl_o;
}

