#include "include/beammeup/Receiver.h"
#include "include/beammeup/Transporter.h"

namespace BeamMeUp {
    Receiver::Receiver(Transporter *transporter) : transporter(transporter) {
        if (transporter != nullptr) {
            transporter->registerObject(this);
        }
    }

    void Receiver::receiveMessage(Signal signal, const Variant &message) {
#ifdef THREAD_SAFE
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
#endif
        messageQueue.push(std::pair<Signal, Variant>(signal, message));
    }

    int Receiver::processMessages() {
        int count = 0;

        while (true) {
            {
#ifdef THREAD_SAFE
                std::shared_lock<std::shared_timed_mutex> lock(mutex);
#endif
                if (messageQueue.empty()) {
                    return count;
                }
            }

            Signal signal;
            Variant message;
            bool hasMessage = false;
            {
#ifdef THREAD_SAFE
                std::unique_lock<std::shared_timed_mutex> lock(mutex);
#endif
                if (!messageQueue.empty()) {
                    auto data = messageQueue.front();
                    messageQueue.pop();
                    signal = data.first;
                    message = data.second;
                    hasMessage = true;
                }
            }

            if (hasMessage) {
                processMessage(signal, message);
                count++;
            }
        }

        return count;
    }

    void Receiver::processMessage(const Signal signal, const Variant &message) {
    }

    Receiver::~Receiver() {
        if (transporter != nullptr) {
            transporter->unregisterObject(this);
        }
    }
}