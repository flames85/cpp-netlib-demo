#include <boost/network/protocol/http/client.hpp>
#include <boost/network/uri.hpp>
#include <string>
#include <fstream>
#include <iostream>

namespace http = boost::network::http;
namespace uri = boost::network::uri;

namespace {
    std::string get_filename(const uri::uri &url) {
        std::string path = uri::path(url);
        std::size_t index = path.find_last_of('/');
        std::string filename = path.substr(index + 1);
        return filename.empty() ? "index.html" : filename;
    }
}  // namespace

int main()
{
    http::client client;
    try
    {
        http::client::request request("http://127.0.0.1:8000/file_server");
        http::client::response response = client.get(request);

        // 从url中获取文件名
        std::string filename = get_filename(request.uri());
        std::cout << "Saving to: " << filename << std::endl;
        // 从get的body中写入文件
        std::ofstream ofs(filename.c_str());
        ofs << static_cast<std::string>(body(response));
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}