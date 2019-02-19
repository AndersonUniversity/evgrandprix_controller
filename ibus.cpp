#include "ibus.hpp"

/*
  The data is serial data, 115200, 8N1.
  Messages arrive every 7 milliseconds, and are read constantly until a
  few tenths of a second after the transmitter is switched off.

  Packet format:
  20 40 CH0 CH1 CH2 CH3 CH4 CH5 CH6 CH7 CH8 CH9 CH10 CH11 CH12 CH13 SUM
  Channels are stored in little endian byte order.  Unused channels read
  5DC (first byte DC, second byte 05).

  Channel 0: Right horizontal: 3E8 -> 7D0
  Channel 1: Right vertical:   3E8 -> 7CF
  Channel 2: Left vertical:    3E8 -> 7D0
  Channel 3: Left horizontal:  3E8 -> 7CD
  Channel 4: Left pot:         3E8 -> 7D0
  Channel 5: Right pot:        3E8 -> 7D0

  The checksum starts at 0xFFFF, then subtract each byte except the
  checksum bytes.
*/


iBUS::iBUS(uint_fast8_t channel_count):
state(0),
channel_count(channel_count)
{}

int iBUS::read(uint16_t* data, uint8_t ch) {
  switch (state) {
  case 0:
    if (ch == 0x20) {
      checksum = 0xFFFF - 0x20;
      state = 1;
    }
    break;
  case 1:
    if (ch == 0x40) {
      state = 2;
      checksum -= ch;
    } else {
      // Unknown packet type
      state = 0;
    }
    break;
  case 30:
    datal = ch;
    state = 31;
    break;
  case 31: {
    uint_fast16_t calc_checksum = (ch << 8) | datal;
    state = 0;
    if (checksum == calc_checksum)
      return 0;
  } break;
  default:
    // Ignore these bytes if we've filled all of the channels
    if (state / 2 <= channel_count) {
      if ((state & 1) == 0) {
        // Data low byte
        datal = ch;
      } else {
        // Data high byte
        data[(state / 2) - 1] = (ch << 8) | datal;
      }
    }
    checksum -= ch;
    ++state;
    break;
  }

  return -1;
}
