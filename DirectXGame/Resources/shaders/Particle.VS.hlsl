#include"Particle.hlsli"

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID) {
	VertexShaderOutput output;
	//output.position = input.position;
	output.position = mul(input.position, mul(gParticle[instanceId].world, mul(gCamera.view, gCamera.projection)));
	output.texcoord = input.texcoord;
	output.color = gParticle[instanceId].color;
	return output;
}