//
// File2Page.cpp
//
// $Id: //poco/1.4/PageCompiler/File2Page/src/File2Page.cpp#3 $
//
// An application that creates a Page Compiler source file from an
// ordinary file.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/FileStream.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTime.h"
#include <iostream>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::NumberFormatter;


class File2PageApp: public Application
{
public:
	File2PageApp(): _helpRequested(false)
	{
	}

protected:	
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);
		// add your own initialization code here
	}
	
	void uninitialize()
	{
		// add your own uninitialization code here
		Application::uninitialize();
	}
	
	void reinitialize(Application& self)
	{
		Application::reinitialize(self);
		// add your own reinitialization code here
	}
	
	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<File2PageApp>(this, &File2PageApp::handleHelp)));

		options.addOption(
			Option("contentType", "t", "specify a content type")
				.required(false)
				.repeatable(false)
				.argument("MIME-Type")
				.callback(OptionCallback<File2PageApp>(this, &File2PageApp::handleContentType)));

		options.addOption(
			Option("contentLanguage", "l", "specify a content language")
				.required(false)
				.repeatable(false)
				.argument("language")
				.callback(OptionCallback<File2PageApp>(this, &File2PageApp::handleContentLang)));
								
		options.addOption(
			Option("class", "c", "specify the handler class name")
				.required(false)
				.repeatable(false)
				.argument("class-name")
				.callback(OptionCallback<File2PageApp>(this, &File2PageApp::handleClassName)));

		options.addOption(
			Option("namespace", "n", "specify the handler class namespace name")
				.required(false)
				.repeatable(false)
				.argument("namespace-name")
				.callback(OptionCallback<File2PageApp>(this, &File2PageApp::handleNamespace)));
	
		options.addOption(
			Option("output", "o", "specify the output file name")
				.required(false)
				.repeatable(false)
				.argument("path")
				.callback(OptionCallback<File2PageApp>(this, &File2PageApp::handleOutput)));
}
	
	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}
	
	void handleContentType(const std::string& name, const std::string& value)
	{
		_contentType = value;
	}

	void handleContentLang(const std::string& name, const std::string& value)
	{
		_contentLang = value;
	}
	
	void handleClassName(const std::string& name, const std::string& value)
	{
		_clazz = value;
	}

	void handleNamespace(const std::string& name, const std::string& value)
	{
		_namespace = value;
	}
				
	void handleOutput(const std::string& name, const std::string& value)
	{
		_output = value;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("Create a PageCompiler source file from a binary file.");
		helpFormatter.format(std::cout);
	}
	
	void convert(const std::string& path)
	{
		Poco::Path p(path);
		Poco::Path op(path);
		if (_output.empty())
		{
			op.setExtension("cpsp");
		}
		else
		{
			op = _output;
		}
		if (_contentType.empty())
		{
			_contentType = extToContentType(p.getExtension());
		}
		if (_clazz.empty())
		{
			_clazz = p.getBaseName();
		}
		Poco::FileInputStream istr(path);
		Poco::FileOutputStream ostr(op.toString());
		ostr << "<%@ page\n"
		     << "    contentType=\"" << _contentType << "\"\n";
		if (!_contentLang.empty())
		{
			ostr << "    contentLanguage=\"" << _contentLang << "\"\n";
		}
		ostr << "    form=\"false\"\n"
		     << "    namespace=\"" << _namespace << "\"\n"
		     << "    class=\"" << _clazz << "\"\n"
		     << "    precondition=\"checkModified(request)\"%><%@"
		     << "    impl include=\"Poco/DateTime.h\"\n"
		     << "         include=\"Poco/DateTimeParser.h\"\n"
		     << "         include=\"Poco/DateTimeFormatter.h\"\n"
		     << "         include=\"Poco/DateTimeFormat.h\"%><%!\n\n";
		ostr << "// " << path << "\n";
		ostr << "static const unsigned char data[] = {\n\t";
		int ch = istr.get();
		int pos = 0;
		while (ch != -1)
		{
			ostr << "0x" << NumberFormatter::formatHex(ch, 2) << ", ";
			if (pos++ == 16)
			{
				ostr << "\n\t";
				pos = 0;
			}
			ch = istr.get();
		}
		Poco::File f(path);
		Poco::DateTime lm = f.getLastModified();
		ostr << "\n};\n\n\n";
		ostr << "static bool checkModified(Poco::Net::HTTPServerRequest& request)\n"
		     << "{\n"
		     << "\tPoco::DateTime modified(" << lm.year() << ", " << lm.month() << ", " << lm.day() << ", " << lm.hour() << ", " << lm.minute() << ", " << lm.second() << ");\n"
		     << "\trequest.response().setChunkedTransferEncoding(false);\n"
		     << "\trequest.response().set(\"Last-Modified\", Poco::DateTimeFormatter::format(modified, Poco::DateTimeFormat::HTTP_FORMAT));\n"
		     << "\tif (request.has(\"If-Modified-Since\"))\n"
		     << "\t{\n"
		     << "\t\tPoco::DateTime modifiedSince;\n"
		     << "\t\tint tzd;\n"
		     << "\t\tPoco::DateTimeParser::parse(request.get(\"If-Modified-Since\"), modifiedSince, tzd);\n"
		     << "\t\tif (modified <= modifiedSince)\n"
		     << "\t\t{\n"
		     << "\t\t\trequest.response().setContentLength(0);\n"
		     << "\t\t\trequest.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_MODIFIED);\n"
		     << "\t\t\trequest.response().send();\n"
		     << "\t\t\treturn false;\n"
		     << "\t\t}\n"
		     << "\t}\n"
		     << "\trequest.response().setContentLength(static_cast<int>(sizeof(data)));\n"
		     << "\treturn true;\n"
		     << "}\n"
			 << "%><%\n"
		     << "\tresponseStream.write(reinterpret_cast<const char*>(data), sizeof(data));\n"
		     << "%>";
	}
	
	std::string extToContentType(const std::string& ext)
	{
		if (ext == "jpg" || ext == "jpeg")
			return "image/jpeg";
		else if (ext == "png")
			return "image/png";
		else if (ext == "gif")
			return "image/gif";
		else if (ext == "ico")
			return "image/x-icon";
		else if (ext == "htm")
			return "text/html";
		else if (ext == "html")
			return "text/html";
		else if (ext == "css")
			return "text/css";
		else if (ext == "js")
			return "application/javascript";
		else if (ext == "xml")
			return "text/xml";
		else
			return "application/binary";
	}
	
	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
			{
				convert(*it);
			}
		}
		return Application::EXIT_OK;
	}
		
private:
	bool _helpRequested;
	std::string _contentType;
	std::string _contentLang;
	std::string _clazz;
	std::string _namespace;
	std::string _output;
};


POCO_APP_MAIN(File2PageApp)
