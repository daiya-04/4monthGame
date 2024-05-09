#include"EasyHeatHaze.hlsli"

struct Material {
	float32_t4 color;
};

struct HeathazeData {
	float offset;
	float roop;
	float width;
	float border;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<HeathazeData> gHeatHazeData : register(b1);
struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);

float Gaussian(float32_t2 drawUV, float32_t2 pickUV, float sigma){
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * 	sigma * sigma));
}

float smoothStep(float edge0, float edge1, float x) {
    x = saturate((x - edge0) / (edge1 - edge0));
    return x * x * (3.0 - 2.0 * x);
}

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float32_t2 transformdTexcoord = input.texcoord;

	float ty = sin(2.0f* 3.141592f * (input.texcoord.x + gHeatHazeData.offset)* gHeatHazeData.roop);
	transformdTexcoord.y += ty * gHeatHazeData.width;

	//plus
	float defLine = gHeatHazeData.border - input.texcoord.y;
	float magmaborder = 1.0f+sin(2.0f* 3.141592f * (input.texcoord.x + gHeatHazeData.offset)* gHeatHazeData.roop) * gHeatHazeData.width*3.0f;
	if(/*0<defLine &&*/ defLine < magmaborder){
		transformdTexcoord.x += ty * gHeatHazeData.width * 3.0f*(magmaborder-defLine);
	}

	output.color = gTexture.Sample(gSampler, transformdTexcoord);
	
	float totalWeight = 0, sigma = 0.003, stepWidth = 0.001;
	float32_t4 sampleColor = float32_t4(0.0, 0.0, 0.0, 0.0);

	for(float py = -sigma * 2; py <= sigma * 2; py += stepWidth){
		for(float px = -sigma * 2; px <= sigma * 2; px += stepWidth){
			float32_t2 pickUV = transformdTexcoord + float32_t2(px, py);
			float weight = Gaussian(transformdTexcoord, pickUV, sigma);
			sampleColor += gTexture.Sample(gSampler, pickUV) * weight;
			totalWeight += weight;
		}
	}

	sampleColor /= totalWeight;

	output.color.rgb = sampleColor.rgb * 2;
	if(/*0<defLine &&*/ defLine < magmaborder){
		//output.color.r -= 0.1f*(magmaborder-defLine);
		output.color.g -= 0.2f*(magmaborder-defLine);
		output.color.b -= 0.2f*(magmaborder-defLine);
	}
	output.color.a = 1.0f;
	return output;
}