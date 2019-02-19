# iBUS MBED Controller Software

### Data Capture
Use 5V FTDI cable [TTL-232R-5V](https://www.ftdichip.com/Support/Documents/DataSheets/Cables/DS_TTL-232R_CABLES.pdf)
Black to ground and yellow (Rx) to signal line in iBUS connector.

`screen -L /dev/tty.usbXYZ 115200`

Take a look at file screenlog.0 for the binary serial data.

[FlySky iBUS protocol description](http://blog.dsp.id.au/posts/2017/10/22/flysky-ibus-protocol/)
[iBUS parsing library written in C](https://github.com/33d/ibus-library/)
