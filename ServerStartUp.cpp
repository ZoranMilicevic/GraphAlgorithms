#include <iostream>
#include <thread>
#include <memory>
#include <utility>
#include "asio.hpp"
#include "ServerCommand.h"

using asio::ip::tcp;

void print_connection_info(const tcp::socket& sock, bool connect) {
	std::string sClientIp = sock.remote_endpoint().address().to_string();
	unsigned short uiClientPort = sock.remote_endpoint().port();
	if(connect)
		std::cout << "Connection established with " << sClientIp << " on port " << uiClientPort <<std::endl;
	else
		std::cout << "Client " << sClientIp << " disconnected from port " << uiClientPort << std::endl;
}

void session(tcp::socket sock)
{
	print_connection_info(sock, true);
	try
	{
		while (true) 
		{
			std::string data_in;
			std::stringstream received_data;
			asio::error_code error;
			size_t size_of_data = 0;
			size_t data_read = 0;

			while (size_of_data == 0)
			{
				data_in.resize(sock.available());
				sock.read_some(asio::buffer(data_in), error);
				if (data_in == "")
					continue;
				else if (error)
					throw asio::system_error(error);

				size_of_data = stoi(data_in);
			}

			asio::write(sock, asio::buffer("size received"));

			while (data_read < size_of_data)
			{
				data_in.resize(sock.available());
				data_read += sock.available();
				sock.read_some(asio::buffer(data_in), error);
				if (data_in == "")
					continue;
				else if (error)
					throw asio::system_error(error);

				received_data << data_in;
			}

			asio::write(sock, asio::buffer("command received"));

			std::string command_str = received_data.str();
			if (command_str == "")
			{
				asio::write(sock, asio::buffer("Unable to parse command. Please send again."));
				return;
			}
			else if (command_str == "disconnect") 
			{
				break;
			}
			else 
			{
				std::shared_ptr<ServerCommand> sc = ServerCommand::create_from_xml(command_str);
				sc->execute_command();

				std::string xml_string = sc->result_xml();
				asio::write(sock, asio::buffer(xml_string));
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}

	print_connection_info(sock, false);
	sock.close();
}

void server(asio::io_context& io_context, unsigned short port)
{
	std::cout << "Server started. Listening on port " << port << std::endl;
	tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
	while(true)
	{
		std::thread(session, a.accept()).detach();
	}
}

int main(int argc, char* argv[])
{
	try
	{
		asio::io_context io_context;
		server(io_context, 54000);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}