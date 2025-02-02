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

#ifndef RMLUICORETYPES_H
#define RMLUICORETYPES_H

// Define NULL as zero.
#if !defined NULL
#define NULL 0
#endif

#include <float.h>
#include <limits.h>
#include <string>
#include <map>
#include <memory>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>

#include "Platform.h"
#include "Debug.h"

namespace Rml {
namespace Core {

// Define commonly used basic types.
typedef unsigned char byte;
typedef unsigned short word;
typedef double Time;
typedef float TimeDelta;
typedef unsigned int Hash;
typedef void* ScriptObject;

}
}

#ifdef RMLUI_PLATFORM_WIN32
typedef unsigned __int64 uint64_t;
#else
#include <inttypes.h>
#endif

#include "Colour.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "String.h"

namespace Rml {
namespace Core {

// Default colour types.
typedef Colour< float, 1 > Colourf;
typedef Colour< byte, 255 > Colourb;
typedef Vector2< int > Vector2i;
typedef Vector2< float > Vector2f;
typedef Vector3< int > Vector3i;
typedef Vector3< float > Vector3f;
typedef Vector4< int > Vector4i;
typedef Vector4< float > Vector4f;

typedef Matrix4< float, ColumnMajorStorage< float > > ColumnMajorMatrix4f;
typedef Matrix4< float, RowMajorStorage< float > > RowMajorMatrix4f;
typedef ColumnMajorMatrix4f Matrix4f;

class Element;
class Dictionary;
class ElementAnimation;
class Property;
class Transform;

// Types for external interfaces.
typedef uintptr_t FileHandle;
typedef uintptr_t TextureHandle;
typedef uintptr_t CompiledGeometryHandle;
typedef uintptr_t DecoratorDataHandle;

// List of elements.
typedef std::vector< Element* > ElementList;
typedef std::set< String > PseudoClassList;
typedef std::unordered_set< String > PropertyNameList;
typedef std::unordered_set< String > AttributeNameList;
typedef Dictionary ElementAttributes;
typedef std::vector< ElementAnimation > ElementAnimationList;
typedef std::map< String, Property > PropertyMap;

// Reference types
typedef std::shared_ptr< Transform > TransformRef;

struct Transition;
struct TransitionList;

}
}

#endif
