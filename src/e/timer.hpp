#pragma once

const int SECONDS = 1000;

struct Timer {
  Timer(int defaultDuration = 0);

  void go();
  void go(int d);
  void stop();
  bool done();
  int now();

  int defaultDuration;

  bool running = false;
  int start = 0;
  int duration = 0;
};
