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
		while(true)
		{
			std::string data_in;
			asio::error_code error;

			data_in.resize(sock.available());
			sock.read_some(asio::buffer(data_in), error);
			if (error == asio::error::eof)
				break; //connection closed
			else if (error)
				throw asio::system_error(error);
			else if (data_in == "")
				break;

			std::shared_ptr<ServerCommand> sc = ServerCommand::create_from_xml(data_in);
			sc->execute_command();

			std::string xml_string = sc->result_xml();
			asio::write(sock, asio::buffer(xml_string));
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