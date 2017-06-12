#include <memory>
#include <boost/network/include/http/server.hpp>
#include <asio.hpp>

struct handler;
typedef boost::network::http::server<handler> server;

std::string password_callback(std::size_t max_length,
                              asio::ssl::context_base::password_purpose purpose)
{
    switch(purpose) {
        case asio::ssl::context_base::for_reading:
            std::cout << "password purpose for reading \n" ;
            break;
        case asio::ssl::context_base::for_writing:
            std::cout << "password purpose for writing \n" ;
            break;
        default:
            std::cout << "password purpose for unknow \n" ;
            break;
    }

    std::cout << "password_callback!\n";
    return std::string("test");
}

struct handler
{
    void operator()(server::request const& req,
                    const server::connection_ptr& conn) {
        static server::response_header headers[] = {{"Connection", "close"},
                                                    {"Content-Type", "text/plain"},
                                                    {"From", "ssl-server-demo"},
                                                    {"Content-Length", "0"}};
        // 更新content-length
        std::string body = "hello !!!";
        headers[3].value = std::to_string(body.length());

        // status
        conn->set_status(server::connection::ok);
        // headers
        conn->set_headers(boost::make_iterator_range(headers, headers + 4));

        // body
        conn->write(body);
    }
};

/**
 * Clean shutdown signal handler
 *
 * @param error
 * @param signal
 * @param p_server_instance
 */
void shut_me_down(const std::error_code& error, int signal,
                  std::shared_ptr<server> p_server_instance)
{
    std::cout << "ok! i will shut down!\n";
    if (!error) p_server_instance->stop();
}

// 双向认证
#define VERIFY_CLIENT

// 测试: curl -v https://127.0.0.1:3344 --cacert ca-cert.pem
// 测试: curl -vvvv https://127.0.0.1:3344 --cacert ca-cert.pem --cert client-cert.pem --key client-key.pem
int main(void) try
{
    // setup asio::io_service
    std::shared_ptr<asio::io_service> p_io_service(std::make_shared<asio::io_service>());

    // Initialize SSL context
    std::shared_ptr<asio::ssl::context> ctx = std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);

    ctx->set_options(asio::ssl::context::default_workarounds |
                     asio::ssl::context::no_sslv2 |
                     asio::ssl::context::single_dh_use);

    // 设置私钥的密码回调函数(会在private_key_file需要密码的时候调用)
    ctx->set_password_callback(password_callback);

    ctx->load_verify_file("../../certificate/ca-cert.pem"); // ca
    ctx->use_certificate_file("../../certificate/server-cert.pem", asio::ssl::context::pem); // server cert
    ctx->use_private_key_file("../../certificate/server-key.pem", asio::ssl::context::pem); // server private key

#ifdef VERIFY_CLIENT
    // 双向认证
    ctx->set_verify_mode(SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT);
#endif

    // setup the async server
    handler request_handler;
    std::shared_ptr<server> p_server_instance(std::make_shared<server>(server::options(request_handler)
                                                                               .address("0.0.0.0")
                                                                               .port("3344")
                                                                               .io_service(p_io_service)
                                                                               .reuse_address(true)
                                                                               .thread_pool(std::make_shared<boost::network::utils::thread_pool>(2))
                                                                               .context(ctx)));

    // setup clean shutdown
    asio::signal_set signals(*p_io_service, SIGINT, SIGTERM);
    signals.async_wait([=] (std::error_code const &ec, int signal) {
        shut_me_down(ec, signal, p_server_instance);
    });

    // run the async server
    p_server_instance->run();

    // we are stopped - shutting down
    p_io_service->stop();

    std::cout << "Terminated normally" << std::endl;
    exit(EXIT_SUCCESS);
}
catch (const std::exception& e) {
    std::cout << "Abnormal termination - exception:" << e.what() << std::endl;
    exit(EXIT_FAILURE);
}
