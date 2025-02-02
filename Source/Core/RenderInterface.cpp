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

#include "precompiled.h"
#include "../../Include/RmlUi/Core/RenderInterface.h"
#include "TextureDatabase.h"

namespace Rml {
namespace Core {

RenderInterface::RenderInterface() : ReferenceCountable(0)
{
	context = NULL;
}

RenderInterface::~RenderInterface()
{
}

// Called by RmlUi when it wants to compile geometry it believes will be static for the forseeable future.
CompiledGeometryHandle RenderInterface::CompileGeometry(Vertex* RMLUI_UNUSED_PARAMETER(vertices), int RMLUI_UNUSED_PARAMETER(num_vertices), int* RMLUI_UNUSED_PARAMETER(indices), int RMLUI_UNUSED_PARAMETER(num_indices), TextureHandle RMLUI_UNUSED_PARAMETER(texture))
{
	RMLUI_UNUSED(vertices);
	RMLUI_UNUSED(num_vertices);
	RMLUI_UNUSED(indices);
	RMLUI_UNUSED(num_indices);
	RMLUI_UNUSED(texture);

	return 0;
}

// Called by RmlUi when it wants to render application-compiled geometry.
void RenderInterface::RenderCompiledGeometry(CompiledGeometryHandle RMLUI_UNUSED_PARAMETER(geometry), const Vector2f& RMLUI_UNUSED_PARAMETER(translation))
{
	RMLUI_UNUSED(geometry);
	RMLUI_UNUSED(translation);
}

// Called by RmlUi when it wants to release application-compiled geometry.
void RenderInterface::ReleaseCompiledGeometry(CompiledGeometryHandle RMLUI_UNUSED_PARAMETER(geometry))
{
	RMLUI_UNUSED(geometry);
}

// Called by RmlUi when a texture is required by the library.
bool RenderInterface::LoadTexture(TextureHandle& RMLUI_UNUSED_PARAMETER(texture_handle), Vector2i& RMLUI_UNUSED_PARAMETER(texture_dimensions), const String& RMLUI_UNUSED_PARAMETER(source))
{
	RMLUI_UNUSED(texture_handle);
	RMLUI_UNUSED(texture_dimensions);
	RMLUI_UNUSED(source);

	return false;
}

// Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
bool RenderInterface::GenerateTexture(TextureHandle& RMLUI_UNUSED_PARAMETER(texture_handle), const byte* RMLUI_UNUSED_PARAMETER(source), const Vector2i& RMLUI_UNUSED_PARAMETER(source_dimensions))
{
	RMLUI_UNUSED(texture_handle);
	RMLUI_UNUSED(source);
	RMLUI_UNUSED(source_dimensions);
	
	return false;
}

// Called by RmlUi when a loaded texture is no longer required.
void RenderInterface::ReleaseTexture(TextureHandle RMLUI_UNUSED_PARAMETER(texture))
{
	RMLUI_UNUSED(texture);
}

// Returns the native horizontal texel offset for the renderer.
float RenderInterface::GetHorizontalTexelOffset()
{
	return 0;
}

// Returns the native vertical texel offset for the renderer.
float RenderInterface::GetVerticalTexelOffset()
{
	return 0;
}

// Returns the number of pixels per inch.
float RenderInterface::GetPixelsPerInch()
{
	return 100;
}

// Called by RmlUi when it wants to change the current transform matrix to a new matrix.
void RenderInterface::PushTransform(const Matrix4f& transform)
{
}

// Called by RmlUi when it wants to revert the latest transform change.
void RenderInterface::PopTransform(const Matrix4f& transform)
{
}

// Called when this render interface is released.
void RenderInterface::Release()
{
}

void RenderInterface::OnReferenceDeactivate()
{
	TextureDatabase::ReleaseTextures(this);
	Release();
}

// Get the context currently being rendered.
Context* RenderInterface::GetContext() const
{
	return context;
}

}
}
