// Timing.h

#pragma once

namespace GangerEngine
{
    ///Calculates FPS and also limits FPS
    class FpsLimiter
    {
    public:
        FpsLimiter();

        /// Initializes the FPS limiter. For now, this is analogous to setMaxFPS
        void Init(float maxFPS);

        /// Sets the desired max FPS
        void SetTargetFPS(float maxFPS) { m_maxFPS = maxFPS; }

        void Begin();

        /// return the current FPS as a float
        float End();

    private:
        /// Calculates the current FPS
        void CalculateFPS();

        float m_fps;
        float m_maxFPS;
        float m_frameTime;
        unsigned int m_startTicks;
    };
}
