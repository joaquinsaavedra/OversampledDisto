/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OversampledDistortionAudioProcessorEditor::OversampledDistortionAudioProcessorEditor (OversampledDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (500, 500);
    setResizable(true, true);
}

OversampledDistortionAudioProcessorEditor::~OversampledDistortionAudioProcessorEditor()
{
    
}

//==============================================================================
void OversampledDistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background gradient
    juce::Rectangle <int> const bounds = getLocalBounds();
    g.setGradientFill(backgroundGradient);
    g.fillRect(bounds);
    
    // Title
    g.setColour (juce::Colours::white);
    g.setFont (getHeight()/12);
    g.drawFittedText ("Distortion", getLocalBounds(), juce::Justification::centredTop, 1);
    
    
}


void OversampledDistortionAudioProcessorEditor::resized()
{
    // Gradient colors
    auto color1 = juce::Colour(194, 109, 222); // pink
    auto color2 = juce::Colour(147, 238, 229); // sky blue
    backgroundGradient = juce::ColourGradient(color1, 0, 0, color2, this->getRight(), this->getBottom(), false);
    backgroundGradient.addColour(0.3f, color1);
}
