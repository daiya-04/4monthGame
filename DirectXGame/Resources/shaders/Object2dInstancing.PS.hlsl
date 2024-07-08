#include"Object2dInstancing.hlsli"

struct Material {
	float32_t4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t> gMaskTexture : register(t2);
SamplerState gSampler : register(s0);


PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	
	//disolve
	float32_t mask = gMaskTexture.Sample(gSampler,input.maskTexcoord.xy);
	if(mask < input.disolveValue_){
		discard;
	}
	
	//float4 transformedUV = mul(float32_t4(input.texcoord,0.0f,1.0f),gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);

	if (textureColor.a <= 0.3f) {
		discard;
	}

	output.color = gMaterial.color * textureColor * input.color;

	return output;
}