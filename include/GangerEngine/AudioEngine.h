// AudioEngine.h

#pragma once

#include <SDL/SDL_mixer.h>

#include <string>
#include <map>

namespace GangerEngine
{
    /// Sound effect manager
    class SoundEffect
    {
    public:
        friend class AudioEngine;

        /// Plays the effect file
        /*!
         * \param loops Number of times to loop the sound
         */
        void Play(int loops = 0);

    private:
        Mix_Chunk* m_chunk = nullptr;

        int m_channel;
    };

    /// Music manager
    class Music
    {
    public:
        friend class AudioEngine;

        /// Plays the music file
        /*!
         * \param loops Number of times to loop the sound (-1 = forever)
         */
        void Play(int loops = -1);

        /// Stops whatever song is currently playing
        static void Stop();
        /// Pauses whatever song is currently playing
        static void Pause();
        /// Resumes whatever song is currently playing
        static void Resume();

    private:
        Mix_Music* m_music = nullptr;
    };

    class AudioEngine
    {
    public:
        AudioEngine();
        ~AudioEngine();

        void Init();
        void Destroy();

        SoundEffect LoadSoundEffect(const std::string& filePath);
        Music LoadMusic(const std::string& filePath);

    private:
        std::map<std::string, Mix_Chunk*> m_effectMap; ///< Effects cache
        std::map<std::string, Mix_Music*> m_musicMap; ///< Music cache

        bool m_isInitialized = false;
    };
}
