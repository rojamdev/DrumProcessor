#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
class DrumAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener, public Button::Listener
{
public:
    DrumAudioProcessorEditor (DrumAudioProcessor&);
    ~DrumAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	
	void sliderValueChanged(Slider *slider) override;
	void createSlider(Slider& slider, float min, float max, float value);
	void setSliderColours(Slider& slider, Colour mainColour, Colour outlineColour, Colour fillColour);
	void createLabel(Label& label, String text);
	void buttonClicked(Button* button) override;

private:

	//Utility
	Slider gainSlider, driveSlider;
	Label gainLabel, driveLabel;
	
	//EQ
	Slider highpassSlider, lowpassSlider, highShelfGainSlider, midCutFreqSlider;
	Label highpassLabel, lowpassLabel, highShelfGainLabel, midCutFreqLabel;
	TextButton midCutButton{ "O" };
	
	//Compression
	Slider thresholdSlider, ratioSlider, attackSlider, releaseSlider;

	Label thresholdLabel, ratioLabel, attackLabel, releaseLabel;

	//Noise Gate
	Slider gateThreshSlider, gateReleaseSlider;
	Label gateThreshLabel, gateReleaseLabel;

	//Group labels
	Label eqLabel, gateLabel, compLabel, utilityLabel; 

	//Tree slider attachments
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> driveValue, highpassValue, lowpassValue, highShelfGainValue, 
		midCutFreqValue, thresholdValue, ratioValue, attackValue, releaseValue, gateThreshValue, gateReleaseValue;

	CustomLookAndFeel customLookAndFeel;
	Font defaultFont;

	// This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DrumAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrumAudioProcessorEditor)
};