/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */


#ifndef _LIBNOTIFY_BINDING_H_
#define _LIBNOTIFY_BINDING_H_

#include <kroll/kroll.h>

using kroll::SharedKObject;

namespace ti
{
	class LibNotifyBinding : public GrowlBinding
	{
	public:
		LibNotifyBinding(SharedKObject);
		~LibNotifyBinding();

	protected:
		bool IsRunning();
		void ShowNotification(
			std::string& title,
			std::string& description,
			std::string& iconURL,
			int notification_delay,
			SharedKMethod callback);

		std::string GetAppName();
	};
}

#endif
