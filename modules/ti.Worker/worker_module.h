/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license. 
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef TI_WORKER_MODULE_H_
#define TI_WORKER_MODULE_H_

#include <kroll/kroll.h>

#if defined(OS_OSX) || defined(OS_LINUX)
#define EXPORT __attribute__((visibility("default")))
#define TITANIUM_WORKER_API EXPORT
#elif defined(OS_WIN32)
# ifdef TITANIUM_WORKER_API_EXPORT
#  define TITANIUM_WORKER_API __declspec(dllexport)
# else
#  define TITANIUM_WORKER_API __declspec(dllimport)
# endif
# define EXPORT __declspec(dllexport)
#endif

namespace ti 
{
	class TITANIUM_WORKER_API WorkerModule : public kroll::Module
	{
		KROLL_MODULE_CLASS(WorkerModule)
		
	private:
		kroll::SharedKObject binding;
	};

}
#endif
