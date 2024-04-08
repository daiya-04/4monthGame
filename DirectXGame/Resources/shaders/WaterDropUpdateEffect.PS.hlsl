#include"NoneEffect.hlsli"

struct Material {
	float32_t4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

float rand(float32_t2 seed)
{
	return saturate(frac(sin(dot(seed.xy, float2(12.9898, 78.233))) * 43758.5453));
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float32_t2 transformedTexcoord = input.texcoord;
	if (rand(float32_t2(input.texcoord.x, input.texcoord.y)) > 0.4f) {
		transformedTexcoord.y -= 0.002f;
	}
	else {
		transformedTexcoord.y -= 0.001f;
	}
	output.color = gTexture.Sample(gSampler, transformedTexcoord);
	float32_t2 left = transformedTexcoord;
	left.x -= 1.0f / 1280.0f;
	float32_t2 right = transformedTexcoord;
	right.x += 1.0f / 1280.0f;
	if ((gTexture.Sample(gSampler, left ).a == 0.0f || 
		gTexture.Sample(gSampler, right).a == 0.0f) && rand(float32_t2(input.texcoord.x, input.texcoord.y))>0.9f) {
		output.color = float32_t4(0.5f, 0.5f, 0.0f, 0.0f);
	}

	return output;
}