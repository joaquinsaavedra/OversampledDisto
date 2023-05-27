/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class OversampledDistortionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OversampledDistortionAudioProcessorEditor (OversampledDistortionAudioProcessor&);
    ~OversampledDistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OversampledDistortionAudioProcessor& audioProcessor;
    
    juce::ColourGradient backgroundGradient;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OversampledDistortionAudioProcessorEditor)
};
