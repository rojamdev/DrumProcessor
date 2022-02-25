/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 25 Feb 2022 6:28:59pm
    Author:  isaac

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

void CustomLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, 
	float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
	//Circle
	auto radius = ((float)jmin(width / 2, height / 2) - 4.0f) * 0.7f;
	auto centreX = (float)x + (float)width * 0.5f;
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
}

void CustomLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool, bool isButtonDown)
{
	Rectangle<int> buttonArea = button.getLocalBounds();
	g.setColour(Colours::grey);
}