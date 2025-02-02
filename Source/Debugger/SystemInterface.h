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

#ifndef RMLUIDEBUGGERSYSTEMINTERFACE_H
#define RMLUIDEBUGGERSYSTEMINTERFACE_H

#include "../../Include/RmlUi/Core/SystemInterface.h"

namespace Rml {
namespace Debugger {

class ElementLog;

/**
	The log interface the debugger installs into RmlUi. This is a pass-through interface, so it holds onto the
	application's system interface and passes all the calls through.

	@author Peter Curry
 */

class SystemInterface : public Core::SystemInterface
{
public:
	/// Instances a new debugging log interface.
	/// @param[in] log The logging element to send messages to.
	SystemInterface(ElementLog* log);
	virtual ~SystemInterface();

	/// Get the number of seconds elapsed since the start of the application.
	/// @return Elapsed time, in seconds.
	virtual double GetElapsedTime();

	/// Translate the input string into the translated string.
	/// @param[out] translated Translated string ready for display.
	/// @param[in] input String as received from XML.
	/// @return Number of translations that occured.
	virtual int TranslateString(Core::String& translated, const Core::String& input);

	/// Log the specified message.
	/// @param[in] type Type of log message, ERROR, WARNING, etc.
	/// @param[in] message Message to log.
	/// @return True to continue execution, false to break into the debugger.
	virtual bool LogMessage(Core::Log::Type type, const Core::String& message);

	/// Set mouse cursor.
	/// @param[in] cursor_name Cursor name to activate.
	virtual void SetMouseCursor(const Core::String& cursor_name);

	/// Activate keyboard (for touchscreen devices)
	virtual void ActivateKeyboard();
	
	/// Deactivate keyboard (for touchscreen devices)
	virtual void DeactivateKeyboard();
private:
	Core::SystemInterface* application_interface;
	ElementLog* log;
};

}
}

#endif
