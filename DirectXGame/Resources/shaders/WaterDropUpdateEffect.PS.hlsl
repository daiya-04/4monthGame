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
	if (rand(float32_t2(input.texcoord.x, input.texcoord.y)) > 0.8f) {
		//transformedTexcoord.y -= 0.003f;
	}
	else if (rand(float32_t2(input.texcoord.x, input.texcoord.y)) > 0.4f) {
		//transformedTexcoord.y -= 0.002f;
	}
	else {
		//transformedTexcoord.y -= 0.001f;
	}
	transformedTexcoord.y -= 0.002f;

	float32_t2 dir = input.texcoord;
	dir.x *= 100.0f;
	dir.y *= 100.0f;
	dir.x = float(int(dir.x)) / 100.0f;
	dir.y = float(int(dir.y)) / 100.0f;

	output.color = gTexture.Sample(gSampler, transformedTexcoord);
	
	if (output.color.a >= 0.45f && output.color.x >= 0.45f) {
		output.color = float32_t4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	float32_t2 left = transformedTexcoord;
	left.x -= 1.0f / 1280.0f;
	float32_t2 right = transformedTexcoord;
	right.x += 1.0f / 1280.0f;
	float32_t2 top = transformedTexcoord;
	top.y -= 1.0f / 720.0f;
	float32_t2 down = transformedTexcoord;
	down.y += 1.0f / 720.0f;

	if ((gTexture.Sample(gSampler, left ).a == 0.0f || 
		gTexture.Sample(gSampler, right).a == 0.0f ||
		gTexture.Sample(gSampler, top).a == 0.0f || 
		gTexture.Sample(gSampler, down).a == 0.0f)
		&& rand(float32_t2(dir.x, input.texcoord.y))>0.70f) {
		output.color = float32_t4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return output;
}