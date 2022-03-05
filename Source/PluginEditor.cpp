#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>

using namespace constants;

//==============================================================================
DrumAudioProcessorEditor::DrumAudioProcessorEditor (DrumAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (GUI_SIZE_X, GUI_SIZE_Y);
	setLookAndFeel(&customLookAndFeel);
	defaultFont.setBold(true);

	// Utility
	createSlider(gainSlider, GAIN_MIN, GAIN_MAX, GAIN_DEFAULT);
	createSlider(driveSlider, DRIVE_MIN, DRIVE_MAX, DRIVE_DEFAULT);

	createLabel(gainLabel, "Gain");
	createLabel(driveLabel, "Drive");

	// EQ
	createSlider(highpassSlider, HPF_MIN, HPF_MAX, HPF_DEFAULT);
	createSlider(lowpassSlider, LPF_MIN, LPF_MAX, LPF_DEFAULT);
	createSlider(highShelfGainSlider, HI_SHELF_MIN, HI_SHELF_MAX, HI_SHELF_DEFAULT);
	createSlider(midCutFreqSlider, MID_CUT_MIN, MID_CUT_MAX, MID_CUT_DEFAULT);

	createLabel(highpassLabel, "Highpass");
	createLabel(lowpassLabel, "Lowpass");
	createLabel(highShelfGainLabel, "Air");
	createLabel(midCutFreqLabel, "Midcut");

	// Compression
	createSlider(thresholdSlider, COMP_THRESH_MIN, COMP_THRESH_MAX, COMP_THRESH_DEFAULT);
	createSlider(ratioSlider, COMP_RATIO_MIN, COMP_RATIO_MAX, COMP_RATIO_DEFAULT);
	createSlider(attackSlider, COMP_ATTACK_MIN, COMP_ATTACK_MAX, COMP_ATTACK_DEFAULT);
	createSlider(releaseSlider, COMP_REL_MIN, COMP_REL_MAX, COMP_REL_DEFAULT);

	createLabel(thresholdLabel, "Thresh");
	createLabel(ratioLabel, "Ratio");
	createLabel(attackLabel, "Attack");
	createLabel(releaseLabel, "Release");

	// Gate
	createSlider(gateThreshSlider, GATE_THRESH_MIN, GATE_THRESH_MAX, GATE_THRESH_DEFAULT);
	createSlider(gateReleaseSlider, GATE_REL_MIN, GATE_REL_MAX, GATE_REL_DEFAULT);

	createLabel(gateThreshLabel, "Thresh");
	createLabel(gateReleaseLabel, "Release");

	// Group labels
	createLabel(eqLabel, "EQ");
	createLabel(gateLabel, "GATE");
	createLabel(compLabel, "COMP");
	createLabel(utilityLabel, "GAIN");

	eqLabel.setJustificationType(Justification::verticallyCentred);
	gateLabel.setJustificationType(Justification::verticallyCentred);
	compLabel.setJustificationType(Justification::verticallyCentred);
	utilityLabel.setJustificationType(Justification::verticallyCentred);

	// Tree parameters
	driveValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, DRIVE_ID, driveSlider);
	
	highpassValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, HPF_ID, highpassSlider);
	lowpassValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, LPF_ID, lowpassSlider);
	highShelfGainValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, HI_SHELF_ID, highShelfGainSlider);
	midCutFreqValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, MID_CUT_ID, midCutFreqSlider);
	
	thresholdValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, COMP_THRESH_ID, thresholdSlider);
	ratioValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, COMP_RATIO_ID, ratioSlider);
	attackValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, COMP_ATTACK_ID, attackSlider);
	releaseValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, COMP_REL_ID, releaseSlider);

	gateThreshValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, GATE_THRESH_ID, gateThreshSlider);
	gateReleaseValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, GATE_REL_ID, gateReleaseSlider);

	// Buttons
	midCutButton.setToggleState(true, NotificationType::dontSendNotification);
	midCutButton.addListener(this);
	addAndMakeVisible(midCutButton);
}

DrumAudioProcessorEditor::~DrumAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
}

// MY FUNCTIONS ================================================================
void DrumAudioProcessorEditor::sliderValueChanged(Slider *slider) 
//TODO: CHANGE THIS INTO A TREE SLIDER ATTACHMENT LIKE OTHER SLIDERS
{
	//if (slider == &gainSlider) processor.rawVolume = pow(10, gainSlider.getValue() / 20);
	if (slider == &gainSlider)
	{
		processor.rawVolume = processor.dBtoRatio(gainSlider.getValue());
	}
}

void DrumAudioProcessorEditor::createSlider(Slider& slider, float min, float max, float value)
{
	slider.setSliderStyle(Slider::SliderStyle::Rotary);
	slider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 15);
	slider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
	slider.setRange(min, max, SLIDER_INTERVAL);
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
// TODO: Fix this section
{	
	/*if (button == &midCutButton)
	{
		if (processor.doMidCut == false)
		{
			midCutButton.onClick = [this]() { processor.doMidCut = true; };
			midCutButton.setButtonText("l");
		}
		
		if (processor.doMidCut == true)
		{
			midCutButton.onClick = [this]() { processor.doMidCut = false; };
			midCutButton.setButtonText("0");
		}
	}*/

	if (button == &midCutButton)
	{
		midCutButton.onClick = [this]() { processor.doMidCut = !processor.doMidCut; };
	}
}

// TODO: Add more constants in the methods below

//==============================================================================
void DrumAudioProcessorEditor::paint (Graphics& g)
{
	const int boxPosY = 140;

	g.fillAll(Colours::black);
	g.setColour(Colours::grey);

	g.drawRoundedRectangle(10, 10, 365, 120, 10, 2);
	g.drawRoundedRectangle(10, boxPosY + 10, 185, 120, 10, 2);
	g.drawRoundedRectangle(10, boxPosY * 2 + 10, 365, 120, 10, 2);
	g.drawRoundedRectangle(190, boxPosY * 3 + 10, 185, 120, 10, 2);
}

void DrumAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	const int
		knobSize = 90,
		knobPosX = 90,
		knobPosY = 140,
	
		shiftX = 5,
		shiftY = 10,

		labelX = 70,
		labelY = 20;

	// EQ
	highpassSlider.setBounds(0 + shiftX, shiftY, knobSize, knobSize);
	lowpassSlider.setBounds(knobPosX + shiftX, shiftY, knobSize, knobSize);
	highShelfGainSlider.setBounds(knobPosX * 2 + shiftX, shiftY, knobSize, knobSize);
	midCutFreqSlider.setBounds(knobPosX * 3 + shiftX, shiftY, knobSize, knobSize);

	highpassLabel.setBounds(10 + shiftX, knobSize + shiftY, labelX, labelY);
	lowpassLabel.setBounds(knobPosX + 10 + shiftX, knobSize + shiftY, labelX, labelY);
	highShelfGainLabel.setBounds(knobPosX * 2 + 10 + shiftX, knobSize + shiftY, labelX, labelY);
	midCutFreqLabel.setBounds(knobPosX * 3 + 10 + shiftX, knobSize + shiftY, labelX, labelY);

	midCutButton.setBounds(335, 10, 40, 40);

	// Gate
	gateThreshSlider.setBounds(0 + shiftX, knobPosY + shiftY, knobSize, knobSize);
	gateReleaseSlider.setBounds(knobPosX + shiftX, knobPosY + shiftY, knobSize, knobSize);

	gateThreshLabel.setBounds(10 + shiftX, knobPosY + shiftY + knobSize, labelX, labelY);
	gateReleaseLabel.setBounds(knobPosX + 10 + shiftX, knobPosY + shiftY + knobSize, labelX, labelY);

	// Compression
	thresholdSlider.setBounds(0 + shiftX, knobPosY * 2 + shiftY, knobSize, knobSize);
	ratioSlider.setBounds(knobPosX + shiftX, knobPosY * 2 + shiftY, knobSize, knobSize);
	attackSlider.setBounds(knobPosX * 2 + shiftX, knobPosY * 2 + shiftY, knobSize, knobSize);
	releaseSlider.setBounds(knobPosX * 3 + shiftX, knobPosY * 2 + shiftY, knobSize, knobSize);

	thresholdLabel.setBounds(10 + shiftX, knobPosY * 2 + shiftY + knobSize, labelX, labelY);
	ratioLabel.setBounds(knobPosX + 10 + shiftX, knobPosY * 2 + shiftY + knobSize, labelX, labelY);
	attackLabel.setBounds(knobPosX * 2 + 10 + shiftX, knobPosY * 2 + shiftY + knobSize, labelX, labelY);
	releaseLabel.setBounds(knobPosX * 3 + 10 + shiftX, knobPosY * 2 + shiftY + knobSize, labelX, labelY);

	// Utility
	gainSlider.setBounds(knobPosX * 3 + shiftX, knobPosY * 3 + shiftY, knobSize, knobSize);
	driveSlider.setBounds(knobPosX * 2 + shiftX, knobPosY * 3 + shiftY, knobSize, knobSize);

	gainLabel.setBounds(knobPosX * 3 + 10 + shiftX, knobPosY * 3 + shiftY + knobSize, labelX, labelY);
	driveLabel.setBounds(knobPosX * 2 + 10 + shiftX, knobPosY * 3 + shiftY + knobSize, labelX, labelY);

	// Group Labels
	eqLabel.setBounds(365.0f, 60.0f, 50, 20);
	gateLabel.setBounds(180.0f, 60.0f + knobPosY , 50, 20);
	compLabel.setBounds(360.0f, 60.0f + knobPosY * 2, 50, 20);
	utilityLabel.setBounds(360.0f, 60.0f + knobPosY * 3, 50, 20);
}

