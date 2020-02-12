#include "batcher.h"

namespace app { namespace renderer {

	Batcher::Batcher(size_t maxVertices) : m_CurrentColor(1.0f) {}

	Batcher::~Batcher() {}

	void Batcher::DrawLine(const Vec2 & start, const Vec2 & end) {
		PolygonBegin(graphics::Topology::LINE);
		Vertex(start);
		Vertex(end);
		PolygonEnd();
	}

	void Batcher::DrawLineThick(const Vec2 & start, const Vec2 & end, pvfloat thickness) {
		Vec2 dir = end - start;
		dir = Vec2::Perpendicular(dir);
		dir = Vec2::Normalize(dir) * thickness * 0.5f;

		DrawQuad(start + dir, end + dir, end - dir, start - dir);
	}

	void Batcher::DrawLineThickWire(const Vec2 & start, const Vec2 & end, pvfloat thickness) {
		Vec2 dir = end - start;
		dir = Vec2::Perpendicular(dir);
		dir = Vec2::Normalize(dir) * thickness * 0.5f;

		DrawQuadWire(start + dir, end + dir, end - dir, start - dir);
	}

	void Batcher::DrawLineDashed(const Vec2 & start, const Vec2 & end, pvfloat stepSize) {
		const Vec2 diff = end - start;
		const pvfloat len = Vec2::Length(diff);

		if (len < 0.01f || stepSize < 0.01f) return;

		const Vec2 dir = diff / len;
		const Vec2 dir_step = dir * stepSize;
		const pvint num_steps = static_cast<pvint>(len / stepSize);

		PolygonBegin(graphics::Topology::LINE);

		for (pvint i = 0; i < num_steps; i += 2) {
			pvfloat index = static_cast<pvfloat>(i);

			Vec2 pos0 = start + dir_step * index;
			Vec2 pos1 = start + dir_step * (index + 1.0f);

			Vertex(pos0);
			Vertex(pos1);
		}

		PolygonEnd();
	}

	void Batcher::DrawTriangle(const Vec2 & a, const Vec2 & b, const Vec2 & c) {
		PolygonBegin(graphics::Topology::TRIANGLE);
		Vertex(a);
		Vertex(b);
		Vertex(c);
		PolygonEnd();
	}

	void Batcher::DrawTriangleWire(const Vec2 & a, const Vec2 & b, const Vec2 & c) {
		PolygonBegin(graphics::Topology::LINE);
		Vertex(a);
		Vertex(b);
		Vertex(b);
		Vertex(c);
		Vertex(c);
		Vertex(a);
		PolygonEnd();
	}

	void Batcher::DrawQuad(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d) {
		PolygonBegin(graphics::Topology::TRIANGLE);
		Vertex(a);
		Vertex(b);
		Vertex(c);
		Vertex(a);
		Vertex(c);
		Vertex(d);
		PolygonEnd();
	}

	void Batcher::DrawQuadWire(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d) {
		PolygonBegin(graphics::Topology::LINE);
		Vertex(a);
		Vertex(b);
		Vertex(b);
		Vertex(c);
		Vertex(c);
		Vertex(d);
		Vertex(d);
		Vertex(a);
		PolygonEnd();
	}

	void Batcher::DrawRect(const Vec2 & pos, const Vec2 & dimen) {
		Vec2 halfDimen = dimen / 2.0f;
		DrawQuad(
			pos + Vec2(-halfDimen.x,  halfDimen.y),
			pos + Vec2( halfDimen.x,  halfDimen.y),
			pos + Vec2( halfDimen.x, -halfDimen.y),
			pos + Vec2(-halfDimen.x, -halfDimen.y));
	}

	void Batcher::DrawRectWire(const Vec2 & pos, const Vec2 & dimen) {
		Vec2 halfDimen = dimen / 2.0f;
		DrawQuadWire(
			pos + Vec2(-halfDimen.x, halfDimen.y),
			pos + Vec2(halfDimen.x, halfDimen.y),
			pos + Vec2(halfDimen.x, -halfDimen.y),
			pos + Vec2(-halfDimen.x, -halfDimen.y));
	}

	void Batcher::DrawRectRounded(const Vec2 & pos, const Vec2 & dimen, pvfloat cornerRadius) {
		PolygonBegin(graphics::Topology::TRIANGLE);
		const Vec2 centerPos = pos;
		const Vec2 bottomLeft = pos - dimen / 2.0f;
		Vec2 firstPos;
		pvfloat angle = 180;
		std::vector<Vec2> positions;

		Vec2 startPositions[4] = {
			bottomLeft + Vec2(cornerRadius),
			bottomLeft + Vec2(dimen.x - cornerRadius, cornerRadius),
			bottomLeft + dimen - Vec2(cornerRadius),
			bottomLeft + Vec2(cornerRadius, dimen.y - cornerRadius)
		};

		for (unsigned int j = 0; j < 4u; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(centerPos);
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			Vertex(centerPos);
			Vertex(positions[positions.size() - 1]);

			angle += 90.0f;
		}

		Vertex(firstPos);

		PolygonEnd();
	}

	void Batcher::DrawRectRoundedWire(const Vec2 pos, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(graphics::Topology::LINE);
		const Vec2 centerPos = pos;
		const Vec2 bottomLeft = pos - dimen / 2.0f;
		Vec2 firstPos;
		pvfloat angle = 180.0f;
		std::vector<Vec2> positions;

		Vec2 startPositions[4] = {
			bottomLeft + Vec2(cornerRadius),
			bottomLeft + Vec2(dimen.x - cornerRadius, cornerRadius),
			bottomLeft + dimen - Vec2(cornerRadius),
			bottomLeft + Vec2(cornerRadius, dimen.y - cornerRadius)
		};

		for (unsigned int j = 0; j < 4; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			Vertex(positions[positions.size() - 1]);

			angle += 90.0f;
		}

		Vertex(firstPos);

		PolygonEnd();
	}

	void Batcher::DrawRectRoundedTop(const Vec2 pos, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(graphics::Topology::TRIANGLE);
		const Vec2 centerPos = pos;
		const Vec2 bottomLeft = pos - dimen / 2.0f;
		Vec2 firstPos;
		pvfloat angle = 0;
		std::vector<Vec2> positions;

		Vec2 startPositions[2] = {
			bottomLeft + dimen - Vec2(cornerRadius),
			bottomLeft + Vec2(cornerRadius, dimen.y - cornerRadius)
		};

		for (unsigned int j = 0u; j < 2u; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(centerPos);
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			if (j != 1) {
				Vertex(centerPos);
				Vertex(positions[positions.size() - 1]);
			}

			angle += 90.0f;
		}

		Vertex(positions[positions.size() - 1]);
		Vertex(bottomLeft);
		Vertex(centerPos);

		Vertex(bottomLeft);
		Vertex(centerPos);
		Vertex(bottomLeft + Vec2(dimen.x, 0.0f));

		Vertex(bottomLeft + Vec2(dimen.x, 0.0f));
		Vertex(centerPos);
		Vertex(firstPos);

		PolygonEnd();
	}

	void Batcher::DrawRectRoundedTopWire(const Vec2 pos, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(graphics::Topology::LINE);
		const Vec2 centerPos = pos;
		const Vec2 bottomLeft = pos - dimen / 2.0f;
		Vec2 firstPos;
		float angle = 0;
		std::vector<Vec2> positions;

		Vec2 startPositions[2] = {
			bottomLeft + dimen - Vec2(cornerRadius),
			bottomLeft + Vec2(cornerRadius, dimen.y - cornerRadius)
		};

		for (unsigned int j = 0; j < 2; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			if (j != 1) {
				Vertex(positions[positions.size() - 1]);
			}

			angle += 90.0f;
		}

		Vertex(positions[positions.size() - 1]);
		Vertex(bottomLeft);

		Vertex(bottomLeft);
		Vertex(bottomLeft + Vec2(dimen.x, 0.0f));

		Vertex(bottomLeft + Vec2(dimen.x, 0.0f));
		Vertex(firstPos);

		PolygonEnd();
	}

	void Batcher::DrawRectRoundedBottom(const Vec2 pos, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(graphics::Topology::TRIANGLE);
		const Vec2 centerPos = pos;
		const Vec2 bottomLeft = pos - dimen / 2.0f;
		Vec2 firstPos;
		float angle = 180.0f;
		std::vector<Vec2> positions;

		Vec2 startPositions[2] = {
			bottomLeft + Vec2(cornerRadius),
			bottomLeft + Vec2(dimen.x - cornerRadius, cornerRadius),
		};

		for (unsigned int j = 0; j < 2; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(centerPos);
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			if (j != 1) {
				Vertex(centerPos);
				Vertex(positions[positions.size() - 1]);
			}

			angle += 90.0f;
		}

		Vertex(positions[positions.size() - 1]);
		Vertex(bottomLeft + dimen);
		Vertex(centerPos);

		Vertex(bottomLeft + dimen);
		Vertex(centerPos);
		Vertex(bottomLeft + Vec2(0.0f, dimen.y));

		Vertex(bottomLeft + Vec2(0.0f, dimen.y));
		Vertex(centerPos);
		Vertex(firstPos);

		PolygonEnd();
	}

	void Batcher::DrawRectRoundedBottomWire(const Vec2 pos, const Vec2 dimen, float cornerRadius) {
		PolygonBegin(graphics::Topology::LINE);
		const Vec2 centerPos = pos;
		const Vec2 bottomLeft = pos - dimen / 2.0f;
		Vec2 firstPos;
		float angle = 180.0f;
		std::vector<Vec2> positions;

		Vec2 startPositions[2] = {
			bottomLeft + Vec2(cornerRadius),
			bottomLeft + Vec2(dimen.x - cornerRadius, cornerRadius),
		};

		for (unsigned int j = 0; j < 2; j++) {
			positions = DrawRoundRectHelper(angle, cornerRadius, startPositions[j]);
			if (j == 0)
				firstPos = positions[0];
			else
				Vertex(positions[0]);
			for (size_t i = 0; i < positions.size() - 1; i++) {
				Vertex(positions[i]);
				Vertex(positions[i + 1]);
			}

			if (j != 1) {
				Vertex(positions[positions.size() - 1]);
			}

			angle += 90.0f;
		}

		Vertex(positions[positions.size() - 1]);
		Vertex(bottomLeft + dimen);

		Vertex(bottomLeft + dimen);
		Vertex(bottomLeft + Vec2(0.0f, dimen.y));

		Vertex(bottomLeft + Vec2(0.0f, dimen.y));
		Vertex(firstPos);

		PolygonEnd();
	}

	void Batcher::DrawCircle(const Vec2 center, const float radius, unsigned int segment) {
		const float delta_angle = 360.0f / segment;

		PolygonBegin(graphics::Topology::TRIANGLE);

		for (unsigned int i = 0; i < segment; ++i) {
			float angle_0 = delta_angle * i;
			float angle_1 = delta_angle * (i + 1);
			Vec2 pos_0 = center + Vec2::UnitCircle(angle_0) * radius;
			Vec2 pos_1 = center + Vec2::UnitCircle(angle_1) * radius;

			Vertex(center);
			Vertex(pos_0);
			Vertex(pos_1);
		}

		PolygonEnd();
	}

	void Batcher::DrawCircleWire(const Vec2 center, const float radius, unsigned int segment) {
		const float delta_angle = 360.0f / segment;

		PolygonBegin(graphics::Topology::LINE_STRIP);

		for (unsigned int i = 0; i < segment; ++i) {
			float angle_0 = delta_angle * i;
			float angle_1 = delta_angle * (i + 1);
			Vec2 pos_0 = center + Vec2::UnitCircle(angle_0) * radius;
			Vec2 pos_1 = center + Vec2::UnitCircle(angle_1) * radius;

			Vertex(pos_0);
			Vertex(pos_1);
		}

		PolygonEnd();
	}

	std::vector<Vec2> Batcher::DrawRoundRectHelper(float startAngle, float radius, Vec2 startPos) {
		const unsigned int numRoundedVerts = 8;
		const pvfloat roundedCornerAngle = 90.0f;
		const pvfloat roundedCircledelta_angle = roundedCornerAngle / (numRoundedVerts - 1);

		std::vector<Vec2> positions;

		for (unsigned int i = 0; i < numRoundedVerts; ++i) {
			float angleRad = startAngle + i * roundedCircledelta_angle;
			Vec2 unitCircle = Vec2::UnitCircle(angleRad);
			Vec2 pos = startPos + unitCircle * radius;
			positions.push_back(pos);
		}

		return positions;
	}

} }