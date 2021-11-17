#!/bin/bash

#cyclonedds
adb push libddsc.so  libdds_security_ac.so  libdds_security_auth.so  libdds_security_crypto.so  /system/lib64

#cyclonedds-cxx
adb push libddscxx.so /system/lib64

#openssl
adb push /usr/local/android/lib/libssl.so.1.1  /system/lib64/
adb push /usr/local/android/lib/libcrypto.so.1.1  /system/lib64/

#hu-demo
adb push hu-demo /system/bin
adb shell mkdir /etc/cyclonedds/
adb push config.xml /etc/cyclonedds/
