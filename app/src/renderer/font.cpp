#include "font.h"

#include <utils/fileutils.h>

#include <freetype-gl.h>

namespace app { namespace renderer {

	static constexpr const unsigned int TEXTURE_SIZE_X = 1024u;
	static constexpr const unsigned int TEXTURE_SIZE_Y = 1024u;

	float PixelsToPoints(float pixels) {
		return (unsigned int)(pixels * 0.75f);
	}

	float PointsToPixels(float pixels) {
		return (unsigned int)(pixels * 1.333333333f);
	}

	Font::Font(const char * file, float points) : m_Points(points) {
		std::string filePath = utils::FileUtils::ConvertToRelativePath(file);
		ASSERTM(utils::FileUtils::FileExists(filePath.c_str()), "Font file not found");

		auto atlas = ftgl::texture_atlas_new(TEXTURE_SIZE_X, TEXTURE_SIZE_Y, 1);
		auto font = ftgl::texture_font_new_from_file(atlas, (float)points, filePath.c_str());

		pvfloat pixels = PointsToPixels(points);

		for (unsigned char i = 0; i < 128u; i++) {
			char ch = static_cast<char>(i);
			auto glyph = ftgl::texture_font_get_glyph(font, &ch);
			ASSERT(glyph);
			Character character;
			character.CharCode = i;
			character.Offset = Vec2(glyph->offset_x, glyph->offset_y) / pixels;
			character.Size = Vec2(glyph->width, glyph->height) / pixels;
			character.TexCoordTopLeft = Vec2(glyph->s0, glyph->t0);
			character.TexCoordBottomRight = Vec2(glyph->s1, glyph->t1 - 0.000001f);
			character.XAdvance = glyph->advance_x / pixels;

			m_Characters.push_back(character);
		}

		graphics::TextureParams params;
		params.Filtering = graphics::TextureFiltering::NEAREST;
		params.InternalFormat = graphics::TextureFormat::RGBA;

		unsigned char * arr = new unsigned char[TEXTURE_SIZE_X * TEXTURE_SIZE_Y * 4];
		for (size_t i = 0, j = 0; i < TEXTURE_SIZE_X * TEXTURE_SIZE_Y * 4; i += 4, j++) {
			arr[i + 0] = atlas->data[j];
			arr[i + 1] = atlas->data[j];
			arr[i + 2] = atlas->data[j];
			arr[i + 3] = atlas->data[j];
		}

		m_Texture = graphics::Texture::Create(Vec2i(TEXTURE_SIZE_X, TEXTURE_SIZE_Y), params);
		m_Texture->SetData(arr, TEXTURE_SIZE_X * TEXTURE_SIZE_Y * 4);

		delete[] arr;

		ftgl::texture_font_delete(font);
		ftgl::texture_atlas_delete(atlas);
	}

	Font::~Font() {}

	float Font::GetWidth(const std::string & text) const {
		float width = 0.0f;
		for (int i = 0; i < text.size(); i++) {
			if (i == text.size() - 1) {
				width += m_Characters[(size_t)text[i]].Size.x;
			} else {
				width += m_Characters[(size_t)text[i]].XAdvance;
			}
		}
		return width;
	}

	float Font::GetHeight(const std::string & text) const {
		float min = 0.0f;
		float max = 0.0f;
		for (int i = 0; i < text.size(); i++) {
			const Character & c = m_Characters[(size_t)text[i]];
			float height = c.Size.y;
			float offset = c.Offset.y - height;
			if (offset < min)
				min = offset;
			if (height > max)
				max = height;
		}
		return Abs(min) + Abs(max);
	}

	float Font::GetWidthPixels(const std::string & text) const {
		float pixels = PointsToPixels(m_Points);

		float width = 0;
		for (int i = 0; i < text.size(); i++) {
			if (i == text.size() - 1) {
				width += m_Characters[(size_t)text[i]].Size.x * pixels;
			} else {
				width += m_Characters[(size_t)text[i]].XAdvance * pixels;
			}
		}
		return width;
	}

	float Font::GetHeightPixels(const std::string & text) const {
		float pixels = PointsToPixels(m_Points);

		float min = 0.0f;
		float max = 0.0f;
		for (int i = 0; i < text.size(); i++) {
			const Character & c = m_Characters[(size_t)text[i]];
			float height = c.Size.y * pixels;
			float offset = c.Offset.y * pixels - height;
			if (offset < min)
				min = offset;
			if (height > max)
				max = height;
		}
		return Abs(min) + Abs(max);
	}

} }