git clone https://github.com/ARMmbed/mbed-os.git mbed-os
cd mbed-os
git checkout mbed-os-6.16.0-rc1
git apply --check ../mbed_lib.json.patch
git apply  ../mbed_lib.json.patch
