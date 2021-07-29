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
	CustomLookAndFeel() {}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
	{
		//Circle
		auto radius = ((float)jmin(width / 2, height / 2) - 4.0f) * 0.7f;
		auto centreX = (float)x + (float)width  * 0.5f;
		auto centreY = (float)y + (float)height * 0.5f;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;
		auto angle = (rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle)) - 0.2f;

		g.setColour(Colours::white);
		g.drawEllipse(rx, ry, rw, rw, 2.0f);
		
		//Dot
		Path dot;
		dot.addEllipse(0.0f, -radius * 0.8f, 6.0f, 6.0f);
		dot.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		g.fillPath(dot);

		/*File f = "";
		Image knob = ImageFileFormat::loadFrom(f);
		knob = knob.rescaled(width, height, Graphics::ResamplingQuality::highResamplingQuality);

		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		auto centreX = (float)x + (float)width  * 0.5f;
		auto centreY = (float)y + (float)height * 0.5f;

		g.drawImageWithin(knob, x, y, width, height, RectanglePlacement::centred);
		g.drawImageTransformed(knob, AffineTransform::rotation(angle));*/
	}

	void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool, bool isButtonDown) override 
	{
		Rectangle<int> buttonArea = button.getLocalBounds();
		g.setColour(Colours::grey);
		//g.drawRect(buttonArea, 2.0f);
	}
};