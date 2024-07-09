#include"NoneEffect.hlsli"

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	output.color = gTexture.Sample(gSampler, input.texcoord);
	if (output.color.a <= 0.01f) {
		discard;
	}
	output.color.a = 1.0f;
	return output;
}