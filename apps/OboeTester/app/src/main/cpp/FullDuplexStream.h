/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OBOETESTER_FULL_DUPLEX_STREAM_H
#define OBOETESTER_FULL_DUPLEX_STREAM_H

#include <unistd.h>
#include <sys/types.h>

#include "oboe/Oboe.h"

class FullDuplexStream : public oboe::AudioStreamCallback {
public:
    FullDuplexStream() {}
    virtual ~FullDuplexStream() = default;

    void setInputStream(oboe::AudioStream *stream) {
        mInputStream = stream;
    }

    oboe::AudioStream *getInputStream() {
        return mInputStream;
    }

    void setOutputStream(oboe::AudioStream *stream) {
        mOutputStream = stream;
    }
    oboe::AudioStream *getOutputStream() {
        return mOutputStream;
    }

    virtual oboe::Result start();

    virtual oboe::Result stop();

    /**
     * Called when data is available on both streams.
     * Caller should override this method.
     */
    virtual oboe::DataCallbackResult onBothStreamsReady(
            const void *inputData,
            int   numInputFrames,
            void *outputData,
            int   numOutputFrames
            ) = 0;

    /**
     * Called by Oboe when the stream is ready to process audio.
     */
    oboe::DataCallbackResult onAudioReady(
            oboe::AudioStream *audioStream,
            void *audioData,
            int numFrames) override;

private:

    // TODO add getters and setters
    int32_t kNumCallbacksToDrain   = 20;
    int32_t kNumCallbacksToNotRead =  0; // let input fill back up, usually 0 or 1
    int32_t kNumCallbacksToDiscard = 20;

    // We want to reach a state where the input buffer is empty and
    // the output buffer is full.
    // These are used in order.
    // Drain several callback so that input is empty.
    int32_t              mCountCallbacksToDrain = kNumCallbacksToDrain;
    // Let the input fill back up slightly so we don't run dry.
    int32_t              mCountCallbacksToNotRead = kNumCallbacksToNotRead;
    // Discard some callbacks so the input and output reach equilibrium.
    int32_t              mCountCallbacksToDiscard = kNumCallbacksToDiscard;

    oboe::AudioStream   *mInputStream = nullptr;
    oboe::AudioStream   *mOutputStream = nullptr;

    int32_t              mBufferSize = 0;
    std::unique_ptr<float[]> mInputBuffer;
};


#endif //OBOETESTER_FULL_DUPLEX_STREAM_H
