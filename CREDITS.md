# Credits and dependency licenses

## Inspiration and learning

- [DroneBot Workshop: Getting Started with ESP32](https://dronebotworkshop.com/esp32-intro/)
  and its [beginner video](https://www.youtube.com/watch?v=xPlN_Tk3VLQ).
  Use this for concepts; the current [Espressif installation guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
  supplies the official Boards Manager URL. The selected board's vendor example
  determines the actual version and options.
- [FluxGarage RoboEyes](https://github.com/FluxGarage/RoboEyes), by Dennis Hoelscher,
  and the [animated eyes demonstration](https://www.youtube.com/watch?v=ibSaDEkfUOI).
  The repository is GPL-3.0; the inspected source header permits GPL-3.0-or-later.
  It uses Adafruit-style drawing and buffered-display methods. Compatibility with
  an arbitrary TFT/round display cannot be inferred from the ESP32 chip alone.
  No RoboEyes source is copied, vendored or linked in this version. Our original
  deterministic core supports this board's buffered color display and repeatable
  rehearsal. If RoboEyes is integrated later, retain its license/notices and review
  the resulting combined project's licensing before publishing it.
- Visual references: [Ella Gonzales](https://www.instagram.com/p/DfWte6kyZc0/),
  [Ethan Fang](https://www.instagram.com/reel/DeqjoLJpiPu/), and
  [Andrew Codesmith](https://www.youtube.com/shorts/vfRSW4BQib4).
  No images, screenshots, music or video from these creators are redistributed.

## Installed host tools

`requirements.lock` pins tools installed locally from PyPI. They are not bundled
into the firmware or copied into this repository. Their own licenses remain in
their installed distributions:

- [esptool 5.1.0](https://github.com/espressif/esptool/tree/v5.1.0): GPL-2.0-or-later.
- [pyserial 3.5](https://github.com/pyserial/pyserial): BSD-3-Clause.
- Transitive dependencies are recorded with exact versions and hashes in the lock.

Original code and docs here: MIT; see [LICENSE](LICENSE). Reused Waveshare
initialization and the pinned Arduino_GFX display dependency are documented in
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
