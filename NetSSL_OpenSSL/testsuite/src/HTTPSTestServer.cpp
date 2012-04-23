//
// HTTPSTestServer.cpp
//
// $Id: //poco/1.4/NetSSL_OpenSSL/testsuite/src/HTTPSTestServer.cpp#1 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "HTTPSTestServer.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timespan.h"
#include "Poco/NumberFormatter.h"
#include <iostream>


using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::SecureStreamSocket;
using Poco::Net::SecureServerSocket;
using Poco::Net::SocketAddress;
using Poco::NumberFormatter;


const std::string HTTPSTestServer::SMALL_BODY("This is some random text data returned by the server");
const std::string HTTPSTestServer::LARGE_BODY(4000, 'x');


HTTPSTestServer::HTTPSTestServer():
	_socket(SocketAddress()),
	_thread("HTTPSTestServer"),
	_stop(false)
{
	_thread.start(*this);
	_ready.wait();
	_lastRequest.reserve(4000);
}


HTTPSTestServer::HTTPSTestServer(Poco::Net::Context::Ptr pContext):
	_socket(SocketAddress(), 64, pContext),
	_thread("HTTPSTestServer"),
	_stop(false)
{
	_thread.start(*this);
	_ready.wait();
	_lastRequest.reserve(4000);
}


HTTPSTestServer::~HTTPSTestServer()
{
	_stop = true;
	_thread.join();
}


Poco::UInt16 HTTPSTestServer::port() const
{
	return _socket.address().port();
}


const std::string& HTTPSTestServer::lastRequest() const
{
	return _lastRequest;
}


void HTTPSTestServer::run()
{
	_ready.set();
	Poco::Timespan span(250000);
	while (!_stop)
	{
		if (_socket.poll(span, Socket::SELECT_READ))
		{
			StreamSocket ss = _socket.acceptConnection();
			try
			{
				_lastRequest.clear();
				char buffer[256];
				int n = ss.receiveBytes(buffer, sizeof(buffer));
				while (n > 0 && !_stop)
				{
					_lastRequest.append(buffer, n);
					if (!requestComplete())
						n = ss.receiveBytes(buffer, sizeof(buffer));
					else
						n = 0;
				}
				std::string response = handleRequest();
				ss.sendBytes(response.data(), (int) response.size());
				Poco::Thread::sleep(1000);
			}
			catch (Poco::Exception& exc)
			{
				std::cerr << "HTTPSTestServer: " << exc.displayText() << std::endl;
			}
		}
	}
}


bool HTTPSTestServer::requestComplete() const
{
	return ((_lastRequest.substr(0, 3) == "GET" || _lastRequest.substr(0, 4) == "HEAD") && 
	       (_lastRequest.find("\r\n\r\n") != std::string::npos)) ||
	       (_lastRequest.find("\r\n0\r\n") != std::string::npos);
}


std::string HTTPSTestServer::handleRequest() const
{
	std::string response;
	response.reserve(16000);
	if (_lastRequest.substr(0, 10) == "GET /small" ||
	    _lastRequest.substr(0, 11) == "HEAD /small")
	{
		std::string body(SMALL_BODY);
		response.append("HTTP/1.0 200 OK\r\n");
		response.append("Content-Type: text/plain\r\n");
		response.append("Content-Length: "); 
		response.append(NumberFormatter::format((int) body.size()));
		response.append("\r\n");
		response.append("Connection: Close\r\n");
		response.append("\r\n");
		if (_lastRequest.substr(0, 3) == "GET")
			response.append(body);
	}
	else if (_lastRequest.substr(0, 10) == "GET /large" ||
	         _lastRequest.substr(0, 11) == "HEAD /large" ||
	         _lastRequest.substr(0, 36) == "GET http://www.somehost.com:80/large")
	{
		std::string body(LARGE_BODY);
		response.append("HTTP/1.0 200 OK\r\n");
		response.append("Content-Type: text/plain\r\n");
		response.append("Content-Length: "); 
		response.append(NumberFormatter::format((int) body.size()));
		response.append("\r\n");
		response.append("Connection: Close\r\n");
		response.append("\r\n");
		if (_lastRequest.substr(0, 3) == "GET")
			response.append(body);
	}
	else if (_lastRequest.substr(0, 4) == "POST")
	{
		std::string::size_type pos = _lastRequest.find("\r\n\r\n");
		pos += 4;
		std::string body = _lastRequest.substr(pos);
		response.append("HTTP/1.0 200 OK\r\n");
		response.append("Content-Type: text/plain\r\n");
		if (_lastRequest.find("Content-Length") != std::string::npos)
		{
			response.append("Content-Length: "); 
			response.append(NumberFormatter::format((int) body.size()));
			response.append("\r\n");
		}
		else if (_lastRequest.find("chunked") != std::string::npos)
		{
			response.append("Transfer-Encoding: chunked\r\n");
		}
		if (_lastRequest.substr(0,15) == "POST /keepAlive")
			response.append("Connection: keep-alive\r\n");
		else
			response.append("Connection: Close\r\n");
		response.append("\r\n");
		response.append(body);
	}
	else if (_lastRequest.substr(0, 15) == "HEAD /keepAlive")
	{
		std::string body(SMALL_BODY);
		response.append("HTTP/1.1 200 OK\r\n");
		response.append("Connection: keep-alive\r\n");
		response.append("Content-Type: text/plain\r\n");
		response.append("Content-Length: "); 
		response.append(NumberFormatter::format((int) body.size()));
		response.append("\r\n\r\n");
		response.append("HTTP/1.1 200 OK\r\n");
		response.append("Connection: Keep-Alive\r\n");
		response.append("Content-Type: text/plain\r\n");
		response.append("Content-Length: "); 
		response.append(NumberFormatter::format((int) body.size()));
		response.append("\r\n\r\n");
		response.append(body);
		body = LARGE_BODY;
		response.append("HTTP/1.1 200 OK\r\n");
		response.append("Connection: keep-alive\r\n");
		response.append("Content-Type: text/plain\r\n");
		response.append("Transfer-Encoding: chunked\r\n\r\n");
		response.append(NumberFormatter::formatHex((unsigned) body.length()));
		response.append("\r\n");
		response.append(body);
		response.append("\r\n0\r\n\r\n");
		response.append("HTTP/1.1 200 OK\r\n");
		response.append("Connection: close\r\n");
		response.append("Content-Type: text/plain\r\n");
		response.append("Content-Length: "); 
		response.append(NumberFormatter::format((int) body.size()));
		response.append("\r\n\r\n");
	}
	else if (_lastRequest.substr(0, 13) == "GET /redirect")
	{
		response.append("HTTP/1.0 302 Found\r\n");
		response.append("Location: /large\r\n");
		response.append("\r\n");
	}
	else if (_lastRequest.substr(0, 13) == "GET /notfound")
	{
		response.append("HTTP/1.0 404 Not Found\r\n");
		response.append("\r\n");
	}
	else if (_lastRequest.substr(0, 5) == "GET /" ||
	    _lastRequest.substr(0, 6) == "HEAD /")
	{
		std::string body(SMALL_BODY);
		response.append("HTTP/1.0 200 OK\r\n");
		response.append("Content-Type: text/plain\r\n");
		response.append("Content-Length: "); 
		response.append(NumberFormatter::format((int) body.size()));
		response.append("\r\n");
		response.append("Connection: Close\r\n");
		response.append("\r\n");
		if (_lastRequest.substr(0, 3) == "GET")
			response.append(body);
	}
	return response;
}
