/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OversampledDistortionAudioProcessor::OversampledDistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    preGainProcessor.setGainDecibels(0.0f);
}

OversampledDistortionAudioProcessor::~OversampledDistortionAudioProcessor()
{
}

//==============================================================================
const juce::String OversampledDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OversampledDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OversampledDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OversampledDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OversampledDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OversampledDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OversampledDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OversampledDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OversampledDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void OversampledDistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void OversampledDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OversampledDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
void OversampledDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    preGainProcessor.prepare(spec);
    preGainProcessor.setRampDurationSeconds(0.1f);
    
    oversampler.initProcessing(spec.maximumBlockSize);
    
    int sampleRateMuliplier = std::pow(2, factor);
    
    juce::dsp::ProcessSpec distoSpec;
    distoSpec.sampleRate = sampleRate * sampleRateMuliplier;
    distoSpec.maximumBlockSize = samplesPerBlock;
    distoSpec.numChannels = getTotalNumInputChannels();
    
    distortionProcessor.prepare(distoSpec);
    
}

void OversampledDistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numChannels = juce::jmax (totalNumInputChannels, totalNumOutputChannels);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // block & context
    auto processingBlock = juce::dsp::AudioBlock<float> (buffer).getSubsetChannelBlock (0, (size_t) numChannels);
    auto processingContext =juce::dsp::ProcessContextReplacing<float> (processingBlock);
    
    // pre gain
    preGainProcessor.process(processingContext);
    
    // oversample
    auto oversampledBlock = oversampler.processSamplesUp(processingContext.getInputBlock());
    juce::dsp::ProcessContextReplacing<float> oversampledContext (oversampledBlock);

    // distortion
    distortionProcessor.process(oversampledContext);

    // downsample
    auto& outputBlock= processingContext.getOutputBlock();
    oversampler.processSamplesDown (outputBlock);
}

//==============================================================================
bool OversampledDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OversampledDistortionAudioProcessor::createEditor()
{
    return new OversampledDistortionAudioProcessorEditor (*this);
}

//==============================================================================
void OversampledDistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OversampledDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OversampledDistortionAudioProcessor();
}
