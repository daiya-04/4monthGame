#include"Particle.hlsli"

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t1);
SamplerState gSampler : register(s0);


PixelShaderOutput main(GeometryShaderOutput input){
	PixelShaderOutput output;
	//float4 transformedUV = mul(float32_t4(input.texcoord,0.0f,1.0f),gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    output.color = gMaterial.color * textureColor * input.color;

	if(output.color.a == 0.0){
		discard;
	}

	//output.color = gMaterial.color * textureColor;

	return output;
}