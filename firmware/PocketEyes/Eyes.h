#pragma once
#include <stdint.h>
#include <math.h>

// Original animation code. All timing is deterministic for repeatable takes.
namespace pocket {
constexpr uint32_t kLoopMs = 16000;
constexpr uint32_t kFrameMs = 33;
enum class Stage { Eyes, Demo };

inline float clamp(float v, float lo, float hi) {
  return v < lo ? lo : (v > hi ? hi : v);
}
inline float ease(float a, float b, float t) {
  float x = clamp((t - a) / (b - a), 0, 1);
  return x * x * (3 - 2 * x);
}
inline float pulse(float t, float start, float rise, float hold, float fall) {
  return ease(start, start + rise, t) *
         (1 - ease(start + rise + hold, start + rise + hold + fall, t));
}
struct Pose { float x, y, left, right, happy, curious; };
inline Pose pose(uint32_t elapsed) {
  const float t = static_cast<float>(elapsed % kLoopMs);
  const float wake = ease(100, 650, t) * (1 - ease(15650, 16000, t));
  float blink = 1;
  const float blinks[] = {1700, 6200, 6800, 11900, 14600};
  for (float start : blinks) blink *= 1 - pulse(t, start, 85, 35, 150);
  const float wink = 1 - pulse(t, 13000, 100, 170, 220);
  return {
    9 * pulse(t, 2400, 500, 650, 500) - 10 * pulse(t, 4200, 500, 650, 500),
    -3 * pulse(t, 4200, 500, 650, 500) - 2 * pulse(t, 8000, 500, 2200, 500),
    wake * blink, wake * blink * wink,
    pulse(t, 8000, 500, 2200, 500),
    pulse(t, 4200, 500, 650, 500)
  };
}

class Playback {
 public:
  Stage stage = Stage::Eyes;
  bool paused = false;
  uint32_t elapsed = 0;
  void tick(uint32_t now) {
    if (started_ && !paused)
      elapsed = static_cast<uint32_t>((static_cast<uint64_t>(elapsed) +
        static_cast<uint32_t>(now - last_)) % kLoopMs);
    last_ = now;
    started_ = true;
  }
  // Call tick(now) before command(). Returns true for a recognized command.
  bool command(char c) {
    if (c == 'e' || c == 'd') {
      stage = c == 'e' ? Stage::Eyes : Stage::Demo;
      elapsed = 0;
      paused = false;
    } else if (c == 'r') {
      elapsed = 0;
      paused = false;
    } else if (c == 'p') {
      paused = !paused;
    } else return false;
    return true;
  }
 private:
  uint32_t last_ = 0;
  bool started_ = false;
};

// Surface implements fillRoundRect(x, y, width, height, radius, on).
// Begin/clear/present belong to the board adapter, keeping bus IO out of here.
template<class Surface>
void render(Surface& surface, int width, int height, Stage stage, uint32_t ms) {
  const float scale = fminf(width / 128.0f, height / 64.0f);
  const float ox = (width - 128 * scale) / 2;
  const float oy = (height - 64 * scale) / 2;
  auto rect = [&](float x, float y, float w, float h, float r, bool on = true) {
    const int iw = static_cast<int>(lroundf(w * scale));
    const int ih = static_cast<int>(lroundf(h * scale));
    if (iw < 1 || ih < 1) return;
    const int radius = static_cast<int>(lroundf(r * scale));
    surface.fillRoundRect(static_cast<int>(lroundf(ox + x * scale)),
      static_cast<int>(lroundf(oy + y * scale)), iw, ih,
      radius < (iw < ih ? iw : ih) / 2 ? radius : (iw < ih ? iw : ih) / 2, on);
  };
  if (stage == Stage::Demo) {
    // A project-made display check, never presented as the factory firmware.
    rect(10, 10, 108, 2, 0); rect(10, 52, 108, 2, 0);
    const float t = static_cast<float>(ms % 4000) / 4000;
    const float x = 15 + 82 * (0.5f - 0.5f * cosf(t * 6.2831853f));
    rect(x, 25, 16, 16, 4);
    return;
  }
  const Pose p = pose(ms);
  for (int eye = 0; eye < 2; ++eye) {
    const float open = eye == 0 ? p.left : p.right;
    const float extra = eye == 1 ? 4 * p.curious : -2 * p.curious;
    const float h = 2 + (29 + extra) * open;
    const float w = 27 + 2 * p.happy;
    const float x = (eye == 0 ? 29 : 72) + p.x - p.happy;
    const float y = 29 + p.y - h / 2;
    rect(x, y, w, h, 7);
    // Raise a rounded lower lid into each eye for a happy crescent.
    if (p.happy > 0) rect(x - 1, y + h * (1 - 0.64f * p.happy),
                         w + 2, h * 0.64f * p.happy + 10, 9, false);
  }
  // A tiny smile appears only during the happy beat.
  if (p.happy > 0.4f) {
    const float y = 46 + p.y;
    rect(58, y, 3, 3, 1); rect(61, y + 2, 6, 3, 1); rect(67, y, 3, 3, 1);
  }
}
}  // namespace pocket
