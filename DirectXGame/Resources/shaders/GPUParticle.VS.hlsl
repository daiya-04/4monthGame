#include"Particle.hlsli"

struct Particle {
    float32_t2 translate;
    float32_t2 scale;
    float32_t lifeTime;
    float32_t2 velocity;
    float32_t currentTime;
    float32_t4 color;
};

StructuredBuffer<Particle> gParticles : register(t0);

struct PerView {
	float32_t4x4 viewProjMat;
	float32_t4x4 billboardMat;
};

ConstantBuffer<PerView> gPerView : register(b0);


struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input,uint32_t instanceId : SV_InstanceID) {
	VertexShaderOutput output;
	Particle particle = gParticles[instanceId];
	float32_t4x4 worldMat = gPerView.billboardMat;
	worldMat[0] *= particle.scale.x;
	worldMat[1] *= particle.scale.y;
	worldMat[2] *= 1.0f;
	worldMat[3].xyz = float32_t3(particle.translate.x, particle.translate.y, 0.0f);

	output.position = mul(input.position, mul(worldMat, gPerView.viewProjMat));
	output.texcoord = input.texcoord;
	output.color = particle.color;
	
	return output;
}