set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR armv7)

set(triple arm-linux-gnueabihf)

set(tools /home/deps/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf)

set(CMAKE_C_COMPILER ${tools}/bin/arm-none-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-none-linux-gnueabihf-g++)
set(CMAKE_AR ${tools}/bin/arm-none-linux-gnueabihf-ar CACHE PATH "" FORCE)
set(CMAKE_RANLIB ${tools}/bin/arm-none-linux-gnueabihf-ranlib CACHE PATH "" FORCE)
set(CMAKE_LINKER ${tools}/bin/arm-none-linux-gnueabihf-ld CACHE PATH "" FORCE)
set(CMAKE_SIZE ${tools}/bin/arm-none-linux-gnueabihf-size)
set(CMAKE_OBJCOPY ${tools}/bin/arm-none-linux-gnueabihf-objcopy)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_C_FLAGS "-I ${tools}/arm-none-linux-gnueabihf/libc/usr/include")
set(CMAKE_CXX_FLAGS "-I ${tools}/arm-none-linux-gnueabihf/libc/usr/include")