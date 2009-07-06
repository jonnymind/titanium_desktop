/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _OSX_INPUT_PIPE_H_
#define _OSX_INPUT_PIPE_H_

#include <kroll/kroll.h>
#include "../input_pipe.h"
#include <Cocoa/Cocoa.h>

namespace ti { class OSXInputPipe; }

@interface TiDataReady : NSObject {
	ti::OSXInputPipe *pipe;
}

-(id)initWithPipe:(ti::OSXInputPipe*)pipe;
-(void)dataReady:(NSNotification *)aNotification;
@end


namespace ti
{
	class OSXInputPipe : public InputPipe
	{
	public:
		OSXInputPipe();
		virtual ~OSXInputPipe();
		
		virtual void Close();
		virtual bool IsClosed();
		virtual SharedPtr<Blob> Read(int bufsize=-1);
		NSPipe* GetPipe() { return pipe; }
		
	protected:
		NSPipe* pipe;
		NSFileHandle* handle;
		TiDataReady* dataReady;
		bool closed;
	};
}

#endif