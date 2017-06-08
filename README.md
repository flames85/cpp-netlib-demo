#### 1. cpp-netlib网站

官网：http://cpp-netlib.org

github：https://github.com/cpp-netlib/cpp-netlib

#### 2. 编译cpp-netlib

依赖：

 + boost（[cpp-netlib-0.12.0-final ](https://github.com/cpp-netlib/cpp-netlib/releases/tag/cpp-netlib-0.12.0-final)版需要boost1.57）
+ openssl(version>=1.0.2, 用1.0.1版本 make test 失败)
+ cmake版本>=2.8
+ 支持c++11的c++编译器



安装boost：

```bash
sudo apt-get install libboost-all-dev
```

安装openssl：

```bash
sudo apt-get install openssl
sudo apt-get install libssl-dev
```

**boost和openssl都安装在默认目录，BOOST_ROOT和OPENSSL_ROOT_DIR都不需要设置。**

通过命令行编译：

```bash
cd cpp-netlib-0.12.0-final
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Debug \
  -DCPP-NETLIB_ENABLE_HTTPS=ON \
  -DCPP-NETLIB_BUILD_TESTS=ON \
  -DCPP-NETLIB_BUILD_EXAMPLES=ON \
  -DCMAKE_CXX_FLAGS="-std=c++11 ${CMAKE_CXX_FLAGS}" \
  -DCMAKE_INSTALL_PREFIX=/opt/cpp-netlib \
  ..
  
make -j4
make test
make install
# 我不知道为什么这块头文件不能通过make install部署。
sudo cp /home/shaoqi/source/cpp-netlib-0.12.0-final/deps/asio/asio/include/asio /opt/cpp-netlib/include -r
```


#### 3. 官方的例子

位置在`cpp-netlib-0.12.0-final/libs/network/example`

官方网页上的例子是旧版的!(可是为何都0.12版了，还在用老版的根本编译不过的example？)

#### 4. 根据官方例子写的更详细的例子

+ http_client
+ http_server

