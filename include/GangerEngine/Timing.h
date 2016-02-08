
#pragma once

namespace GangerEngine
{
    class FpsLimiter
    {
    public:
        FpsLimiter();

        void Init(float maxFPS);

        void SetTargetFPS(float maxFPS) { m_maxFPS = maxFPS; }

        void Begin();
        float End();

    private:
        void CalculateFPS();

        float m_fps;
        float m_maxFPS;
        float m_frameTime;
        unsigned int m_startTicks;
    };
}