//
// SecureServerSocket.cpp
//
// $Id: //poco/1.4/NetSSL_OpenSSL/src/SecureServerSocket.cpp#1 $
//
// Library: NetSSL_OpenSSL
// Package: SSLSockets
// Module:  SecureServerSocket
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/SecureServerSocketImpl.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Exception.h"


using Poco::InvalidArgumentException;


namespace Poco {
namespace Net {


SecureServerSocket::SecureServerSocket(): 
	ServerSocket(new SecureServerSocketImpl(SSLManager::instance().defaultServerContext()), true)
{
}


SecureServerSocket::SecureServerSocket(Context::Ptr pContext):
	ServerSocket(new SecureServerSocketImpl(pContext), true)
{
}


SecureServerSocket::SecureServerSocket(const Socket& socket): 
	ServerSocket(socket)
{
	if (!dynamic_cast<SecureServerSocketImpl*>(impl()))
		throw InvalidArgumentException("Cannot assign incompatible socket");
}


SecureServerSocket::SecureServerSocket(const SocketAddress& address, int backlog): 
	ServerSocket(new SecureServerSocketImpl(SSLManager::instance().defaultServerContext()), true)
{
	impl()->bind(address, true);
	impl()->listen(backlog);
}


SecureServerSocket::SecureServerSocket(const SocketAddress& address, int backlog, Context::Ptr pContext): 
	ServerSocket(new SecureServerSocketImpl(pContext), true)
{
	impl()->bind(address, true);
	impl()->listen(backlog);
}


SecureServerSocket::SecureServerSocket(Poco::UInt16 port, int backlog): 
	ServerSocket(new SecureServerSocketImpl(SSLManager::instance().defaultServerContext()), true)
{
	IPAddress wildcardAddr;
	SocketAddress address(wildcardAddr, port);
	impl()->bind(address, true);
	impl()->listen(backlog);
}

SecureServerSocket::SecureServerSocket(Poco::UInt16 port, int backlog, Context::Ptr pContext): 
	ServerSocket(new SecureServerSocketImpl(pContext), true)
{
	IPAddress wildcardAddr;
	SocketAddress address(wildcardAddr, port);
	impl()->bind(address, true);
	impl()->listen(backlog);
}


SecureServerSocket::~SecureServerSocket()
{
}


SecureServerSocket& SecureServerSocket::operator = (const Socket& socket)
{
	if (&socket != this)
	{
		if (dynamic_cast<SecureServerSocketImpl*>(socket.impl()))
			ServerSocket::operator = (socket);
		else
			throw InvalidArgumentException("Cannot assign incompatible socket");
	}
	return *this;
}


StreamSocket SecureServerSocket::acceptConnection(SocketAddress& clientAddr)
{
	return SecureStreamSocket(impl()->acceptConnection(clientAddr));
}


StreamSocket SecureServerSocket::acceptConnection()
{
	SocketAddress clientAddr;
	return SecureStreamSocket(impl()->acceptConnection(clientAddr));
}


Context::Ptr SecureServerSocket::context() const
{
	return static_cast<SecureServerSocketImpl*>(impl())->context();
}


} } // namespace Poco::Net
