/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Simple_TunerAudioProcessor::Simple_TunerAudioProcessor()
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
}

Simple_TunerAudioProcessor::~Simple_TunerAudioProcessor()
{
}

//==============================================================================
const juce::String Simple_TunerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Simple_TunerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Simple_TunerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Simple_TunerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Simple_TunerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Simple_TunerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Simple_TunerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Simple_TunerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Simple_TunerAudioProcessor::getProgramName (int index)
{
    return {};
}

void Simple_TunerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Simple_TunerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Simple_TunerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Simple_TunerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void Simple_TunerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    const float* channelData = buffer.getReadPointer(0);
    int numSamples = buffer.getNumSamples();

    float detectedFreq = detectPitch(channelData, numSamples, (float)getSampleRate());


    if (detectedFreq > 0.0f)
    {
        currentFrequency.store(detectedFreq);
        currentNote = freqToNote(detectedFreq);
    }

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool Simple_TunerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Simple_TunerAudioProcessor::createEditor()
{
    return new Simple_TunerAudioProcessorEditor (*this);
}

//==============================================================================
void Simple_TunerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Simple_TunerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Simple_TunerAudioProcessor();
}


juce::String Simple_TunerAudioProcessor::freqToNote(float freq) {

    if (freq < 20.0f) // Too low
    {
        return "----";
    };

    // Based on equal temperament tuning system 
    /*
    * A4 is 440hz and note number 69;
    */
    float noteNum = 12.0f * std::log2(freq / 440.0f) + 69.0f;
    int midiNote = (int)std::round(noteNum); // ex A4 is 69


    float centsOff = 100.0f * (noteNum - midiNote);

    const char* noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    int octave = (midiNote / 12) - 1;
    int noteIndex = midiNote % 12;

    return juce::String(noteNames[noteIndex]) + juce::String(octave);

}   

float Simple_TunerAudioProcessor::detectPitch(const float* audioData, int numSamples, float sampleRate)
{
    if (numSamples < 2)
    {
        return 0.0f;
    }


    int minPeriod = (int)(sampleRate / 1000.0f);
    int maxPeriod = (int)(sampleRate / 50.0f);

    float bestCorrelation = 0.0f;
    int bestLag = 0;

    for (int lag = minPeriod; lag < maxPeriod && lag < numSamples / 2; lag++)
    {
        float correlation = 0.0f;
        float normalization = 0.0f;

        for (int i = 0; i < numSamples - lag; ++i)
        {
            correlation += audioData[i] * audioData[i + lag];
            normalization += audioData[i] * audioData[i];
        }

        //Normalize:
        if (normalization > 0.0f)
        {
            correlation /= normalization;
        }

        // Keep track of lag with highest correlation:
        if (correlation > bestCorrelation)
        {
            bestCorrelation = correlation;
            bestLag = lag;
        }

    }

    if (bestLag > 0 && bestCorrelation > 0.3f)
    {
        return sampleRate / (float)bestLag;
    }

    return 0.0f;

}

float Simple_TunerAudioProcessor::getCurrentFrequency() {
    return currentFrequency.load();

}

juce::String Simple_TunerAudioProcessor::getCurrentNote()
{
    return currentNote;
}
