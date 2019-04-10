# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.So库名称
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp)

#设置一个变量
# CMAKE_CXX_FLAGS 会传给c++编译器
# CMAKE_C_FLAGS 会传给c编译器
# CMAKE_CXX_FLAGS=--sysroot=XX
# 重新定义  CMAKE_CXX_FLAGS 变量  +
# CMAKE_SOURCE_DIR 的值是当前这个文件的地址
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a")

find_library( # Sets the name of the path variable.
              log-lib
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
target_link_libraries( # Specifies the target library.
                       native-lib
                       Test
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )