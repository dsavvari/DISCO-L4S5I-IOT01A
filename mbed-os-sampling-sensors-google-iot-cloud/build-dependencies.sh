PRIVATE_KEYS_DIR="private"
MBED_OS_DIR="mbed-os"
GOOGLE_IOT_DIR="mbed-client-for-google-iot-cloud"
GOOGLE_IOT_SDK_DIR="mbed-client-for-google-iot-cloud/iot-device-sdk-embedded-c"
WIFI_DIR="wifi-ism43362"
NTP_CLIENT_DIR="ntp-client"

if [ -d "$MBED_OS_DIR" ];
then
	echo "[$MBED_OS_DIR] exist.[OK]"
else
	git clone https://github.com/ARMmbed/mbed-os.git mbed-os
	cd "$MBED_OS_DIR"
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
	cd "$GOOGLE_IOT_DIR"
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
	cd "$NTP_CLIENT_DIR"
	git checkout e919cfb
	cd ..
fi

if [ -d "$WIFI_DIR" ];
then
	echo "[$WIFI_DIR] exist.[OK]"
else
	git clone https://github.com/ARMmbed/wifi-ism43362/
	cd "$WIFI_DIR"
	git checkout 3813a4bb8623cc9b0
	cd ..
fi

if [ -d "$PRIVATE_KEYS_DIR" ];
then
	# create the private keys and update the following files
	cp $PRIVATE_KEYS_DIR/mbed_app.json .
	cp $PRIVATE_KEYS_DIR/google_cloud_credentials.h .
else
	echo "Error [$PRIVATE_KEYS_DIR] don't exist. Please create the secret keys first"
fi
