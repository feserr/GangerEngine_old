
#include <GangerEngine/AudioEngine.h>
#include <GangerEngine/GangerErrors.h>

namespace GangerEngine
{
    void SoundEffect::Play(int loops /* = 0 */)
    {
        m_channel = Mix_PlayChannel(-1, m_chunk, loops);
        
        if(m_channel == -1)
            if(Mix_PlayChannel(0, m_chunk, loops) == -1)
                FatalError("Mix_PlayChannel error:" + std::string(Mix_GetError()));
    }
    
    void Music::Play(int loops /* = -1 */)
    {
        Mix_PlayMusic(m_music, loops);
    }
    
    void Music::Stop()
    {
        Mix_HaltMusic();
    }
    
    void Music::Pause()
    {
        Mix_PauseMusic();
    }
    
    void Music::Resume()
    {
        Mix_ResumeMusic();
    }
    
    AudioEngine::AudioEngine()
    {
    }
    
    AudioEngine::~AudioEngine()
    {
        Destroy();
    }
    
    void AudioEngine::Init()
    {
        if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
            FatalError("Mix_init error: " + std::string(Mix_GetError()));
        
        if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
            FatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
        
        m_isInitialized = true;
    }
    
    void AudioEngine::Destroy()
    {
        if(m_isInitialized)
        {
            m_isInitialized = false;
            Mix_Quit();
        }
    }
    
    SoundEffect AudioEngine::LoadSoundEffect(const std::string& filePath)
    {
        SoundEffect effect;
        
        // Try to find the audio in cache
        auto it = m_effectMap.find(filePath);
        
        if(it == m_effectMap.end()) // if failed to find it, it must be loaded
        {
            Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

            if(chunk == nullptr) // Check for errors
                FatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
            
            effect.m_chunk = chunk;
            m_effectMap[filePath] = chunk;
        }
        else // it is alredy cached
        {
            effect.m_chunk = it->second;
        }
        
        return effect;
    }
    
    Music AudioEngine::LoadMusic(const std::string &filePath)
    {
        Music music;
        
        // Try to find the music in cache
        auto it = m_musicMap.find(filePath);
        
        if(it == m_musicMap.end()) // if failed to find it, it must be loaded
        {
            Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
        
            if(mixMusic == nullptr) // Check for errors
                FatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
            
            music.m_music = mixMusic;
            m_musicMap[filePath] = mixMusic;
        }
        else // it is alredy cached
        {
            music.m_music = it->second;
        }
        
        return music;
    }
}