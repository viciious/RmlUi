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

#include <ShellRenderInterfaceExtensions.h>
#include <ShellRenderInterfaceOpenGL.h>
#include <RmlUi/Core.h>

#define GL_CLAMP_TO_EDGE 0x812F

ShellRenderInterfaceOpenGL::ShellRenderInterfaceOpenGL() : m_width(0), m_height(0), m_transforms(0), m_rmlui_context(NULL)
{

}

// Called by RmlUi when it wants to render geometry that it does not wish to optimise.
void ShellRenderInterfaceOpenGL::RenderGeometry(Rml::Core::Vertex* vertices, int RMLUI_UNUSED_PARAMETER(num_vertices), int* indices, int num_indices, const Rml::Core::TextureHandle texture, const Rml::Core::Vector2f& translation)
{
	RMLUI_UNUSED(num_vertices);
	
	glPushMatrix();
	glTranslatef(translation.x, translation.y, 0);

	glVertexPointer(2, GL_FLOAT, sizeof(Rml::Core::Vertex), &vertices[0].position);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Rml::Core::Vertex), &vertices[0].colour);

	if (!texture)
	{
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, (GLuint) texture);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Rml::Core::Vertex), &vertices[0].tex_coord);
	}

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

	glPopMatrix();
}

// Called by RmlUi when it wants to compile geometry it believes will be static for the forseeable future.		
Rml::Core::CompiledGeometryHandle ShellRenderInterfaceOpenGL::CompileGeometry(Rml::Core::Vertex* RMLUI_UNUSED_PARAMETER(vertices), int RMLUI_UNUSED_PARAMETER(num_vertices), int* RMLUI_UNUSED_PARAMETER(indices), int RMLUI_UNUSED_PARAMETER(num_indices), const Rml::Core::TextureHandle RMLUI_UNUSED_PARAMETER(texture))
{
	RMLUI_UNUSED(vertices);
	RMLUI_UNUSED(num_vertices);
	RMLUI_UNUSED(indices);
	RMLUI_UNUSED(num_indices);
	RMLUI_UNUSED(texture);

	return (Rml::Core::CompiledGeometryHandle) NULL;
}

// Called by RmlUi when it wants to render application-compiled geometry.		
void ShellRenderInterfaceOpenGL::RenderCompiledGeometry(Rml::Core::CompiledGeometryHandle RMLUI_UNUSED_PARAMETER(geometry), const Rml::Core::Vector2f& RMLUI_UNUSED_PARAMETER(translation))
{
	RMLUI_UNUSED(geometry);
	RMLUI_UNUSED(translation);
}

// Called by RmlUi when it wants to release application-compiled geometry.		
void ShellRenderInterfaceOpenGL::ReleaseCompiledGeometry(Rml::Core::CompiledGeometryHandle RMLUI_UNUSED_PARAMETER(geometry))
{
	RMLUI_UNUSED(geometry);
}

// Called by RmlUi when it wants to enable or disable scissoring to clip content.		
void ShellRenderInterfaceOpenGL::EnableScissorRegion(bool enable)
{
	if (enable) {
		if (m_transforms <= 0) {
			glEnable(GL_SCISSOR_TEST);
			glDisable(GL_STENCIL_TEST);
		} else {
			glDisable(GL_SCISSOR_TEST);
			glEnable(GL_STENCIL_TEST);
		}
	} else {
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_STENCIL_TEST);
	}
}

// Called by RmlUi when it wants to change the scissor region.		
void ShellRenderInterfaceOpenGL::SetScissorRegion(int x, int y, int width, int height)
{
	if (m_transforms <= 0) {
		glScissor(x, m_height - (y + height), width, height);
	} else {
		// clear the stencil buffer
		glStencilMask(-1);
		glClear(GL_STENCIL_BUFFER_BIT);

		// fill the stencil buffer
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_NEVER, 1, -1);
		glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

		float fx = (float)x;
		float fy = (float)y;
		float fwidth = (float)width;
		float fheight = (float)height;

		// draw transformed quad
		GLfloat vertices[] = {
			fx, fy, 0,
			fx, fy + fheight, 0,
			fx + fwidth, fy + fheight, 0,
			fx + fwidth, fy, 0
		};
		glDisableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		GLushort indices[] = { 1, 2, 0, 3 };
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, indices);
		glEnableClientState(GL_COLOR_ARRAY);
	
		// prepare for drawing the real thing
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilMask(0);
		glStencilFunc(GL_EQUAL, 1, -1);
	}
}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1) 
struct TGAHeader 
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()

// Called by RmlUi when a texture is required by the library.		
bool ShellRenderInterfaceOpenGL::LoadTexture(Rml::Core::TextureHandle& texture_handle, Rml::Core::Vector2i& texture_dimensions, const Rml::Core::String& source)
{
	Rml::Core::FileInterface* file_interface = Rml::Core::GetFileInterface();
	Rml::Core::FileHandle file_handle = file_interface->Open(source);
	if (!file_handle)
	{
		return false;
	}
	
	file_interface->Seek(file_handle, 0, SEEK_END);
	size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);
	
	RMLUI_ASSERTMSG(buffer_size > sizeof(TGAHeader), "Texture file size is smaller than TGAHeader, file must be corrupt or otherwise invalid");
	if(buffer_size <= sizeof(TGAHeader))
	{
		file_interface->Close(file_handle);
		return false;
	}

	char* buffer = new char[buffer_size];
	file_interface->Read(buffer, buffer_size, file_handle);
	file_interface->Close(file_handle);

	TGAHeader header;
	memcpy(&header, buffer, sizeof(TGAHeader));
	
	int color_mode = header.bitsPerPixel / 8;
	int image_size = header.width * header.height * 4; // We always make 32bit textures 
	
	if (header.dataType != 2)
	{
		Rml::Core::Log::Message(Rml::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}
	
	// Ensure we have at least 3 colors
	if (color_mode < 3)
	{
		Rml::Core::Log::Message(Rml::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
		return false;
	}
	
	const char* image_src = buffer + sizeof(TGAHeader);
	unsigned char* image_dest = new unsigned char[image_size];
	
	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; y++)
	{
		long read_index = y * header.width * color_mode;
		long write_index = ((header.imageDescriptor & 32) != 0) ? read_index : (header.height - y - 1) * header.width * color_mode;
		for (long x = 0; x < header.width; x++)
		{
			image_dest[write_index] = image_src[read_index+2];
			image_dest[write_index+1] = image_src[read_index+1];
			image_dest[write_index+2] = image_src[read_index];
			if (color_mode == 4)
				image_dest[write_index+3] = image_src[read_index+3];
			else
				image_dest[write_index+3] = 255;
			
			write_index += 4;
			read_index += color_mode;
		}
	}

	texture_dimensions.x = header.width;
	texture_dimensions.y = header.height;
	
	bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);
	
	delete [] image_dest;
	delete [] buffer;
	
	return success;
}

// Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
bool ShellRenderInterfaceOpenGL::GenerateTexture(Rml::Core::TextureHandle& texture_handle, const Rml::Core::byte* source, const Rml::Core::Vector2i& source_dimensions)
{
	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	if (texture_id == 0)
	{
		printf("Failed to generate textures\n");
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	texture_handle = (Rml::Core::TextureHandle) texture_id;

	return true;
}

// Called by RmlUi when a loaded texture is no longer required.		
void ShellRenderInterfaceOpenGL::ReleaseTexture(Rml::Core::TextureHandle texture_handle)
{
	glDeleteTextures(1, (GLuint*) &texture_handle);
}

// Called by RmlUi when it wants to set the current transform matrix to a new matrix.
void ShellRenderInterfaceOpenGL::PushTransform(const Rml::Core::RowMajorMatrix4f& transform)
{
	glPushMatrix();
	glLoadMatrixf(transform.Transpose().data());
	++m_transforms;
}
void ShellRenderInterfaceOpenGL::PushTransform(const Rml::Core::ColumnMajorMatrix4f& transform)
{
	glPushMatrix();
	glLoadMatrixf(transform.data());
	++m_transforms;
}

// Called by RmlUi when it wants to revert the latest transform change.
void ShellRenderInterfaceOpenGL::PopTransform(const Rml::Core::Matrix4f& transform)
{
	glPopMatrix();
	--m_transforms;
}
