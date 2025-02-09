//
// Created by ianpo on 31/01/2025.
//

#pragma once

#include "Basics.hpp"
#include <queue>

namespace TRG::Math {
	class MeshGraph {
	public:
		using T = Real;
		using Vector2 = glm::vec<2,T>;
		using Vector3 = glm::vec<3,T>;

		struct Vertex {
			Vector2 Position;
		};

		struct Edge {
			uint32_t VertexA;
			uint32_t VertexB;
			std::optional<uint32_t> TriangleLeft{std::nullopt};
			std::optional<uint32_t> TriangleRight{std::nullopt};
		};

		struct Triangle {
			uint32_t EdgeAB;
			uint32_t EdgeBC;
			uint32_t EdgeCA;
		};
	public:
		void AddPoint(Vector2 point);
		void DelaunayTriangulation();
	public:
		void clear();
	private:
		void ReverseEdge(uint32_t edgeId);
	private:
		[[nodiscard]] uint32_t GenerateVertexId() { return m_VertexIdGenerator++; };
		[[nodiscard]] uint32_t GenerateEdgeId() { return m_EdgeIdGenerator++; };
		[[nodiscard]] uint32_t GenerateTriangleId() { return m_TriangleIdGenerator++; };
	public:
		std::map<uint32_t, Vertex> m_Vertices;
		std::map<uint32_t, Edge> m_Edges;
		std::map<uint32_t, Triangle> m_Triangles;
	private:
		uint32_t m_VertexIdGenerator{0};
		uint32_t m_EdgeIdGenerator{0};
		uint32_t m_TriangleIdGenerator{0};
	};

	inline void MeshGraph::AddPoint(const Vector2 point) {
		auto it = std::find_if(m_Vertices.begin(), m_Vertices.end(), [point](std::pair<uint32_t, Vertex> vert) {return vert.second.Position == point;});
		if (it != m_Vertices.end()) return;


		const uint32_t newVertId = GenerateVertexId();
		m_Vertices[newVertId] = {point};

		if (m_Vertices.size() > 2) {
			// Check if outside
			std::unordered_set<uint32_t> compatibleVertices;
			compatibleVertices.reserve(m_Edges.size() - 1);

			std::vector<uint32_t> compatibleEdges{};
			compatibleEdges.reserve(m_Edges.size());

			// Check if inside
			for (auto [trId, ABC] : m_Triangles) {

				Edge& AB = m_Edges[ABC.EdgeAB];
				Edge& secondEdge = m_Edges[ABC.EdgeBC];
				Edge& thirdEdge = m_Edges[ABC.EdgeCA];

				const uint32_t aId = AB.VertexA;
				const uint32_t bId = AB.VertexB;
				const uint32_t cId = (secondEdge.VertexA != AB.VertexB) && (secondEdge.VertexA != AB.VertexA) ? secondEdge.VertexA : secondEdge.VertexB;

				const Vertex& A = m_Vertices[aId];
				const Vertex& B = m_Vertices[bId];
				const Vertex& C = m_Vertices[cId];

				if (Math::PointIsInsideTriangle(A.Position,B.Position,C.Position,point)) {
					// Remove existance of triangle.
					m_Triangles.erase(m_Triangles.find(trId));

					compatibleVertices.insert(aId);
					compatibleVertices.insert(bId);
					compatibleVertices.insert(cId);

					if(AB.TriangleLeft == trId) AB.TriangleLeft = std::nullopt;
					else AB.TriangleRight = std::nullopt;

					if(secondEdge.TriangleLeft == trId) secondEdge.TriangleLeft = std::nullopt;
					else secondEdge.TriangleRight = std::nullopt;

					if(thirdEdge.TriangleLeft == trId) thirdEdge.TriangleLeft = std::nullopt;
					else thirdEdge.TriangleRight = std::nullopt;

					compatibleEdges.push_back(ABC.EdgeAB);
					compatibleEdges.push_back(ABC.EdgeBC);
					compatibleEdges.push_back(ABC.EdgeCA);
					break;
				}
			}

			if (compatibleEdges.empty()) {
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
			}

			if (compatibleVertices.empty()) {
				T distance = 0;
				uint32_t closest = m_Vertices.begin()->first;
				for (const auto&[vId, vert] : m_Vertices) {
					if (vId == newVertId) continue;
					const auto d = Math::Magnitude(vert.Position - point);
					if (distance == 0 || d < distance) {
						closest = vId;
						distance = d;
					}
				}
				m_Edges[GenerateEdgeId()] = {closest, newVertId};
			}
			else
			{
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

					const auto& A = m_Vertices[AB.VertexA];
					const auto& B = m_Vertices[AB.VertexB];
					const auto& C = m_Vertices[newVertId];

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

	inline void MeshGraph::DelaunayTriangulation() {
		std::queue<uint32_t> edgeToCheck;
		for (auto [id, edge]: m_Edges) {
			if (edge.TriangleLeft && edge.TriangleRight)
				edgeToCheck.push(id);
		}

		while (!edgeToCheck.empty()) {
			const auto edgeId = edgeToCheck.front();
			edgeToCheck.pop();
			const auto& edge = m_Edges.at(edgeId);
			if (!edge.TriangleLeft || !edge.TriangleRight) return;

			const uint32_t t1Id = edge.TriangleLeft.value();
			const uint32_t t2Id = edge.TriangleRight.value();

			const Triangle& t1 = m_Triangles.at(t1Id);
			const Triangle& t2 = m_Triangles.at(t2Id);

			uint32_t a1Id = -1;
			uint32_t a4Id = -1;
			if (t1.EdgeAB == edgeId) {
				a1Id = t1.EdgeBC;
				a4Id = t1.EdgeCA;
			} else if (t1.EdgeBC == edgeId) {
				a1Id = t1.EdgeCA;
				a4Id = t1.EdgeAB;
			} else /* t1.EdgeCA == edgeId */ {
				a1Id = t1.EdgeAB;
				a4Id = t1.EdgeBC;
			}

			uint32_t a2Id = -1;
			uint32_t a3Id = -1;
			if (t2.EdgeAB == edgeId) {
				a3Id = t2.EdgeBC;
				a2Id = t2.EdgeCA;
			} else if (t2.EdgeBC == edgeId) {
				a3Id = t2.EdgeCA;
				a2Id = t2.EdgeAB;
			} else /* t2.EdgeCA == edgeId */ {
				a3Id = t2.EdgeAB;
				a2Id = t2.EdgeBC;
			}
			const uint32_t s1Id = edge.VertexB;
			const uint32_t s2Id = edge.VertexA;

			const Vertex& s1 = m_Vertices.at(s1Id);
			const Vertex& s2 = m_Vertices.at(s2Id);

			const Edge& a1 = m_Edges.at(a1Id);
			const uint32_t s4Id = a1.VertexA == s1Id || a1.VertexA == s2Id ? a1.VertexB : a1.VertexA;

			const Edge& a2 = m_Edges.at(a2Id);
			const uint32_t s3Id = a2.VertexA == s1Id || a2.VertexA == s2Id ? a2.VertexB : a2.VertexA;

			const Vertex& s3 = m_Vertices.at(s3Id);
			const Vertex& s4 = m_Vertices.at(s4Id);

			const Circle t1Circle = Math::GetCircle(s1.Position, s4.Position, s2.Position);
			const Circle t2Circle = Math::GetCircle(s1.Position, s2.Position, s3.Position);

			const bool shouldInvert = PointIsInsideCircle(t1Circle, s3.Position) || PointIsInsideCircle(t2Circle, s4.Position);
			if (shouldInvert) {
				ReverseEdge(edgeId);
				edgeToCheck.push(a1Id);
				edgeToCheck.push(a2Id);
				edgeToCheck.push(a3Id);
				edgeToCheck.push(a4Id);
			}
		}
	}

	inline void MeshGraph::clear() {
		m_Vertices.clear();
		m_Edges.clear();
		m_Triangles.clear();
	}

	inline void MeshGraph::ReverseEdge(const uint32_t edgeId) {
		if (!m_Edges.contains(edgeId)) return;
		Edge& edge = m_Edges.at(edgeId);
		if (!edge.TriangleLeft || !edge.TriangleRight) return;

		const auto t1Id = edge.TriangleLeft.value();
		const auto t2Id = edge.TriangleRight.value();

		if (!m_Triangles.contains(t1Id)) return;
		if (!m_Triangles.contains(t2Id)) return;

		Triangle& t1 = m_Triangles.at(t1Id);
		Triangle& t2 = m_Triangles.at(t2Id);

		uint32_t a1Id = -1;
		uint32_t a4Id = -1;
		if (t1.EdgeAB == edgeId) {
			a1Id = t1.EdgeBC;
			a4Id = t1.EdgeCA;
		} else if (t1.EdgeBC == edgeId) {
			a1Id = t1.EdgeCA;
			a4Id = t1.EdgeAB;
		} else /* t1.EdgeCA == edgeId */ {
			a1Id = t1.EdgeAB;
			a4Id = t1.EdgeBC;
		}

		uint32_t a2Id = -1;
		uint32_t a3Id = -1;
		if (t2.EdgeAB == edgeId) {
			a3Id = t2.EdgeBC;
			a2Id = t2.EdgeCA;
		} else if (t2.EdgeBC == edgeId) {
			a3Id = t2.EdgeCA;
			a2Id = t2.EdgeAB;
		} else /* t2.EdgeCA == edgeId */ {
			a3Id = t2.EdgeAB;
			a2Id = t2.EdgeBC;
		}

		const uint32_t s2Id = edge.VertexA;
		const uint32_t s1Id = edge.VertexB;

		const Edge& a1 = m_Edges.at(a1Id);
		Edge& a4 = m_Edges.at(a4Id);
		const uint32_t s4Id = a1.VertexA == s1Id || a1.VertexA == s2Id ? a1.VertexB : a1.VertexA;

		const Edge& a2 = m_Edges.at(a2Id);
		Edge& a3 = m_Edges.at(a3Id);
		const uint32_t s3Id = a2.VertexA == s1Id || a2.VertexA == s2Id ? a2.VertexB : a2.VertexA;



		if (a4.TriangleLeft == t1Id) a4.TriangleLeft = t2Id;
		else  a4.TriangleRight = t2Id;

		if (a3.TriangleLeft == t2Id) a3.TriangleLeft = t1Id;
		else a3.TriangleRight = t1Id;

		edge = {s3Id, s4Id, t2Id,t1Id};
		t1 = {edgeId, a2Id, a1Id};
		t2 = {edgeId, a4Id, a3Id};
	}
}
