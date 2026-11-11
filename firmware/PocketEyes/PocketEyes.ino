#include "Eyes.h"
#include "Board.h"

pocket::Playback playback;
uint32_t lastFrame = 0;
bool dirty = true;
bool displayReady = false;

void setup() {
  Serial.begin(115200);
  // Never wait for Serial: the character must work from a USB power supply.
  displayReady = board::begin();
  Serial.println("Pocket Eyes: e=eyes d=project demo r=restart p=pause/resume c=color");
}

void loop() {
  const uint32_t now = millis();
  if (!displayReady) { delay(100); return; }
  playback.tick(now);
  // Bound command work so an incoming stream cannot starve drawing.
  for (int i = 0; i < 16 && Serial.available(); ++i) {
    const char command = static_cast<char>(Serial.read());
    if (command == 'c') { board::toggleColor(); dirty = true; }
    else dirty = playback.command(command) || dirty;
  }
  dirty = playback.command(board::buttonCommand(now, playback.stage == pocket::Stage::Eyes)) || dirty;
  if (dirty || (!playback.paused && uint32_t(now - lastFrame) >= pocket::kFrameMs)) {
    lastFrame = now;
    board::clear();
    pocket::render(board::surface(), board::width(), board::height(),
                   playback.stage, playback.elapsed);
    board::present();
    dirty = false;
  }
  delay(1);
}
