#include"Particle.hlsli"

//四角形の頂点数
static const uint32_t vertexNum = 6;

//センターからのオフセット
/*static const float32_t4 offsets[vertexNum] = {
    float32_t4(-0.5f,-0.5f,0.0f,0.0f),//左下
    float32_t4(-0.5f,0.5f,0.0f,0.0f),//左上
    float32_t4(0.5f,-0.5f,0.0f,0.0f),//右下
    float32_t4(0.5f,0.5f,0.0f,0.0f),//右上
};

static const float32_t2 uvOffset[vertexNum] = {
    float32_t2(0.0f,1.0f),
    float32_t2(0.0f,0.0f),
    float32_t2(1.0f,1.0f),
    float32_t2(1.0f,0.0f),
};*/

[maxvertexcount(vertexNum)]

void main(triangle VertexShaderOutput input[3], inout TriangleStream<GeometryShaderOutput> output){

    /*GeometryShaderOutput element;
    for(uint32_t i = 0; i < vertexNum; i++){
        element.position = input[0].position + offsets[i];
        element.texcoord = uvOffset[i];
        output.Append(element);
    }*/

    GeometryShaderOutput element;

    for(uint32_t i = 0; i < 3; i++){
        element.position = input[i].position;
        element.texcoord = input[i].texcoord;
        element.color = input[i].color;
        output.Append(element);
    }

    //output.RestartStrip();

    /*for(uint32_t j = 0; j < 3; j++){
        element.position = input[j].position + float32_t4(0.02f,0.0f,0.0f,0.0f);
        element.texcoord = input[j].texcoord;
        element.color = input[j].color;
        output.Append(element);
    }*/
    
    //element.color = input[0].color;
}