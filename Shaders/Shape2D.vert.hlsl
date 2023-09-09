#define SHADER_VERTEX
#include "ZeronShaderUtils.h.hlsl"

struct MVP {
	float4x4 Projection;
};

cbuffer mvpBuffer : register(b0) {
	MVP mvp;
};

struct VSInput
{
	location(0) float3 Pos : POSITION;
	location(1) float2 UV : UV;
	location(2) float4 Color : COLOR;
};

struct VSOutput
{
	float4 Pos : SV_POSITION;
	location(0) float2 UV : UV;
	location(1) float4 Color : COLOR;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.UV = input.UV;
	output.Color = input.Color;
	output.Pos = mul(float4(input.Pos.xyz, 1.f), mvp.Projection);
	return output;
}