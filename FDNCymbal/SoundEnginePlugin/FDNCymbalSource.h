/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2021 Audiokinetic Inc.
*******************************************************************************/

#ifndef FDNCymbalSource_H
#define FDNCymbalSource_H

#include "FDNCymbalSourceParams.h"
#include "source/source/dsp/dspcore.hpp"
#include "source/source/dsp/constants.hpp"
#include "source/source/dsp/delay.hpp"
#include "source/source/dsp/envelope.hpp"
#include "source/source/dsp/iir.hpp"
#include "source/source/dsp/oscillator.hpp"

#include <array>
#include <cmath>
#include <memory>
#include <vector>

#include <AK/Plugin/PluginServices/AkFXDurationHandler.h>

/// See https://www.audiokinetic.com/library/edge/?source=SDK&id=soundengine__plugins__source.html
/// for the documentation about source plug-ins
/// 
using namespace SomeDSP;

constexpr size_t fdnMatrixSize = 12;
constexpr size_t nAP1 = 8;
constexpr size_t nAP2 = 8;
constexpr double highpassQ = 0.01;
constexpr float tremoloDelayMaxTime = 0.001f;

class FDNCymbalSource
    : public AK::IAkSourcePlugin
{
public:
    FDNCymbalSource();
    ~FDNCymbalSource();

    /// Plug-in initialization.
    /// Prepares the plug-in for data processing, allocates memory and sets up the initial conditions.
    AKRESULT Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkSourcePluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat) override;

    /// Release the resources upon termination of the plug-in.
    AKRESULT Term(AK::IAkPluginMemAlloc* in_pAllocator) override;

    /// The reset action should perform any actions required to reinitialize the
    /// state of the plug-in to its original state (e.g. after Init() or on effect bypass).
    AKRESULT Reset() override;

    /// Plug-in information query mechanism used when the sound engine requires
    /// information about the plug-in to determine its behavior.
    AKRESULT GetPluginInfo(AkPluginInfo& out_rPluginInfo) override;

    /// Source plug-in DSP execution.
    void Execute(AkAudioBuffer* io_pBuffer) override;

    /// This method is called to determine the approximate duration (in ms) of the source.
    AkReal32 GetDuration() const override;

    void setup(float inSampleRate)
    {
        SmootherCommon<float>::setSampleRate(inSampleRate);
        SmootherCommon<float>::setTime(0.01f);


        pulsar.sampleRate = inSampleRate;

        stickEnvelope.setup(inSampleRate, 0.1f);
        for (auto& osc : stickOscillator) osc.setup(inSampleRate, 100.0f);
        velvet.setup(inSampleRate, inSampleRate * 0.004f, 0);

        for (auto& fdn : fdnCascade) fdn.setup(inSampleRate, 0.5f);

        serialAP1.setup(this->sampleRate, 0.01f);
        for (auto& ap : serialAP2) ap.setup(inSampleRate, 0.01f);
        serialAP1Highpass.setup(inSampleRate);
        serialAP2Highpass.setup(inSampleRate);

        tremoloDelay.setup(inSampleRate, tremoloDelayMaxTime, tremoloDelayMaxTime);

        reset();
        startup();
    }

    void reset()
    {

        pulsar.reset();
        stickEnvelope.terminate(m_pParams->RTPC.fStickDecay);
        for (auto& osc : stickOscillator) osc.reset();
        velvet.reset(0);

        fdnSig = 0.0f;
        const float fdnTime = m_pParams->RTPC.fFdnTime;
        for (size_t n = 0; n < fdnCascade.size(); ++n) {
            fdnCascade[n].reset();
            float diagMod = float(n + 1) / fdnCascade.size();
            float delayTimeMod = std::pow(diagMod * 2.0f, 0.8f);
            for (size_t i = 0; i < fdnMatrixSize; ++i)
                fdnCascade[n].delayTime[i].reset(delayTimeMod * fdnTime);
        }

        serialAP1Sig = 0.0f;
        serialAP1.reset();
        serialAP1Highpass.reset();

        serialAP2Sig = 0.0f;
        for (auto& ap : serialAP2) ap.reset();
        serialAP2Highpass.reset();

        tremoloDelay.reset();


        startup();
    }

    void startup()
    {
        rng.seed = m_pParams->RTPC.fSeed;
        rngStick.seed = 0;
        rngTremolo.seed = 0;

        tremoloPhase = 0.0f;
        randomTremoloDepth = 0.0f;
        randomTremoloFrequency = 0.0f;
        randomTremoloDelayTime = 0.0f;
    }

private:

    FDNCymbalSourceParams* m_pParams;
    AK::IAkPluginMemAlloc* m_pAllocator;
    AK::IAkSourcePluginContext* m_pContext;
    AkFXDurationHandler m_durationHandler;
    uint64_t lastState = 0;
    float sampleRate = 44100.0f;
    float velocity = 0;

    Random<float> rng{ 0 };
    Random<float> rngStick{ 0 };
    Random<float> rngTremolo{ 0 };
    Pulsar<float> pulsar{ 44100.0f, 0.0f };

    ExpDecay<float> stickEnvelope;
    std::array<BiquadOsc<float>, 16> stickOscillator;
    VelvetNoise<float> velvet;

    float fdnSig = 0.0f;
    std::array<FeedbackDelayNetwork<float, fdnMatrixSize>, 8> fdnCascade;

    float serialAP1Sig = 0.0f;
    SerialAllpass<float, nAP1> serialAP1;
    BiquadHighPass<double> serialAP1Highpass;

    float serialAP2Sig = 0.0f;
    std::array<SerialAllpass<float, nAP2>, 4> serialAP2;
    BiquadHighPass<double> serialAP2Highpass;

    Delay<float> tremoloDelay;
    float tremoloPhase = 0.0f;
    float randomTremoloDepth = 0.0f;
    float randomTremoloFrequency = 0.0f;
    float randomTremoloDelayTime = 0.0f;
    static const size_t maxVoice = 32;
    bool Trigger = false;
};

#endif // FDNCymbalSource_H
