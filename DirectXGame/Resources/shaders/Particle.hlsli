struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
	//float32_t2 texcoord : TEXCOORD0;
	//float32_t4 color : COLOR0;
};

struct GeometryShaderOutput{
	float32_t4 position : SV_POSITION;
	float32_t2 texcoord : TEXCOORD0;
	//float32_t4 color : COLOR0;
};

struct Material {
	float32_t4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

struct ParticleGPU {
	float32_t4x4 world;
	//float32_t4 color;
};

StructuredBuffer<ParticleGPU> gParticle : register(t0);

struct Camera {
	float32_t4x4 view;
	float32_t4x4 projection;
	float32_t3 cameraPos;
};

ConstantBuffer<Camera> gCamera : register(b1);