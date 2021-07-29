/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DrumAudioProcessor::DrumAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
	:
#endif
	tree(*this, nullptr), 
		highpassFilter(dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, 20.0f, 0.02f)), 
		lowpassFilter(dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, 20000.0f, 0.02f)),
		highShelf(dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, 5000.0f, 0.02f, 10.0f)),
		midCut(dsp::IIR::Coefficients<float>::makeNotch(lastSampleRate, 250.0f, 8.0f)),
		noiseGate()
{
	//UTILITY
	NormalisableRange<float> driveRange(1.0f, 100.0f, sliderInterval);
	tree.createAndAddParameter("drive", "Drive", "drive", driveRange, 1.0f, nullptr, nullptr);
		
	//EQ
	NormalisableRange<float> highpassCutoffRange(20.0f, 20000.0f, sliderInterval);
	highpassCutoffRange.setSkewForCentre(500.f);
	tree.createAndAddParameter("highpassCutoff", "HighpassCutoff", "highpassCutoff", highpassCutoffRange, 20.0f, nullptr, nullptr);

	NormalisableRange<float> lowpassCutoffRange(20.0f, 20000.0f, sliderInterval);
	lowpassCutoffRange.setSkewForCentre(500.f);
	tree.createAndAddParameter("lowpassCutoff", "LowpassCutoff", "lowpassCutoff", lowpassCutoffRange, 20000.0f, nullptr, nullptr);

	NormalisableRange<float> highShelfGainRange(-12.0f, 12.0f, sliderInterval);
	tree.createAndAddParameter("highShelfGain", "HighShelfGain", "highShelfGain", highShelfGainRange, 0.0f, nullptr, nullptr);

	NormalisableRange<float> midCutFreqRange(20.0f, 1000.0f, sliderInterval);
	midCutFreqRange.setSkewForCentre(300.f);
	tree.createAndAddParameter("midCutFreq", "MidCutFreq", "midCutFreq", midCutFreqRange, 300.0f, nullptr, nullptr);

	//COMPRESSOR
	NormalisableRange<float> thresholdRange(-48.0f, 0.0f, sliderInterval);
	tree.createAndAddParameter("threshold", "Threshold", "threshold", thresholdRange, 0.0f, nullptr, nullptr);

	NormalisableRange<float> ratioRange(1.0f, 10.0f, sliderInterval);
	ratioRange.setSkewForCentre(3.0f);
	tree.createAndAddParameter("ratio", "Ratio", "ratio", ratioRange, 1.0f, nullptr, nullptr);

	NormalisableRange<float> attackRange(0.01f, 500.0f, sliderInterval);
	attackRange.setSkewForCentre(100.0f);
	tree.createAndAddParameter("attack", "Attack", "attack", attackRange, 100.0f, nullptr, nullptr);

	NormalisableRange<float> releaseRange(0.01f, 2000.0f, sliderInterval);
	releaseRange.setSkewForCentre(500.0f);
	tree.createAndAddParameter("release", "Release", "release", releaseRange, 500.0f, nullptr, nullptr);

	//NOISE GATE
	NormalisableRange<float> gateThreshRange(-100.0f, 0.0f, sliderInterval);
	tree.createAndAddParameter("gateThresh", "GateThreshold", "gateThresh", gateThreshRange, -100.0f, nullptr, nullptr);

	NormalisableRange<float> gateReleaseRange(50.0f, 5000.0f, sliderInterval);
	gateReleaseRange.setSkewForCentre(1000.0f);
	tree.createAndAddParameter("gateRelease", "GateRelease", "gateRelease", gateReleaseRange, 1000.0f, nullptr, nullptr);
}

DrumAudioProcessor::~DrumAudioProcessor()
{
}

//==============================================================================
const String DrumAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DrumAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DrumAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DrumAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DrumAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DrumAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DrumAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DrumAudioProcessor::setCurrentProgram (int index)
{
}

const String DrumAudioProcessor::getProgramName (int index)
{
    return {};
}

void DrumAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DrumAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	lastSampleRate = sampleRate;
	rawVolume = 1.0f;

	//Compressor
	circularBuffer = CircularBuffer(150, 20);
	compGain = 1.0f;
	tav = 0.01f;
	rms = 0.0f;

	//DSP
	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getMainBusNumOutputChannels();

	//EQ
	highpassFilter.prepare(spec);
	highpassFilter.reset();

	lowpassFilter.prepare(spec);
	lowpassFilter.reset();

	highShelf.prepare(spec);
	highShelf.reset();

	midCut.prepare(spec);
	midCut.reset();

	//Noise Gate
	noiseGate.prepare(spec);
	noiseGate.reset();
	noiseGate.setRatio(100.0f);
}

void DrumAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DrumAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DrumAudioProcessor::updateFilters() 
{	
	float highpassFreq = *tree.getRawParameterValue("highpassCutoff");
	float lowpassFreq = *tree.getRawParameterValue("lowpassCutoff");
	float highShelfGain = pow(10, *tree.getRawParameterValue("highShelfGain") / 20);
	float midCutFreq = *tree.getRawParameterValue("midCutFreq");

	*highpassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, highpassFreq);
	*lowpassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, lowpassFreq);
	
	*highShelf.state = *dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, 5000.0f, 1.0f, highShelfGain);
	*midCut.state = *dsp::IIR::Coefficients<float>::makeNotch(lastSampleRate, midCutFreq, 1.0f);
}

void DrumAudioProcessor::updateCompressor()
{
	threshold = *tree.getRawParameterValue("threshold");
	ratio = *tree.getRawParameterValue("ratio");
	attack = *tree.getRawParameterValue("attack");
	release = *tree.getRawParameterValue("release");
}

void DrumAudioProcessor::updateNoiseGate()
{
	noiseGate.setThreshold(*tree.getRawParameterValue("gateThresh"));
	noiseGate.setRelease(*tree.getRawParameterValue("gateRelease"));
}

float DrumAudioProcessor::compressSample(float data, float thresh, float ratio, float attack, float release)
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

void DrumAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	dsp::AudioBlock<float> block(buffer);

	updateFilters();

	highpassFilter.process(dsp::ProcessContextReplacing<float>(block));
	lowpassFilter.process(dsp::ProcessContextReplacing<float>(block));
	highShelf.process(dsp::ProcessContextReplacing<float>(block));
	if (doMidCut == true) midCut.process(dsp::ProcessContextReplacing<float>(block));

	updateCompressor();

	float attackCoeff = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (attack / 1000.0f));
	float releaseCoeff = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (release / 1000.0f));

	drive = *tree.getRawParameterValue("drive");

	updateNoiseGate();
	noiseGate.process(dsp::ProcessContextReplacing<float>(block));

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
	
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
	{
		for (int channel = 0; channel < totalNumInputChannels; ++channel)
		{
			auto* channelData = buffer.getWritePointer(channel);
			
			channelData[sample] = buffer.getSample(channel, sample);
			channelData[sample] = compressSample(channelData[sample], threshold, ratio, attackCoeff, releaseCoeff);
			channelData[sample] = ((2 / float_Pi) * atan(drive * channelData[sample])) / drive;

			channelData[sample] *= rawVolume;
		}
	}
}

//==============================================================================
bool DrumAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DrumAudioProcessor::createEditor()
{
	return new DrumAudioProcessorEditor (*this);
}

//==============================================================================
void DrumAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DrumAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DrumAudioProcessor();
}
