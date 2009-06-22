/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license. 
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */	
#include "worker_context.h"

namespace ti
{
	WorkerContext::WorkerContext(Host *host, SharedKObject scope, SharedKObject worker) : host(host), scope(scope), worker(worker)
	{
		this->SetMethod("postMessage",&WorkerContext::PostMessage);
		this->SetMethod("importScript",&WorkerContext::ImportScripts);
		this->SetMethod("importScripts",&WorkerContext::ImportScripts);
	}
	WorkerContext::~WorkerContext()
	{
		worker = NULL;
		scope = NULL;
		host = NULL;
	}
	void WorkerContext::OnMessageAttached(SharedKMethod onmessage)
	{
		if (messages.size()>0)
		{
			std::list<SharedValue>::iterator i = messages.begin();
			while(i!=messages.end())
			{
				SharedValue v = (*i++);
				ValueList args;
				args.push_back(v);
				host->InvokeMethodOnMainThread(onmessage,args,false);
			}
		}
	}
	void WorkerContext::PostMessage(const ValueList &args, SharedValue result)
	{
		Logger *logger = Logger::Get("WorkerContext");
		logger->Debug("PostMessage called");
		try
		{
			// get the onmessage property and then dispatch the message to the 
			// function on the main thread
			SharedValue onmessage = worker->Get("onmessage");
			if (onmessage->IsMethod())
			{
				host->InvokeMethodOnMainThread(onmessage->ToMethod(),args,false);
			}
			else
			{
				messages.push_back(args.at(0));
			}
		}
		catch(std::exception &e)
		{
			logger->Error("Error calling onmessage for worker. Error = %s",e.what());
		}
	}
	void WorkerContext::ImportScripts(const ValueList &args, SharedValue result)
	{
		Logger *logger = Logger::Get("WorkerContext");
		
		SharedKMethod appURLToPath = host->GetGlobalObject()->GetNS("App.appURLToPath")->ToMethod();
		SharedKMethod evaluate = host->GetGlobalObject()->GetNS("Javascript.evaluate")->ToMethod();
		SharedValue ctx = this->Get("$js_context");
		
		for (size_t c = 0; c < args.size(); c++)
		{
			// first convert the path to a full URL file path
			ValueList a;
			a.push_back(args.at(c));
			SharedValue result = appURLToPath->Call(a);
			const char *path = result->ToString();
			
			logger->Debug("attempting to import worker script = %s",path);
			
			// import each script
			ValueList ea;
			ea.push_back(Value::NewString("text/url"));
			ea.push_back(Value::NewString(path));
			ea.push_back(ctx);
			evaluate->Call(ea);
		}
	}
}	