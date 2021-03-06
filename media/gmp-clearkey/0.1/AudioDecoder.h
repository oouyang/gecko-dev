/*
 * Copyright 2013, Mozilla Foundation and contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __AudioDecoder_h__
#define __AudioDecoder_h__

#include "gmp-audio-decode.h"
#include "gmp-audio-host.h"
#include "WMFAACDecoder.h"

#include "mfobjects.h"

class AudioDecoder : public GMPAudioDecoder
{
public:
  AudioDecoder(GMPAudioHost *aHostAPI);

  virtual ~AudioDecoder();

  virtual void InitDecode(const GMPAudioCodec& aCodecSettings,
                          GMPAudioDecoderCallback* aCallback) override;

  virtual void Decode(GMPAudioSamples* aEncodedSamples);

  virtual void Reset() override;

  virtual void Drain() override;

  virtual void DecodingComplete() override;

private:

  void EnsureWorker();

  void DecodeTask(GMPAudioSamples* aEncodedSamples);
  void DrainTask();

  void ReturnOutput(IMFSample* aSample);

  HRESULT MFToGMPSample(IMFSample* aSample,
                        GMPAudioSamples* aAudioFrame);

  GMPAudioHost *mHostAPI; // host-owned, invalid at DecodingComplete
  GMPAudioDecoderCallback* mCallback; // host-owned, invalid at DecodingComplete
  GMPThread* mWorkerThread;
  GMPMutex* mMutex;
  wmf::AutoPtr<wmf::WMFAACDecoder> mDecoder;

  int32_t mNumInputTasks;
};

#endif // __AudioDecoder_h__
