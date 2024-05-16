#include"CameraFrozen.hlsli"

struct Material {
	float32_t4 color;
};

struct CameraFrozenData {
	float radius;
	float border;
	float seed;
	int32_t mode;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<CameraFrozenData> gCameraFrozenData : register(b1);

float rand(float32_t2 seed) 
{
	return saturate(frac(sin(dot(seed.xy, float2(12.9898, 78.233))) * 43758.5453));
}

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

SamplerState gSampler : register(s0);


PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	
	//textureEdgeDelete
	if ((input.texcoord.x>0.001f && input.texcoord.x < 0.999f && 
		input.texcoord.y > 0.001f && input.texcoord.y < 0.999f)) {
		float32_t2 center = { 0.5f,0.5f };
		//output.color = {1.0f,1.0f,1.0f,1.0f};
		float32_t2 distance = input.texcoord - center;
		float32_t2 internalSeed = input.texcoord;

		internalSeed.x *= 1280.0f;
		internalSeed.y *= 720.0f;
		int32_t2 internalSeedInt = int32_t2(internalSeed);
		internalSeedInt *= rcp(4);
		internalSeed.x = float(internalSeedInt.x);
		internalSeed.y = float(internalSeedInt.y);
		internalSeed.x *= rcp(1280.0f);
		internalSeed.y *= rcp(720.0f);

		if (gCameraFrozenData.mode == 0) {
			output.color = float32_t4(1.0f, 1.0f, 1.0f, 1.0f);
			if (gCameraFrozenData.radius + (rand(internalSeed) * 2.0f - 1.0f) * rcp(4.0f) > sqrt(dot(distance, distance))) {
				output.color = float32_t4(0.0f, 0.0f, 0.0f, 0.0f);
				discard;
			}
			if (gCameraFrozenData.border > rand(internalSeed + gCameraFrozenData.seed)) {
				output.color = float32_t4(0.0f, 0.0f, 0.0f, 0.0f);
				discard;
			}
		}
		else {
			output.color = float32_t4(0.0f, 0.0f, 0.0f, 0.0f);
			if (gCameraFrozenData.radius + (rand(internalSeed) * 2.0f - 1.0f) * rcp(4.0f) > sqrt(dot(distance, distance))) {
				output.color = float32_t4(0.0f, 0.0f, 0.0f, 0.0f);
				discard;
			}
			if (gCameraFrozenData.border * rcp(1.25f) > rand(internalSeed + gCameraFrozenData.seed)) {
				output.color = float32_t4(0.0f, 0.0f, 0.0f, 0.0f);
				discard;
			}
		}

	}
	else {
		output.color = float32_t4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	//output.color.a = 1.0f;
	return output;
}