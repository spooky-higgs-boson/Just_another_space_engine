#ifndef FASTNOISE_H
#define FASTNOISE_H

class FastNoise {
private:
    int m_seed;
    float m_frequency;
    
public:
    FastNoise(int seed = 1337) : m_seed(seed), m_frequency(0.01f) {}
    
    void SetSeed(int seed) { m_seed = seed; }
    void SetFrequency(float frequency) { m_frequency = frequency; }
    
    float GetNoise(float x, float y, float z);
    float GetSimplexNoise(float x, float y, float z);
    
private:
    float grad(int hash, float x, float y, float z);
    float fade(float t);
    float lerp(float t, float a, float b);
    int fastfloor(float fp);
};

#endif
