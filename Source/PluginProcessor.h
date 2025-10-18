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
class Simple_TunerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Simple_TunerAudioProcessor();
    ~Simple_TunerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override; // Pre process call: set vars/resources here.
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override; // Main processing block

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

    float getCurrentFrequency();
    juce::String getCurrentNote();

private:
    //==============================================================================
    std::atomic<float> currentFrequency{ 0.0f };
    juce::String currentNote;

    float detectPitch(const float* audioData, int numSamples, float sampleRate);
    juce::String freqToNote(float freq);





    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Simple_TunerAudioProcessor)
};
