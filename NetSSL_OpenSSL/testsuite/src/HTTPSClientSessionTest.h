//
// HTTPSClientSessionTest.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/testsuite/src/HTTPSClientSessionTest.h#1 $
//
// Definition of the HTTPSClientSessionTest class.
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


#ifndef HTTPSClientSessionTest_INCLUDED
#define HTTPSClientSessionTest_INCLUDED


#include "Poco/Net/Net.h"
#include "CppUnit/TestCase.h"


class HTTPSClientSessionTest: public CppUnit::TestCase
{
public:
	HTTPSClientSessionTest(const std::string& name);
	~HTTPSClientSessionTest();

	void testGetSmall();
	void testGetLarge();
	void testHead();
	void testPostSmallIdentity();
	void testPostLargeIdentity();
	void testPostSmallChunked();
	void testPostLargeChunked();
	void testPostLargeChunkedKeepAlive();
	void testKeepAlive();
	void testInterop();
	void testProxy();
	void testCachedSession();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // HTTPSClientSessionTest_INCLUDED
