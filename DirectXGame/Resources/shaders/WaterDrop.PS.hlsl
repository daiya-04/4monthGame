#include"WaterDrop.hlsli"

struct Material {
	float32_t4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

float length(float32_t2 distance) {
	return sqrt(dot(distance, distance));
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	//float4 transformedUV = mul(float32_t4(input.texcoord,0.0f,1.0f),gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
	float32_t2 center = float32_t2(0.5f, 0.5f) - input.texcoord.xy;
	if (textureColor.a < 0.1f) {
		discard;
	}
	//output.color = float32_t4(input.texcoord.x, input.texcoord.y,0.0f,1.0f-length(center));
	output.color = float32_t4(input.texcoord.x, input.texcoord.y, 0.0f, 1.0f);

	return output;
}