
export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/darwin-x86_64
export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64
export SYS_ROOT="$ANDROID_NDK/platforms/android-27/arch-arm64"

# Only choose one of these, depending on your device...
# export TARGET=armv7a-linux-androideabi
export TARGET=aarch64-linux-android
# Set this to your minSdkVersion.
export API=27
# Configure and build.
export AR=$TOOLCHAIN/bin/llvm-ar
export CC=$TOOLCHAIN/bin/$TARGET$API-clang
export AS=$CC
export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
export LD=$TOOLCHAIN/bin/ld
export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
export STRIP=$TOOLCHAIN/bin/llvm-strip

export OPENSSL_ROOT_DIR=/usr/local/android
export ANDROID_PLATFORM=android-27

export INSTALL_DIR=/usr/local/android
    # -DANDROID_ABI="armeabi-v7a" \
    # -DANDROID_TOOLCHAIN_NAME="armv7a-linux-androideabi" \
    # -DENABLE_SSL=OFF \
    # -DCMAKE_FRAMEWORK_PATH=/usr/local/android/lib/cmake/ \
    # -DCMAKE_FIND_DEBUG_MODE=ON \
cmake -DANDROID=true \
    -DCycloneDDS_DIR=/usr/local/android/lib/cmake/CycloneDDS \
    -DCycloneDDS-CXX_DIR=/usr/local/android/lib/cmake/CycloneDDS-CXX \
    -DDataTypesCloud_DIR=/usr/local/android/lib/cmake/DataTypesCloud \
    -DDataTypesHvac_DIR=/usr/local/android/lib/cmake/DataTypesHvac \
    -DDataTypesCabin_DIR=/usr/local/android/lib/cmake/DataTypesCabin \
    -DDataTypesBattery_DIR=/usr/local/android/lib/cmake/DataTypesBattery \
    -DDataTypesDrive_DIR=/usr/local/android/lib/cmake/DataTypesDrive \
    -DDataTypesHabit_DIR=/usr/local/android/lib/cmake/DataTypesHabit \
    -DDataTypesSeat_DIR=/usr/local/android/lib/cmake/DataTypesSeat \
    -DDataTypesWindow_DIR=/usr/local/android/lib/cmake/DataTypesWindow \
    -DDataTypesStd_DIR=/usr/local/android/lib/cmake/DataTypesStd \
    -Dfmt_DIR=/usr/local/android/lib/cmake/fmt \
    -DCMAKE_CROSSCOMPILING=true \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
    -DANDROID_NDK=$ANDROID_NDK \
    -DANDROID_PLATFORM=$ANDROID_PLATFORM \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_NDK_HOST_X64="YES" \
    -DANDROID_TOOLCHAIN_NAME="aarch64-linux-android" \
    -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
    -DOPENSSL_CRYPTO_LIBRARY="/usr/local/android/lib/libcrypto.so" \
    -DOPENSSL_INCLUDE_DIR="$OPENSSL_ROOT_DIR/include" \
    -DOPENSSL_SSL_LIBRARY="$OPENSSL_ROOT_DIR/lib/libssl.so" \
    ..


