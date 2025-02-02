/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2014 Markus Schöngart
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

#ifndef RMLUICORETRANSFORM_H
#define RMLUICORETRANSFORM_H

#include "Header.h"
#include "TransformPrimitive.h"

namespace Rml {
namespace Core {

class Property;

/**
	The Transform class holds the information parsed from an element's
	`transform' property.  It is one of the primitive types that a Variant
	can assume.  The method `ComputeFinalTransform()' computes the
	transformation matrix that is to be applied to the current
	projection/view matrix in order to render the associated element.

	@author Markus Schöngart
	@see Rml::Core::Variant
 */

class RMLUICORE_API Transform
{
public:
	typedef std::vector< Transforms::Primitive > Primitives;

	/// Default constructor, initializes an identity transform
	Transform();

	/// Construct transform with a list of primitives
	Transform(std::vector<Transforms::Primitive> primitives);

	/// Helper function to create a Property with TransformRef from list of primitives
	static Property MakeProperty(std::vector<Transforms::Primitive> primitives);

	/// Remove all Primitives from this Transform
	void ClearPrimitives();

	/// Add a Primitive to this Transform
	void AddPrimitive(const Transforms::Primitive& p);

	/// Return the number of Primitives in this Transform
	int GetNumPrimitives() const noexcept;

	/// Return the i-th Primitive in this Transform
	const Transforms::Primitive& GetPrimitive(int i) const noexcept;

	Primitives& GetPrimitives() noexcept { return primitives; }
	const Primitives& GetPrimitives() const noexcept { return primitives; }

private:
	Primitives primitives;
};


RMLUICORE_API String ToString(const Transform& transform);

}
}

#endif
