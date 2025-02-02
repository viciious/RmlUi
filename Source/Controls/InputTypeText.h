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

#ifndef RMLUICONTROLSINPUTTYPETEXT_H
#define RMLUICONTROLSINPUTTYPETEXT_H

#include "InputType.h"

namespace Rml {
namespace Controls {

class WidgetTextInput;

/**
	A single-line input type handler.

	@author Peter Curry
 */

class InputTypeText : public InputType
{
public:
	enum Visibility
	{
		VISIBLE,
		OBSCURED
	};

	InputTypeText(ElementFormControlInput* element, Visibility visibility = VISIBLE);
	virtual ~InputTypeText();

	/// Called every update from the host element.
	virtual void OnUpdate();

	/// Called every render from the host element.
	virtual void OnRender();

	/// Checks for necessary functional changes in the control as a result of changed attributes.
	/// @param[in] changed_attributes The list of changed attributes.
	/// @return True if no layout is required, false if the layout needs to be dirtied.
	virtual bool OnAttributeChange(const Core::AttributeNameList& changed_attributes);
	/// Called when properties on the control are changed.
	/// @param[in] changed_properties The properties changed on the element.
	virtual void OnPropertyChange(const Core::PropertyNameList& changed_properties);

	/// Checks for necessary functional changes in the control as a result of the event.
	/// @param[in] event The event to process.
	virtual void ProcessEvent(Core::Event& event);

	/// Sizes the dimensions to the element's inherent size.
	/// @return True.
	virtual bool GetIntrinsicDimensions(Rml::Core::Vector2f& dimensions);

private:
	int size;

	WidgetTextInput* widget;
};

}
}

#endif
