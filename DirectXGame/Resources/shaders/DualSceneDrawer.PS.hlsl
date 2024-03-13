#include"NoneEffect.hlsli"

struct Material {
	float32_t4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexturePrevScene : register(t0);
Texture2D<float32_t4> gTextureNextScene : register(t1);
Texture2D<float32_t4> gTextureWeight : register(t2);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	output.color = gTexturePrevScene.Sample(gSampler, input.texcoord) * (1.0f - gTextureWeight.Sample(gSampler, input.texcoord).r) +
		gTextureNextScene.Sample(gSampler, input.texcoord) * (gTextureWeight.Sample(gSampler, input.texcoord).r);
	output.color.a = 1.0f;
	return output;
}