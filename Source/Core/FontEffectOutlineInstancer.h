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

#ifndef RMLUICOREFONTEFFECTOUTLINEINSTANCER_H
#define RMLUICOREFONTEFFECTOUTLINEINSTANCER_H

#include "../../Include/RmlUi/Core/FontEffectInstancer.h"

namespace Rml {
namespace Core {

/**
	A concrete font effect instancer for the outline effect.

	@author Peter Curry
 */

class FontEffectOutlineInstancer : public FontEffectInstancer
{
public:
	FontEffectOutlineInstancer();
	virtual ~FontEffectOutlineInstancer();

	/// Instances an outline font effect.
	/// @param[in] name The type of font effect desired.
	/// @param[in] properties All RCSS properties associated with the outline effect.
	/// @return The font effect if it was instanced successfully, NULL if an error occured.
	virtual FontEffect* InstanceFontEffect(const String& name, const PropertyDictionary& properties);
	/// Releases the outline effect.
	/// @param[in] font_effect Font effect to release.
	virtual void ReleaseFontEffect(FontEffect* font_effect);

	/// Releases the instancer.
	virtual void Release();
};

}
}

#endif
