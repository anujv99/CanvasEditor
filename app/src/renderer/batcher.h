
#ifndef __BATCHER_H__
#define __BATCHER_H__

#include <unordered_map>
#include <vector>

#include <math/math.h>
#include <graphics/renderstate.h>
#include <utils/handledobject.h>

namespace app { namespace renderer {

	struct VertexColor {
		unsigned char r, g, b, a;
		VertexColor() : r(0), g(0), b(0), a(0) {}
		VertexColor(unsigned char val) : r(val), g(val), b(val), a(val) {}
		VertexColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) :
			r(red), g(green), b(blue), a(alpha) {
		}
		VertexColor(float red, float green, float blue, float alpha) : r(0), g(0), b(0), a(0) {
			r = (unsigned char)(red	* 255.0f);
			g = (unsigned char)(green	* 255.0f);
			b = (unsigned char)(blue	* 255.0f);
			a = (unsigned char)(alpha	* 255.0f);
		}
		VertexColor(float val) : r(0), g(0), b(0), a(0) {
			r = (unsigned char)(val * 255.0f);
			g = (unsigned char)(val * 255.0f);
			b = (unsigned char)(val * 255.0f);
			a = (unsigned char)(val * 255.0f);
		}
	};

	struct Vertex {
		Vertex() : Position(0.0f), Color() {}
		Vertex(const Vec2 & pos, VertexColor col) : Position(pos), Color(col) {}

		inline void Reset(const Vec2 & pos, const VertexColor & color) { Position = pos; Color = color; }

		Vec2 Position;
		VertexColor Color;
	};

	class Batcher {
	protected:
		Batcher(size_t maxVertices);
		virtual ~Batcher();
	public:
		inline void Color(VertexColor color) { m_CurrentColor = color; }
		inline void Color(unsigned char color) { m_CurrentColor = VertexColor(color); }
		inline void Color(pvfloat color) { m_CurrentColor = VertexColor(color); }
		inline void Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { m_CurrentColor = VertexColor(r, g, b, a); }
		inline void Color(pvfloat r, pvfloat g, pvfloat b, pvfloat a) { m_CurrentColor = VertexColor(r, g, b, a); }
		inline void Color(Vec4 col) { m_CurrentColor = VertexColor(col.x, col.y, col.z, col.w); }
		inline void Color(Vec3 col, float alpha) { m_CurrentColor = VertexColor(col.x, col.y, col.z, alpha); }

		virtual void PolygonBegin(graphics::Topology topology) = 0;
		virtual void PolygonEnd() = 0;
		virtual void Vertex(const Vec2 & pos) = 0;
	public:
		void DrawLine(const Vec2 & start, const Vec2 & end);
		void DrawLineThick(const Vec2 & start, const Vec2 & end, pvfloat thickness);
		void DrawLineThickWire(const Vec2 & start, const Vec2 & end, pvfloat thickness);
		void DrawLineDashed(const Vec2 & start, const Vec2 & end, pvfloat stepSize);
		void DrawTriangle(const Vec2 & a, const Vec2 & b, const Vec2 & c);
		void DrawTriangleWire(const Vec2 & a, const Vec2 & b, const Vec2 & c);
		void DrawQuad(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d);
		void DrawQuadWire(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d);
		void DrawRect(const Vec2 & pos, const Vec2 & dimen);
		void DrawRectWire(const Vec2 & pos, const Vec2 & dimen);
		void DrawRectRounded(const Vec2 & pos, const Vec2 & dimen, pvfloat cornerRadius);
		void DrawRectRoundedWire(const Vec2 pos, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedTop(const Vec2 pos, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedTopWire(const Vec2 pos, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedBottom(const Vec2 pos, const Vec2 dimen, float cornerRadius);
		void DrawRectRoundedBottomWire(const Vec2 pos, const Vec2 dimen, float cornerRadius);

		void DrawCircle(const Vec2 center, const float radius, unsigned int segment = 64);
		void DrawCircleWire(const Vec2 center, const float radius, unsigned int segment = 64);

		//To Be Continued...
	private:
		std::vector<Vec2> DrawRoundRectHelper(float startAngle, float radius, Vec2 startPos);
	protected:
		VertexColor m_CurrentColor;
	};

} }

#endif //__BATCHER_H__
