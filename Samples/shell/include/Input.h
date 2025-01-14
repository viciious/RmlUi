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

#ifndef SHELLINPUT_H
#define SHELLINPUT_H

#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/Types.h>

namespace Rml {
namespace Core {

class Context;

}
}

/**
 */

class Input
{
public:
	/// Sets the context to send input events to.
	/// @param[in] context The context to send input events to.
	static void SetContext(Rml::Core::Context* context);
	/// Returns the character code for a key identifer / key modifier combination.
	/// @param[in] key_identifier The key to generate a character code for.
	/// @param[in] key_modifier_state The configuration of the key modifiers.
	/// @return The character code.
	static Rml::Core::word GetCharacterCode(Rml::Core::Input::KeyIdentifier key_identifier, int key_modifier_state);

protected:
	static Rml::Core::Context* context;
};

#endif
