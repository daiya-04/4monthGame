#include"Particle.hlsli"

//四角形の頂点数
static const uint32_t vertexNum = 4;

//センターからのオフセット
static const float32_t4 offsets[vertexNum] = {
    float32_t4(-0.5f,-0.5f,0.0f,0.0f),//左下
    float32_t4(-0.5f,0.5f,0.0f,0.0f),//左上
    float32_t4(0.5f,-0.5f,0.0f,0.0f),//右下
    float32_t4(0.5f,0.5f,0.0f,0.0f),//右上
}

[maxvertexcount(vertexNum)]

void main(point VertexShaderOutput input[1], inout TriangleStream<GeometryShaderOutput> output,uint32_t instanceId : SV_InstanceID){


    GeometryShaderOutput element;
    element.color = input[0].color;
    element.texcoord = input[0].texcoord;

    element.position = input[0].position;
    output.Append(element);

    element.position = input[0].position + float32_t4(0.1f,0.1f,0.0f,0.0f);
    output.Append(element);

    element.position = input[0].position + float32_t4(0.1f,0.0f,0.0f,0.0f);
    output.Append(element);

}