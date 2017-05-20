#ifndef BEAMMEUP_MOCKMUTEX_H
#define BEAMMEUP_MOCKMUTEX_H

#include <mutex>

#include "gmock/gmock.h"

namespace BeamMeUp {
    class MockMutex : public std::mutex {
    public:
        MOCK_METHOD0(lock, void());

        MOCK_METHOD0(unlock, void());
    };
}

#endif //BEAMMEUP_MOCKMUTEX_H
