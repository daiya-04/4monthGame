#include"Object2dInstancing.hlsli"

struct InstancingGPU {
	float32_t4x4 world;
	float32_t2 size;
	float32_t2 texcoord[4];
	float32_t4 color;
	float32_t disolveValue_;
};

StructuredBuffer<InstancingGPU> gInstancing : register(t1);

struct Camera {
	float32_t4x4 view;
	float32_t4x4 projection;
	float32_t3 cameraPos;
};

ConstantBuffer<Camera> gCamera : register(b2);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
};

static const uint32_t kNumVertex = 4;
static const float32_t2 kMaskTexcoords[kNumVertex]={
	{0.0f,0.0f},
	{0.0f,1.0f},
	{1.0f,1.0f},
	{1.0f,0.0f}
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID,uint32_t vertexId : SV_VertexID) {
	VertexShaderOutput output;
	float32_t4 pos =  input.position;
	pos.xy = pos.xy * gInstancing[instanceId].size;
	output.position = mul(pos, mul(gInstancing[instanceId].world, mul(gCamera.view, gCamera.projection)));
	output.texcoord = gInstancing[instanceId].texcoord[vertexId];
	output.maskTexcoord = kMaskTexcoords[vertexId];
	//output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.World));
	output.color = gInstancing[instanceId].color;
	output.disolveValue_ = gInstancing[instanceId].disolveValue_;
	return output;
}