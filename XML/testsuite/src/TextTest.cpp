//
// TextTest.cpp
//
// $Id: //poco/1.4/XML/testsuite/src/TextTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "TextTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/CDATASection.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/AutoPtr.h"


using Poco::XML::Text;
using Poco::XML::CDATASection;
using Poco::XML::Element;
using Poco::XML::Document;
using Poco::XML::AutoPtr;
using Poco::XML::XMLString;


TextTest::TextTest(const std::string& name): CppUnit::TestCase(name)
{
}


TextTest::~TextTest()
{
}


void TextTest::testLength()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Text> pText1 = pDoc->createTextNode("");
	assert (pText1->length() == 0);
	AutoPtr<Text> pText2 = pDoc->createTextNode("foo bar");
	assert (pText2->length() == 7);
}


void TextTest::testSubstring()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Text> pText1 = pDoc->createTextNode("foo bar");
	XMLString str = pText1->substringData(0, 3);
	assert (str == "foo");
	str = pText1->substringData(4, 3);
	assert (str == "bar");
	str = pText1->substringData(3, 0);
	assert (str == "");
}


void TextTest::testAppend()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Text> pText1 = pDoc->createTextNode("foo");
	pText1->appendData("bar");
	assert (pText1->data() == "foobar");
}


void TextTest::testInsert()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Text> pText1 = pDoc->createTextNode("bar");
	pText1->insertData(0, "foo");
	assert (pText1->data() == "foobar");
	pText1->insertData(pText1->length(), "!");
	assert (pText1->data() == "foobar!");
	pText1->insertData(3, " ");
	assert (pText1->data() == "foo bar!");
}


void TextTest::testDelete()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Text> pText1 = pDoc->createTextNode("foo bar");
	pText1->deleteData(3, 1);
	assert (pText1->data() == "foobar");
	pText1->deleteData(0, 3);
	assert (pText1->data() == "bar");
	pText1->deleteData(1, 0);
	assert (pText1->data() == "bar");
	pText1->deleteData(0, pText1->length());
	assert (pText1->data() == "");
}


void TextTest::testReplace()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Text> pText1 = pDoc->createTextNode("foo bar");
	pText1->replaceData(0, 3, "FOO");
	assert (pText1->data() == "FOO bar");
	pText1->replaceData(4, 3, "BAR!!!");
	assert (pText1->data() == "FOO BAR!!!");
	pText1->replaceData(3, 1, "-");
	assert (pText1->data() == "FOO-BAR!!!");
	pText1->replaceData(3, 1, "---");
	assert (pText1->data() == "FOO---BAR!!!");
	pText1->replaceData(3, 3, " ");
	assert (pText1->data() == "FOO BAR!!!");
	pText1->replaceData(0, pText1->length(), "foo bar");
	assert (pText1->data() == "foo bar");
}


void TextTest::testSplit()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pElem = pDoc->createElement("elem");
	AutoPtr<Text> pText1 = pDoc->createCDATASection("foobar");
	pElem->appendChild(pText1);
	pText1->splitText(3);
	assert (pElem->firstChild()->nodeValue() == "foo");
	assert (pElem->lastChild()->nodeValue() == "bar");
}


void TextTest::testSplitCDATA()
{
	AutoPtr<Document> pDoc = new Document;
	AutoPtr<Element> pElem = pDoc->createElement("elem");
	AutoPtr<Text> pText1 = pDoc->createTextNode("foobar");
	pElem->appendChild(pText1);
	pText1->splitText(3);
	assert (pElem->firstChild()->nodeValue() == "foo");
	assert (pElem->lastChild()->nodeValue() == "bar");

}


void TextTest::setUp()
{
}


void TextTest::tearDown()
{
}


CppUnit::Test* TextTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TextTest");

	CppUnit_addTest(pSuite, TextTest, testLength);
	CppUnit_addTest(pSuite, TextTest, testSubstring);
	CppUnit_addTest(pSuite, TextTest, testAppend);
	CppUnit_addTest(pSuite, TextTest, testInsert);
	CppUnit_addTest(pSuite, TextTest, testDelete);
	CppUnit_addTest(pSuite, TextTest, testReplace);
	CppUnit_addTest(pSuite, TextTest, testSplit);
	CppUnit_addTest(pSuite, TextTest, testSplitCDATA);

	return pSuite;
}
