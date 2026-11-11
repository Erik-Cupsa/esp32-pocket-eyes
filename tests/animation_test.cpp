#include <assert.h>
#include <iostream>
#include "../firmware/PocketEyes/Eyes.h"

struct Surface {
  int width, height, visible;
  void fillRoundRect(int x, int y, int w, int h, int r, bool on) {
    assert(w > 0 && h > 0 && r >= 0 && r <= w / 2 && r <= h / 2);
    if (!(x >= 0 && y >= 0 && x + w <= width && y + h <= height)) {
      std::cerr << "Outside " << width << 'x' << height << ": " << x << ',' << y
                << ',' << w << ',' << h << '\n';
      assert(false);
    }
    if (on) ++visible;
  }
};
int main() {
  using namespace pocket;
  assert(pose(0).left == 0);
  assert(pose(700).left == 1);
  assert(pose(1800).left == 0); // Blink really closes.
  assert(pose(13300).left > pose(13300).right); // Wink is asymmetric.
  assert(pose(9500).happy == 1);
  assert(pose(3000).x > 0 && pose(5000).x < 0);
  assert(fabsf(pose(15999).left - pose(0).left) < 0.001f);
  const int sizes[][2] = {{128,64},{128,32},{240,240},{320,170},{80,160},{466,466},{466,240}};
  for (auto& size : sizes) {
    Surface surface{size[0], size[1], 0};
    for (uint32_t t = 0; t < kLoopMs; ++t) {
      const auto p = pose(t);
      assert(p.left >= 0 && p.left <= 1 && p.right >= 0 && p.right <= 1);
      render(surface, size[0], size[1], Stage::Eyes, t);
      render(surface, size[0], size[1], Stage::Demo, t);
    }
    assert(surface.visible > 0);
  }
  Playback player;
  player.tick(UINT32_MAX - 100);
  player.tick(99);
  assert(player.elapsed == 200); // millis() wraps safely.
  player.command('p'); player.tick(1099);
  assert(player.elapsed == 200);
  player.command('p'); player.tick(1199);
  assert(player.elapsed == 300);
  player.command('d');
  assert(player.stage == Stage::Demo && player.elapsed == 0);
  player.tick(1299); player.command('r');
  assert(player.elapsed == 0 && player.stage == Stage::Demo && !player.paused);
  player.command('e');
  assert(player.stage == Stage::Eyes && !player.paused);
  assert(!player.command('\n'));
  player.tick(17299);
  assert(player.elapsed == 0);
  std::cout << "PASS: expressions, bounds at seven sizes, reset, pause, stages, clock rollover\n";
}
