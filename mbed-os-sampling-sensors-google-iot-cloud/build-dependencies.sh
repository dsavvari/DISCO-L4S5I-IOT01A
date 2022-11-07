PRIVATE_KEYS_DIR="private"
MBED_OS_DIR="mbed-os"
GOOGLE_IOT_DIR="mbed-client-for-google-iot-cloud"
GOOGLE_IOT_SDK_DIR="mbed-client-for-google-iot-cloud/iot-device-sdk-embedded-c"

NTP_CLIENT_DIR="ntp-client"

if [ -d "$MBED_OS_DIR" ];
then
	echo "[$MBED_OS_DIR] exist.[OK]"
else
	git clone https://github.com/ARMmbed/mbed-os.git mbed-os
	cd mbed-os
	git checkout mbed-os-6.16.0-rc1
	git apply --check ../mbed_lib.json.patch
	git apply  ../mbed_lib.json.patch
	cd ..
fi

if [ -d "$GOOGLE_IOT_DIR" ];
then
	echo "[$GOOGLE_IOT_DIR] exist.[OK]"
else
	git clone https://github.com/ARMmbed/mbed-client-for-google-iot-cloud
	cd mbed-client-for-google-iot-cloud
	git checkout cbf19de
	cd ..
fi

if [ -d "$GOOGLE_IOT_SDK_DIR" ];
then
	echo "[$GOOGLE_IOT_SDK_DIR] exist.[OK]"
else
	cd "$GOOGLE_IOT_DIR" 
	git clone https://github.com/GoogleCloudPlatform/iot-device-sdk-embedded-c.git/
	cd iot-device-sdk-embedded-c
	git checkout 32b03168
	cd ../../
fi

if [ -d "$NTP_CLIENT_DIR" ];
then
	echo "[$NTP_CLIENT_DIR] exist.[OK]"
else
	git clone https://github.com/ARMmbed/ntp-client.git
	cd ntp-client
	git checkout e919cfb
	cd ..
fi

if [ -d "$PRIVATE_KEYS_DIR" ];
then
	# create the private keys and update the following files
	cp private/mbed_app.json .
	cp private/google_cloud_credentials.h .
else
	echo "Error [$PRIVATE_KEYS_DIR] don't exist. Please create the secret keys first"
fi
