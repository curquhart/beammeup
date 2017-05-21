#include "include/beammeup/Receiver.h"
#include "include/beammeup/Signaler.h"
#include "include/beammeup/Transporter.h"

namespace BeamMeUp {
    Signaler::Signaler(Transporter *transporter) : transporter(transporter) {
    }

    void Signaler::connect(Signal signal, Receiver *receiver) {
        if (receiver == nullptr) {
            // Abort if the receiver isn't valid
            return;
        }

#ifdef THREAD_SAFE
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
#endif
        connectedObjects.insert(std::pair<Signal, Receiver *>(signal, receiver));
    }

    void Signaler::disconnect(Signal signal) {
#ifdef THREAD_SAFE
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
#endif
        connectedObjects.erase(signal);
    }

    void Signaler::disconnect(const Receiver *receiver) {
#ifdef THREAD_SAFE
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
#endif
        for (auto it = connectedObjects.begin(); it != connectedObjects.end(); ++it) {
            if (it->second == receiver) {
                it = connectedObjects.erase(it);
            }
        }
    }

    void Signaler::disconnect(Signal signal, const Receiver *receiver) {
#ifdef THREAD_SAFE
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
#endif
        auto range = connectedObjects.equal_range(signal);

        for (auto it = range.first; it != range.second; ++it) {
            if (it->second == receiver) {
                connectedObjects.erase(it);
            }
        }
    }

    void Signaler::disconnectAll() {
#ifdef THREAD_SAFE
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
#endif
        connectedObjects.clear();
    }

    std::multimap<Signal, Receiver *> Signaler::getConnectedObjects() {
        return connectedObjects;
    };

    void Signaler::notify(Signal signal, const Variant &message) {
#ifdef THREAD_SAFE
        std::shared_lock<std::shared_timed_mutex> lock(mutex);
#endif
        // Find all of the connected functions matching this id
        auto range = connectedObjects.equal_range(signal);

        // Add to queue(s)
        if (transporter != nullptr) {
            for (auto it = range.first; it != range.second; ++it) {
                if (transporter->isObjectRegistered(it->second)) {
                    it->second->receiveMessage(signal, message);
                }
            }
        }
    }
}
