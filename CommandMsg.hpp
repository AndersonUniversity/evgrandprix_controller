#pragma once

#include <mbed.h>

enum Gear {forward=1, reverse=-1, neutral=0};

struct CommandMsg{

  float steering; // [-1 to 1], corresponding to [left, right]
  float throttle_regen; // [-1, 1] corresponding to [full regen, full throttle]
  float ebrake; // [0, 1] corresponding to [no brake, full brake]
  Gear gear;

  CommandMsg():
  steering(0.0f),
  throttle_regen(0.0f),
  ebrake(0.0f),
  gear(neutral)
  {}

  void dump() const;
};
