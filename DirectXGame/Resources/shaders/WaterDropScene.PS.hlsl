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
	
	///effect
	if (sourceColor.a > 0.1f) {
		float32_t2 offset;
		//sourceColor.x *= 1.0f * sourceColor.a;
		//sourceColor.y *= 1.0f * sourceColor.a;
		offset.x = sourceColor.x * 2.0f - 1.0f;
		offset.y = sourceColor.y * 2.0f - 1.0f;
		//offset.x *= 1.0f / sourceColor.a;
		//offset.y *= 1.0f / sourceColor.a;
		//offset.x = offset.x * offset.x * offset.x;
		//offset.y = offset.y * offset.y * offset.y;
		float32_t2 normalizedOffset = normalize(offset);
		//offset = normalize(offset);
		
		normalizedOffset.x *= 6.0f/1280.0f;
		normalizedOffset.y *= 6.0f/720.0f;
		float32_t4 afterColor = gTextureNextScene.Sample(gSampler, (input.texcoord - normalizedOffset));
		afterColor.x *= 2.0f;
		afterColor.y *= 2.0f;
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
		output.color = output.color + float32_t4(0.4f, 0.4f, 0.4f, 0.1f);
	}

	///outline
	if (gTextureNextScene.Sample(gSampler, input.texcoord).a>0.05f) {
		float32_t2 transformdR = input.texcoord;
		float32_t2 transformdL = input.texcoord;
		float32_t2 transformdU = input.texcoord;
		float32_t2 transformdD = input.texcoord;
		transformdR.x += 1.0f / 1280.0f;
		transformdL.x -= 1.0f / 1280.0f;
		transformdU.y -= 1.0f / 720.0f;
		transformdD.y += 1.0f / 720.0f;
		if (gTextureNextScene.Sample(gSampler, transformdR).a == 0.0f ||
			gTextureNextScene.Sample(gSampler, transformdL).a == 0.0f ||
			gTextureNextScene.Sample(gSampler, transformdU).a == 0.0f ||
			gTextureNextScene.Sample(gSampler, transformdD).a == 0.0f ) {
			output.color = output.color +  float32_t4(0.2f, 0.2f, 0.2f, 0.1f);
		}
	}

	output.color.a = 1.0f;
	return output;
}