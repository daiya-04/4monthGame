#include"WaterDropScene.hlsli"

struct Material {
	float32_t4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

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
	float32_t4 sourceColor = gTextureNextScene.Sample(gSampler, input.texcoord);
	//sourceColor.x *= 2.0f;
	//sourceColor.y *= 2.0f;
	if (sourceColor.a > 0.1f) {
		float32_t2 offset;
		offset.x = sourceColor.x * 2.0f - 1.0f;
		offset.y = sourceColor.y * 2.0f - 1.0f;
		//offset.x = offset.x * offset.x * offset.x;
		//offset.y = offset.y * offset.y * offset.y;
		float32_t2 normalizedOffset = normalize(offset);
		//offset = normalize(offset);
		
		normalizedOffset.x *= 6.0f/1280.0f;
		normalizedOffset.y *= 6.0f/720.0f;
		float32_t4 afterColor = gTextureNextScene.Sample(gSampler, (input.texcoord - normalizedOffset));
		//afterColor.x *= 2.0f;
		//afterColor.y *= 2.0f;
		float32_t2 afterOffset;
		afterOffset.x = afterColor.x * 2.0f - 1.0f;
		afterOffset.y = afterColor.y * 2.0f - 1.0f;
		//if (dot(normalize(offset),normalize(afterOffset))<0.0f) {
		//	normalizedOffset*= offset/ afterOffset;
			//normalizedOffset.x *= 6.0f / 1280.0f;
			//normalizedOffset.y *= 6.0f / 720.0f;
			//normalizedOffset.y = 0;
		//}

		offset.x = input.texcoord.x - normalizedOffset.x;
		offset.y = input.texcoord.y - normalizedOffset.y;
		output.color = gTexturePrevScene.Sample(gSampler, offset);
	}

	output.color.a = 1.0f;
	return output;
}