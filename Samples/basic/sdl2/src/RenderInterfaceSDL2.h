/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 Nuno Silva
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
#ifndef RENDERINTERFACESDL2_H
#define RENDERINTERFACESDL2_H

#include <RmlUi/Core/RenderInterface.h>

#include <SDL.h>
#include <GL/glew.h>

#if !(SDL_VIDEO_RENDER_OGL)
    #error "Only the opengl sdl backend is supported."
#endif


class RmlUiSDL2Renderer : public Rml::Core::RenderInterface
{
public:
	RmlUiSDL2Renderer(SDL_Renderer* renderer, SDL_Window* screen);

	/// Called by RmlUi when it wants to render geometry that it does not wish to optimise.
	virtual void RenderGeometry(Rml::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::Core::TextureHandle texture, const Rml::Core::Vector2f& translation);

	/// Called by RmlUi when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(bool enable);
	/// Called by RmlUi when it wants to change the scissor region.
	virtual void SetScissorRegion(int x, int y, int width, int height);

	/// Called by RmlUi when a texture is required by the library.
	virtual bool LoadTexture(Rml::Core::TextureHandle& texture_handle, Rml::Core::Vector2i& texture_dimensions, const Rml::Core::String& source);
	/// Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
	virtual bool GenerateTexture(Rml::Core::TextureHandle& texture_handle, const Rml::Core::byte* source, const Rml::Core::Vector2i& source_dimensions);
	/// Called by RmlUi when a loaded texture is no longer required.
	virtual void ReleaseTexture(Rml::Core::TextureHandle texture_handle);

private:
    SDL_Renderer* mRenderer;
    SDL_Window* mScreen;
};

#endif
