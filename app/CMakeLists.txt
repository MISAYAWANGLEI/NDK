
cmake_minimum_required(VERSION 3.4.1)
#src/main/cpp/native-lib.cpp 会编译为libnative-lib.so动态库
#SHARED编译动态库 STATIC:静态库
add_library( # Sets the name of the library.So库名称
             native-lib
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp)

#引入静态库
# IMPORTED: 表示静态库是以导入的形式添加进来(预编译静态库)
add_library(StaticTest STATIC IMPORTED)

message("CMAKE_SOURCE_DIR : ${CMAKE_SOURCE_DIR}")
message("ANDROID_ABI : ${ANDROID_ABI}")
#设置静态库的导入路径
set_target_properties(StaticTest PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/static/armeabi-v7a/libStaticTest.a)

# CMAKE_CXX_FLAGS 会传给c++编译器
# CMAKE_C_FLAGS 会传给c编译器
# CMAKE_SOURCE_DIR 的值是当前CMakelist.txt所在目录
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a")

#find_library( # Sets the name of the path variable.
#              log-lib
#              log )
#生成native-lib动态库需要用到Test StaticTest log动态或者静态库
target_link_libraries( # Specifies the target library.
                       native-lib
                       # libTest.so 可以去掉lib与.so
                       Test
                       StaticTest
                       # Links the target library to the log library
                       # included in the NDK.
                       log )