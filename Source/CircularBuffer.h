/*
  ==============================================================================

    CircularBuffer.h
    Created: 3 Jul 2020 4:08:59pm
    Author:  https://github.com/aRycroft/JuceTutorial5

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CircularBuffer {

public:
	CircularBuffer();
	CircularBuffer(int bufferSize, int delayLength);	
	float getData();
	void setData(float data);
	void nextSample();

private:
	AudioSampleBuffer buffer;
	int writeIndex;
	int readIndex;
	int delayLength;
};