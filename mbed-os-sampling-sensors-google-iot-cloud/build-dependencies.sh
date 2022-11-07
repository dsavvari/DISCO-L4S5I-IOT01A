DIR="private"

git clone https://github.com/ARMmbed/mbed-os.git mbed-os
cd mbed-os
git checkout mbed-os-6.16.0-rc1
git apply --check ../mbed_lib.json.patch
git apply  ../mbed_lib.json.patch

cd ..
git clone https://github.com/ARMmbed/mbed-client-for-google-iot-cloud
cd mbed-client-for-google-iot-cloud
git checkout cbf19de

cd ..
git clone https://github.com/ARMmbed/ntp-client.git
cd ntp-client
git checkout e919cfb

cd ..
if [ -d "$DIR" ];
then
# create the private keys and update the following files
    cp private/mbed_app.json .
    cp private/google_cloud_credentials.h .
else
    echo "Error [$DIR] don't exist. Please create the secret keys first"
fi
