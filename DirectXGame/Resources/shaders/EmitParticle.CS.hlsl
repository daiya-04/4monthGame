
float32_t rand3dTo1d(float32_t3 value, float32_t3 dotDir = float32_t3(12.9898, 78.233, 37.719)){
    //make value smaller to avoid artefacts
    float32_t3 smallValue = sin(value);
    //get scalar value from 3d vector
    float32_t random = dot(smallValue, dotDir);
    //make value more random by making it bigger and then taking teh factional part
    random = frac(sin(random) * 143758.5453);
    return random;
}

float32_t2 rand3dTo2d(float32_t3 value){
    return float32_t2(
        rand3dTo1d(value, float32_t3(12.989, 78.233, 37.719)),
        rand3dTo1d(value, float32_t3(39.346, 11.135, 83.155))
    );
}

class RandomGenerator {
    float32_t3 seed;
    float32_t2 Generate2d(){
        float32_t2 result = rand3dTo2d(seed);
        seed.x = result.x;
        seed.y = result.y;
        return result;
    }
    float32_t Generate1d(){
        float32_t result = rand3dTo1d(seed);
        seed.x = result;
        return result;
    }
};


struct Particle {
    float32_t2 translate;
    float32_t2 scale;
    float32_t lifeTime;
    float32_t2 velocity;
    float32_t currentTime;
    float32_t4 color;
};

RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int32_t> gFreeListIndex : register(u1);
RWStructuredBuffer<int32_t> gFreeList : register(u2);

struct EmitterSphere {
    float32_t2 translate;
    float32_t2 size;
    //float32_t2 direction;
    //float32_t angle;
    uint32_t count;
    float32_t frequency;
    float32_t frequencyTime;
    uint32_t emit;
};

ConstantBuffer<EmitterSphere> gEmitter : register(b0);

struct PerFrame {
    float32_t time;
    float32_t deltaTime;
};

ConstantBuffer<PerFrame> gPerFrame : register(b1);

struct ParticleNum {
    int32_t num;
};

ConstantBuffer<ParticleNum> gMaxParticleNum : register(b2);

[numthreads(1, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID) {

    RandomGenerator generator;
    generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;

    if(gEmitter.emit != 0){
        for(uint32_t countIndex = 0; countIndex < gEmitter.count; ++countIndex){
            int32_t freeListIndex;
            InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);

            if((0 <= freeListIndex) && (freeListIndex < gMaxParticleNum.num)) {
                uint32_t particleIndex = gFreeList[freeListIndex];
                //gParticles[particleIndex].translate = gEmitter.translate + (generator.Generate2d() - 0.5f) * gEmitter.size;
                gParticles[particleIndex].translate.x = gEmitter.translate.x + (generator.Generate1d() - 0.5f) * gEmitter.size.x;
                gParticles[particleIndex].translate.y = gEmitter.translate.y + (generator.Generate1d() - 0.5f) * gEmitter.size.y;
                gParticles[particleIndex].scale = float32_t2(100.0f, 100.0f);
                gParticles[particleIndex].color.rgb = float32_t3(1.0f, 1.0f, 1.0f);
                gParticles[particleIndex].color.a = 1.0f;

                gParticles[particleIndex].velocity.y = -(generator.Generate1d() * 7);
                gParticles[particleIndex].lifeTime = generator.Generate1d() * 2;
                gParticles[particleIndex].currentTime = 0.0f;
            }else {
                InterlockedAdd(gFreeListIndex[0], 1);
                break;
            }
        }
    }
}

