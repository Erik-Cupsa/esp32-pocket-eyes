# Film episode one

## Before the first upload

- [ ] Connect the Waveshare board with a USB data cable and film its factory demo.
- [ ] Capture the [DroneBot Workshop beginner tutorial](https://www.youtube.com/watch?v=xPlN_Tk3VLQ)
      as your learning reference. Use this project's board-specific setup for uploading.
- [ ] Back up the factory flash; keep the verified vendor recovery file locally.
- [ ] Run the build, upload, then observe the real eyes and BOOT/serial controls.
- [ ] Record a short test clip to check focus, exposure, panel orientation and banding.

## The take

1. Show the setup/code/upload progression. The project display-check stage is
   optional; it is not what the board originally came with.
2. Start recording, then **tap BOOT** to restart the eyes reveal.
3. Let it play: wake up in the first second, look right around 3 seconds, left
   around 5 seconds, smile around 9–11 seconds, wink around 13 seconds.
4. For another take, tap BOOT again. The 16-second animation is deterministic.
5. To switch to/from the basic display check, hold BOOT for at least 0.8 seconds
   and release. To hold a pose, send `p` in Serial Monitor; send it again to resume.
6. For a power-up shot, disconnect/reconnect USB with BOOT released and no battery
   connected. Eyes is the default boot stage. PWR is not our animation button.

Serial shortcuts: `e` eyes, `d` project demo, `r` restart current stage, `p` pause,
`c` amber/cyan. Restarting/switching these stages does not write flash. Recovering
an actual factory-demo shot requires [restoring factory firmware](RECOVERY.md).

## If something goes wrong

| Symptom | First check |
| --- | --- |
| Power but no USB port | Known data cable, direct USB connection; compare before/after port lists |
| Port busy | Close Serial Monitor before backup/upload |
| Cannot connect | Hold BOOT while reconnecting USB, release, list the new port and retry |
| Upload succeeds, no animation | Power-cycle with BOOT released; check serial errors and `PSRAM=opi` |
| Blank/offset screen | Confirm the board is 1.75 (not 1.75C) and use the pinned vendor driver/offset |
| Serial commands do nothing | 115200 baud, lowercase command, correct re-enumerated port, CDC enabled |
| Flicker or tearing | Record a clip; measure actual refresh and adjust frame rate if needed |
| Camera shows bands | Adjust shutter speed/exposure or `kBrightness` and test another clip |
| Original demo is gone | Normal: the eyes firmware replaced it; use the verified recovery copy |

Physical button, display and recovery checks remain to be performed on the board.
No battery, speaker, microSD card or external wiring is required for this episode.
