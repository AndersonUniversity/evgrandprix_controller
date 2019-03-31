# iBUS

## Protocol (messaging format)

Data is transmitted as serial UART data, 115200bps, 8N1. A message is sent every 7 milliseconds. My receiver sends this over its white wire, and stops sending a few tenths of a second after the transmitter is switched off (unlike the PPM signal on the yellow wire, which keeps sending its last value).

The first byte is 0x20, the second is 0x40.

Next are 14 pairs of bytes, which is the channel value in little endian byte order. The FS-i6 is a 6 channel receiver, so it fills in the first 6 values. The remainder are set to 0x05DC. My transmitter sends values between 0x03E8 (1000) and 0x07D0 (2000).  They are the pulse widths in microseconds that would be sent a normal PWM signals.  0x05DC (1500) is the middle/idle value.

Finally a 2 byte checksum is sent. It’s in little endian byte order, it starts at 0xFFFF, from which every byte’s value is subtracted except for the checksum.


### Data Capture
Use 5V FTDI cable [TTL-232R-5V](https://www.ftdichip.com/Support/Documents/DataSheets/Cables/DS_TTL-232R_CABLES.pdf)
Black to ground and yellow (Rx) to signal line in iBUS connector.

`screen -L /dev/tty.usbXYZ 115200`

Take a look at file screenlog.0 for the binary serial data.

[FlySky iBUS protocol description](http://blog.dsp.id.au/posts/2017/10/22/flysky-ibus-protocol/)
[iBUS parsing library written in C](https://github.com/33d/ibus-library/)
