# Beam Me Up
## Multi-Platform, Inter-thread Communication (ITC) library

Beam Me Up is an inter-object (thread safe) communication library designed to provide message queuing functionality
between objects/threads without all the bloat of general purpose libraries. By default, a thread safe and unsafe version
is of the library (in static and dynamic flavours) are built. Thread unsafe is more efficient, but it will crash if run
in a threaded environment.

### Status
[![Build Status (Master)](https://travis-ci.org/curquhart/beammeup.svg?branch=master)](https://travis-ci.org/curquhart/beammeup)

### Basic Usage
The Transporter class tracks object life-cycle. That is, if object A is connected to object B through a signal and
object B is destroyed, new signals from object A won't crash the application.

Receiver should be implemented by any object that needs to receive messages, and typically processMessages
(on Transporter) should be run on a single thread in a loop (or during your application's event loop). Transporter will
send messages to the correct objects via each object (Receiver)'s processMessages method, which will dispatch to
processMessage.

