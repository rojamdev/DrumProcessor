/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 11 Jul 2020 11:03:30pm
    Author:  isaac

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomLookAndFeel : public LookAndFeel_V4
{
public:
	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, 
		float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

	void drawButtonBackground(Graphics& g, Button& button, 
		const Colour& backgroundColour, bool isHighlighted, bool isButtonDown) override;
};