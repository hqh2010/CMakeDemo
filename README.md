# 说明

This is a cmake test.

演示libcurl、glib2.0、gio2.0库的引用。

# 使用方式

mkdir build

cd build

cmake ..

开Debug模式命令

cmake .. -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_DEBUG_ASAN=ON

make -j16

运行
./bin/llcmd

注意:
也可以通过提供的makefile文件直接构建目标二进制

# 清理

make clean

# 参考文档

[https://github.com/chaneyzorn/CMake-tutorial](https://github.com/chaneyzorn/CMake-tutorial)

[https://www.cnblogs.com/zhoudayang/p/5458861.html](https://www.cnblogs.com/zhoudayang/p/5458861.html)

[https://blog.csdn.net/daijingxin/article/details/111826612](https://blog.csdn.net/daijingxin/article/details/111826612)
