/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */
#ifndef _TI_FILE_H_
#define _TI_FILE_H_
#include <kroll/kroll.h>

#ifdef OS_WIN32
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <shlobj.h>
#elif OS_OSX
#import <Foundation/Foundation.h>
#endif

#include "file_stream.h" // TODO remove this include when read/write methods are removed from File class
#include <string>

namespace ti
{
	class File : public StaticBoundObject
	{
		public:
		File(std::string filename);
		virtual ~File();

		std::string& GetFilename() { return this->filename; }
		virtual SharedString DisplayString(int levels=3)
		{
			return new string(GetFilename());
		}

		private:
		std::string filename;
		ti::FileStream* readLineFS;

		void ToString(const ValueList& args, KValueRef result);
		void ToURL(const ValueList& args, KValueRef result);
		void IsFile(const ValueList& args, KValueRef result);
		void IsDirectory(const ValueList& args, KValueRef result);
		void IsHidden(const ValueList& args, KValueRef result);
		void IsSymbolicLink(const ValueList& args, KValueRef result);
		void IsExecutable(const ValueList& args, KValueRef result);
		void IsReadonly(const ValueList& args, KValueRef result);
		void IsWriteable(const ValueList& args, KValueRef result);
		void Resolve(const ValueList& args, KValueRef result);
		void Write(const ValueList& args, KValueRef result);
		void Read(const ValueList& args, KValueRef result);
		void ReadLine(const ValueList& args, KValueRef result);
		void Copy(const ValueList& args, KValueRef result);
		void Move(const ValueList& args, KValueRef result);
		void Rename(const ValueList& args, KValueRef result);
		void CreateDirectory(const ValueList& args, KValueRef result);
		void DeleteDirectory(const ValueList& args, KValueRef result);
		void DeleteFile(const ValueList& args, KValueRef result);
		void Equals(const ValueList& args, KValueRef result);
		void GetDirectoryListing(const ValueList& args, KValueRef result);
		void GetParent(const ValueList& args, KValueRef result);
		void GetExists(const ValueList& args, KValueRef result);
		void GetCreateTimestamp(const ValueList& args, KValueRef result);
		void GetModificationTimestamp(const ValueList& args, KValueRef result);
		void GetName(const ValueList& args, KValueRef result);
		void GetExtension(const ValueList& args, KValueRef result);
		void GetNativePath(const ValueList& args, KValueRef result);
		void GetSize(const ValueList& args, KValueRef result);
		void GetSpaceAvailable(const ValueList& args, KValueRef result);
		void CreateShortcut(const ValueList& args, KValueRef result);
		void SetExecutable(const ValueList& args, KValueRef result);
		void SetReadonly(const ValueList& args, KValueRef result);
		void SetWriteable(const ValueList& args, KValueRef result);
		void Unzip(const ValueList& args, KValueRef result);
	};
}

#endif 
