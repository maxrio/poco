//
// HostEntry.cpp
//
// $Id: //poco/1.3/Net/src/HostEntry.cpp#3 $
//
// Library: Net
// Package: NetCore
// Module:  HostEntry
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


#include "Poco/Net/HostEntry.h"
#include "Poco/Exception.h"
#include <algorithm>


namespace Poco {
namespace Net {


HostEntry::HostEntry()
{
}

	
HostEntry::HostEntry(struct hostent* entry)
{
	poco_check_ptr (entry);
	
	_name = entry->h_name;	
	char** alias = entry->h_aliases;
	if (alias)
	{
		while (*alias)
		{
			_aliases.push_back(std::string(*alias));
			++alias;
		}
	}
	char** address = entry->h_addr_list;
	if (address)
	{
		while (*address)
		{
			_addresses.push_back(IPAddress(*address, entry->h_length));
			++address;
		}
	}
}


#if defined(_WIN32) && defined(POCO_HAVE_IPv6)


HostEntry::HostEntry(struct addrinfo* ainfo)
{
	poco_check_ptr (ainfo);
	
	for (struct addrinfo* ai = ainfo; ai; ai = ai->ai_next)
	{
		if (ai->ai_canonname)
		{
			_name.assign(ai->ai_canonname);
		}
		if (ai->ai_addrlen && ai->ai_addr)
		{
			switch (ai->ai_addr->sa_family)
			{
			case AF_INET:
				_addresses.push_back(IPAddress(&reinterpret_cast<struct sockaddr_in*>(ai->ai_addr)->sin_addr, sizeof(in_addr)));
				break;
			case AF_INET6:
				_addresses.push_back(IPAddress(&reinterpret_cast<struct sockaddr_in6*>(ai->ai_addr)->sin6_addr, sizeof(in6_addr)));
				break;
			}
		}
	}
}


#endif


HostEntry::HostEntry(const HostEntry& entry):
	_name(entry._name),
	_aliases(entry._aliases),
	_addresses(entry._addresses)
{
}


HostEntry& HostEntry::operator = (const HostEntry& entry)
{
	if (&entry != this)
	{
		_name          = entry._name;
		_aliases       = entry._aliases;
		_addresses     = entry._addresses;
	}
	return *this;
}


void HostEntry::swap(HostEntry& hostEntry)
{
	std::swap(_name, hostEntry._name);
	std::swap(_aliases, hostEntry._aliases);
	std::swap(_addresses, hostEntry._addresses);
}


HostEntry::~HostEntry()
{
}


} } // namespace Poco::Net
