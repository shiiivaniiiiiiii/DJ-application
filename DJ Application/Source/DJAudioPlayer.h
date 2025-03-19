#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ReverbEffect.h"

// Class to handle playback, resampling and audio effects
class DJAudioPlayer : public AudioSource {
  public:
    // Initialises audio player
    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    // Audio samples prepare to play, play next block and release
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Load audio file
    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    
    // Control reverb effect
    void setReverbWetDry(float mix);
    void setReverbActive(bool isActive);
    void setReverbRoomSize(float size);
    void setReverbDamping(float damping);
    
    bool isReverbActive() const { return reverbEffect.getActive(); }
    float getReverbWetDry() const { return reverbEffect.getWetDryMix(); }

    // Starts and stops audio
    void start();
    void stop();

    // Relative position of playback
    double getPositionRelative();


private:
    // Audio file handling
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
    ReverbEffect reverbEffect;
    AudioBuffer<float> tempBuffer;
};





