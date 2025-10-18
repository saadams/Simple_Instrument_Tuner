/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Simple_TunerAudioProcessorEditor::Simple_TunerAudioProcessorEditor (Simple_TunerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // setup the freq label
    freqLabel.setText("0.0 Hz", juce::dontSendNotification);
    freqLabel.setFont(juce::Font(48.0f, juce::Font::bold));
    freqLabel.setJustificationType(juce::Justification::centred);
    freqLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(freqLabel);

    // setup the note label
    noteLabel.setText("---", juce::dontSendNotification);
    noteLabel.setFont(juce::Font(72.0f, juce::Font::bold));
    noteLabel.setJustificationType(juce::Justification::centred);
    noteLabel.setColour(juce::Label::textColourId, juce::Colours::lightblue);
    addAndMakeVisible(noteLabel);


    // Start the timer to update 30 times a second.
    startTimer(33);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

Simple_TunerAudioProcessorEditor::~Simple_TunerAudioProcessorEditor()
{
}

//==============================================================================
void Simple_TunerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f));
    g.drawFittedText ("Simple Tuner", getLocalBounds(), juce::Justification::centred, 1);
}

void Simple_TunerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();

    bounds.removeFromTop(40);

    noteLabel.setBounds(bounds.removeFromTop(bounds.getHeight() / 2));
    freqLabel.setBounds(bounds);
}


void Simple_TunerAudioProcessorEditor::timerCallback()
{

    // Read the freq from proc (atomic/thread safe)
    float freq = audioProcessor.getCurrentFrequency();
    
    if (freq > 0.0f)
    {
        freqLabel.setText(juce::String(freq, 1) + "Hz", juce::dontSendNotification);
        noteLabel.setText(audioProcessor.getCurrentNote(), juce::dontSendNotification);

    }
    else
    {
        freqLabel.setText("---", juce::dontSendNotification);
        noteLabel.setText("---", juce::dontSendNotification);
    }



}
