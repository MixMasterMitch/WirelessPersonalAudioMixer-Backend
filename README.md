Wireless Personal Audio Mixer - Backend
==================

## Installation

Requires npm and node (v0.10.0+). 

OS support:

* Mac - Supported

* Linux - Supported (not tested)

* Windows - Not Supported

To install and build the backend, just run `npm install`

## Test programs

The loopback program will stream the audio from all of the default audio input channels to the default audio output channel. From the
root of the project, run:

```
node test/loopback
```

To transmit between two computers, use the `transmitter` and `receiver` apps. The transmitter will wirelessly stream audio from
the default input channels to the specified receiver. The receiver will play the audio on the default audio output device.
From the root of the project on the transmitting computer, run:

```
node test/transmitter receiverIpAddress
```

From the root of the project on the receiving computer, run:

```
node test/receiver
```
