/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license. 
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */	
#include <kroll/kroll.h>
#include "worker_binding.h"
#include "worker.h"
#include <Poco/FileStream.h>
#include <sstream>

namespace ti
{
	WorkerBinding::WorkerBinding(Host *host, SharedKObject global) : host(host), global(global)
	{
		this->SetMethod("createWorker",&WorkerBinding::CreateWorker);
	}
	WorkerBinding::~WorkerBinding()
	{
	}
	void WorkerBinding::CreateWorker(const ValueList& args, SharedValue result)
	{
		if (args.size()!=3)
		{
			throw ValueException::FromString("invalid argument specified");
		}
		
		SharedKObject scope = args.at(0)->ToObject();
		bool is_function = args.at(2)->ToBool();
		
		std::string code;
		Logger *logger = Logger::Get("Worker");
		
		if (is_function)
		{
			// convert the function to a string block 
			code =  "(";
			code += args.at(1)->ToString();
			code += ")()";
		}
		else 
		{
			// this is a path -- probably should verify that this is relative and not an absolute URL to remote
			SharedKMethod appURLToPath = global->GetNS("App.appURLToPath")->ToMethod();
			ValueList a;
			a.push_back(args.at(1));
			SharedValue result = appURLToPath->Call(a);
			const char *path = result->ToString();
			
			logger->Debug("worker file path = %s",path);
			
			std::ios::openmode flags = std::ios::in;
			Poco::FileInputStream *fis = new Poco::FileInputStream(path,flags);
			std::stringstream ostr;
			char buf[8096];
			int count = 0;
			while(!fis->eof())
			{
				fis->read((char*)&buf,8095);
				std::streamsize len = fis->gcount();
				if (len>0)
				{
					buf[len]='\0';
					count+=len;
					ostr << buf;
				}
				else break;
			}
			fis->close();
			code = std::string(ostr.str());
		}
		
		logger->Debug("Worker script code = %s", code.c_str());
		
		SharedKObject worker = new Worker(host,global,scope,code);
		result->SetObject(worker);
	}
}
