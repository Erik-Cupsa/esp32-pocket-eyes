# Third-party notices

## Waveshare board initialization

`firmware/PocketEyes/Board.h` adapts the display initialization and pin assignments
from Waveshare's `examples/arduino/01_HelloWorld/01_HelloWorld.ino` and
`examples/arduino/libraries/Mylibrary/pin_config.h` at revision
[`e4344e70c2fa78a13e8a06566507f1ba8af6672a`](https://github.com/waveshareteam/ESP32-S3-Touch-AMOLED-1.75/tree/e4344e70c2fa78a13e8a06566507f1ba8af6672a).

Upstream: Waveshare, Apache License 2.0. A copy is retained in
[`third_party/WAVESHARE-LICENSE`](third_party/WAVESHARE-LICENSE).

Changes in this project: scoped board adapter; explicit PSRAM framebuffer;
center-strip presentation; error handling; configurable foreground color; and
debounced recording controls. The vendor's display pins, controller offset and
initialization order are retained. No audio, touch, storage or charging code is reused.

## Arduino_GFX

The build fetches Waveshare's bundled `GFX_Library_for_Arduino` (reported version
1.6.4) from that same immutable revision. Its source headers retain upstream
attribution, including Moon On Our Nation and Adafruit. The bundled license is
BSD-2-Clause, copyright 2012 Adafruit Industries; a copy is in
[`third_party/ARDUINO-GFX-LICENSE`](third_party/ARDUINO-GFX-LICENSE).
The library's source remains in the ignored local dependency checkout.

## Arduino-ESP32 and host tools

The Arduino-ESP32 3.3.10 core and its ESP-IDF/toolchain dependencies keep their own
licenses in the project-local installation; the core is LGPL-2.1. Arduino CLI
1.6.0 is GPL-3.0. These tools are downloaded from official sources, not vendored.
The Python tools and other references are listed in [CREDITS.md](CREDITS.md).

The original animation, preview, scripts and documentation are MIT licensed.
Upstream portions retain the licenses above; the MIT license does not replace them.
