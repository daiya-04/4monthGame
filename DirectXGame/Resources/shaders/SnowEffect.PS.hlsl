#include"WaterDropScene.hlsli"

struct Material {
	float32_t4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct SlideData {
	float32_t2 slide;
};

ConstantBuffer<SlideData> gSlide : register(b1);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexturePrevScene : register(t0);
Texture2D<float32_t4> gTextureNextScene : register(t1);

SamplerState gSampler : register(s0);

float length(float32_t2 distance) {
	return sqrt(dot(distance, distance));
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	output.color = gTexturePrevScene.Sample(gSampler, input.texcoord);
	if (gTextureNextScene.Sample(gSampler, input.texcoord).a>0.5f || output.color.a <=0.1f) {
		discard;
	}
	output.color.a = 1.0f;
	return output;
}