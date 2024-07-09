#include"WaterDropScene.hlsli"
static const uint32_t kNumVertex = 3;

static const float32_t4 kPositions[kNumVertex] = {
	{-1.0f, 1.0f, 0.0f, 1.0f}, // 左上
	{ 3.0f, 1.0f, 0.0f, 1.0f}, // 右上
	{-1.0f,-3.0f, 0.0f, 1.0f}, // 左下
};

static const float32_t2 kTexcoords[kNumVertex] = {
	{0.0f, 0.0f},
	{2.0f, 0.0f},
	{0.0f, 2.0f},
};

VertexShaderOutput main(uint32_t vertexID : SV_VertexID) {
	VertexShaderOutput output;
	output.position = kPositions[vertexID];
	output.texcoord = kTexcoords[vertexID];
	return output;
}