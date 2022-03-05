/*
  ==============================================================================

    Compressor.h
    Created: 28 Feb 2022 4:22:12pm
    Author:  Alex Rycroft (https://audioordeal.co.uk/how-to-build-a-vst-compressor/)

  ==============================================================================
*/

#pragma once

#include "CircularBuffer.h"

class Compressor
{
public:
    Compressor();
    float compressSample(float data, float thresh, float ratio, float attack, float release);
private:
    CircularBuffer circularBuffer;
    float compGain, tav, rms;
};