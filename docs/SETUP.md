# Setup and upload

## Prepared local workspace

The toolchain, source dependencies, vendor recovery image and compiled firmware
are already present locally. Start with `python3 tools/project.py ports` after
connecting the board. **Film the factory demo and follow RECOVERY.md before uploading.**

## Recreate from a clean checkout

Requirements: macOS (Apple Silicon/Intel) or Linux (x86-64/ARM64), Python 3, Git,
and internet access for setup. The first toolchain download is several GB.

```sh
python3 tools/project.py setup
python3 tools/project.py build
```

Everything is installed under ignored `local/`. Setup verifies the Arduino CLI
archive checksum, pins the vendor Git revision, installs Arduino-ESP32 3.3.10 and
checks the manufacturer's factory-image SHA-256. The display dependency is the
bundled Arduino_GFX 1.6.4 revision, not a floating Library Manager version.

To compile the original vendor display example without uploading it:

```sh
python3 tools/project.py vendor-check
```

Build output is in `build/firmware/`. `manifest.json` records source and binary
hashes plus the dependency lock. Builds need no board connection.

## Connect and upload

1. Use a data-capable USB cable. With USB disconnected, list ports, connect the
   board, list again and identify the new port:

   ```sh
   python3 tools/project.py ports
   ```

2. Complete [factory backup](RECOVERY.md). The normal upload path requires the
   matching-read record at `backups/factory-first/VERIFIED.json`. Pass
   `--backup path/to/another-backup` if you chose another directory.
3. Close any serial monitor. Upload the current source:

   ```sh
   python3 tools/project.py upload --port /dev/cu.YOUR_ACTUAL_PORT
   ```

4. If connection fails, hold **BOOT while plugging in USB**, then release it,
   list ports again and retry with the bootloader's port. No full-flash erase is
   needed. This follows the vendor's BOOT-and-power-cycle recovery guidance.
5. If the firmware stays in download mode afterward, unplug/replug USB with BOOT
   released (USB-only power). Observe the eyes before claiming the board works.
6. Optional serial control:

   ```sh
   python3 tools/project.py monitor --port /dev/cu.YOUR_ACTUAL_PORT
   ```

   Send lowercase `e`, `d`, `r`, `p` or `c`. Line endings are ignored. The monitor
   prints its exit shortcut when opening. It may reset/re-enumerate the board;
   BOOT-button controls are simpler for filming after upload.

## Arduino IDE alternative

The CLI above is the tested build path. For the IDE, use these equivalent settings
and install the exact graphics library downloaded under
`local/waveshare/examples/arduino/libraries/GFX_Library_for_Arduino` into a dedicated
sketchbook. Avoid selecting another globally installed graphics-library version.

| Setting | Value |
| --- | --- |
| Additional Boards Manager URL | `https://espressif.github.io/arduino-esp32/package_esp32_index.json` |
| Board package | esp32 by Espressif Systems **3.3.10** |
| Board | ESP32S3 Dev Module |
| Flash size | 16 MB |
| Partition scheme | 16M Flash (3MB APP/9.9MB FATFS) |
| PSRAM | OPI PSRAM |
| USB mode | Hardware CDC and JTAG |
| USB CDC on boot | Enabled |
| Upload mode | UART0 / Hardware CDC |
| Sketch | `firmware/PocketEyes/PocketEyes.ino` |
| Serial baud | 115200 |

The exact CLI board identifier is in `dependencies.json`. Use the vendor's current
[pinned setup guide](https://github.com/waveshareteam/ESP32-S3-Touch-AMOLED-1.75/blob/e4344e70c2fa78a13e8a06566507f1ba8af6672a/docs/getting-started.md)
for context; an older wiki mentions 3.1.0, while this tested source revision uses
3.3.10. Do not replace the driver or core version mid-recording.
