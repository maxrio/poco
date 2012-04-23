//
// SocketReactorTest.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/SocketReactorTest.cpp#1 $
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#include "SocketReactorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketConnector.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Observer.h"
#include <sstream>


using Poco::Net::SocketReactor;
using Poco::Net::SocketConnector;
using Poco::Net::SocketAcceptor;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::SocketNotification;
using Poco::Net::ReadableNotification;
using Poco::Net::WritableNotification;
using Poco::Net::TimeoutNotification;
using Poco::Net::ShutdownNotification;
using Poco::Observer;


namespace
{
	class EchoServiceHandler
	{
	public:
		EchoServiceHandler(StreamSocket& socket, SocketReactor& reactor):
			_socket(socket),
			_reactor(reactor)
		{
			_reactor.addEventHandler(_socket, Observer<EchoServiceHandler, ReadableNotification>(*this, &EchoServiceHandler::onReadable));
		}
		
		~EchoServiceHandler()
		{
			_reactor.removeEventHandler(_socket, Observer<EchoServiceHandler, ReadableNotification>(*this, &EchoServiceHandler::onReadable));
		}
		
		void onReadable(ReadableNotification* pNf)
		{
			pNf->release();
			char buffer[8];
			int n = _socket.receiveBytes(buffer, sizeof(buffer));
			if (n > 0)
			{
				_socket.sendBytes(buffer, n);
			}
			else
			{
				_socket.shutdownSend();
				delete this;
			}
		}
		
	private:
		StreamSocket   _socket;
		SocketReactor& _reactor;
	};
	
	class ClientServiceHandler
	{
	public:
		ClientServiceHandler(StreamSocket& socket, SocketReactor& reactor):
			_socket(socket),
			_reactor(reactor)
		{
			_timeout = false;
			_reactor.addEventHandler(_socket, Observer<ClientServiceHandler, ReadableNotification>(*this, &ClientServiceHandler::onReadable));
			_reactor.addEventHandler(_socket, Observer<ClientServiceHandler, WritableNotification>(*this, &ClientServiceHandler::onWritable));
			_reactor.addEventHandler(_socket, Observer<ClientServiceHandler, TimeoutNotification>(*this, &ClientServiceHandler::onTimeout));
		}
		
		~ClientServiceHandler()
		{
		}
		
		void onReadable(ReadableNotification* pNf)
		{
			pNf->release();
			char buffer[32];
			int n = _socket.receiveBytes(buffer, sizeof(buffer));
			if (n > 0)
			{
				_str.write(buffer, n);
			}
			else
			{
				_reactor.removeEventHandler(_socket, Observer<ClientServiceHandler, ReadableNotification>(*this, &ClientServiceHandler::onReadable));
				_reactor.stop();
				_data = _str.str();
				delete this;
			}
		}
		
		void onWritable(WritableNotification* pNf)
		{
			pNf->release();
			_reactor.removeEventHandler(_socket, Observer<ClientServiceHandler, WritableNotification>(*this, &ClientServiceHandler::onWritable));
			std::string data(1024, 'x');
			_socket.sendBytes(data.data(), (int) data.length());
			_socket.shutdownSend();
		}
		
		void onTimeout(TimeoutNotification* pNf)
		{
			pNf->release();
			_timeout = true;
			if (_closeOnTimeout) 
			{
				_reactor.stop();
				delete this;
			}
		}
		
		static std::string data()
		{
			return _data;
		}
		
		static bool timeout()
		{
			return _timeout;
		}

		static bool getCloseOnTimeout()
		{
			return _closeOnTimeout;
		}
		
		static void setCloseOnTimeout(bool flag)
		{
			_closeOnTimeout = flag;
		}
		
	private:
		StreamSocket       _socket;
		SocketReactor&     _reactor;
		std::stringstream  _str;
		static std::string _data;
		static bool        _timeout;
		static bool        _closeOnTimeout;
	};
	
	
	std::string ClientServiceHandler::_data;
	bool ClientServiceHandler::_timeout = false;
	bool ClientServiceHandler::_closeOnTimeout = false;
	
	
	class FailConnector: public SocketConnector<ClientServiceHandler>
	{
	public:
		FailConnector(SocketAddress& address, SocketReactor& reactor):
			SocketConnector<ClientServiceHandler>(address, reactor),
			_failed(false),
			_shutdown(false)
		{
			reactor.addEventHandler(socket(), Observer<FailConnector, ShutdownNotification>(*this, &FailConnector::onShutdown));
		}
		
		void onShutdown(ShutdownNotification* pNf)
		{
			pNf->release();
			_shutdown = true;
		}
		
		void onError(int error)
		{
			_failed = true;
			reactor()->stop();
		}
		
		bool failed() const
		{
			return _failed;
		}

		bool shutdown() const
		{
			return _shutdown;
		}
		
	private:
		bool _failed;
		bool _shutdown;
	};
}


SocketReactorTest::SocketReactorTest(const std::string& name): CppUnit::TestCase(name)
{
}


SocketReactorTest::~SocketReactorTest()
{
}


void SocketReactorTest::testSocketReactor()
{
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor;
	SocketAcceptor<EchoServiceHandler> acceptor(ss, reactor);
	SocketAddress sa("localhost", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor);
	reactor.run();
	std::string data(ClientServiceHandler::data());
	assert (data.size() == 1024);
}


void SocketReactorTest::testSocketConnectorFail()
{
	SocketReactor reactor;
	SocketAddress sa("192.168.168.192", 12345);
	FailConnector connector(sa, reactor);
	assert (!connector.failed());
	assert (!connector.shutdown());
	reactor.run();
	assert (connector.failed());
	assert (connector.shutdown());
}


void SocketReactorTest::testSocketConnectorTimeout()
{
	ClientServiceHandler::setCloseOnTimeout(true);
	
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor;
	SocketAddress sa("localhost", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor);
	reactor.run();
	assert (ClientServiceHandler::timeout());
}


void SocketReactorTest::setUp()
{
	ClientServiceHandler::setCloseOnTimeout(false);
}


void SocketReactorTest::tearDown()
{
}


CppUnit::Test* SocketReactorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SocketReactorTest");

	CppUnit_addTest(pSuite, SocketReactorTest, testSocketReactor);
	CppUnit_addTest(pSuite, SocketReactorTest, testSocketConnectorFail);
	CppUnit_addTest(pSuite, SocketReactorTest, testSocketConnectorTimeout);

	return pSuite;
}
