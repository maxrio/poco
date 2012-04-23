//
// HTTPSStreamFactory.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/include/Poco/Net/HTTPSStreamFactory.h#1 $
//
// Library: NetSSL_OpenSSL
// Package: HTTPSClient
// Module:  HTTPSStreamFactory
//
// Definition of the HTTPSStreamFactory class.
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


#ifndef NetSSL_HTTPSStreamFactory_INCLUDED
#define NetSSL_HTTPSStreamFactory_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/URIStreamFactory.h"


namespace Poco {
namespace Net {


class NetSSL_API HTTPSStreamFactory: public Poco::URIStreamFactory
	/// An implementation of the URIStreamFactory interface
	/// that handles secure Hyper-Text Transfer Protocol (https) URIs.
{
public:
	HTTPSStreamFactory();
		/// Creates the HTTPSStreamFactory.

	HTTPSStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort = HTTPSession::HTTP_PORT);
		/// Creates the HTTPSStreamFactory.
		///
		/// HTTPS connections will use the given proxy.

	HTTPSStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort, const std::string& proxyUsername, const std::string& proxyPassword);
		/// Creates the HTTPSStreamFactory.
		///
		/// HTTPS connections will use the given proxy and
		/// will be authorized against the proxy using Basic authentication
		/// with the given proxyUsername and proxyPassword.

	~HTTPSStreamFactory();
		/// Destroys the HTTPSStreamFactory.
		
	std::istream* open(const Poco::URI& uri);
		/// Creates and opens a HTTPS stream for the given URI.
		/// The URI must be a https://... URI.
		///
		/// Throws a NetException if anything goes wrong.
		
	static void registerFactory();
		/// Registers the HTTPSStreamFactory with the
		/// default URIStreamOpener instance.	
		
private:
	enum
	{
		MAX_REDIRECTS = 10
	};
	
	std::string  _proxyHost;
	Poco::UInt16 _proxyPort;
	std::string  _proxyUsername;
	std::string  _proxyPassword;
};


} } // namespace Poco::Net


#endif // Net_HTTPSStreamFactory_INCLUDED
