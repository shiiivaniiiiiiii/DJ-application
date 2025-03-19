#include "DJAudioPlayer.h"

// Initialises audio player with given audio format manager
DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
: formatManager(_formatManager)
{
}
DJAudioPlayer::~DJAudioPlayer()
{
}

// Prepares to play, intialises buffers
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Prepare transport and resampling sources
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // Initialises reverb effect
    reverbEffect.prepare(sampleRate, samplesPerBlockExpected);
        tempBuffer.setSize(2, samplesPerBlockExpected);
}

// Fetch next audio block
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
    // Apply the reverb effect if active
    if (reverbEffect.getActive() && reverbEffect.getWetDryMix() > 0.0f)
    {
        tempBuffer.clear();
        for (int channel = 0; channel <bufferToFill.buffer->getNumChannels(); ++channel)
        {
            tempBuffer.copyFrom(channel, 0,
                                *bufferToFill.buffer,
                                channel,
                                bufferToFill.startSample,
                                bufferToFill.numSamples);
        }
            
        // Process temporary buffer with reverb
        reverbEffect.process(tempBuffer, bufferToFill.numSamples);
            
        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            bufferToFill.buffer->copyFrom(channel,
                                        bufferToFill.startSample,
                                        tempBuffer,
                                            channel,
                                            0,
                                            bufferToFill.numSamples);
        }
    }

}

// Release resources
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

// Loads audio file from URL
void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {       
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset (newSource.release());          
    }
}

// Set gain(volume) of audio player
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain(gain);
    }
   
}

// Adjusts playback speed of audio file
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0.1 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

// Set playback position and also as a relative value
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


// Reverb effect controls
void DJAudioPlayer::setReverbWetDry(float mix)
{
    reverbEffect.setWetDryMix(mix);
}

void DJAudioPlayer::setReverbActive(bool isActive)
{
    reverbEffect.setActive(isActive);
}

void DJAudioPlayer::setReverbRoomSize(float size)
{
    reverbEffect.setRoomSize(size);
}

void DJAudioPlayer::setReverbDamping(float damping)
{
    reverbEffect.setDamping(damping);
}

// Start audio playback
void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
