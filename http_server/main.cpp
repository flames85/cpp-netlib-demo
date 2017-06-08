#include <iostream>
#include <sstream>
// cpp-netlib
#include <boost/network/protocol/http/server.hpp>
// boost
#include <boost/range/algorithm/find_if.hpp>

namespace http = boost::network::http;

/*<< Defines the server. >>*/
struct hello_world;
typedef http::server<hello_world> server;

/*<< Defines the request handler.  It's a class that defines two
     functions, `operator()` and `log()` >>*/
struct hello_world {
    struct is_from {
        template <class Header>
        bool operator()(Header const& header) {
            return boost::iequals(header.name, "From");
        }
    };

    /*<< This is the function that handles the incoming request. >>*/
    void operator()(server::request const &request, server::connection_ptr connection) {

        //! 1. request
        server::string_type remote_addr = source(request);
        std::cout << "client: " << remote_addr;
        std::cout << "; path: " << request.destination;
        std::cout << "; method: " << request.method;
        std::cout << std::endl;

        // 遍历方式查看request headers
        for (auto const& header : request.headers) {
            std::cout << "request header: " << header.name << ": " << header.value << std::endl;
        }

        // 搜索方式查看request headers
        server::request::headers_container_type::iterator found =
                boost::find_if(request.headers, is_from());
        if (found != request.headers.end()) {
            std::cout << "found from header: " << found->name << ": " << found->value << std::endl;
        } else {
            std::cerr << "not found from header" << std::endl;
        }

        //! 2. response
        static server::response_header headers[] = {{"Connection", "close"},
                                                    {"Content-Type", "text/plain"},
                                                    {"From", "server-demo"},
                                                    {"Content-Length", "0"}};

        // status
        connection->set_status(server::connection::ok);
        // headers
        connection->set_headers(boost::make_iterator_range(headers, headers + 4));

        // 更新content-length
        std::string body = "hello !!!";
        headers[3].value = std::to_string(body.length());
        // body
        connection->write(body);
    }
    void log(const server::string_type& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

int main() {
    try {
        /*<< Creates the request handler. >>*/
        hello_world handler;
        /*<< Creates the server. >>*/
        server::options options(handler);
        // 地址复用
        options.reuse_address(true);
        server server_(options.address("0.0.0.0").port("12345"));
        /*<< Runs the server. >>*/
        server_.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
