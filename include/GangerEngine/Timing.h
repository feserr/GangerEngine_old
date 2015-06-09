
#pragma once

namespace GangerEngine
{
    class FpsLimiter
    {
    public:
        FpsLimiter();
        
        void Init(float maxFPS);

        void SetTargetFPS(float maxFPS) { _maxFPS = maxFPS; }

        void Begin();
        float End();

    private:
        void CalculateFPS();

        float _fps;
        float _maxFPS;
        float _frameTime;
        unsigned int _startTicks;
    };
}