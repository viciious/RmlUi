/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "EventManager.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/ElementUtilities.h>
#include <Shell.h>
#include "EventHandler.h"
#include "GameDetails.h"
#include <map>

// The game's element context (declared in main.cpp).
extern Rml::Core::Context* context;

// The event handler for the current screen. This may be NULL if the current screen has no specific functionality.
static EventHandler* event_handler = NULL;

// The event handlers registered with the manager.
typedef std::map< Rml::Core::String, EventHandler* > EventHandlerMap;
EventHandlerMap event_handlers;

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

// Releases all event handlers registered with the manager.
void EventManager::Shutdown()
{
	for (EventHandlerMap::iterator i = event_handlers.begin(); i != event_handlers.end(); ++i)
		delete (*i).second;

	event_handlers.clear();
	event_handler = NULL;
}

// Registers a new event handler with the manager.
void EventManager::RegisterEventHandler(const Rml::Core::String& handler_name, EventHandler* handler)
{
	// Release any handler bound under the same name.
	EventHandlerMap::iterator iterator = event_handlers.find(handler_name);
	if (iterator != event_handlers.end())
		delete (*iterator).second;

	event_handlers[handler_name] = handler;
}

// Processes an event coming through from RmlUi.
void EventManager::ProcessEvent(Rml::Core::Event& event, const Rml::Core::String& value)
{
	Rml::Core::StringList commands;
	Rml::Core::StringUtilities::ExpandString(commands, value, ';');
	for (size_t i = 0; i < commands.size(); ++i)
	{
		// Check for a generic 'load' or 'exit' command.
		Rml::Core::StringList values;
		Rml::Core::StringUtilities::ExpandString(values, commands[i], ' ');

		if (values.empty())
			return;

		if (values[0] == "goto" &&
 			values.size() > 1)
		{
			// Load the window, and if successful close the old window.
			if (LoadWindow(values[1]))
				event.GetTargetElement()->GetOwnerDocument()->Close();
		}
		else if (values[0] == "load" &&
 			values.size() > 1)
		{
			// Load the window.
			LoadWindow(values[1]);
		}
		else if (values[0] == "close")
		{
			Rml::Core::ElementDocument* target_document = NULL;

			if (values.size() > 1)
				target_document = context->GetDocument(values[1].CString());
			else
				target_document = event.GetTargetElement()->GetOwnerDocument();

			if (target_document != NULL)
				target_document->Close();
		}
		else if (values[0] == "exit")
		{
			Shell::RequestExit();
		}
		else if (values[0] == "pause")
		{
			GameDetails::SetPaused(true);
		}
		else if (values[0] == "unpause")
		{
			GameDetails::SetPaused(false);
		}
		else
		{
			if (event_handler != NULL)
				event_handler->ProcessEvent(event, commands[i]);
		}
	}
}

// Loads a window and binds the event handler for it.
Rml::Core::ElementDocument* EventManager::LoadWindow(const Rml::Core::String& window_name)
{
	// Set the event handler for the new screen, if one has been registered.
	EventHandler* old_event_handler = event_handler;
	EventHandlerMap::iterator iterator = event_handlers.find(window_name);
	if (iterator != event_handlers.end())
		event_handler = (*iterator).second;
	else
		event_handler = NULL;

	// Attempt to load the referenced RML document.
	Rml::Core::String document_path = Rml::Core::String("invaders/data/") + window_name + Rml::Core::String(".rml");
	Rml::Core::ElementDocument* document = context->LoadDocument(document_path.CString());
	if (document == NULL)
	{
		event_handler = old_event_handler;
		return nullptr;
	}

	// Set the element's title on the title; IDd 'title' in the RML.
	Rml::Core::Element* title = document->GetElementById("title");
	if (title != NULL)
		title->SetInnerRML(document->GetTitle());

	document->Focus();
	document->Show();

	// Remove the caller's reference.
	document->RemoveReference();

	return document;
}
