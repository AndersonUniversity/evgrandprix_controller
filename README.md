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
This will fail to install a non-essential package (cmsis-pack-manager).  The package requires cargo (from rust).
```
git clone https://github.com/ARMmbed/cmsis-pack-manager.git
cd cmsis-pack-manager
python setup.py install
```
But this fails to install due to needing cargo (rust).

Simply remove the package from mbed-os/requirements.txt and install with
`pip install -r requirements.txt`


### MBED Information
[printf for debugging](https://os.mbed.com/docs/mbed-os/v5.9/tutorials/debugging-using-printf-statements.html)

[mbed serial API](https://os.mbed.com/docs/mbed-os/v5.10/apis/serial.html#serial-class-reference)
