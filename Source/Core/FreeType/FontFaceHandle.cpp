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
#include "FontFaceHandle.h"
#include <algorithm>
#include "../../../Include/RmlUi/Core.h"
#include "../FontFaceLayer.h"
#include "../TextureLayout.h"

namespace Rml {
namespace Core {
namespace FreeType {

class FontEffectSort
{
public:
	bool operator()(const FontEffect* lhs, const FontEffect* rhs)
	{
		return lhs->GetZIndex() < rhs->GetZIndex();
	}
};

FontFaceHandle::FontFaceHandle()
{
	ft_face = NULL;
}

FontFaceHandle::~FontFaceHandle()
{
}

// Initialises the handle so it is able to render text.
bool FontFaceHandle::Initialise(FT_Face ft_face, const String& _charset, int _size)
{
	size = _size;

	raw_charset = _charset;
	if (!UnicodeRange::BuildList(charset, raw_charset))
	{
		Log::Message(Log::LT_ERROR, "Invalid font charset '%s'.", raw_charset.CString());
		return false;
	}

	// Set the character size on the font face.
	FT_Error error = FT_Set_Char_Size(ft_face, 0, size << 6, 0, 0);
	if (error != 0)
	{
		Log::Message(Log::LT_ERROR, "Unable to set the character size '%d' on the font face '%s %s'.", size, ft_face->family_name, ft_face->style_name);
		return false;
	}

	this->ft_face = ft_face;

	// find the maximum character we are interested in
	max_codepoint = 0;
	for (size_t i = 0; i < charset.size(); ++i)
		max_codepoint = Math::Max(max_codepoint, charset[i].max_codepoint);

	// Construct the list of the characters specified by the charset.
	glyphs.resize(max_codepoint+1, FontGlyph());
	for (size_t i = 0; i < charset.size(); ++i)
		BuildGlyphMap(charset[i]);

	// Generate the metrics for the handle.
	GenerateMetrics();

	// Generate the default layer and layer configuration.
	base_layer = GenerateLayer(NULL);
	layer_configurations.push_back(LayerConfiguration());
	layer_configurations.back().push_back(base_layer);


	return true;
}

// Returns the width a string will take up if rendered with this handle.
int FontFaceHandle::GetStringWidth(const WString& string, word prior_character) const
{
	int width = 0;

	for (size_t i = 0; i < string.Length(); i++)
	{
		word character_code = string[i];

		if (character_code >= glyphs.size())
			continue;
		const FontGlyph &glyph = glyphs[character_code];

		// Adjust the cursor for the kerning between this character and the previous one.
		if (prior_character != 0)
			width += GetKerning(prior_character, string[i]);
		// Adjust the cursor for this character's advance.
		width += glyph.advance;

		prior_character = character_code;
	}

	return width;
}

// Generates, if required, the layer configuration for a given array of font effects.
int FontFaceHandle::GenerateLayerConfiguration(FontEffectMap& font_effects)
{
	if (font_effects.empty())
		return 0;

	// Prepare a list of effects, sorted by z-index.
	FontEffectList sorted_effects;
	for (FontEffectMap::const_iterator i = font_effects.begin(); i != font_effects.end(); ++i)
		sorted_effects.push_back(i->second);

	std::sort(sorted_effects.begin(), sorted_effects.end(), FontEffectSort());

	// Check each existing configuration for a match with this arrangement of effects.
	int configuration_index = 1;
	for (; configuration_index < (int) layer_configurations.size(); ++configuration_index)
	{
		const LayerConfiguration& configuration = layer_configurations[configuration_index];

		// Check the size is correct. For a math, there should be one layer in the configuration
		// plus an extra for the base layer.
		if (configuration.size() != sorted_effects.size() + 1)
			continue;

		// Check through each layer, checking it was created by the same effect as the one we're
		// checking.
		size_t effect_index = 0;
		for (size_t i = 0; i < configuration.size(); ++i)
		{
			// Skip the base layer ...
			if (configuration[i]->GetFontEffect() == NULL)
				continue;

			// If the ith layer's effect doesn't match the equivalent effect, then this
			// configuration can't match.
			if (configuration[i]->GetFontEffect() != sorted_effects[effect_index])
				break;

			// Check the next one ...
			++effect_index;
		}

		if (effect_index == sorted_effects.size())
			return configuration_index;
	}

	// No match, so we have to generate a new layer configuration.
	layer_configurations.push_back(LayerConfiguration());
	LayerConfiguration& layer_configuration = layer_configurations.back();

	bool added_base_layer = false;

	for (size_t i = 0; i < sorted_effects.size(); ++i)
	{
		if (!added_base_layer &&
			sorted_effects[i]->GetZIndex() >= 0)
		{
			layer_configuration.push_back(base_layer);
			added_base_layer = true;
		}

		layer_configuration.push_back(GenerateLayer(sorted_effects[i]));
	}

	// Add the base layer now if we still haven't added it.
	if (!added_base_layer)
		layer_configuration.push_back(base_layer);

	return (int) (layer_configurations.size() - 1);
}

// Generates the texture data for a layer (for the texture database).
bool FontFaceHandle::GenerateLayerTexture(const byte*& texture_data, Vector2i& texture_dimensions, FontEffect* layer_id, int texture_id)
{
	FontLayerMap::iterator layer_iterator = layers.find(layer_id);
	if (layer_iterator == layers.end())
		return false;

	return layer_iterator->second->GenerateTexture(texture_data, texture_dimensions, texture_id);
}

// Generates the geometry required to render a single line of text.
int FontFaceHandle::GenerateString(GeometryList& geometry, const WString& string, const Vector2f& position, const Colourb& colour, int layer_configuration_index) const
{
	int geometry_index = 0;
	int line_width = 0;

	RMLUI_ASSERT(layer_configuration_index >= 0);
	RMLUI_ASSERT(layer_configuration_index < (int) layer_configurations.size());

	// Fetch the requested configuration and generate the geometry for each one.
	const LayerConfiguration& layer_configuration = layer_configurations[layer_configuration_index];
	for (size_t i = 0; i < layer_configuration.size(); ++i)
	{
		FontFaceLayer* layer = layer_configuration[i];

		Colourb layer_colour;
		if (layer == base_layer)
			layer_colour = colour;
		else
			layer_colour = layer->GetColour();

		// Resize the geometry list if required.
		if ((int) geometry.size() < geometry_index + layer->GetNumTextures())
			geometry.resize(geometry_index + layer->GetNumTextures());

		// Bind the textures to the geometries.
		for (int i = 0; i < layer->GetNumTextures(); ++i)
			geometry[geometry_index + i].SetTexture(layer->GetTexture(i));

		line_width = 0;
		word prior_character = 0;

		const word* string_iterator = string.CString();
		const word* string_end = string.CString() + string.Length();

		for (; string_iterator != string_end; string_iterator++)
		{
			if (*string_iterator >= glyphs.size())
				continue;
			const FontGlyph &glyph = glyphs[*string_iterator];

			// Adjust the cursor for the kerning between this character and the previous one.
			if (prior_character != 0)
				line_width += GetKerning(prior_character, *string_iterator);

			layer->GenerateGeometry(&geometry[geometry_index], *string_iterator, Vector2f(position.x + line_width, position.y), layer_colour);

			line_width += glyph.advance;
			prior_character = *string_iterator;
		}

		geometry_index += layer->GetNumTextures();
	}

	// Cull any excess geometry from a previous generation.
	geometry.resize(geometry_index);

	return line_width;
}

// Generates the geometry required to render a line above, below or through a line of text.
void FontFaceHandle::GenerateLine(Geometry* geometry, const Vector2f& position, int width, Font::Line height, const Colourb& colour) const
{
	std::vector< Vertex >& line_vertices = geometry->GetVertices();
	std::vector< int >& line_indices = geometry->GetIndices();

	float offset;
	switch (height)
	{
		case Font::UNDERLINE:			offset = -underline_position; break;
		case Font::OVERLINE:			// where to place? offset = -line_height - underline_position; break;
		case Font::STRIKE_THROUGH:		// where to place? offset = -line_height * 0.5f; break;
		default:						return;
	}

	line_vertices.resize(line_vertices.size() + 4);
	line_indices.resize(line_indices.size() + 6);
	GeometryUtilities::GenerateQuad(&line_vertices[0] + (line_vertices.size() - 4), &line_indices[0] + (line_indices.size() - 6), Vector2f(position.x, position.y + offset).Round(), Vector2f((float) width, underline_thickness), colour, (int)line_vertices.size() - 4);
}

// Destroys the handle.
void FontFaceHandle::OnReferenceDeactivate()
{
	delete this;
}

void FontFaceHandle::GenerateMetrics()
{
	line_height = ft_face->size->metrics.height >> 6;
	baseline = line_height - (ft_face->size->metrics.ascender >> 6);

	underline_position = FT_MulFix(ft_face->underline_position, ft_face->size->metrics.y_scale) / float(1 << 6);
	underline_thickness = FT_MulFix(ft_face->underline_thickness, ft_face->size->metrics.y_scale) / float(1 << 6);
	underline_thickness = Math::Max(underline_thickness, 1.0f);

	average_advance = 0;
	unsigned int num_visible_glyphs = 0;
	for (FontGlyphList::iterator i = glyphs.begin(); i != glyphs.end(); ++i)
	{
		if (i->advance)
		{
			average_advance += i->advance;
			num_visible_glyphs++;
		}
	}

	// Bring the total advance down to the average advance, but scaled up 10%, just to be on the safe side.
	if (num_visible_glyphs)
		average_advance = Math::RealToInteger((float) average_advance / (num_visible_glyphs * 0.9f));

	// Determine the x-height of this font face.
	word x = (word) 'x';
	int index = FT_Get_Char_Index(ft_face, x);
	if (FT_Load_Glyph(ft_face, index, 0) == 0)
		x_height = ft_face->glyph->metrics.height >> 6;
	else
		x_height = 0;
}

void FontFaceHandle::BuildGlyphMap(const UnicodeRange& unicode_range)
{
	for (word character_code = (word) (Math::Max< unsigned int >(unicode_range.min_codepoint, 32)); character_code <= unicode_range.max_codepoint; ++character_code)
	{
		int index = FT_Get_Char_Index(ft_face, character_code);
		if (index != 0)
		{
			FT_Error error = FT_Load_Glyph(ft_face, index, 0);
			if (error != 0)
			{
				Log::Message(Log::LT_WARNING, "Unable to load glyph for character '%u' on the font face '%s %s'; error code: %d.", character_code, ft_face->family_name, ft_face->style_name, error);
				continue;
			}

			error = FT_Render_Glyph(ft_face->glyph, FT_RENDER_MODE_NORMAL);
			if (error != 0)
			{
				Log::Message(Log::LT_WARNING, "Unable to render glyph for character '%u' on the font face '%s %s'; error code: %d.", character_code, ft_face->family_name, ft_face->style_name, error);
				continue;
			}

			FontGlyph glyph;
			glyph.character = character_code;
			BuildGlyph(glyph, ft_face->glyph);
			glyphs[character_code] = glyph;
		}
	}
}

void FontFaceHandle::BuildGlyph(FontGlyph& glyph, FT_GlyphSlot ft_glyph)
{
	// Set the glyph's dimensions.
	glyph.dimensions.x = ft_glyph->metrics.width >> 6;
	glyph.dimensions.y = ft_glyph->metrics.height >> 6;

	// Set the glyph's bearing.
	glyph.bearing.x = ft_glyph->metrics.horiBearingX >> 6;
	glyph.bearing.y = ft_glyph->metrics.horiBearingY >> 6;

	// Set the glyph's advance.
	glyph.advance = ft_glyph->metrics.horiAdvance >> 6;

	// Set the glyph's bitmap dimensions.
	glyph.bitmap_dimensions.x = ft_glyph->bitmap.width;
	glyph.bitmap_dimensions.y = ft_glyph->bitmap.rows;

	// Copy the glyph's bitmap data from the FreeType glyph handle to our glyph handle.
	if (glyph.bitmap_dimensions.x * glyph.bitmap_dimensions.y != 0)
	{
		// Check the pixel mode is supported.
		if (ft_glyph->bitmap.pixel_mode != FT_PIXEL_MODE_MONO &&
			ft_glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
		{
			glyph.bitmap_data = NULL;
			Log::Message(Log::LT_WARNING, "Unable to render glyph on the font face '%s %s'; unsupported pixel mode (%d).", ft_glyph->face->family_name, ft_glyph->face->style_name, ft_glyph->bitmap.pixel_mode);
		}
		else
		{
			glyph.bitmap_data = new byte[glyph.bitmap_dimensions.x * glyph.bitmap_dimensions.y];

			byte* source_bitmap = ft_glyph->bitmap.buffer;
			byte* destination_bitmap = glyph.bitmap_data;

			// Copy the bitmap data into the newly-allocated space on our glyph.
			switch (ft_glyph->bitmap.pixel_mode)
			{
				// Unpack 1-bit data into 8-bit.
				case FT_PIXEL_MODE_MONO:
				{
					for (int i = 0; i < glyph.bitmap_dimensions.y; ++i)
					{
						int mask = 0x80;
						byte* source_byte = source_bitmap;
						for (int j = 0; j < glyph.bitmap_dimensions.x; ++j)
						{
							if ((*source_byte & mask) == mask)
								destination_bitmap[j] = 255;
							else
								destination_bitmap[j] = 0;

							mask >>= 1;
							if (mask <= 0)
							{
								mask = 0x80;
								++source_byte;
							}
						}

						destination_bitmap += glyph.bitmap_dimensions.x;
						source_bitmap += ft_glyph->bitmap.pitch;
					}
				}
				break;

				// Directly copy 8-bit data.
				case FT_PIXEL_MODE_GRAY:
				{
					for (int i = 0; i < glyph.bitmap_dimensions.y; ++i)
					{
						memcpy(destination_bitmap, source_bitmap, glyph.bitmap_dimensions.x);
						destination_bitmap += glyph.bitmap_dimensions.x;
						source_bitmap += ft_glyph->bitmap.pitch;
					}
				}
				break;
			}
		}
	}
	else
		glyph.bitmap_data = NULL;
}

int FontFaceHandle::GetKerning(word lhs, word rhs) const
{
	if (!FT_HAS_KERNING(ft_face))
		return 0;

	FT_Vector ft_kerning;

	FT_Error ft_error = FT_Get_Kerning(ft_face,
		FT_Get_Char_Index(ft_face, lhs), FT_Get_Char_Index(ft_face, rhs),
		FT_KERNING_DEFAULT, &ft_kerning);

	if (ft_error != 0)
		return 0;

	int kerning = ft_kerning.x >> 6;
	return kerning;
}

}
}
}
