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
    
    // pre gain slider
    addAndMakeVisible(preGainSlider);
    preGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    preGainSlider.setRange(0, 40);
    preGainSlider.onValueChange = [&]()
    {
        auto value_db = preGainSlider.getValue();
        audioProcessor.preGainProcessor.setGainDecibels(value_db);
    };
    preGainSlider.setValue(0);
    preGainSlider.setLookAndFeel(&sliderLNF);
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
    auto titleRec = juce::Rectangle<float> (0.1f * getWidth(), 0.05f * getHeight(), 0.8f * getWidth(), 0.1f * getHeight());
    g.drawText("EAR CANDY DISTO", titleRec, juce::Justification::centred);
    
    // Slider name
    g.setColour (juce::Colours::white);
    g.setFont (getHeight()/18);
    auto nameRec = juce::Rectangle<float> (0.1f * getWidth(), preGainSlider.getBottom(), 0.8f * getWidth(), 0.08f * getHeight());
    g.drawText("PRE GAIN", nameRec, juce::Justification::centred);
}

void OversampledDistortionAudioProcessorEditor::resized()
{
    // Gradient colors
    auto color1 = juce::Colour(194, 109, 222); // pink
    auto color2 = juce::Colour(147, 238, 229); // sky blue
    backgroundGradient = juce::ColourGradient(color1, 0, 0, color2, this->getRight(), this->getBottom(), false);
    backgroundGradient.addColour(0.3f, color1);
    
    // pre gain slider
    auto preGainSliderBounds = juce::Rectangle<float>(0.2f * getWidth(), 0.3f * getHeight(), 0.6f * getWidth(), 0.4f * getHeight());
    preGainSlider.setBounds(preGainSliderBounds.toNearestInt());
}

// ====== IMAGE SLIDER LOOK AND FEEL

ImageSliderLNF::ImageSliderLNF()
{
    image = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
}

void ImageSliderLNF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    if (image.isValid())
    {
        auto TotalAngle = rotaryEndAngle - rotaryStartAngle;
        auto angle = rotaryStartAngle + sliderPos * TotalAngle;
        auto area = juce::Rectangle<float>(x,y,width,height);
        g.addTransform(juce::AffineTransform::rotation(angle, area.getCentreX(), area.getCentreY()));
        g.drawImage(image,area,juce::RectanglePlacement::centred);
    }
}
