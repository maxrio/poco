//
// SharedLibrary_VX.cpp
//
// $Id: //poco/1.4/Foundation/src/SharedLibrary_VX.cpp#1 $
//
// Library: Foundation
// Package: SharedLibrary
// Module:  SharedLibrary
//
// Copyright (c) 2004-2011, Applied Informatics Software Engineering GmbH.
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


#include "Poco/SharedLibrary_VX.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"
#include <loadLib.h>
#include <unldLib.h>
#include <ioLib.h>
#include <symLib.h>
#include <sysSymTbl.h>
#include <cstring>


struct SymLookup
{
	const char* name;
	int         group;
	void*       addr;
};


extern "C" bool lookupFunc(char* name, int val, SYM_TYPE type, int arg, UINT16 group)
{
	SymLookup* symLookup = reinterpret_cast<SymLookup*>(arg);
	if (group == symLookup->group && std::strcmp(name, symLookup->name) == 0)
	{
		symLookup->addr = reinterpret_cast<void*>(val);
		return TRUE;
	}
	else return FALSE;
}


namespace Poco {


FastMutex SharedLibraryImpl::_mutex;


SharedLibraryImpl::SharedLibraryImpl():
	_moduleId(0)
{
}


SharedLibraryImpl::~SharedLibraryImpl()
{
}


void SharedLibraryImpl::loadImpl(const std::string& path)
{
	FastMutex::ScopedLock lock(_mutex);

	if (_moduleId) throw LibraryAlreadyLoadedException(path);
	int fd = open(const_cast<char*>(path.c_str()), O_RDONLY, 0);
	if (fd)
	{
		_moduleId = loadModule(fd, LOAD_GLOBAL_SYMBOLS);
		if (!_moduleId)
		{
			int err = errno;
			close(fd);
			throw LibraryLoadException(Poco::format("error %d", err));
		}
	}
	else
	{
		int err = errno;
		throw LibraryLoadException(Poco::format("cannot open library (error %d)", err));
	}
	_path = path;
}


void SharedLibraryImpl::unloadImpl()
{
	FastMutex::ScopedLock lock(_mutex);

	if (_moduleId)
	{
		unldByModuleId(_moduleId, 0);
		_moduleId = 0;
	}
}


bool SharedLibraryImpl::isLoadedImpl() const
{
	return _moduleId != 0; 
}


void* SharedLibraryImpl::findSymbolImpl(const std::string& name)
{
	poco_assert (_moduleId != 0);
	
	FastMutex::ScopedLock lock(_mutex);

	MODULE_INFO mi;
	if (!moduleInfoGet(_moduleId, &mi)) return 0;
	SymLookup symLookup;
	symLookup.name  = name.c_str();
	symLookup.group = mi.group;
	symLookup.addr  = 0;
	symEach(sysSymTbl, reinterpret_cast<FUNCPTR>(lookupFunc), reinterpret_cast<int>(&symLookup));
	return symLookup.addr;
}


const std::string& SharedLibraryImpl::getPathImpl() const
{
	return _path;
}


std::string SharedLibraryImpl::suffixImpl()
{
	return ".out";
}


} // namespace Poco
