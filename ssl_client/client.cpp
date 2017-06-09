#include <boost/network/protocol/http/client.hpp>
#include <iostream
#include <asio/ssl.hpp>

namespace http = boost::network::http;

// todo did not run well
int main() {
    try {
        http::client::options options;
        http::client client(options.openssl_verify_path("../../my_cert/rootCA.pem"));

        http::client::request request("https://127.0.0.1:3344");
        http::client::response response = client.get(request);

        std::cout << status(response) << " " << status_message(response) << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
