/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _WORKER_H_
#define _WORKER_H_

#include <kroll/kroll.h>
#include <Poco/Thread.h>
#include <Poco/RunnableAdapter.h>
#include <Poco/Condition.h>
#include <Poco/Mutex.h>
#include <Poco/ScopedLock.h>

#include "worker_context.h"

namespace ti
{
	class Worker : public StaticBoundObject
	{
	public:
		Worker(Host *host, kroll::SharedKObject global, kroll::SharedKObject scope, std::string& code);
		virtual ~Worker();
		
		kroll::SharedKObject GetGlobalWorker() { return global_worker; };
		
	private:
		kroll::Host *host;
		kroll::SharedKObject global;
		kroll::SharedKObject scope;
		std::string code;
		bool stopped;

		kroll::SharedKObject global_worker;
		kroll::SharedKObject context;
		
		Poco::Thread thread;
		Poco::RunnableAdapter<Worker>* adapter;
		Poco::Condition condition;
		Poco::Mutex condmutex;
		Poco::Mutex mutex;
		std::list<std::string> messages;
		
		void Run();
		void Bound(const char *name, SharedValue value);

		void Terminate(const ValueList& args, SharedValue result);
		void PostMessage(const ValueList& args, SharedValue result);

	};
}

#endif
