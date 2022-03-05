#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace constants;

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
	highpassFilter(dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, HPF_DEFAULT, HPF_Q)), 
	lowpassFilter(dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, LPF_DEFAULT, LPF_Q)),
	highShelf(dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, HI_SHELF_CUTOFF, HI_SHELF_Q, 1.0f)), //TODO: Use dBtoRatio for 1.0f
	midCut(dsp::IIR::Coefficients<float>::makeNotch(lastSampleRate, MID_CUT_DEFAULT, MID_CUT_Q)),
	noiseGate()
{
	// UTILITY
	NormalisableRange<float>
		driveRange(DRIVE_MIN, DRIVE_MAX, SLIDER_INTERVAL),
		highpassCutoffRange(HPF_MIN, HPF_MAX, SLIDER_INTERVAL),
		lowpassCutoffRange(LPF_MIN, LPF_MAX, SLIDER_INTERVAL),
		highShelfGainRange(HI_SHELF_MIN, HI_SHELF_MAX, SLIDER_INTERVAL),
		midCutFreqRange(MID_CUT_MIN, MID_CUT_MAX, SLIDER_INTERVAL),
		thresholdRange(COMP_THRESH_MIN, COMP_THRESH_MAX, SLIDER_INTERVAL),
		ratioRange(COMP_RATIO_MIN, COMP_RATIO_MAX, SLIDER_INTERVAL),
		attackRange(COMP_ATTACK_MIN, COMP_ATTACK_MAX, SLIDER_INTERVAL),
		releaseRange(COMP_REL_MIN, COMP_REL_MAX, SLIDER_INTERVAL),
		gateThreshRange(GATE_THRESH_MIN, GATE_THRESH_MAX, SLIDER_INTERVAL),
		gateReleaseRange(GATE_REL_MIN, GATE_REL_MAX, SLIDER_INTERVAL);

	highpassCutoffRange.setSkewForCentre(HPF_CENTRE);
	lowpassCutoffRange.setSkewForCentre(LPF_CENTRE);
	midCutFreqRange.setSkewForCentre(MID_CUT_CENTRE);
	ratioRange.setSkewForCentre(COMP_RATIO_CENTRE);
	attackRange.setSkewForCentre(COMP_ATTACK_CENTRE);
	releaseRange.setSkewForCentre(COMP_REL_CENTRE);
	gateReleaseRange.setSkewForCentre(GATE_REL_CENTRE);

	tree.createAndAddParameter(DRIVE_ID, "Drive", "drive", driveRange, DRIVE_DEFAULT, nullptr, nullptr);
	tree.createAndAddParameter(HPF_ID, "HighpassCutoff", "Hz", highpassCutoffRange, HPF_DEFAULT, nullptr, nullptr);
	tree.createAndAddParameter(LPF_ID, "LowpassCutoff", "Hz", lowpassCutoffRange, LPF_DEFAULT, nullptr, nullptr);
	tree.createAndAddParameter(HI_SHELF_ID, "HighShelfGain", "dB", highShelfGainRange, HI_SHELF_DEFAULT, nullptr, nullptr);
	tree.createAndAddParameter(MID_CUT_ID, "MidCutFreq", "dB", midCutFreqRange, MID_CUT_CENTRE, nullptr, nullptr);
	tree.createAndAddParameter(COMP_THRESH_ID, "Threshold", "dB", thresholdRange, COMP_THRESH_DEFAULT, nullptr, nullptr);
	tree.createAndAddParameter(COMP_RATIO_ID, "Ratio", ":1", ratioRange, COMP_RATIO_DEFAULT, nullptr, nullptr);
	tree.createAndAddParameter(COMP_ATTACK_ID, "Attack", "ms", attackRange, COMP_ATTACK_DEFAULT, nullptr, nullptr);
	tree.createAndAddParameter(COMP_REL_ID, "Release", "ms", releaseRange, COMP_REL_DEFAULT, nullptr, nullptr);
	tree.createAndAddParameter(GATE_THRESH_ID, "GateThreshold", "dB", gateThreshRange, GATE_THRESH_DEFAULT, nullptr, nullptr);
	tree.createAndAddParameter(GATE_REL_ID, "GateRelease", "ms", gateReleaseRange, GATE_REL_DEFAULT, nullptr, nullptr);
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

	// DSP
	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getMainBusNumOutputChannels();

	// EQ
	highpassFilter.prepare(spec);
	highpassFilter.reset();
	lowpassFilter.prepare(spec);
	lowpassFilter.reset();
	highShelf.prepare(spec);
	highShelf.reset();
	midCut.prepare(spec);
	midCut.reset();

	// Noise Gate
	noiseGate.prepare(spec);
	noiseGate.reset();
	noiseGate.setRatio(GATE_RATIO);
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
	float highShelfGain = dBtoRatio(*tree.getRawParameterValue("highShelfGain"));
	float midCutFreq = *tree.getRawParameterValue("midCutFreq");

	*highpassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, highpassFreq);
	*lowpassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, lowpassFreq);
	*highShelf.state = *dsp::IIR::Coefficients<float>::makeHighShelf(lastSampleRate, HI_SHELF_CUTOFF, HI_SHELF_Q, highShelfGain);
	*midCut.state = *dsp::IIR::Coefficients<float>::makeNotch(lastSampleRate, midCutFreq, MID_CUT_Q);
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
	updateCompressor();
	updateNoiseGate();

	highpassFilter.process(dsp::ProcessContextReplacing<float>(block));
	lowpassFilter.process(dsp::ProcessContextReplacing<float>(block));
	highShelf.process(dsp::ProcessContextReplacing<float>(block));

	if (doMidCut == true) midCut.process(dsp::ProcessContextReplacing<float>(block));

	float attackCoeff = calcCompCoeff(attack);
	float releaseCoeff = calcCompCoeff(release);

	drive = *tree.getRawParameterValue("drive");
	
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
			channelData[sample] = compressor.compressSample(channelData[sample], threshold, ratio, attackCoeff, releaseCoeff);
			channelData[sample] = atan(drive * channelData[sample]) / drive;
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
