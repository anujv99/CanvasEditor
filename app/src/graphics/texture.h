
/*
 *  Interface to graphics API specific texture
 *  Actual implementation is in the folder external/opengl
 *  Texture object is an array of colors which can be used in the shader
 */

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <utils/stronghandle.h>
#include <utils/handledobject.h>

#include <math/math.h>

namespace app { namespace graphics {

	enum class TextureFiltering {
		NEAREST,
		LINEAR,
	};

	enum class TextureWrapping {
		REPEAT,
		CLAMP
	};

	enum class TextureFormat {
		R,
		RG,
		RGB,
		RGBA
	};

	struct TextureParams {
		TextureFiltering Filtering		= TextureFiltering::LINEAR;
		TextureWrapping Wrapping		= TextureWrapping::CLAMP;
		TextureFormat InternalFormat	= TextureFormat::RGBA;
	};

    class Texture : public utils::HandledObject<Texture> {
        friend class utils::StrongHandle<Texture>;
    protected:
		Texture() {}
		virtual ~Texture() {}
	public:
		virtual void Bind(unsigned int slot) const = 0;
		virtual void UnBind() const = 0;
		virtual void SetData(const unsigned char * pixels, size_t size) = 0;
		virtual Vec2i GetSize() const = 0;
	public:
		static utils::StrongHandle<Texture> Create(Vec2i size, TextureParams params);
    };

} }

#endif //__TEXTURE_H__
