/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _UI_BINDING_H_
#define _UI_BINDING_H_

#include <kroll/kroll.h>
#include "menu_item.h"

namespace ti
{
	class UIBinding : public AccessorBoundObject
	{

	public:
		static int CENTERED;
		static std::string FOCUSED;
		static std::string UNFOCUSED;
		static std::string OPEN;
		static std::string OPENED;
		static std::string CLOSE;
		static std::string CLOSED;
		static std::string HIDDEN;
		static std::string SHOWN;
		static std::string FULLSCREENED;
		static std::string UNFULLSCREENED;
		static std::string MAXIMIZED;
		static std::string MINIMIZED;
		static std::string RESIZED;
		static std::string MOVED;
		static std::string PAGE_INITIALIZED;
		static std::string PAGE_LOADED;
		static std::string CREATE;
		static std::string ACTIVATE;
		static std::string CLICKED;
		UIBinding(Host *host);

		virtual ~UIBinding();
		Host* GetHost();
		static UIBinding* GetInstance() { return instance; }
		virtual void CreateMainWindow(WindowConfig*);
		virtual AutoUserWindow CreateWindow(WindowConfig*, AutoUserWindow& parent) = 0;
		virtual void ErrorDialog(std::string);

		std::vector<AutoUserWindow>& GetOpenWindows();
		void AddToOpenWindows(AutoUserWindow);
		void RemoveFromOpenWindows(AutoUserWindow);
		void ClearTray();
		void UnregisterTrayItem(TrayItem*);
		static void SendEventToListeners(
			std::vector<SharedKMethod> eventListeners,
			std::string eventType,
			SharedKObject eventSource,
			SharedKObject event = new StaticBoundObject());
		static void SendEventToListener(SharedKMethod listener, SharedKObject event);

		void _GetOpenWindows(const ValueList& args, SharedValue result);
		void _GetMainWindow(const ValueList& args, SharedValue result);
		void _CreateMenu(const ValueList& args, SharedValue result);
		void _CreateMenuItem(const ValueList& args, SharedValue result);
		void _CreateCheckMenuItem(const ValueList& args, SharedValue result);
		void _CreateSeparatorMenuItem(const ValueList& args, SharedValue result);
		AutoMenu __CreateMenu(const ValueList& args);
		AutoMenuItem __CreateMenuItem(const ValueList& args);
		AutoMenuItem __CreateCheckMenuItem(const ValueList& args);
		AutoMenuItem __CreateSeparatorMenuItem(const ValueList& args);
		void _SetMenu(const ValueList& args, SharedValue result);
		void _GetMenu(const ValueList& args, SharedValue result);
		void _SetContextMenu(const ValueList& args, SharedValue result);
		void _GetContextMenu(const ValueList& args, SharedValue result);
		void _SetIcon(const ValueList& args, SharedValue result);
		void _AddTray(const ValueList& args, SharedValue result);
		void _ClearTray(const ValueList& args, SharedValue result);
		void _GetIdleTime(const ValueList& args, SharedValue result);

		/* OS X specific callbacks */
		void _SetDockIcon(const ValueList& args, SharedValue result);
		void _SetDockMenu(const ValueList& args, SharedValue result);
		void _SetBadge(const ValueList& args, SharedValue result);
		void _SetBadgeImage(const ValueList& args, SharedValue result);

		virtual AutoMenu CreateMenu() = 0;
		virtual AutoMenuItem CreateMenuItem() = 0;;
		virtual AutoMenuItem CreateCheckMenuItem() = 0;
		virtual AutoMenuItem CreateSeparatorMenuItem() = 0;
		virtual void SetMenu(AutoMenu) = 0;
		virtual void SetContextMenu(AutoMenu) = 0;
		virtual void SetIcon(std::string& iconPath) = 0;
		virtual AutoTrayItem AddTray(std::string& iconPath, SharedKMethod cb) = 0;
		virtual AutoMenu GetMenu() = 0;
		virtual AutoMenu GetContextMenu() = 0;
		virtual long GetIdleTime() = 0;

		/* These have empty impls, because are OS X-only for now */
		virtual void SetDockIcon(std::string& icon_path) {}
		virtual void SetDockMenu(AutoMenu) {}
		virtual void SetBadge(std::string& badgeLabel) {}
		virtual void SetBadgeImage(std::string& badgeImagePath) {}

	protected:
		static UIBinding* instance;
		Host* host;
		AutoUserWindow mainWindow;
		std::vector<AutoUserWindow> openWindows;
		std::vector<AutoTrayItem> trayItems;
		std::string iconURL;
	};
}

#endif
