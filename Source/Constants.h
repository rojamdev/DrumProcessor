/*
  ==============================================================================

    Constants.h
    Created: 3 Mar 2022 6:03:49pm
    Author:  isaac

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// Constants that are shared across both the GUI
// and processor classes

namespace constants
{
    const float
        SLIDER_INTERVAL = 0.01f,

        GAIN_MIN = -12.0f,
        GAIN_MAX = 12.0f,
        GAIN_DEFAULT = 0.0f,

        DRIVE_MIN = 0.0f,
        DRIVE_MAX = 24.0f,
        DRIVE_DEFAULT = 0.0f,

        HPF_MIN = 20.0f,
        HPF_MAX = 20000.0f,
        HPF_DEFAULT = 20.0f,
        HPF_Q = 0.02f,
        HPF_CENTRE = 500.0f,

        LPF_MIN = 20.0f,
        LPF_MAX = 20000.0f,
        LPF_DEFAULT = 20000.0f,
        LPF_Q = 0.02f,
        LPF_CENTRE = 500.0f,

        HI_SHELF_MIN = -12.0f,
        HI_SHELF_MAX = 12.0f,
        HI_SHELF_DEFAULT = 0.0f,
        HI_SHELF_CUTOFF = 8000.0f,
        HI_SHELF_Q = 1.0f,

        MID_CUT_MIN = 200.0f,
        MID_CUT_MAX = 1000.0f,
        MID_CUT_DEFAULT = 500.0f,
        MID_CUT_Q = 1.0f,
        MID_CUT_CENTRE = 300.0f,

        COMP_THRESH_MIN = -48.0f,
        COMP_THRESH_MAX = 0.0f,
        COMP_THRESH_DEFAULT = 0.0f,

        COMP_ATTACK_MIN = 0.01f,
        COMP_ATTACK_MAX = 400.0f,
        COMP_ATTACK_DEFAULT = 20.0f,
        COMP_ATTACK_CENTRE = 20.0f,

        COMP_REL_MIN = 1.0f,
        COMP_REL_MAX = 2000.0f,
        COMP_REL_DEFAULT = 100.0f,
        COMP_REL_CENTRE = 100.0f,

        COMP_RATIO_MIN = 1.0f,
        COMP_RATIO_MAX = 10.0f,
        COMP_RATIO_DEFAULT = 1.0f,
        COMP_RATIO_CENTRE = 3.0f,

        GATE_THRESH_MIN = -100.0f,
        GATE_THRESH_MAX = 0.0f,
        GATE_THRESH_DEFAULT = -100.0f,

        GATE_REL_MIN = 1.0f,
        GATE_REL_MAX = 4000.0f,
        GATE_REL_DEFAULT = 100.0f,
        GATE_REL_CENTRE = 100.0f,

        GATE_RATIO = 100.0f;


    const juce::String
        GAIN_ID = "gain",
        DRIVE_ID = "drive",

        HPF_ID = "highpassCutoff",
        LPF_ID = "lowpassCutoff",
        HI_SHELF_ID = "highShelfGain",
        MID_CUT_ID = "midCutFreq",

        COMP_THRESH_ID = "threshold",
        COMP_RATIO_ID = "ratio",
        COMP_ATTACK_ID = "attack",
        COMP_REL_ID = "release",

        GATE_THRESH_ID = "gateThresh",
        GATE_REL_ID = "gateRelease";
}