/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license. 
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */	
#include "worker.h"

namespace ti
{
	Worker::Worker(Host *host, kroll::SharedKObject global, kroll::SharedKObject scope, std::string &code) : 
		host(host), global(global), scope(scope), code(code), stopped(false)
	{
		this->SetMethod("terminate",&Worker::Terminate);
		this->SetMethod("postMessage",&Worker::PostMessage);
		
		this->context = new WorkerContext(host,scope,this);
		this->adapter = new Poco::RunnableAdapter<Worker>(*this, &Worker::Run);
		this->thread.start(*adapter);
	}
	Worker::~Worker()
	{
		if (!this->stopped)
		{
			this->stopped=true;
			this->condition.signal();
		}
		if (this->thread.isRunning())
		{
			try
			{
				this->thread.join();
			}
			catch (Poco::Exception& e)
			{
				Logger *logger = Logger::Get("Worker");
				logger->Error(
					"Exception while try to join with thread: %s",
					e.displayText().c_str());
			}
		}
		delete this->adapter;
		this->global_worker = NULL;
		this->global = NULL;
		this->context = NULL;
	}
	void Worker::Run()
	{
		Logger *logger = Logger::Get("Worker");
		
		SharedKMethod onmessage;
		bool error = false;
		
		try
		{
			SharedValue evaluateCValue = global->GetNS("Javascript.evaluateInNewContext");
			if (!evaluateCValue->IsMethod())
			{
				Logger *logger = Logger::Get("Worker");
				logger->Error("Couldn't find Javascript.evaluateInNewContext");
				return;
			}
			SharedKMethod evaluate = evaluateCValue->ToMethod();
			ValueList args;
			args.push_back(Value::NewString(this->code));
			args.push_back(Value::NewNull());
			args.push_back(Value::NewObject(this->context));

			logger->Debug("before evaluate of worker code = %s",this->code.c_str());
			SharedValue global_result = evaluate->Call(args);
			this->global_worker = global_result->ToObject();
			this->code = "";
			
			// get the onmessage (if one is found)
			SharedValue evaluateValue = global->GetNS("Javascript.evaluate");
			evaluate = evaluateValue->ToMethod();
			ValueList args2;
			args2.push_back(Value::NewString("text/javascript"));
			args2.push_back(Value::NewString("onmessage"));
			args2.push_back(global_worker->Get("$js_context"));
			SharedValue onmessage_result = evaluate->Call(args2);
			if (onmessage_result->IsMethod())
			{
				onmessage = onmessage_result->ToMethod();
			}
		}
		catch(Poco::Exception &e)
		{
			error = true;
			logger->Error("Error loading worker. Error = %s",e.displayText().c_str());
		}
		catch(std::exception &e)
		{
			error = true;
			logger->Error("Error loading worker. Error = %s", e.what());
		}
		
		if (!error)
		{
			if (!onmessage.isNull())
			{
				logger->Debug("waiting for messages");

				// run this thread and wait for pending messages or to be 
				// woken up to stop
				for(;;)
				{
					bool wait = true;
					{
						Poco::ScopedLock<Poco::Mutex> lock(mutex);
						if (this->messages.size()>0)
						{
							wait=false;
						}
					}
					if (wait)
					{
						condition.wait(condmutex);
					}
					// we have to make a copy since calling the onmessage could be re-entrant
					// which would cause the postMessage to deadlock. we hold the lock to 
					// make a copy of the contents of the list and then iterate w/o lock
					std::list<std::string> copy;
					{
						// lock inside block only to make copy
						Poco::ScopedLock<Poco::Mutex> lock(mutex);
						if (this->messages.size()>0)
						{
							std::list<std::string>::iterator i = this->messages.begin();
							while (i!=this->messages.end())
							{
								std::string message = (*i++);
								copy.push_back(message);
							}
							this->messages.clear();
						}
					}
					if (copy.size()>0)
					{
						std::list<std::string>::iterator i = copy.begin();
						while(i!=copy.end())
						{
							std::string message = (*i++);
							ValueList args;
							args.push_back(Value::NewString(message));
							try
							{
								logger->Debug("calling worker with 'onmessage' and message = %s",message.c_str());
								onmessage->Call(args);
							}
							catch(std::exception &e)
							{
								logger->Error("Error dispatching worker message, exception = %s",e.what());
							}
						}
					}
					if (stopped) break;
				}
			}
			else
			{
				logger->Debug("no 'onmessage' so we're going to just wait");

				while(!stopped)
				{
					condition.wait(condmutex);
				}
			}
		}
		
		this->global_worker = NULL;
		this->global = NULL;
		this->context = NULL;
		
		logger->Debug("exiting Worker thread");
	}
	
	void Worker::Terminate(const ValueList& args, SharedValue result)
	{
		stopped=true;
		this->condition.signal();
	}

	void Worker::PostMessage(const ValueList& args, SharedValue result)
	{
		Logger *logger = Logger::Get("Worker");
		logger->Debug("Begin PostMessage");
		
		// store the message in our queue (waiting for the lock) and
		// then signal the thread to wake up to process the message
		std::string message = args.at(0)->ToString();
		{
			logger->Debug("Begin PostMessage waiting for lock");
			Poco::ScopedLock<Poco::Mutex> lock(mutex);
			this->messages.push_back(message);
		}
		this->condition.signal();
		logger->Debug("After PostMessage");
	}
	
	void Worker::Bound(const char *name, SharedValue value)
	{
		std::string n = name;
		
		if (this->context && n == "onmessage")
		{
			SharedPtr<WorkerContext> c = this->context.cast<WorkerContext>();
			c->OnMessageAttached(value->ToMethod());
		}
	}
	
}