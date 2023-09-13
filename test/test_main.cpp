#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <spdlog/spdlog.h>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

int rollDice() {
    // Implement your dice rolling logic here
    int sides = 6;
    return rand() % sides + 1;
}

std::string ReadClientSecretFromConfig() {
    std::ifstream config_file("config.json");
    nlohmann::json config_json;

    if (config_file.is_open()) {
        try {
            config_file >> config_json;
        } catch (const std::exception& ex) {
            spdlog::error("Failed to parse configuration file: {}", ex.what());
            return "";
        }
    } else {
        spdlog::error("Failed to open configuration file.");
        return "";
    }

    std::string client_secret = config_json.value("client_secret", "");
    if (client_secret.empty()) {
        spdlog::error("Client secret not found in the configuration file.");
    }

    return client_secret;
}

int main() {
    spdlog::info("Starting Twitch IRC bot.");

    asio::io_context io_context;
    ssl::context ssl_context(ssl::context::sslv23);

    // Read the client secret from config.json
    std::string client_secret = ReadClientSecretFromConfig();
    if (client_secret.empty()) {
        spdlog::error("Client secret not found. Exiting.");
        return 1;
    }

    // Create a resolver for the token endpoint
    tcp::resolver token_resolver(io_context);
    tcp::resolver::results_type token_endpoints = token_resolver.resolve("id.twitch.tv", "https");
    ssl::stream<tcp::socket> token_socket(io_context, ssl_context);

    // Connect to the token endpoint
    boost::asio::connect(token_socket.next_layer(), token_endpoints.begin(), token_endpoints.end());
    token_socket.handshake(ssl::stream_base::client);

    // Obtain an access token using client credentials
    http::request<http::string_body> token_request{http::verb::post, "/oauth2/token", 11};
    token_request.set(http::field::host, "id.twitch.tv");
    token_request.set(http::field::content_type, "application/x-www-form-urlencoded");
    std::string client_id = "mvqmkhwnhruku46rqh34ibm9ij33ky";

    // Use the extracted client_secret
    token_request.body() = "client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=client_credentials";
    token_request.prepare_payload();

    http::write(token_socket, token_request);

    beast::flat_buffer token_buffer;
    http::response<http::dynamic_body> token_response;
    http::read(token_socket, token_buffer, token_response);

    spdlog::info("Token Response status code: {}", token_response.result_int());
    spdlog::info("Token Response body: {}", beast::buffers_to_string(token_response.body().data()));

    nlohmann::json token_json_response = nlohmann::json::parse(beast::buffers_to_string(token_response.body().data()));

    // Extract the "access_token" from the token JSON response
    std::string access_token = token_json_response["access_token"];

    spdlog::info("Access token: {}", access_token);

    // Initialize Boost ASIO and create a context
    boost::asio::io_context irc_context;

    // Create a resolver to resolve the Twitch IRC server hostname
    tcp::resolver resolver(irc_context);
    auto const results = resolver.resolve("irc.chat.twitch.tv", "6667");

    // Connect to the Twitch IRC server
    tcp::socket socket(irc_context);
    boost::asio::connect(socket, results.begin(), results.end());

    // Send PASS and NICK messages for authentication
    std::string oauth_token = "oauth:" + access_token;
    std::string bot_username = "d0lph1nman_bot";
    std::string channel_name = "#d0lph1nman";

    // Authenticate with Twitch IRC
    boost::asio::write(socket, boost::asio::buffer("PASS " + oauth_token + "\r\n"));
    boost::asio::write(socket, boost::asio::buffer("NICK " + bot_username + "\r\n"));

    // Join the desired Twitch channel
    boost::asio::write(socket, boost::asio::buffer("JOIN " + channel_name + "\r\n"));

    spdlog::info("Connected to Twitch IRC channel: {}", channel_name);

    // Main event loop to read and process messages
    while (true) {
        char data[1024];
        boost::system::error_code ec;
        std::size_t length = socket.read_some(boost::asio::buffer(data), ec);

        if (ec == boost::asio::error::eof) {
            spdlog::info("Connection closed by server");
            break;
        } else if (ec) {
            spdlog::error("Error reading from socket: {}", ec.message());
            throw boost::system::system_error(ec);
        }

        // Process the received data (IRC messages)
        std::string message(data, length);

        // Implement your message processing logic here
        // Parse the IRC message and respond accordingly

        // Example: Respond to the !dice command
        if (message.find("!dice") != std::string::npos) {
            int num = rollDice();
            std::string response = "PRIVMSG " + channel_name + " :You rolled a " + std::to_string(num) + "\r\n";
            boost::asio::write(socket, boost::asio::buffer(response));
        }
    }

    spdlog::info("Twitch IRC bot stopped.");
    return 0;
}
