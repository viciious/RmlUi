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

#include "../../Include/RmlUi/Controls/ElementForm.h"
#include "../../Include/RmlUi/Core/Dictionary.h"
#include "../../Include/RmlUi/Core/ElementUtilities.h"
#include "../../Include/RmlUi/Controls/ElementFormControl.h"

namespace Rml {
namespace Controls {

// Constructs a new ElementForm. This should not be called directly; use the Factory instead.
ElementForm::ElementForm(const Rml::Core::String& tag) : Core::Element(tag)
{
}

ElementForm::~ElementForm()
{
}

// Submits the form.
void ElementForm::Submit(const Rml::Core::String& name, const Rml::Core::String& submit_value)
{
	Rml::Core::Dictionary values;
	if (name.Empty())
		values.Set("submit", submit_value);
	else
		values.Set(name, submit_value);

	Core::ElementList form_controls;
	Core::ElementUtilities::GetElementsByTagName(form_controls, this, "input");
	Core::ElementUtilities::GetElementsByTagName(form_controls, this, "textarea");
	Core::ElementUtilities::GetElementsByTagName(form_controls, this, "select");
	Core::ElementUtilities::GetElementsByTagName(form_controls, this, "dataselect");

	for (size_t i = 0; i < form_controls.size(); i++)
	{
		ElementFormControl* control = dynamic_cast< ElementFormControl* >(form_controls[i]);
		if (!control)
			continue;

		// Skip disabled controls.
		if (control->IsDisabled())
			continue;

		// Only process controls that should be submitted.
		if (!control->IsSubmitted())
			continue;

		Rml::Core::String control_name = control->GetName();
		Rml::Core::String control_value = control->GetValue();

		// Skip over unnamed form controls.
		if (control_name.Empty())
			continue;

		// If the item already exists, append to it.
		Rml::Core::Variant* value = values.Get(control_name);
		if (value != NULL)
			value->Reset(value->Get< Rml::Core::String >() + ", " + control_value);
		else
			values.Set< Rml::Core::String >(control_name, control_value);					
	}

	DispatchEvent("submit", values);
}

}
}
