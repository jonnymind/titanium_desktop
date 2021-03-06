/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _IRC_CLIENT_BINDING_H_
#define _IRC_CLIENT_BINDING_H_

#include <kroll/kroll.h>
#include <Poco/Thread.h>
#ifdef OS_LINUX
#include <unistd.h>
#endif
#include "IRC.h"

namespace ti
{
	class IRCClientBinding : public StaticBoundObject
	{
	public:
		IRCClientBinding(Host*);
		virtual ~IRCClientBinding();
	private:
		Host* host;
		SharedKObject global;
		IRC irc;
		SharedKMethod callback;
		Poco::Thread *thread;
		
		static void Run(void*);
		static int Callback(char *cmd, char* params, irc_reply_data* data, void* conn, void* pd);
		
		
		void Connect(const ValueList& args, SharedValue result);
		void Disconnect(const ValueList& args, SharedValue result);
		void Send(const ValueList& args, SharedValue result);
		void SetNick(const ValueList& args, SharedValue result);
		void GetNick(const ValueList& args, SharedValue result);
		void Join(const ValueList& args, SharedValue result);
		void Unjoin(const ValueList& args, SharedValue result);
		void IsOp(const ValueList& args, SharedValue result);
		void IsVoice(const ValueList& args, SharedValue result);
		void GetUsers(const ValueList& args, SharedValue result);

	};
}

#endif
