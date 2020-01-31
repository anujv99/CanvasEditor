
/*
 *  Opens a font file and make font object which can be used to draw text
 *  Uses freetype and freetype-gl to convert vector fonts to texture
 */

#ifndef __FONT_H__
#define __FONT_H__

#include <vector>

#include <math/math.h>

#include <utils/handledobject.h>
#include <utils/stronghandle.h>

#include <graphics/texture.h>

namespace app { namespace renderer {

    struct Label {
        enum class Alignment {
            CENTER,
            LEFT,
            RIGHT
        };

        std::string Text    = "";
        Vec2 Scale          = Vec2(1.0f);
        Vec2 Pos            = Vec2(0.0f);
        Vec4 Color          = Vec4(1.0f);
        Alignment Align     = Alignment::CENTER;
    };

    struct Character {
        unsigned char CharCode;
        Vec2 TexCoordTopLeft, TexCoordBottomRight;
        Vec2 Size;
        Vec2 Offset;
        float XAdvance;
    };

    class Font : public utils::HandledObject<Font> {
    public:
        Font(const char * filePath, float points);
        virtual ~Font();
    public:
        float GetWidth(const std::string & text) const;
        float GetHeight(const std::string & text) const;
        inline Vec2 GetSize(const std::string & text) const { return Vec2(GetWidth(text), GetHeight(text)); }

        float GetWidthPixels(const std::string & text) const;
        float GetHeightPixels(const std::string & text) const;
        inline Vec2 GetSizePixels(const std::string & text) const { return Vec2(GetWidthPixels(text), GetHeightPixels(text)); }

        inline utils::StrongHandle<graphics::Texture> GetTexture() { return m_Texture; }
        inline const Character & GetCharacter(unsigned char charCode) const { return m_Characters[(size_t)charCode]; }
    private:
        utils::StrongHandle<graphics::Texture> m_Texture;
        std::vector<Character> m_Characters;
        float m_Points;
    };

} }


#endif //__FONT_H__
