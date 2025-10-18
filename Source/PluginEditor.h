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
class Simple_TunerAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    Simple_TunerAudioProcessorEditor (Simple_TunerAudioProcessor&);
    ~Simple_TunerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Simple_TunerAudioProcessor& audioProcessor;

    juce::Label freqLabel;
    juce::Label noteLabel;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Simple_TunerAudioProcessorEditor)
};
