/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>

//==============================================================================
DrumAudioProcessorEditor::DrumAudioProcessorEditor (DrumAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (415, 565);
	setLookAndFeel(&customLookAndFeel);
	defaultFont.setBold(true);

	//Utility
	createSlider(gainSlider, -48.0f, 24.0f, 0.0f);
	createSlider(driveSlider, 1.0f, 100.0f, 1.0f);

	createLabel(gainLabel, "Gain");
	createLabel(driveLabel, "Drive");

	//EQ
	createSlider(highpassSlider, 20.0f, 20000.0f, 20.0f);
	createSlider(lowpassSlider, 20.0f, 20000.0f, 20000.0f);
	createSlider(highShelfGainSlider, -12.0f, 12.0f, 0.0f);
	createSlider(midCutFreqSlider, 20.0f, 1000.0f, 200.0f);

	createLabel(highpassLabel, "Highpass");
	createLabel(lowpassLabel, "Lowpass");
	createLabel(highShelfGainLabel, "Air");
	createLabel(midCutFreqLabel, "Midcut");

	//Compression
	createSlider(thresholdSlider, -48.0f, 0.0f, 0.0f);
	createSlider(ratioSlider, 1.0f, 10.0f, 1.0f);
	createSlider(attackSlider, 0.01f, 500.0f, 100.0f);
	createSlider(releaseSlider, 0.01f, 2000.0f, 500.0f);

	createLabel(thresholdLabel, "Threshold");
	createLabel(ratioLabel, "Ratio");
	createLabel(attackLabel, "Attack");
	createLabel(releaseLabel, "Release");

	//Gate
	createSlider(gateThreshSlider, -100.0f, 0.0f, -100.0f);
	createSlider(gateReleaseSlider, 50.0f, 5000.0f, 1000.0f);

	createLabel(gateThreshLabel, "Thresh");
	createLabel(gateReleaseLabel, "Release");

	//Group labels
	createLabel(eqLabel, "EQ");
	createLabel(gateLabel, "GATE");
	createLabel(compLabel, "COMP");
	createLabel(utilityLabel, "GAIN");

	eqLabel.setJustificationType(Justification::verticallyCentred);
	gateLabel.setJustificationType(Justification::verticallyCentred);
	compLabel.setJustificationType(Justification::verticallyCentred);
	utilityLabel.setJustificationType(Justification::verticallyCentred);

	//TREE PARAMETERS
	driveValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "drive", driveSlider);
	
	highpassValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "highpassCutoff", highpassSlider);
	lowpassValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "lowpassCutoff", lowpassSlider);
	highShelfGainValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "highShelfGain", highShelfGainSlider);
	midCutFreqValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "midCutFreq", midCutFreqSlider);
	
	thresholdValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "threshold", thresholdSlider);
	ratioValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "ratio", ratioSlider);
	attackValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "attack", attackSlider);
	releaseValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "release", releaseSlider);

	gateThreshValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "gateThresh", gateThreshSlider);
	gateReleaseValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "gateRelease", gateReleaseSlider);

	//Buttons
	midCutButton.setToggleState(true, NotificationType::dontSendNotification);
	midCutButton.addListener(this);
	addAndMakeVisible(midCutButton);
}

DrumAudioProcessorEditor::~DrumAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
}

//MY FUNCTIONS
void DrumAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
	if (slider == &gainSlider) processor.rawVolume = pow(10, gainSlider.getValue() / 20);
}

void DrumAudioProcessorEditor::createSlider(Slider& slider, float min, float max, float value)
{
	slider.setSliderStyle(Slider::SliderStyle::Rotary);
	slider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 15);
	slider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
	slider.setRange(min, max, processor.sliderInterval);
	slider.setValue(value);
	slider.addListener(this);
	addAndMakeVisible(&slider);
}

void DrumAudioProcessorEditor::setSliderColours(Slider& slider, Colour mainColour, Colour outlineColour, Colour fillColour)
{
	slider.setColour(Slider::thumbColourId, mainColour);
	slider.setColour(Slider::rotarySliderOutlineColourId, outlineColour);
	slider.setColour(Slider::rotarySliderFillColourId, fillColour);
}

void DrumAudioProcessorEditor::createLabel(Label& label, String text)
{
	addAndMakeVisible(label);
	label.setFont(defaultFont);
	label.setText(text, dontSendNotification);
	label.setJustificationType(Justification::centred);
	label.setColour(Label::backgroundColourId, Colours::black);
}

void DrumAudioProcessorEditor::buttonClicked(Button* button)
{	
	if (button == &midCutButton) 
	{
		if (processor.doMidCut == false)
		{
			midCutButton.onClick = [this]() { processor.doMidCut = true; };
			midCutButton.setButtonText("l");
			//button->setColour(TextButton::buttonColourId, Colours::white);
		}
		
		if (processor.doMidCut == true)
		{
			midCutButton.onClick = [this]() { processor.doMidCut = false; };
			midCutButton.setButtonText("0");
			//button->setColour(TextButton::buttonColourId, Colours::grey);
		}
	}
}

//==============================================================================
void DrumAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

	int boxPosY = 140;

	g.fillAll(Colours::black);

	g.setColour(Colours::grey);

	g.drawRoundedRectangle(10, 10, 365, 120, 10, 2);
	g.drawRoundedRectangle(10, boxPosY + 10, 185, 120, 10, 2);
	g.drawRoundedRectangle(10, boxPosY * 2 + 10, 365, 120, 10, 2);
	g.drawRoundedRectangle(190, boxPosY * 3 + 10, 185, 120, 10, 2);
	
	/*g.fillRect(25.0f, 27.0f, dividerWidth, 1.0f);
	g.fillRect(25.0f, dividerPosY + 27, dividerWidth, 1.0f);
	g.fillRect(25.0f, dividerPosY * 2 + 27, dividerWidth, 1.0f);
	g.fillRect(25.0f, dividerPosY * 3 + 27, dividerWidth, 1.0f);*/

	/*File f = "";
	Image background = ImageFileFormat::loadFrom(f);
	g.drawImageWithin(background, 0, 0, 500, 400, RectanglePlacement::centred);*/
}

void DrumAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	const int knobSize = 90;
	const int knobPosX = 90;
	const int knobPosY = 140;
	
	const int shiftX = 5;
	const int shiftY = 10;

	const int labelX = 70;
	const int labelY = 20;

	//EQ
	highpassSlider.setBounds(0 + shiftX, shiftY, knobSize, knobSize);
	lowpassSlider.setBounds(knobPosX + shiftX, shiftY, knobSize, knobSize);
	highShelfGainSlider.setBounds(knobPosX * 2 + shiftX, shiftY, knobSize, knobSize);
	midCutFreqSlider.setBounds(knobPosX * 3 + shiftX, shiftY, knobSize, knobSize);

	highpassLabel.setBounds(10 + shiftX, knobSize + shiftY, labelX, labelY);
	lowpassLabel.setBounds(knobPosX + 10 + shiftX, knobSize + shiftY, labelX, labelY);
	highShelfGainLabel.setBounds(knobPosX * 2 + 10 + shiftX, knobSize + shiftY, labelX, labelY);
	midCutFreqLabel.setBounds(knobPosX * 3 + 10 + shiftX, knobSize + shiftY, labelX, labelY);

	midCutButton.setBounds(335, 10, 40, 40);

	//Gate
	gateThreshSlider.setBounds(0 + shiftX, knobPosY + shiftY, knobSize, knobSize);
	gateReleaseSlider.setBounds(knobPosX + shiftX, knobPosY + shiftY, knobSize, knobSize);

	gateThreshLabel.setBounds(10 + shiftX, knobPosY + shiftY + knobSize, labelX, labelY);
	gateReleaseLabel.setBounds(knobPosX + 10 + shiftX, knobPosY + shiftY + knobSize, labelX, labelY);

	//Compression
	thresholdSlider.setBounds(0 + shiftX, knobPosY * 2 + shiftY, knobSize, knobSize);
	ratioSlider.setBounds(knobPosX + shiftX, knobPosY * 2 + shiftY, knobSize, knobSize);
	attackSlider.setBounds(knobPosX * 2 + shiftX, knobPosY * 2 + shiftY, knobSize, knobSize);
	releaseSlider.setBounds(knobPosX * 3 + shiftX, knobPosY * 2 + shiftY, knobSize, knobSize);

	thresholdLabel.setBounds(10 + shiftX, knobPosY * 2 + shiftY + knobSize, labelX, labelY);
	ratioLabel.setBounds(knobPosX + 10 + shiftX, knobPosY * 2 + shiftY + knobSize, labelX, labelY);
	attackLabel.setBounds(knobPosX * 2 + 10 + shiftX, knobPosY * 2 + shiftY + knobSize, labelX, labelY);
	releaseLabel.setBounds(knobPosX * 3 + 10 + shiftX, knobPosY * 2 + shiftY + knobSize, labelX, labelY);

	//Utility
	gainSlider.setBounds(knobPosX * 3 + shiftX, knobPosY * 3 + shiftY, knobSize, knobSize);
	driveSlider.setBounds(knobPosX * 2 + shiftX, knobPosY * 3 + shiftY, knobSize, knobSize);

	gainLabel.setBounds(knobPosX * 3 + 10 + shiftX, knobPosY * 3 + shiftY + knobSize, labelX, labelY);
	driveLabel.setBounds(knobPosX * 2 + 10 + shiftX, knobPosY * 3 + shiftY + knobSize, labelX, labelY);

	//Group Labels
	eqLabel.setBounds(365.0f, 60.0f, 50, 20);
	gateLabel.setBounds(180.0f, 60.0f + knobPosY , 50, 20);
	compLabel.setBounds(360.0f, 60.0f + knobPosY * 2, 50, 20);
	utilityLabel.setBounds(360.0f, 60.0f + knobPosY * 3, 50, 20);
}

