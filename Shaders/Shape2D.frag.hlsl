#define SHADER_FRAGMENT
#include "ZeronShaderUtils.h.hlsl"

Texture2D mFontTexture	: TEXTURE : register(t1);
SamplerState mFontSampler : SAMPLER : register(s2);

struct FSInput
{
	float4 Position : SV_POSITION;
	location(0) float2 UV: UV;
	location(1) float4 Color : COLOR;
};

float4 main(FSInput input) : SV_TARGET
{
	if(input.UV.x == 0 && input.UV.y == 0) {
		return input.Color;
	}
	return input.Color * mFontTexture.Sample(mFontSampler, input.UV);
}