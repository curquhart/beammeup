#include "include/beammeup/Receiver.h"
#include "include/beammeup/Signaler.h"
#include "include/beammeup/Transporter.h"

namespace BeamMeUp {
    Transporter::Transporter() : Signaler(this) {
    }

    void Transporter::processMessages() {
        ObjectSet *workingObjects;

#ifdef THREAD_SAFE
        {
            std::shared_lock<std::shared_timed_mutex> lock(mutex);
            workingObjects = new ObjectSet(objects);
        }
        bool checkRegistered = true;
#else
        workingObjects = &objects;
        bool checkRegistered = false;
#endif

        for (auto object : *workingObjects) {
            // Verify the object is still registered. It could have been deleted and
            // then this would crash.
            if (!checkRegistered || isObjectRegistered(object)) {
                object->processMessages();
            }
        }

#ifdef THREAD_SAFE
        delete workingObjects;
#endif
    }

    void Transporter::registerObject(Receiver *object) {
#ifdef THREAD_SAFE
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
#endif
        objects.insert(object);
    }

    void Transporter::unregisterObject(Receiver *object) {
#ifdef THREAD_SAFE
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
#endif
        objects.erase(object);
    }

    bool Transporter::isObjectRegistered(Receiver *object) {
#ifdef THREAD_SAFE
        std::shared_lock<std::shared_timed_mutex> lock(mutex);
#endif
    return objects.find(object) != objects.end();
    }
}
