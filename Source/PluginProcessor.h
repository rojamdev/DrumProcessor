#pragma once

#include <JuceHeader.h>
#include "Compressor.h"
#include "Constants.h"

//==============================================================================
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

    //==============================================================================
    inline float dBtoRatio(float dBvalue) { return pow(10, dBvalue / 20); }

	float lastSampleRate;
    float drive;
	double rawVolume;
	bool doMidCut = false;

    float highpassFreq, lowpassFreq, midCutFreq, highShelfGain;
    float threshold, ratio, attack, release;
    float attackCoeff, releaseCoeff;

    // Declares a processor that can be duplicated across two channels for stereo processing
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>>
        highpassFilter,
        lowpassFilter,
        highShelf,
        midCut;

    dsp::NoiseGate<float> noiseGate; // TODO: Use ProcessorDuplicator to process stereo

    Compressor compressor; // TODO: Make an array of compressors to process stereo
	
	AudioProcessorValueTreeState tree;

private:
    void updateParameters();
    
    inline float calcCompCoeff(float compControl)
    {
        return (1 - std::pow(MathConstants<float>::euler, 
            ((1 / getSampleRate()) * -2.2f) / (compControl / 1000.0f)));
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DrumAudioProcessor)
};
