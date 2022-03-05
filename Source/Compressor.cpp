/*
  ==============================================================================

    Compressor.cpp
    Created: 28 Feb 2022 4:22:12pm
    Author:  Alex Rycroft (https://audioordeal.co.uk/how-to-build-a-vst-compressor/)

  ==============================================================================
*/

#include "Compressor.h"
#include "CircularBuffer.h"

Compressor::Compressor()
{
    circularBuffer = CircularBuffer(150, 20);
    compGain = 1.0f;
    tav = 0.01f;
    rms = 0.0f;
}

float Compressor::compressSample(float data, float thresh, 
	float ratio, float attack, float release)
{
	rms = (1 - tav) * rms + tav * std::pow(data, 2.0f);
	float dbRMS = 10 * std::log10(rms);

	float slope = 1 - (1 / ratio);
	float dbGain = std::min(0.0f, (slope * (thresh - dbRMS)));
	float newGain = std::pow(10, dbGain / 20);

	float coeff;
	if (newGain < compGain) coeff = attack;
	else coeff = release;
	compGain = (1 - coeff) * compGain + coeff * newGain;

	float compressedSample = compGain * circularBuffer.getData();
	circularBuffer.setData(data);
	circularBuffer.nextSample();

	return compressedSample;
}