
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

struct PerFrame {
    float32_t time;
    float32_t deltaTime;
};

ConstantBuffer<PerFrame> gPerFrame : register(b0);

struct ParticleNum {
    int32_t num;
};

ConstantBuffer<ParticleNum> gMaxParticleNum : register(b1);

[numthreads(1024, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID) {

    uint32_t particleIndex = DTid.x;

    if(particleIndex < gMaxParticleNum.num){
        if(gParticles[particleIndex].color.a != 0){
            gParticles[particleIndex].translate += gParticles[particleIndex].velocity;
            gParticles[particleIndex].currentTime += gPerFrame.deltaTime;
            float32_t alpha = 1.0f - (gParticles[particleIndex].currentTime / gParticles[particleIndex].lifeTime);
            gParticles[particleIndex].color.a = saturate(alpha);
        }

        if(gParticles[particleIndex].color.a == 0){
            gParticles[particleIndex].scale = float32_t2(0.0f, 0.0f);
            int32_t freeListIndex;
            InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);

            if((freeListIndex + 1) < gMaxParticleNum.num){
                gFreeList[freeListIndex + 1] = particleIndex;
            }else {
                InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
            }
        }
    }

}

