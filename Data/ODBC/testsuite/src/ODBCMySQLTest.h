//
// ODBCMySQLTest.h
//
// $Id: //poco/Main/Data/ODBC/testsuite/src/ODBCMySQLTest.h#4 $
//
// Definition of the ODBCMySQLTest class.
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


#ifndef ODBCMySQLTest_INCLUDED
#define ODBCMySQLTest_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "ODBCTest.h"


class ODBCMySQLTest: public ODBCTest
	/// MySQL ODBC test class
	/// Tested:
	/// 
	/// Driver          | DB                        | OS                                        | Driver Manager 
	/// ----------------+---------------------------+-------------------------------------------+---------------------
	/// 03.51.12.00     | MySQL 5.0.27-community-nt | MS Windows XP Professional x64 v.2003/SP1 | 3.526.3959.0
	///  3.51.11.-6     | MySQL 5.0.27-community-nt | Ubuntu 7.04 (2.6.20-15-generic #2 SMP)    | unixODBC 2.2.11.-13
	///

{
public:
	ODBCMySQLTest(const std::string& name);
	~ODBCMySQLTest();

	void testBareboneODBC();

	void testBLOB();

	void testStoredProcedure();
	void testStoredFunction();

	void testNull();
	
	void testMultipleResults();
	void testFilter();

	static CppUnit::Test* suite();

private:
	void dropObject(const std::string& type, const std::string& name);
	void recreatePersonTable();
	void recreatePersonBLOBTable();
	void recreatePersonDateTable();
	void recreatePersonTimeTable();
	void recreatePersonDateTimeTable();
	void recreateStringsTable();
	void recreateIntsTable();
	void recreateFloatsTable();
	void recreateTuplesTable();
	void recreateVectorsTable();
	void recreateAnysTable();
	void recreateNullsTable(const std::string& notNull = "");
	void recreateMiscTable();
	void recreateLogTable();

	static ODBCTest::SessionPtr  _pSession;
	static ODBCTest::ExecPtr     _pExecutor;
	static std::string _driver;
	static std::string _dsn;
	static std::string _uid;
	static std::string _pwd;
	static std::string _connectString;
};


#endif // ODBCMySQLTest_INCLUDED
