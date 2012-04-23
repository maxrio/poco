//
// EnvironmentHandle.h
//
// $Id: //poco/Main/Data/ODBC/include/Poco/Data/ODBC/EnvironmentHandle.h#3 $
//
// Library: ODBC
// Package: ODBC
// Module:  EnvironmentHandle
//
// Definition of EnvironmentHandle.
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


#ifndef Data_ODBC_EnvironmentHandle_INCLUDED
#define Data_ODBC_EnvironmentHandle_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqltypes.h>


namespace Poco {
namespace Data {
namespace ODBC {


class ODBC_API EnvironmentHandle
/// ODBC environment handle class
{
public:
	EnvironmentHandle();
		/// Creates the EnvironmentHandle.

	~EnvironmentHandle();
		/// Destroys the EnvironmentHandle.

	operator const SQLHENV& () const;
		/// Const conversion operator into reference to native type.

	const SQLHENV& handle() const;
		/// Returns const reference to handle.

private:
	operator SQLHENV& ();
		/// Conversion operator into reference to native type.

	SQLHENV& handle();
		/// Returns reference to handle.

	EnvironmentHandle(const EnvironmentHandle&);
	const EnvironmentHandle& operator=(const EnvironmentHandle&);

	SQLHENV _henv;
	bool    _isOwner;
};


///
/// inlines
///
inline EnvironmentHandle::operator const SQLHENV& () const
{
	return handle();
}


inline const SQLHENV& EnvironmentHandle::handle() const
{
	return _henv;
}


inline EnvironmentHandle::operator SQLHENV& ()
{
	return handle();
}


inline SQLHENV& EnvironmentHandle::handle()
{
	return _henv;
}


} } } // namespace Poco::Data::ODBC


#endif
