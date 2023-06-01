/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class OversampledDistortionAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    OversampledDistortionAudioProcessor();
    ~OversampledDistortionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    
    // pre gain
    juce::dsp::Gain<float> preGainProcessor;
    
    //distortion
    struct myDistoFunction
    {
        float operator()(float x)const
        {
//            return juce::jlimit(-1.0f, 1.0f, x); // use hard clip [-1,1] as clipping function
            return std::tanh(x); // use tanh as clipping function
        }
    };

    juce::dsp::WaveShaper<float, myDistoFunction> distortionProcessor;
    

    // oversampling
    size_t factor {4}; // 0 (no oversampling), 1 (x2), 2 (x4), 3 (x8), 4 (x16)

    juce::dsp::Oversampling<float> oversampler { /*numChannels*/ 2, factor, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, true, true };
    
    //
    
    friend class OversampledDistortionAudioProcessorEditor;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OversampledDistortionAudioProcessor)
};
