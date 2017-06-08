#include <boost/network/protocol/http/client.hpp>
#include <iostream>
#include <boost/network/uri.hpp>
#include <boost/network/uri/uri_io.hpp>


namespace http = boost::network::http;
namespace uri = boost::network::uri;

int main() {
    try {
        //! 1. request
        // client
        http::client client;
        // make url
        uri::uri url;
        url << uri::scheme("http") << uri::host("127.0.0.1:12345") << uri::path("/cpp-netlib");
        std::cout << "url: " << url << std::endl;
        // set url
        http::client::request request(url);
        // add header
        request.add_header(std::make_pair("From", "client-demo"));
        request.add_header(std::make_pair("To", "server-demo"));

        // request and get response
        http::client::response response = client.get(request);

        //! 2. response
        // response status
        std::cout << status(response) << " " << status_message(response) << std::endl;

        // 遍历方式查看response headers
        auto headers = response.headers();
        for (auto const& header : headers) {
            std::cout << "response header: " << header.first << ": " << header.second << std::endl;
        }

        // 搜索方式查看response headers(multimap的搜索方式)
        auto beg = headers.equal_range("From").first;
        auto end = headers.equal_range("From").second;
        if(beg != end) {
            for(auto m = beg; m != end; m++) {
                std::cout << "found from header: " << m->first << "--" << m->second << std::endl;
            }
        } else {
            std::cerr << "not found from header" << std::endl;
        }

        // response body
        std::cout << body(response) << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
