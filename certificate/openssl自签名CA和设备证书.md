### openssl自签名CA证书和设备证书

#### 1. 生成CA证书
+ 创建私钥 [ca-key.pem]
```bash
openssl genrsa -out ca-key.pem 1024
```
+  可以为私钥添加密码(该操作可选, 添加完密码后, 凡是使用私钥都需要密码)
```bash
openssl genrsa -des3 -out ca-key.pem 1024
```
+ 创建证书请求[ca-req.csr]
```bash
openssl req -new -out ca-req.csr -key ca-key.pem

Country Name (2 letter code) [AU]:
State or Province Name (full name) [Some-State]:
Locality Name (eg, city) []:
Organization Name (eg, company) [Internet Widgits Pty Ltd]:hyc
Organizational Unit Name (eg, section) []:it
Common Name (e.g. server FQDN or YOUR name) []:
Email Address []:
A challenge password []:      
An optional company name []:
```
+ 自签名[ca-cert.pem]
```bash
openssl x509 -req -in ca-req.csr -out ca-cert.pem -signkey ca-key.pem -days 3650
```
+ 将私钥和证书导出成浏览器支持的.p12格式[ca.p12]
```bash
openssl pkcs12 -export -clcerts -in ca-cert.pem -inkey ca-key.pem -out ca.p12
```
#### 2. 生成设备证书(服务端或客户端) 
+ 创建私钥 [device-key.pem]
```bash
openssl genrsa -out device-key.pem 1024
```
+  创建证书请求 [device-req.csr]
```bash
openssl req -new -out device-req.csr -key device-key.pem

Country Name (2 letter code) [AU]:
State or Province Name (full name) [Some-State]:
Locality Name (eg, city) []:
Organization Name (eg, company) [Internet Widgits Pty Ltd]:
Organizational Unit Name (eg, section) []:
Common Name (e.g. server FQDN or YOUR name) []:127.0.0.1
Email Address []:
A challenge password []:
An optional company name []:
```
+ 自签署证书 [device-cert.pem]
```bash
openssl x509 -req -in device-req.csr -out device-cert.pem -signkey device-key.pem -CA ca-cert.pem -CAkey ca-key.pem -CAcreateserial -days 3650
```
+ 将私钥和证书导出成浏览器支持的.p12格式[device.p12]
```bash
openssl pkcs12 -export -clcerts -in device-cert.pem -inkey device-key.pem -out device.p12  
```

#### 3. 生成实例

```bash
openssl genrsa -out ca-key.pem 1024
openssl req -new -out ca-req.csr -key ca-key.pem
-----
Country Name (2 letter code) [AU]:
State or Province Name (full name) [Some-State]:
Locality Name (eg, city) []:
Organization Name (eg, company) [Internet Widgits Pty Ltd]:abc
Organizational Unit Name (eg, section) []:it
Common Name (e.g. server FQDN or YOUR name) []:
Email Address []:
A challenge password []:      
An optional company name []:
-----
openssl x509 -req -in ca-req.csr -out ca-cert.pem -signkey ca-key.pem -days 3650


openssl genrsa -out server-key.pem 1024
openssl req -new -out server-req.csr -key server-key.pem
-----
Country Name (2 letter code) [AU]:
State or Province Name (full name) [Some-State]:
Locality Name (eg, city) []:
Organization Name (eg, company) [Internet Widgits Pty Ltd]:
Organizational Unit Name (eg, section) []:
Common Name (e.g. server FQDN or YOUR name) []:127.0.0.1
Email Address []:
A challenge password []:
An optional company name []:
-----
openssl x509 -req -in server-req.csr -out server-cert.pem -signkey server-key.pem -CA ca-cert.pem -CAkey ca-key.pem -CAcreateserial -days 3650


openssl genrsa -out client-key.pem 1024
openssl req -new -out client-req.csr -key client-key.pem
-----
Country Name (2 letter code) [AU]:
State or Province Name (full name) [Some-State]:
Locality Name (eg, city) []:
Organization Name (eg, company) [Internet Widgits Pty Ltd]:
Organizational Unit Name (eg, section) []:
Common Name (e.g. server FQDN or YOUR name) []:127.0.0.1
Email Address []:
A challenge password []:
An optional company name []:
-----
openssl x509 -req -in client-req.csr -out client-cert.pem -signkey client-key.pem -CA ca-cert.pem -CAkey ca-key.pem -CAcreateserial -days 3650
```

说明:

1. 生成CA的Organization Name/Organizational Unit Name2个属性一定不可以和设备的这2个属性加起来一样. 否则认证会失败. 
2. 生成设备的Common Name需要是https的域名或者ip地址. 否则认证会失败. 
3. 其他信息可以忽略.

#### 4. 文件使用
```
ca-cert.pem                                 [CA证书, 用于client/server]
server-cert.pem                          [服务端证书/公钥]
server-key.pem                           [服务端密钥]
client-cert.pem                           [客户端证书/公钥]
client-key.pem                            [客户端密钥]
```
__如果给私钥设置了密码, 如:__
```bash
openssl genrsa -des3 -out server-key.pem 1024
密码:test
```
则, 在使用它时必须带上密码:
+ curl命令行
```bash
curl -vvvv https://127.0.0.1:8899 --cacert ca-cert.pem --cert client-cert.pem --key client-key.pem --pass test
```
+ openssl编程
```cpp
char password[64] = "test";
SSL_CTX_set_default_passwd_cb_userdata(ssl_ctx_, password);
```
+ cpp-netlib编程
```cpp
std::shared_ptr<asio::ssl::context> ctx = std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);
ctx->set_password_callback(password_callback);
```
+ boost编程
```cpp
ssl::context::set_password_callback()
```

__如果想去掉私钥的密码:__
```bash
openssl rsa -in server-key.pem -out server-key.pem
```
