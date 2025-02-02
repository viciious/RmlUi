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

#include "DecoratorStarfield.h"
#include <RmlUi/Core/Math.h>
#include <RmlUi/Core/Types.h>
#include "DecoratorInstancerStarfield.h"

DecoratorInstancerStarfield::DecoratorInstancerStarfield()
{
	RegisterProperty("num-layers", "3").AddParser("number");
	RegisterProperty("top-colour", "#dddc").AddParser("color");
	RegisterProperty("bottom-colour", "#333c").AddParser("color");
	RegisterProperty("top-speed", "10.0").AddParser("number");
	RegisterProperty("bottom-speed", "2.0").AddParser("number");
	RegisterProperty("top-density", "15").AddParser("number");
	RegisterProperty("bottom-density", "10").AddParser("number");
}

DecoratorInstancerStarfield::~DecoratorInstancerStarfield()
{
}

// Instances a decorator given the property tag and attributes from the RCSS file.
Rml::Core::Decorator* DecoratorInstancerStarfield::InstanceDecorator(const Rml::Core::String& RMLUI_UNUSED_PARAMETER(name), const Rml::Core::PropertyDictionary& properties)
{
	RMLUI_UNUSED(name);

	int num_layers = Rml::Core::Math::RealToInteger(properties.GetProperty("num-layers")->Get< float >());
	Rml::Core::Colourb top_colour = properties.GetProperty("top-colour")->Get< Rml::Core::Colourb >();
	Rml::Core::Colourb bottom_colour = properties.GetProperty("bottom-colour")->Get< Rml::Core::Colourb >();
	float top_speed = properties.GetProperty("top-speed")->Get< float >();
	float bottom_speed = properties.GetProperty("bottom-speed")->Get< float >();
	int top_density = Rml::Core::Math::RealToInteger(properties.GetProperty("top-density")->Get< float >());
	int bottom_density = Rml::Core::Math::RealToInteger(properties.GetProperty("bottom-density")->Get< float >());

	DecoratorStarfield* decorator = new DecoratorStarfield();
	if (decorator->Initialise(num_layers, top_colour, bottom_colour, top_speed, bottom_speed, top_density, bottom_density))
		return decorator;

	decorator->RemoveReference();
	ReleaseDecorator(decorator);
	return NULL;
}

// Releases the given decorator.
void DecoratorInstancerStarfield::ReleaseDecorator(Rml::Core::Decorator* decorator)
{
	delete decorator;
}

// Releases the instancer.
void DecoratorInstancerStarfield::Release()
{
	delete this;
}
