/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#include <kroll/kroll.h>
#include "ui_module.h"
namespace ti
{
	using std::string;
	using std::vector;

	MenuItem::MenuItem(MenuItemType type) :
		StaticBoundObject("UI.MenuItem"),
		type(type),
		enabled(true),
		label(""),
		submenu(0),
		state(false),
		autoCheck(true)
	{
		this->SetMethod("isSeparator", &MenuItem::_IsSeparator);
		this->SetMethod("isCheck", &MenuItem::_IsCheck);

		if (this->type == NORMAL || this->type == CHECK)
		{
			this->SetMethod("setLabel", &MenuItem::_SetLabel);
			this->SetMethod("getLabel", &MenuItem::_GetLabel);
			this->SetMethod("setSubmenu", &MenuItem::_SetSubmenu);
			this->SetMethod("getSubmenu", &MenuItem::_GetSubmenu);
			this->SetMethod("enable", &MenuItem::_Enable);
			this->SetMethod("disable", &MenuItem::_Disable);
			this->SetMethod("isEnabled", &MenuItem::_IsEnabled);

			this->SetMethod("addSubmenu", &MenuItem::_AddSubmenu);
			this->SetMethod("addItem", &MenuItem::_AddItem);
			this->SetMethod("addSeparatorItem", &MenuItem::_AddSeparatorItem);
			this->SetMethod("addCheckItem", &MenuItem::_AddCheckItem);

			this->SetMethod("addEventListener", &MenuItem::_AddEventListener);
			this->SetMethod("removeEventListener", &MenuItem::_RemoveEventListener);
			this->SetMethod("getEventListeners", &MenuItem::_GetEventListeners);

			this->SetMethod("click", &MenuItem::_Click);
		}

		if (this->type == NORMAL)
		{
			this->SetMethod("setIcon", &MenuItem::_SetIcon);
			this->SetMethod("getIcon", &MenuItem::_GetIcon);
		}

		if (this->type == CHECK)
		{
			this->SetMethod("setState", &MenuItem::_SetState);
			this->SetMethod("getState", &MenuItem::_GetState);
			this->SetMethod("setAutoCheck", &MenuItem::_SetAutoCheck);
			this->SetMethod("isAutoCheck", &MenuItem::_IsAutoCheck);
		}
	}

	MenuItem::~MenuItem()
	{
	}

	void MenuItem::_IsSeparator(const ValueList& args, SharedValue result)
	{
		result->SetBool(this->type == SEPARATOR);
	}

	void MenuItem::_IsCheck(const ValueList& args, SharedValue result)
	{
		result->SetBool(this->type == CHECK);
	}

	void MenuItem::_SetLabel(const ValueList& args, SharedValue result)
	{
		args.VerifyException("setLabel", "s|0");
		string newLabel = args.GetString(0, "");
		this->SetLabel(newLabel);
	}

	void MenuItem::_GetLabel(const ValueList& args, SharedValue result)
	{
		result->SetString(this->label);
	}

	void MenuItem::_SetIcon(const ValueList& args, SharedValue result)
	{
		args.VerifyException("setIcon", "s|0");
		std::string newIcon = "";
		if (args.size() > 0) {
			newIcon = args.GetString(0);
		}
		this->SetIcon(newIcon);
	}

	void MenuItem::_GetIcon(const ValueList& args, SharedValue result)
	{
		result->SetString(this->iconURL);
	}

	void MenuItem::_SetState(const ValueList& args, SharedValue result)
	{
		args.VerifyException("setState", "b");
		this->SetState(args.GetBool(0));
	}

	void MenuItem::_GetState(const ValueList& args, SharedValue result)
	{
		result->SetBool(this->state);
	}

	void MenuItem::_AddEventListener(const ValueList& args, SharedValue result)
	{
		args.VerifyException("addEventListener", "m|0");
		SharedKMethod newCallback = args.GetMethod(0, NULL);
		this->AddEventListener(newCallback);
	}

	void MenuItem::_RemoveEventListener(const ValueList& args, SharedValue result)
	{
	}

	void MenuItem::_GetEventListeners(const ValueList& args, SharedValue result)
	{
	}

	void MenuItem::_SetSubmenu(const ValueList& args, SharedValue result)
	{
		args.VerifyException("setCallback", "o|0");
		AutoMenu newSubmenu = NULL;

		if (args.at(0)->IsObject())
		{
			SharedKObject o = args.at(0)->ToObject();
			o = KObject::Unwrap(o);
			newSubmenu = o.cast<Menu>();
		}

		this->submenu = newSubmenu;
		this->SetSubmenuImpl(newSubmenu);
	}

	void MenuItem::_GetSubmenu(const ValueList& args, SharedValue result)
	{
		if (this->submenu.isNull())
		{
			result->SetNull();
		}
		else
		{
			result->SetObject(this->submenu);
		}
	}

	void MenuItem::_Enable(const ValueList& args, SharedValue result)
	{
		this->enabled = true;
		this->SetEnabledImpl(true);
	}

	void MenuItem::_Disable(const ValueList& args, SharedValue result)
	{
		this->enabled = false;
		this->SetEnabledImpl(true);
	}

	void MenuItem::_SetAutoCheck(const ValueList& args, SharedValue result)
	{
		args.VerifyException("setAutoCheck", "b");
		this->autoCheck = args.GetBool(0);
	}

	void MenuItem::_IsAutoCheck(const ValueList& args, SharedValue result)
	{
		result->SetBool(this->autoCheck);
	}

	void MenuItem::_IsEnabled(const ValueList& args, SharedValue result)
	{
		result->SetBool(this->enabled);
	}

	void MenuItem::_Click(const ValueList& args, SharedValue result)
	{
		this->HandleClickEvent(0);
	}

	void MenuItem::_AddSubmenu(const ValueList& args, SharedValue result)
	{
		UIBinding* binding = UIBinding::GetInstance();

		AutoMenuItem newItem = binding->__CreateMenuItem(args);
		newItem->EnsureHasSubmenu();
		this->EnsureHasSubmenu();
		this->submenu->AppendItem(newItem);

		result->SetObject(newItem);
	}

	void MenuItem::_AddItem(const ValueList& args, SharedValue result)
	{
		args.VerifyException("addItem", "?s m|0 s|0");
		UIBinding* binding = UIBinding::GetInstance();

		// Create a menu item object and add it to this item's submenu
		AutoMenuItem newItem = binding->__CreateMenuItem(args);
		this->EnsureHasSubmenu();
		this->submenu->AppendItem(newItem);

		result->SetObject(newItem);
	}

	void MenuItem::_AddSeparatorItem(const ValueList& args, SharedValue result)
	{
		UIBinding* binding = UIBinding::GetInstance();
		AutoMenuItem newItem = binding->__CreateSeparatorMenuItem(args);
		this->EnsureHasSubmenu();
		this->submenu->AppendItem(newItem);

		result->SetObject(newItem);
	}

	void MenuItem::_AddCheckItem(const ValueList& args, SharedValue result)
	{
		UIBinding* binding = UIBinding::GetInstance();

		// Create a menu item object
		AutoMenuItem newItem = binding->__CreateCheckMenuItem(args);
		this->EnsureHasSubmenu();
		this->submenu->AppendItem(newItem);

		result->SetObject(newItem);
	}

	void MenuItem::AddEventListener(SharedKMethod eventListener)
	{
		std::vector<SharedKMethod>::iterator i = this->eventListeners.begin();
		while (i != this->eventListeners.end())
		{
			SharedKMethod m = (*i++);
			if (eventListener->Equals(m))
			{
				return;
			}
		}

		this->eventListeners.push_back(eventListener);
	}

	void MenuItem::HandleClickEvent(SharedKObject source)
	{
		if (this->IsCheck() && this->autoCheck)
		{
			// Execute this later on the main thread
			Host* host = Host::GetInstance();
			host->InvokeMethodOnMainThread(
				this->Get("setState")->ToMethod(),
				ValueList(Value::NewBool(!this->GetState())),
				false);
		}

		UIBinding::SendEventToListeners(
			this->eventListeners,
			UIBinding::CLICKED,
			source);
	}

	void MenuItem::SetState(bool newState)
	{
		this->state = newState;
		this->SetStateImpl(newState);
	}

	void MenuItem::SetLabel(string& newLabel)
	{
		this->label = newLabel;
		this->SetLabelImpl(newLabel);
	}

	void MenuItem::SetIcon(string& iconURL)
	{
		this->iconPath = this->iconURL = iconURL;
		if (!iconURL.empty()) {
			this->iconPath = URLToPathOrURL(this->iconURL);
		}
		this->SetIconImpl(this->iconPath); // platform-specific impl
	}

	bool MenuItem::GetState()
	{
		return this->state;
	}

	std::string& MenuItem::GetLabel()
	{
		return this->label;
	}

	bool MenuItem::IsSeparator()
	{
		return this->type == SEPARATOR;
	}

	bool MenuItem::IsCheck()
	{
		return this->type == CHECK;
	}

	bool MenuItem::IsEnabled()
	{
		return this->enabled;
	}

	void MenuItem::EnsureHasSubmenu()
	{
		if (this->submenu.isNull())
		{
			UIBinding* binding = UIBinding::GetInstance();
			AutoMenu newSubmenu = binding->CreateMenu();
			this->SetSubmenuImpl(newSubmenu);
			this->submenu = newSubmenu;
		}
	}
}
