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
sudo make install
# 我不知道为什么这块头文件不能通过make install部署。
sudo cp ../deps/asio/asio/include/asio /opt/cpp-netlib/include -r
sudo cp ../deps/asio/asio/include/asio/asio.hpp /opt/cpp-netlib/include 
```


#### 3. 官方的例子

位置在`cpp-netlib-0.12.0-final/libs/network/example`

官方网页上的例子有些问题. 

#### 4. 根据官方例子写的更详细的例子, 都是http协议相关

+ http_server (http服务)
+ http_client (作为http_server测试客户端)
+ work_queue_http_server(http多线程加队列多线程处理消息服务)
+ file_server (文件下载在服务)
+ file_download (作为file_server测试客户端)
+ ssl_server (https服务)
+ ssl_client (作为ssl_server测试客户端)
