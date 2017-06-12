#include <boost/network/protocol/http/client.hpp>

namespace http = boost::network::http;

int main() {
    try {
        http::client::options options;
        http::client client(options.openssl_certificate("../../certificate/ca-cert.pem") // ca
                                    .openssl_certificate_file("../../certificate/client-cert.pem") // client cert
                                    .openssl_private_key_file("../../certificate/client-key.pem") // client private key
//                                    .openssl_ciphers("RC4-MD5")
        );

        http::client::request request("https://127.0.0.1:3344");
        http::client::response response = client.post(request);

        std::cout << response.version() << " " << status(response) << " " << status_message(response) << std::endl;
        std::cout << body(response) << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << "Abnormal termination - exception:" << e.what() << std::endl;
        return 1;
    }

    return 0;
}
