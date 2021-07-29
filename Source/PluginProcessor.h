/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"

//==============================================================================
/**
*/
class DrumAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DrumAudioProcessor();
    ~DrumAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	
	//Functions
	float compressSample(float data, float thresh, float ratio, float attack, float release);
	
	void updateFilters();
	void updateCompressor();
	void updateNoiseGate();

	//Variables
	float lastSampleRate;
	float sliderInterval = 0.01f;
	
	double rawVolume;
	float drive;

	float threshold;
	float ratio;
	float attack;
	float release;

	bool doMidCut = false;
	
	AudioProcessorValueTreeState tree;

private:
	//Declares a processor that is duplicated across both channels so it can be mono and stereo (in this case a filter)
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> highpassFilter;
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowpassFilter;
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> highShelf;
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> midCut;

	dsp::NoiseGate<float> noiseGate;

	CircularBuffer circularBuffer;
	float compGain, tav, rms;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrumAudioProcessor)
};
