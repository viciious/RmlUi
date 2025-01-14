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

#include "../../Include/RmlUi/Controls/ElementDataGridCell.h"
#include "../../Include/RmlUi/Core/Event.h"
#include "../../Include/RmlUi/Core/Property.h"
#include "../../Include/RmlUi/Controls/ElementDataGrid.h"

namespace Rml {
namespace Controls {

ElementDataGridCell::ElementDataGridCell(const Rml::Core::String& tag) : Core::Element(tag)
{
}

ElementDataGridCell::~ElementDataGridCell()
{
	if (header) {
		header->RemoveEventListener("resize", this);
		header->RemoveReference();
	}
}

void ElementDataGridCell::Initialise(int _column, Core::Element* _header)
{
	column = _column;
	header = _header;
	if (header)
	{
		header->AddReference();
		header->AddEventListener("resize", this);
		SetProperty("width", Core::Property(header->GetBox().GetSize(Core::Box::MARGIN).x, Core::Property::PX));
	}
}

int ElementDataGridCell::GetColumn()
{
	return column;
}

void ElementDataGridCell::ProcessEvent(Core::Event& event)
{
	Core::Element::ProcessEvent(event);

	if (event == "resize")
	{
		if (event.GetTargetElement() == header)
		{
			SetProperty("width", Core::Property(header->GetBox().GetSize(Core::Box::MARGIN).x, Core::Property::PX));
		}
	}
}

}
}
