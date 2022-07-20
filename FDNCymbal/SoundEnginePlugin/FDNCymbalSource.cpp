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

#include "FDNCymbalSource.h"
#include "../FDNCymbalConfig.h"
#include "Source/source/dsp/juce_FastMathApproximations.h"

#include <AK/AkWwiseSDKVersion.h>

inline float clamp(float value, float min, float max)
{
    return (value < min) ? min : (value > max) ? max : value;
}





AK::IAkPlugin* CreateFDNCymbalSource(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, FDNCymbalSource());
}

AK::IAkPluginParam* CreateFDNCymbalSourceParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, FDNCymbalSourceParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(FDNCymbalSource, AkPluginTypeSource, FDNCymbalConfig::CompanyID, FDNCymbalConfig::PluginID)

FDNCymbalSource::FDNCymbalSource()
    : m_pParams(nullptr)
    , m_pAllocator(nullptr)
    , m_pContext(nullptr)
{
}

FDNCymbalSource::~FDNCymbalSource()
{
}

AKRESULT FDNCymbalSource::Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkSourcePluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat)
{
    m_pParams = (FDNCymbalSourceParams*)in_pParams;
    m_pAllocator = in_pAllocator;
    m_pContext = in_pContext;

    m_durationHandler.Setup(0.1f, 0, in_rFormat.uSampleRate);
    setup(in_rFormat.uSampleRate);
    //void DSPCore::setParameters(float smooth, float ptch, float stickTone, float stickPulse, float stickVelvet, float fdnFB, float fdnCas, float allpMix, float ap1fb, float ap2fb, float tmMix, float tmdepth, float tmFreq, float tmDelay, float ap1HPC, float ap2HPC);
    return AK_Success;
}

AKRESULT FDNCymbalSource::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT FDNCymbalSource::Reset()
{
    return AK_Success;
}

AKRESULT FDNCymbalSource::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
    out_rPluginInfo.eType = AkPluginTypeSource;
    out_rPluginInfo.bIsInPlace = true;
    out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
    return AK_Success;
}

void FDNCymbalSource::Execute(AkAudioBuffer* out_pBuffer)
{
    m_durationHandler.ProduceBuffer(out_pBuffer);

    const AkUInt32 uNumChannels = out_pBuffer->NumChannels();
    AkUInt16 uFramesProduced;

    out_pBuffer->uValidFrames = out_pBuffer->MaxFrames();
    m_durationHandler.SetLooping(0);

    SmootherCommon<float>::setBufferSize(float(out_pBuffer->MaxFrames()));

    for (auto& fdn : fdnCascade)
        for (auto& time : fdn.delayTime) time.refresh();
    for (auto& ap : serialAP1.allpass) ap.delayTime.refresh();
    for (auto& section : serialAP2)
        for (auto& ap : section.allpass) ap.delayTime.refresh();

    AkReal32 m_Pitch = m_pParams->RTPC.fPitch;
    AkReal32 m_PitchBegin = m_Pitch, m_PitchEnd = 0.0f, m_PitchStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_PITCH_ID))

    {
        m_PitchEnd = m_pParams->RTPC.fPitch;
        m_PitchStep = ((m_PitchEnd - m_PitchBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32 m_Seed = m_pParams->RTPC.fSeed;

    bool m_RetriggerTime = !(m_pParams->RTPC.fRetriggerTime);
    bool m_RetriggerStick = !(m_pParams->RTPC.fRetriggerStick);
    bool m_RetriggerTremolo = !(m_pParams->RTPC.fRetriggerTremolo);


    bool m_FDN = m_pParams->RTPC.fFDN;

    AkReal32 m_FdnTime = m_pParams->RTPC.fFdnTime;
    AkReal32  m_FdnTimeBegin = m_FdnTime, m_FdnTimeEnd = 0.0f, m_FdnTimeStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_FDNTIME_ID))

    {
        m_FdnTimeEnd = m_pParams->RTPC.fFdnTime;
        m_FdnTimeStep = ((m_FdnTimeEnd - m_FdnTimeBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32 m_FdnFeedback = m_pParams->RTPC.fFdnFeedback;
    AkReal32  m_FdnFeedbackBegin = m_FdnFeedback, m_FdnFeedbackEnd = 0.0f, m_FdnFeedbackStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_FDNFEEDBACK_ID))

    {
        m_FdnFeedbackEnd = m_pParams->RTPC.fFdnFeedback;
        m_FdnFeedbackStep = ((m_FdnFeedbackEnd - m_FdnFeedbackBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32 m_FdnCascadeMix = m_pParams->RTPC.fFdnCascadeMix;
    AkReal32  m_FdnCascadeMixBegin = m_FdnCascadeMix, m_FdnCascadeMixEnd = 0.0f, m_FdnCascadeMixStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_FDNCASCADEMIX_ID))

    {
        m_FdnCascadeMixEnd = m_pParams->RTPC.fFdnCascadeMix;
        m_FdnCascadeMixStep = ((m_FdnCascadeMixEnd - m_FdnCascadeMixBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32 m_AllpassMix = m_pParams->RTPC.fAllpassMix;
    AkReal32  m_AllpassMixBegin = m_AllpassMix, m_AllpassMixEnd = 0.0f, m_AllpassMixStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_ALLPASSMIX_ID))

    {
        m_AllpassMixEnd = m_pParams->RTPC.fAllpassMix;
        m_AllpassMixStep = ((m_AllpassMixEnd - m_AllpassMixBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32 m_Allpass1Saturation = m_pParams->RTPC.fAllpass1Saturation;


    AkReal32  m_Allpass1Time = m_pParams->RTPC.fAllpass1Time;
    AkReal32  m_Allpass1TimeBegin = m_Allpass1Time, m_Allpass1TimeEnd = 0.0f, m_Allpass1TimeStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_ALLPASS1TIME_ID))

    {
        m_Allpass1TimeEnd = m_pParams->RTPC.fAllpass1Time;
        m_Allpass1TimeStep = ((m_Allpass1TimeEnd - m_Allpass1TimeBegin) / out_pBuffer->MaxFrames());
    }


    AkReal32  m_Allpass1Feedback = m_pParams->RTPC.fAllpass1Feedback;
    AkReal32  m_Allpass1FeedbackBegin = m_Allpass1Feedback, m_Allpass1FeedbackEnd = 0.0f, m_Allpass1FeedbackStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_ALLPASS1FEEDBACK_ID))

    {
        m_Allpass1FeedbackEnd = m_pParams->RTPC.fAllpass1Feedback;
        m_Allpass1FeedbackStep = ((m_Allpass1FeedbackEnd - m_Allpass1FeedbackBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_Allpass1HighpassCutoff = m_pParams->RTPC.fAllpass1HighpassCutoff;
    AkReal32  m_Allpass1HighpassCutoffBegin = m_Allpass1HighpassCutoff, m_Allpass1HighpassCutoffEnd = 0.0f, m_Allpass1HighpassCutoffStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_ALLPASS1HIGHPASSCUTOFF_ID))

    {
        m_Allpass1HighpassCutoffEnd = m_pParams->RTPC.fAllpass1HighpassCutoff;
        m_Allpass1HighpassCutoffStep = ((m_Allpass1HighpassCutoffEnd - m_Allpass1HighpassCutoffBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32 m_Allpass2Time = m_pParams->RTPC.fAllpass2Time;
    AkReal32  m_Allpass2TimeBegin = m_Allpass2Time, m_Allpass2TimeEnd = 0.0f, m_Allpass2TimeStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_ALLPASS2TIME_ID))

    {
        m_Allpass2TimeEnd = m_pParams->RTPC.fAllpass2Time;
        m_Allpass2TimeStep = ((m_Allpass2TimeEnd - m_Allpass2TimeBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32 m_Allpass2Feedback = m_pParams->RTPC.fAllpass2Feedback;
    AkReal32  m_Allpass2FeedbackBegin = m_Allpass2Feedback, m_Allpass2FeedbackEnd = 0.0f, m_Allpass2FeedbackStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_ALLPASS2FEEDBACK_ID))

    {
        m_Allpass2FeedbackEnd = m_pParams->RTPC.fAllpass2Feedback;
        m_Allpass2FeedbackStep = ((m_Allpass2FeedbackEnd - m_Allpass2FeedbackBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_Allpass2HighpassCutoff = m_pParams->RTPC.fAllpass2HighpassCutoff;
    AkReal32  m_Allpass2HighpassCutoffBegin = m_Allpass2HighpassCutoff, m_Allpass2HighpassCutoffEnd = 0.0f, m_Allpass2HighpassCutoffStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_ALLPASS2HIGHPASSCUTOFF_ID))

    {
        m_Allpass2HighpassCutoffEnd = m_pParams->RTPC.fAllpass2HighpassCutoff;
        m_Allpass2HighpassCutoffStep = ((m_Allpass2HighpassCutoffEnd - m_Allpass2HighpassCutoffBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_TremoloMix = m_pParams->RTPC.fTremoloMix;
    AkReal32  m_TremoloMixBegin = m_TremoloMix, m_TremoloMixEnd = 0.0f, m_TremoloMixStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_TREMOLOMIX_ID))

    {
        m_TremoloMixEnd = m_pParams->RTPC.fTremoloMix;
        m_TremoloMixStep = ((m_TremoloMixEnd - m_TremoloMixBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_TremoloDepth = m_pParams->RTPC.fTremoloDepth;
    AkReal32  m_TremoloDepthBegin = m_TremoloDepth, m_TremoloDepthEnd = 0.0f, m_TremoloDepthStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_TREMOLODEPTH_ID))

    {
        m_TremoloDepthEnd = m_pParams->RTPC.fTremoloDepth;
        m_TremoloDepthStep = ((m_TremoloDepthEnd - m_TremoloDepthBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_TremoloFrequency = m_pParams->RTPC.fTremoloFrequency;
    AkReal32  m_TremoloFrequencyBegin = m_TremoloFrequency, m_TremoloFrequencyEnd = 0.0f, m_TremoloFrequencyStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_TREMOLOFREQUENCY_ID))

    {
        m_TremoloFrequencyEnd = m_pParams->RTPC.fTremoloFrequency;
        m_TremoloFrequencyStep = ((m_TremoloFrequencyEnd - m_TremoloFrequencyBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_TremoloDelayTime = m_pParams->RTPC.fTremoloDelayTime;
    AkReal32  m_TremoloDelayTimeBegin = m_TremoloDelayTime, m_TremoloDelayTimeEnd = 0.0f, m_TremoloDelayTimeStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_TREMOLODELAYTIME_ID))

    {
        m_TremoloDelayTimeEnd = m_pParams->RTPC.fTremoloDelayTime;
        m_TremoloDelayTimeStep = ((m_TremoloDelayTimeEnd - m_TremoloDelayTimeBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_RandomTremoloDepth = m_pParams->RTPC.fRandomTremoloDepth;
    AkReal32  m_RandomTremoloDepthBegin = m_RandomTremoloDepth, m_RandomTremoloDepthEnd = 0.0f, m_RandomTremoloDepthStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_RANDOMTREMOLODEPTH_ID))

    {
        m_RandomTremoloDepthEnd = m_pParams->RTPC.fRandomTremoloDepth;
        m_RandomTremoloDepthStep = ((m_RandomTremoloDepthEnd - m_RandomTremoloDepthBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_RandomTremoloFrequency = m_pParams->RTPC.fRandomTremoloFrequency;
    AkReal32  m_RandomTremoloFrequencyBegin = m_RandomTremoloFrequency, m_RandomTremoloFrequencyEnd = 0.0f, m_RandomTremoloFrequencyStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_RANDOMTREMOLOFREQUENCY_ID))

    {
        m_RandomTremoloFrequencyEnd = m_pParams->RTPC.fRandomTremoloFrequency;
        m_RandomTremoloFrequencyStep = ((m_RandomTremoloFrequencyEnd - m_RandomTremoloFrequencyBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_RandomTremoloDelayTime = m_pParams->RTPC.fRandomTremoloDelayTime;
    AkReal32  m_RandomTremoloDelayTimeBegin = m_RandomTremoloDelayTime, m_RandomTremoloDelayTimeEnd = 0.0f, m_RandomTremoloDelayTimeStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_RANDOMTREMOLODELAYTIME_ID))

    {
        m_RandomTremoloDelayTimeEnd = m_pParams->RTPC.fRandomTremoloDelayTime;
        m_RandomTremoloDelayTimeStep = ((m_RandomTremoloDelayTimeEnd - m_RandomTremoloDelayTimeBegin) / out_pBuffer->MaxFrames());
    }

    bool  m_Stick = m_pParams->RTPC.fStick;


    AkReal32  m_StickDecay = m_pParams->RTPC.fStickDecay;
    AkReal32  m_StickDecayBegin = m_StickDecay, m_StickDecayEnd = 0.0f, m_StickDecayStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_STICKDECAY_ID))

    {
        m_StickDecayEnd = m_pParams->RTPC.fStickDecay;
        m_StickDecayStep = ((m_StickDecayEnd - m_StickDecayBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_StickToneMix = m_pParams->RTPC.fStickToneMix;
    AkReal32  m_StickToneMixBegin = m_StickToneMix, m_StickToneMixEnd = 0.0f, m_StickToneMixStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_STICKTONEMIX_ID))

    {
        m_StickToneMixEnd = m_pParams->RTPC.fStickToneMix;
        m_StickToneMixStep = ((m_StickToneMixEnd - m_StickToneMixBegin) / out_pBuffer->MaxFrames());
    }


    AkReal32  m_StickPulseMix = m_pParams->RTPC.fStickPulseMix;
    AkReal32  m_StickPulseMixBegin = m_StickPulseMix, m_StickPulseMixEnd = 0.0f, m_StickPulseMixStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_STICKPULSEMIX_ID))

    {
        m_StickPulseMixEnd = m_pParams->RTPC.fStickPulseMix;
        m_StickPulseMixStep = ((m_StickPulseMixEnd - m_StickPulseMixBegin) / out_pBuffer->MaxFrames());
    }

    AkReal32  m_StickVelvetMix = m_pParams->RTPC.fStickVelvetMix;
    AkReal32  m_StickVelvetMixBegin = m_StickVelvetMix, m_StickVelvetMixEnd = 0.0f, m_StickVelvetMixStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_STICKVELVETMIX_ID))

    {
        m_StickVelvetMixEnd = m_pParams->RTPC.fStickVelvetMix;
        m_StickVelvetMixStep = ((m_StickVelvetMixEnd - m_StickVelvetMixBegin) / out_pBuffer->MaxFrames());
    }



    AkReal32  m_Smoothness = m_pParams->RTPC.fSmoothness;
    AkReal32  m_SmoothnessBegin = m_Smoothness, m_SmoothnessEnd = 0.0f, m_SmoothnessStep = 0.0f;

    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_SMOOTHNESS_ID))

    {
        m_SmoothnessEnd = m_pParams->RTPC.fSmoothness;
        m_SmoothnessStep = ((m_SmoothnessEnd - m_SmoothnessBegin) / out_pBuffer->MaxFrames());
    }

    const bool enableFDN = m_pParams->RTPC.fFDN;
    const bool allpass1Saturation = m_pParams->RTPC.fAllpass1Saturation;

    const bool m_Trigger = m_pParams->RTPC.fTrigger;
    if (m_pParams->m_paramChangeHandler.HasChanged(PARAM_TRIGGER_ID))

    {
        Trigger = m_pParams->RTPC.fTrigger;
        m_pParams->m_paramChangeHandler.ResetParamChange(PARAM_TRIGGER_ID);
        m_pParams->RTPC.fTrigger = false;
        m_pParams->m_paramChangeHandler.SetParamChange(PARAM_TRIGGER_ID);
    }


    for (AkUInt32 i = 0; i < uNumChannels; ++i)
    {
        AkReal32* AK_RESTRICT pBuf = (AkReal32* AK_RESTRICT)out_pBuffer->GetChannel(i);


        uFramesProduced = 0;
        while (uFramesProduced < out_pBuffer->uValidFrames)
        {
            float sample = 0.0f;

            if (Trigger)
            {
                float m_Frequency = m_PitchBegin;

                auto seed = m_Seed;

                // Set stick oscillator.
                if (m_Stick) {
                    if (m_RetriggerStick) {
                        rngStick.seed = seed;
                        velvet.rng.seed = seed;
                    }

                    pulsar.phase = 1.0f;

                    stickEnvelope.reset(m_StickDecayBegin);
                    const auto upperBound = 0.95f * sampleRate / 2.0f;
                    auto oscFreq = m_Frequency;
                    while (oscFreq >= upperBound) oscFreq *= 0.5f;
                    for (auto& osc : stickOscillator) {
                        if (oscFreq < 20.0f) oscFreq += 20.0f;
                        osc.setFrequency(oscFreq);
                        oscFreq *= 1.0f + rngStick.process();
                        if (oscFreq > upperBound) oscFreq = m_Frequency * (1.0f + rngStick.process());
                    }

                    velvet.setDensity(sampleRate * 0.004f * (m_PitchBegin + 1) / 32.0f);
                }

                // Set fdnCascade.
                if (m_RetriggerTime)
                    rng.seed = m_Seed;

                const float fdnTime = m_FdnTimeBegin;
                for (size_t n = 0; n < fdnCascade.size(); ++n) {
                    float diagMod = float(n + 1) / fdnCascade.size();
                    float delayTimeMod = std::pow(diagMod * 2.0f, 0.8f);
                    for (size_t i = 0; i < fdnMatrixSize; ++i) {
                        for (size_t j = 0; j < fdnMatrixSize; ++j) {
                            if (i == j)
                                fdnCascade[n].matrix[i][j] = 1 - diagMod - 0.5f * (rng.process() - diagMod);
                            else
                                fdnCascade[n].matrix[i][j] = -0.5f * rng.process();
                        }
                        fdnCascade[n].gain[i] = (rng.process() < 0.5f ? 1.0f : -1.0f)
                            * (0.1f + rng.process()) * 2.0f / fdnMatrixSize;
                        fdnCascade[n].delayTime[i].push(rng.process() * delayTimeMod * fdnTime);
                    }
                }

                // Set serialAP.
                float ap1Time = m_Allpass1TimeBegin;
                for (auto& ap : serialAP1.allpass) {
                    ap.set(0.001f + 0.999f * rng.process(), ap1Time + ap1Time * rng.process());
                    ap1Time *= 1.5f;
                }

                float ap2Time = m_Allpass2TimeBegin;
                for (auto& allpass : serialAP2) {
                    for (auto& ap : allpass.allpass)
                        ap.set(0.001f + 0.999f * rng.process(), ap2Time + ap2Time * rng.process());
                    ap2Time *= 1.5f;
                }

                // Set tremolo.
                if (m_RetriggerTremolo) rngTremolo.seed = seed;

                randomTremoloDepth = 1.0f
                    + m_RandomTremoloDepthBegin
                    * (rngTremolo.process() - 1.0f);
                randomTremoloFrequency = 1.0f
                    + m_RandomTremoloFrequencyBegin
                    * (rngTremolo.process() - 1.0f);
                randomTremoloDelayTime = 1.0f
                    + m_RandomTremoloDelayTimeBegin
                    * (rngTremolo.process() - 1.0f);

                Trigger = false;
            }

            //if (pBuf != nullptr) { sample += pBuf[uFramesProduced]; }

            const float pitch = m_PitchBegin;
            if (!stickEnvelope.isTerminated) {
                const float toneMix = m_StickToneMixBegin;//interpStickToneMix.process();
                const float pulseMix = m_StickPulseMixBegin; //interpStickPulseMix.process();
                const float velvetMix = m_StickVelvetMixBegin;//interpStickVelvetMix.process();
                const float stickEnv = stickEnvelope.process();
                float stickTone = 0.0f;
                for (auto& osc : stickOscillator) stickTone += osc.process();
                velvet.setDensity(pitch);
                sample += pulseMix * pulsar.process()+ stickEnv * ((toneMix * stickTone) + velvetMix * velvet.process());
            }

            // FDN.
            if (enableFDN) {
                const float fdnFeedback = m_FdnFeedbackBegin;//interpFDNFeedback.process();
                fdnSig = fdnCascade[0].process(
                    juce::dsp::FastMathApproximations::tanh<float>(sample + fdnFeedback * fdnSig));
                const float fdnCascadeMix = m_FdnCascadeMixBegin;//interpFDNCascadeMix.process();
                for (size_t j = 1; j < fdnCascade.size(); ++j) {
                    fdnSig += fdnCascadeMix * (fdnCascade[j].process(fdnSig * 2.0f) - fdnSig);
                }
                sample = fdnSig * 1024.0f;
            }

            // Allpass.
            serialAP1Sig = m_Allpass1Saturation
                ? juce::dsp::FastMathApproximations::tanh(serialAP1Sig)
                : serialAP1Sig;
            serialAP1Sig
                = serialAP1.process(sample + m_Allpass1FeedbackBegin * serialAP1Sig);
            float apOut = float(serialAP1Highpass.process(serialAP1Sig));

            serialAP2Sig = apOut + m_Allpass2FeedbackBegin * serialAP2Sig;
            float sum = 0.0f;
            for (auto& ap : serialAP2) sum += ap.process(serialAP2Sig);
            serialAP2Sig = sum / serialAP2.size();
            apOut += 4.0f * float(serialAP2Highpass.process(serialAP2Sig));

            const float allpassMix = m_AllpassMixBegin;//interpAllpassMix.process();
            sample += allpassMix * (apOut - sample);

            // Tremolo.
            tremoloPhase += m_TremoloFrequencyBegin * float(twopi) / sampleRate;
            if (tremoloPhase >= float(twopi)) tremoloPhase -= float(twopi);

            const float tremoloLFO = 0.5f * (sinf(tremoloPhase) + 1.0f);
            tremoloDelay.setTime(m_TremoloDelayTimeBegin * tremoloLFO);

            const float tremoloDepth = m_TremoloDepthBegin;
            sample += m_TremoloMixBegin
                * ((tremoloDepth * tremoloLFO + 1.0f - tremoloDepth) * tremoloDelay.process(sample)
                    - sample);

             *pBuf++ = sample;

            ++uFramesProduced;

            m_FdnTimeBegin += m_FdnTimeStep;
            m_FdnFeedbackBegin += m_FdnFeedbackStep;
            m_FdnCascadeMixBegin += m_FdnCascadeMixStep;
            m_AllpassMixBegin += m_AllpassMixStep;
            m_Allpass1TimeBegin += m_Allpass1TimeStep;
            m_Allpass1FeedbackBegin += m_Allpass1FeedbackStep;
            m_Allpass1HighpassCutoffBegin += m_Allpass1HighpassCutoffStep;
            m_Allpass2TimeBegin += m_Allpass2TimeStep;
            m_Allpass2FeedbackBegin += m_Allpass2FeedbackStep;
            m_Allpass2HighpassCutoffBegin += m_Allpass2HighpassCutoffStep;
            m_TremoloMixBegin += m_TremoloMixStep;
            m_TremoloDepthBegin += m_TremoloDepthStep;
            m_TremoloFrequencyBegin += m_TremoloFrequencyStep;
            m_TremoloDelayTimeBegin += m_TremoloDelayTimeStep;
            m_RandomTremoloDepthBegin += m_RandomTremoloDepthStep;
            m_RandomTremoloFrequencyBegin += m_RandomTremoloFrequencyStep;
            m_RandomTremoloDelayTimeBegin += m_RandomTremoloDelayTimeStep;
            m_StickDecayBegin += m_StickDecayStep;
            m_StickToneMixBegin += m_StickToneMixStep;
            m_StickPulseMixBegin += m_StickPulseMixStep;
            m_StickVelvetMixBegin += m_StickVelvetMixStep;
            m_SmoothnessBegin += m_SmoothnessStep;
            m_PitchBegin += m_PitchStep;

        }
 
        m_Pitch = m_PitchBegin;
        m_FdnTime = m_FdnTimeBegin;
        m_FdnFeedback = m_FdnFeedbackBegin;
        m_FdnCascadeMix = m_FdnCascadeMixBegin;
        m_AllpassMix = m_AllpassMixBegin;
        m_Allpass1Time = m_Allpass1TimeBegin;
        m_Allpass1Feedback = m_Allpass1FeedbackBegin;
        m_Allpass1HighpassCutoff = m_Allpass1HighpassCutoffBegin;
        m_Allpass2Time = m_Allpass2TimeBegin;
        m_Allpass2Feedback = m_Allpass2FeedbackBegin;
        m_Allpass2HighpassCutoff = m_Allpass2HighpassCutoffBegin;
        m_TremoloMix = m_TremoloMixBegin;
        m_TremoloDepth = m_TremoloDepthBegin;
        m_TremoloFrequency = m_TremoloFrequencyBegin;
        m_TremoloDelayTime = m_TremoloDelayTimeBegin;
        m_RandomTremoloDepth = m_RandomTremoloDepthBegin;
        m_RandomTremoloFrequency = m_RandomTremoloFrequencyBegin;
        m_RandomTremoloDelayTime = m_RandomTremoloDelayTimeBegin;
        m_StickDecay = m_StickDecayBegin;
        m_StickToneMix = m_StickToneMixBegin;
        m_StickPulseMix = m_StickPulseMixBegin;
        m_StickVelvetMix = m_StickVelvetMixBegin;
        m_Smoothness = m_SmoothnessBegin;
        m_Pitch = m_PitchBegin;
    }
}

AkReal32 FDNCymbalSource::GetDuration() const
{
    return m_durationHandler.GetDuration() * 1000.0f;
}
