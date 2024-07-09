#include"NoneEffect.hlsli"

struct SlideData {
	float32_t2 slide;
};

ConstantBuffer<SlideData> gSlide : register(b0);


struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float32_t2 slide = gSlide.slide;
	slide.x *= rcp(1280);
	//slide.y += 1.0f;
	slide.y *= rcp(720);
	output.color = gTexture.Sample(gSampler, input.texcoord+slide);
	if (output.color.a <= 0.01f|| input.texcoord.x <= 0.001f || input.texcoord.x >= 0.999f) {
		discard;
	}
	output.color.a = 1.0f;
	return output;
}