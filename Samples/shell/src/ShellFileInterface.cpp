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

#include <ShellFileInterface.h>
#include <stdio.h>

ShellFileInterface::ShellFileInterface(const Rml::Core::String& root) : root(root)
{
}

ShellFileInterface::~ShellFileInterface()
{
}

// Opens a file.
Rml::Core::FileHandle ShellFileInterface::Open(const Rml::Core::String& path)
{
	// Attempt to open the file relative to the application's root.
	FILE* fp = fopen((root + path).CString(), "rb");
	if (fp != NULL)
		return (Rml::Core::FileHandle) fp;

	// Attempt to open the file relative to the current working directory.
	fp = fopen(path.CString(), "rb");
	return (Rml::Core::FileHandle) fp;
}

// Closes a previously opened file.
void ShellFileInterface::Close(Rml::Core::FileHandle file)
{
	fclose((FILE*) file);
}

// Reads data from a previously opened file.
size_t ShellFileInterface::Read(void* buffer, size_t size, Rml::Core::FileHandle file)
{
	return fread(buffer, 1, size, (FILE*) file);
}

// Seeks to a point in a previously opened file.
bool ShellFileInterface::Seek(Rml::Core::FileHandle file, long offset, int origin)
{
	return fseek((FILE*) file, offset, origin) == 0;
}

// Returns the current position of the file pointer.
size_t ShellFileInterface::Tell(Rml::Core::FileHandle file)
{
	return ftell((FILE*) file);
}
