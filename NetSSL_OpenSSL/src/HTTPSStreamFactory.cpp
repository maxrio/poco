//
// HTTPSStreamFactory.cpp
//
// $Id: //poco/1.4/NetSSL_OpenSSL/src/HTTPSStreamFactory.cpp#2 $
//
// Library: NetSSL_OpenSSL
// Package: HTTPSClient
// Module:  HTTPSStreamFactory
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPIOStream.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/NetException.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/UnbufferedStreamBuf.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"


using Poco::URIStreamFactory;
using Poco::URI;
using Poco::URIStreamOpener;
using Poco::UnbufferedStreamBuf;


namespace Poco {
namespace Net {


HTTPSStreamFactory::HTTPSStreamFactory():
	_proxyPort(HTTPSession::HTTP_PORT)
{
}


HTTPSStreamFactory::HTTPSStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort):
	_proxyHost(proxyHost),
	_proxyPort(proxyPort)
{
}


HTTPSStreamFactory::HTTPSStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort, const std::string& proxyUsername, const std::string& proxyPassword):
	_proxyHost(proxyHost),
	_proxyPort(proxyPort),
	_proxyUsername(proxyUsername),
	_proxyPassword(proxyPassword)
{
}


HTTPSStreamFactory::~HTTPSStreamFactory()
{
}


std::istream* HTTPSStreamFactory::open(const URI& uri)
{
	poco_assert (uri.getScheme() == "https" || uri.getScheme() == "http");

	URI resolvedURI(uri);
	URI proxyUri;
	HTTPClientSession* pSession = 0;
	HTTPResponse res;
	try
	{
		bool retry = false;
		bool authorize = false;
		int redirects = 0;
		std::string username;
		std::string password;
		
		do
		{
			if (!pSession)
			{
				if (resolvedURI.getScheme() != "http")
					pSession = new HTTPSClientSession(resolvedURI.getHost(), resolvedURI.getPort());
				else
					pSession = new HTTPClientSession(resolvedURI.getHost(), resolvedURI.getPort());
				if (proxyUri.empty())
					pSession->setProxy(_proxyHost, _proxyPort);
				else
					pSession->setProxy(proxyUri.getHost(), proxyUri.getPort());
				pSession->setProxyCredentials(_proxyUsername, _proxyPassword);
			}
			std::string path = resolvedURI.getPathAndQuery();
			if (path.empty()) path = "/";
			HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
			
			if (authorize)
			{
				HTTPCredentials::extractCredentials(uri, username, password);
				HTTPCredentials cred(username, password);
				cred.authenticate(req, res);
			}

			pSession->sendRequest(req);
			std::istream& rs = pSession->receiveResponse(res);
			bool moved = (res.getStatus() == HTTPResponse::HTTP_MOVED_PERMANENTLY || 
			              res.getStatus() == HTTPResponse::HTTP_FOUND || 
			              res.getStatus() == HTTPResponse::HTTP_SEE_OTHER ||
						  res.getStatus() == HTTPResponse::HTTP_TEMPORARY_REDIRECT);
			if (moved)
			{
				resolvedURI.resolve(res.get("Location"));
				if (!username.empty())
				{
					resolvedURI.setUserInfo(username + ":" + password);
					authorize = false;
				}
				delete pSession; pSession = 0;
				++redirects;
				retry = true;
			}
			else if (res.getStatus() == HTTPResponse::HTTP_OK)
			{
				return new HTTPResponseStream(rs, pSession);
			}
			else if (res.getStatus() == HTTPResponse::HTTP_USEPROXY && !retry)
			{
				// The requested resource MUST be accessed through the proxy 
				// given by the Location field. The Location field gives the 
				// URI of the proxy. The recipient is expected to repeat this 
				// single request via the proxy. 305 responses MUST only be generated by origin servers.
				// only use for one single request!
				proxyUri.resolve(res.get("Location"));
				delete pSession; pSession = 0;
				retry = true; // only allow useproxy once
			}
			else if (res.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED && !authorize)
			{
				authorize = true;
				retry = true;
				Poco::NullOutputStream null;
				Poco::StreamCopier::copyStream(rs, null);
			}
			else throw HTTPException(res.getReason(), uri.toString());
		}
		while (retry && redirects < MAX_REDIRECTS);
		throw HTTPException("Too many redirects", uri.toString());
	}
	catch (...)
	{
		delete pSession;
		throw;
	}
}


void HTTPSStreamFactory::registerFactory()
{
	std::string https("https");
	URIStreamOpener::defaultOpener().registerStreamFactory(https, new HTTPSStreamFactory);
}


} } // namespace Poco::Net
