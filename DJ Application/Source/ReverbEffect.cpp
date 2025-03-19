#include "ReverbEffect.h"

ReverbEffect::ReverbEffect()
{
    // Initialize with default reverb settings
    params.roomSize = 0.5f;
    params.damping = 0.5f;
    params.wetLevel = 1.0f;
    params.dryLevel = 0.0f;
    params.width = 1.0f;
    params.freezeMode = 0.0f;
}

ReverbEffect::~ReverbEffect()
{
}

// Configures reverb effect
void ReverbEffect::prepare(double sampleRate, int samplesPerBlock)
{
    reverb.setSampleRate(sampleRate);
    dryBuffer.setSize(2, samplesPerBlock);
}

// Applies reverb effect
void ReverbEffect::process(juce::AudioBuffer<float>& buffer, int numSamples)
{
    if (!isActive || wetDryMix <= 0.0f)
        return;

    dryBuffer.clear();
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        dryBuffer.copyFrom(channel, 0, buffer, channel, 0, numSamples);
    
    // Process with reverb
    reverb.setParameters(params);
    reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), numSamples);
    
    // Mix dry and wet signals
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        const float* dryData = dryBuffer.getReadPointer(channel);
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Mix dry and wet signals according to wetDryMix
            channelData[sample] = (dryData[sample] * (1.0f - wetDryMix)) + (channelData[sample] * wetDryMix);
        }
    }
}

// Sets virtual room size
void ReverbEffect::setRoomSize(float size)
{
    params.roomSize = juce::jlimit(0.0f, 1.0f, size);
}

// Adjusts damping amount
void ReverbEffect::setDamping(float dampAmount)
{
    params.damping = juce::jlimit(0.0f, 1.0f, dampAmount);
}
