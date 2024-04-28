#include"Object2dInstancing.hlsli"

struct InstancingGPU {
	float32_t4x4 world;
	float32_t2 texcoord[4];
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

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID,uint32_t vertexId : SV_VertexID) {
	VertexShaderOutput output;
	output.position = mul(input.position, mul(gInstancing[instanceId].world, mul(gCamera.view, gCamera.projection)));
	output.texcoord = gInstancing[instanceId].texcoord[vertexId];
	//output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.World));
	return output;
}