//
// Created by ianpo on 31/01/2025.
//

#pragma once

#include "Basics.hpp"

namespace TRG::Math {
	class MeshGraph {
	public:
		using T = Real;
		static inline constexpr glm::size_t L = 2;

		using Vector2 = glm::vec<L,T>;

		struct Vertex {
			Vector2 Position;
			// std::vector<uint32_t> AssociatedEdges{};
		};

		struct Edge {
			uint32_t VertexA;
			uint32_t VertexB;
			std::optional<uint32_t> TriangleLeft{std::nullopt};
			std::optional<uint32_t> TriangleRight{std::nullopt};
		};

		struct Triangle {
			uint32_t EdgeA;
			uint32_t EdgeB;
			uint32_t EdgeC;
		};
	public:
		void AddPoint(Vector2 point);
	public:
		std::map<uint32_t, Vertex> m_Vertices;
		std::map<uint32_t, Edge> m_Edges;
		std::map<uint32_t, Triangle> m_Triangles;
	private:
		[[nodiscard]] uint32_t GenerateVertexId() { return m_VertexIdGenerator++; };
		[[nodiscard]] uint32_t GenerateEdgeId() { return m_EdgeIdGenerator++; };
		[[nodiscard]] uint32_t GenerateTriangleId() { return m_TriangleIdGenerator++; };
	private:
		uint32_t m_VertexIdGenerator{0};
		uint32_t m_EdgeIdGenerator{0};
		uint32_t m_TriangleIdGenerator{0};
	};

	inline void MeshGraph::AddPoint(const Vector2 point) {
		const uint32_t newVertId = GenerateVertexId();
		m_Vertices[newVertId] = {point};

		if (m_Vertices.size() > 2) {
			std::unordered_set<uint32_t> compatibleVertices;
			compatibleVertices.reserve(m_Edges.size() - 1);

			std::vector<uint32_t> compatibleEdges{};
			compatibleEdges.reserve(m_Edges.size());

			for (auto&[ABId, edgeAB] : m_Edges) {
				if (edgeAB.TriangleLeft && edgeAB.TriangleRight) continue;

				const auto& vertA = m_Vertices.at(edgeAB.VertexA);
				const auto& vertB = m_Vertices.at(edgeAB.VertexB);

				const auto aToB = vertB.Position - vertA.Position;
				const auto aToC = point - vertA.Position;

				const bool isAligned = std::abs(Math::Dot(Math::Normalize(aToB), Math::Normalize(aToC))) >= 1-REAL_EPSILON;
				if (isAligned) continue;

				const bool isLeft = Math::IsTriangleOriented(aToB, aToC);
				if (isLeft && edgeAB.TriangleLeft) continue;
				if (!isLeft && edgeAB.TriangleRight) continue;

				compatibleEdges.push_back(ABId);
				compatibleVertices.insert(edgeAB.VertexA);
				compatibleVertices.insert(edgeAB.VertexB);
			}

			std::unordered_map<uint32_t, uint32_t> verticeToPromotedVertices;
			verticeToPromotedVertices.reserve(compatibleVertices.size());

			for (const auto vertId : compatibleVertices) {
				const auto newEdgeId = GenerateEdgeId();
				verticeToPromotedVertices[vertId] = newEdgeId;
				m_Edges[newEdgeId] = {vertId, newVertId};
			}

			for (const auto ABId: compatibleEdges) {
				auto& AB = m_Edges[ABId];
				const auto BCId = verticeToPromotedVertices[AB.VertexB];
				const auto ACId = verticeToPromotedVertices[AB.VertexA];
				auto& BC = m_Edges[BCId];
				auto& AC = m_Edges[ACId];

				const auto A = m_Vertices[AB.VertexA];
				const auto B = m_Vertices[AB.VertexB];
				const auto C = m_Vertices[newVertId];

				if (Math::IsTriangleOriented(A.Position, B.Position, C.Position)) {
					const auto ABCId = GenerateTriangleId();
					const auto ABC = Triangle{ABId, BCId, ACId};
					m_Triangles[ABCId] = ABC;

					AB.TriangleLeft = ABCId;
					BC.TriangleLeft = ABCId;
					AC.TriangleRight = ABCId;
				} else {
					const auto ABCId = GenerateTriangleId();
					const auto ABC = Triangle{ABId, ACId, BCId};
					m_Triangles[ABCId] = ABC;

					AB.TriangleRight = ABCId;
					BC.TriangleRight = ABCId;
					AC.TriangleLeft = ABCId;
				}
			}
		}
		else if (m_Vertices.size() == 2) {
			uint32_t otherId = -1;
			for (const auto&[id,vert] : m_Vertices) {
				if (id != newVertId) {
					otherId = id;
					break;
				}
			}
			if (otherId == -1) return; // Too much safety but is okay.
			const uint32_t newEdge = GenerateEdgeId();
			m_Edges[newEdge] = {otherId, newVertId};
		}
	}
}
