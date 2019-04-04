# Controller Software Stack

## Install
This is the install procedure on the Jetson
```
sudo apt-get install python3-venv python3-dev mercurial libffi-dev libxml2-dev libxslt1-dev libssl-dev
python3 -m venv myenv
source myenv/bin/activate
pip install wheel
pip install mbed-cli
mbed target -g NUCLEO_L432KC
mbed toolchain -g GCC_ARM
```

## cmsis-pack-manager Issue
This will fail to install a non-essential package (cmsis-pack-manager) because there is no wheel.

Simply remove the cmsis-pack-manager line from mbed-os/requirements.txt and install with
`pip install -r requirements.txt`

### This approach did not work
The package requires cargo (from rust).
```
git clone https://github.com/ARMmbed/cmsis-pack-manager.git
cd cmsis-pack-manager
python setup.py install
```
But this fails to install due to needing cargo (rust).


## Toolchain Issue
We are being hit by [this](https://bugs.launchpad.net/ubuntu/+source/newlib/+bug/1767223) bug in Ubuntu 18.04 when we try to `mbed compile`.

Using a new deb packages works fine.
```
wget http://mirrors.kernel.org/ubuntu/pool/universe/n/newlib/libnewlib-dev_3.0.0.20180802-2_all.deb
wget http://mirrors.kernel.org/ubuntu/pool/universe/n/newlib/libnewlib-arm-none-eabi_3.0.0.20180802-2_all.deb
sudo dpkg -i *.deb
```

## Usage

Setup `source myenv/bin/activate`

Compile and flash `mbed compile --profile develop.json --flash`

Console `mbed sterm -b 115200 -p /dev/ttyUSB0`



### MBED Information
[printf for debugging](https://os.mbed.com/docs/mbed-os/v5.9/tutorials/debugging-using-printf-statements.html)

[mbed serial API](https://os.mbed.com/docs/mbed-os/v5.10/apis/serial.html#serial-class-reference)
