#include "CommandMsg.hpp"
#include "Log.hpp"

void CommandMsg::dump() const {
  // logging
  char gear_ch = 'N';
  if(gear == Gear::forward) gear_ch = 'F';
  else if(gear == Gear::reverse) gear_ch = 'R';
  LOG("CMD: %c %.2f %.2f %.2f \r\n", gear_ch, steering, throttle_regen, ebrake);
}
