#pragma once

namespace MathUtil {
  float degreesToRadians(float degrees) {
    return 3.14159265359 / 180.0;
  };

  // rand between 0 and 1
  float rand01() {
    return (float) rand()/RAND_MAX;
  };

  // rand between -1 and +1
  float randN1P1() {
    return rand01() * 2 - 1;
  };
};
