# Hardware

Target: **Waveshare ESP32-S3-Touch-AMOLED-1.75**, identified from the linked
[product listing](https://www.amazon.ca/dp/B0F7XTJ1ZL). The standard board and `-B`
case version share the display wiring. This profile is not for the separately
named `1.75C` product.

Manufacturer references: [product documentation](https://docs.waveshare.com/ESP32-S3-Touch-AMOLED-1.75),
[pinned hardware reference](https://github.com/waveshareteam/ESP32-S3-Touch-AMOLED-1.75/blob/e4344e70c2fa78a13e8a06566507f1ba8af6672a/HARDWARE_REFERENCE.md),
and [schematic](https://files.waveshare.com/wiki/ESP32-S3-Touch-AMOLED-1.75/ESP32-S3-Touch-AMOLED-1.75.pdf).

| Part | Configuration |
| --- | --- |
| MCU | ESP32-S3R8 |
| Flash / PSRAM | 16 MiB flash / 8 MiB octal PSRAM |
| Display | Integrated 1.75-inch, 466 × 466 round AMOLED |
| Display controller/bus | CO5300, QSPI at 40 MHz |
| Touch | CST9217; unused by this project |
| Power | USB-C for this episode; AXP2101 configuration left unchanged |
| Upload and serial | Native USB, hardware CDC/JTAG, CDC enabled on boot |
| Animation button | BOOT, GPIO0, active low |

## Wiring

```text
Laptop ─── USB data cable ─── board USB-C
                              └── built-in AMOLED
```

No jumpers are needed. The table below describes existing PCB connections; it is
not an instruction to add wires.

| Signal | ESP32-S3 GPIO |
| --- | ---: |
| Display CS | 12 |
| Display clock | 38 |
| Display D0 / D1 / D2 / D3 | 4 / 5 / 6 / 7 |
| Display reset | 39 |
| Shared I2C SDA / SCL | 15 / 14 |
| BOOT | 0 |
| USB D− / D+ | 19 / 20 |

The adapter retains the vendor's six-column controller offset and native rotation
zero. AMOLED brightness uses a panel command (128/255 by default), not a separate
LED backlight pin. Change `kBrightness` in `Board.h` and rebuild if recording needs
another level. Use serial `c` to change eye color without reflashing.

The framebuffer consumes 223,680 bytes of PSRAM. QSPI uses the driver's internal
DMA buffers. Touch, PWR-button handling, microphones, speaker, IMU, SD card and
battery charging are outside this project's scope. USB-only startup is the first
physical check still needed.
