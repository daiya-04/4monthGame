struct VertexShaderOutput {
	float32_t4 position : SV_POSITION;
	float32_t2 texcoord : TEXCOORD0;
	float32_t2 maskTexcoord : TEXCOORD1;
	float32_t4 color : COLOR0;
	float32_t disolveValue_ : VALUE0;
};

