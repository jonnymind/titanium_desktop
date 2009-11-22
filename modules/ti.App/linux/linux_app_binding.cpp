/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#include <kroll/kroll.h>
#include "../app_binding.h"

namespace ti
{

void AppBinding::Restart(const ValueList& args, KValueRef result)
{
	Host* host = Host::GetInstance();
	std::string cmdline(host->GetApplication()->arguments.at(0));

	// Remove all quotes.
	size_t i = cmdline.find('\"');
	while (i != std::string::npos)
	{
		cmdline.replace(i, 1, "");
		i = cmdline.find('\"');
	}

	std::string script = "\"" + cmdline + "\" &";
	int retval = system(script.c_str());
	if ( retval != 0 )
	{
		//TODO; signal error
	}

	host->Exit(0);
}

}
