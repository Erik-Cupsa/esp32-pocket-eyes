#include <iostream>
#include "../firmware/PocketEyes/Eyes.h"

struct Commands {
  bool first = true;
  void fillRoundRect(int x, int y, int w, int h, int r, bool on) {
    if (!first) std::cout << ',';
    first = false;
    std::cout << '[' << x << ',' << y << ',' << w << ',' << h << ',' << r
              << ',' << (on ? 1 : 0) << ']';
  }
};
int main() {
  std::cout << "// Generated from firmware/PocketEyes/Eyes.h. Run make preview.\n"
               "window.POCKET_FRAMES={fps:30,duration:16000,width:466,height:466,";
  for (int stage = 0; stage < 2; ++stage) {
    if (stage) std::cout << ',';
    std::cout << (stage == 0 ? "eyes:[" : "demo:[");
    for (int frame = 0; frame < 480; ++frame) {
      if (frame) std::cout << ',';
      std::cout << '[';
      Commands surface;
      pocket::render(surface, 466, 466,
        stage == 0 ? pocket::Stage::Eyes : pocket::Stage::Demo, frame * 1000 / 30);
      std::cout << ']';
    }
    std::cout << ']';
  }
  std::cout << "};\n";
}
