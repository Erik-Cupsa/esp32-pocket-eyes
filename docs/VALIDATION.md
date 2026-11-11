# Validation — November 11, 2026

## Passed locally

- Arduino CLI 1.6.0 and the manufacturer factory image matched their published
  SHA-256 checksums. The factory image is 16,777,216 bytes, with expected hash
  `2b7e01ff1f37027385a3820e006f945464cfcee2a05954b3239cf75f250080b6`.
- Waveshare's unmodified HelloWorld display example compiled with Arduino-ESP32
  3.3.10, the pinned vendor libraries, 16 MB flash, OPI PSRAM and USB CDC.
  Reported application size: **414,046 bytes**; static RAM: **24,340 bytes**.
- Pocket Eyes compiled for the board profile in `dependencies.json`.
  Reported application size: **424,742 bytes / 3,145,728 bytes**;
  static RAM: **24,644 bytes / 327,680 bytes**. The separate runtime framebuffer
  requests **223,680 bytes in PSRAM**; it is not included in the static RAM figure.
- C++11 host checks with strict warnings and UndefinedBehaviorSanitizer:
  expression timing, every millisecond of the loop at seven viewport sizes,
  geometry bounds, restart, stage changes, pause and `millis()` rollover.
- Python tests use simulated reads to cover successful backup verification,
  overwrite refusal, mismatching/short/failed reads, and the upload/restore gate's
  rejection of missing, single-read, truncated or corrupted backups.
- JavaScript syntax and regression checks cover a click/frame timestamp race,
  stage rendering, scrubbing and pause.
- Chrome visual inspection: round-screen eyes and happy pose, project-demo
  rendering, stage switching and scrub-to-pause. Screenshot: `preview/screenshot.jpg`.
- Pinned Python dependencies installed; esptool 5.1.0 starts and exposes the
  documented commands. Only Bluetooth/debug serial ports were present during
  inspection; no ESP32 port was identified or opened.

Local host: Apple Silicon macOS, Apple Clang 17, Python 3.12 for USB tools and
Node 24. The portable C++ check also uses the system Python for stdlib-only tests.
An AddressSanitizer attempt stalled on this host and was stopped; it is not a
passing result. The completed C++ sanitizer run used UndefinedBehaviorSanitizer.

## Not physically validated

USB upload/reset, actual display orientation/color, panel refresh/tearing,
USB-only power initialization, PSRAM allocation, BOOT-button timings, serial
commands, flash backup and factory restoration all require the connected board.
A successful compile and browser preview do not establish any of these.

## First board check

1. Record the factory demo and identify the USB port.
2. Confirm ESP32-S3 / 16 MB and readable, unprotected flash; save matching backups.
3. Upload Pocket Eyes. Check startup, a complete 16-second loop and both stages.
4. Test tap BOOT, hold/release BOOT, all five serial commands and USB power-cycle.
5. Record a camera test for banding, then set the desired brightness/color.
6. If recovery is tested, verify the original demo after restoration and record
   that observation separately. Never mark recovery tested based only on a hash.
