
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

struct ParticleNum {
    int32_t num;
};

ConstantBuffer<ParticleNum> gMaxParticleNum : register(b0);

[numthreads(1024, 1, 1)]
void main(uint32_t3 DTid : SV_DispatchThreadID) {
    uint32_t particleIndex = DTid.x;
    if(particleIndex < gMaxParticleNum.num) {
         gParticles[particleIndex] = (Particle)0;
         gFreeList[particleIndex] = particleIndex;
    }
    if(particleIndex == 0){
        gFreeListIndex[0] = gMaxParticleNum.num - 1;
    }
}

