# x86_64 build
g++ -g ace_refcount_ptr.cpp -lACE -o ace_refcount_ptr.out -I/usr/local/include/ -L/usr/local/lib

# arm_32 build
/home/developer/gateway_1410s/bitbake_build/tmp/sysroots/x86_64-linux/usr/bin/arm-wrs-linux-gnueabi/arm-wrs-linux-gnueabi-g++ -march=armv7-a -mfloat-abi=hard -mfpu=neon  -marm -mthumb-interwork -mtune=cortex-a7 -g ace_refcount_ptr.cpp -lACE -o ace_refcount_ptr.out --sysroot=/home/developer/gateway_1410s/bitbake_build/tmp/sysroots/fsl-ls10xx
