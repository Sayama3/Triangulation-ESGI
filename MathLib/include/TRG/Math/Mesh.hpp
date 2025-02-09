//
// Created by ianpo on 31/01/2025.
//

#pragma once

#include "Basics.hpp"

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
			uint32_t EdgeA;
			uint32_t EdgeB;
			uint32_t EdgeC;
		};
	public:
		void AddPoint(Vector2 point);
		void RemovePoint(uint32_t pointId);

		std::vector<Vector2> ToMesh2D();
		std::vector<Vector3> ToMesh3DXZ(T Y = 0);
		std::vector<Vector3> ToMesh3DXY(T Z = 0);
	public:
		void clear();
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

				Edge& AB = m_Edges[ABC.EdgeA];
				Edge& secondEdge = m_Edges[ABC.EdgeB];
				Edge& thirdEdge = m_Edges[ABC.EdgeC];

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

					compatibleEdges.push_back(ABC.EdgeA);
					compatibleEdges.push_back(ABC.EdgeB);
					compatibleEdges.push_back(ABC.EdgeC);
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

	inline std::vector<typename MeshGraph::Vector2> MeshGraph::ToMesh2D() {
		std::vector<MeshGraph::Vector2> mesh;
		mesh.reserve(m_Triangles.size() * 3);
		for (const auto&[trId, ABC] : m_Triangles) {
			const auto& AB = m_Edges[ABC.EdgeA];
			const auto& MaybeBC = m_Edges[ABC.EdgeB];

			const auto& A = m_Vertices[AB.VertexA];
			const auto& B = m_Vertices[AB.VertexB];
			const auto& C = m_Vertices[MaybeBC.VertexB];

			const bool isOriented = Math::IsTriangleOriented(A.Position, B.Position, C.Position);
			if (isOriented) {
				mesh.push_back(A.Position);
				mesh.push_back(B.Position);
				mesh.push_back(C.Position);
			} else {
				mesh.push_back(A.Position);
				mesh.push_back(C.Position);
				mesh.push_back(B.Position);
			}
		}
		return mesh;
	}

	inline std::vector<typename MeshGraph::Vector3> MeshGraph::ToMesh3DXZ(T y) {
		std::vector<MeshGraph::Vector3> mesh;
		mesh.reserve(m_Triangles.size() * 3);
		for (const auto&[trId, ABC] : m_Triangles) {
			const auto& AB = m_Edges[ABC.EdgeA];
			const auto& MaybeBC = m_Edges[ABC.EdgeB];

			const auto& A = m_Vertices[AB.VertexA];
			const auto& B = m_Vertices[AB.VertexB];
			const auto& C = m_Vertices[MaybeBC.VertexB];

			const bool isOriented = Math::IsTriangleOriented(A.Position, B.Position, C.Position);
			if (isOriented) {
				mesh.push_back(Vector3{A.Position.x, y, A.Position.y});
				mesh.push_back(Vector3{C.Position.x, y, C.Position.y});
				mesh.push_back(Vector3{B.Position.x, y, B.Position.y});
			} else {
				mesh.push_back(Vector3{A.Position.x, y, A.Position.y});
				mesh.push_back(Vector3{B.Position.x, y, B.Position.y});
				mesh.push_back(Vector3{C.Position.x, y, C.Position.y});
			}
		}
		return mesh;
	}

	inline std::vector<typename MeshGraph::Vector3> MeshGraph::ToMesh3DXY(T z) {
		std::vector<MeshGraph::Vector3> mesh;
		mesh.reserve(m_Triangles.size() * 3);
		for (const auto&[trId, ABC] : m_Triangles) {
			const auto& AB = m_Edges[ABC.EdgeA];
			const auto& MaybeBC = m_Edges[ABC.EdgeB];

			const auto& A = m_Vertices[AB.VertexA];
			const auto& B = m_Vertices[AB.VertexB];
			const auto& C = m_Vertices[MaybeBC.VertexB];

			const bool isOriented = Math::IsTriangleOriented(A.Position, B.Position, C.Position);
			if (isOriented) {
				mesh.push_back(Vector3{A.Position.x, A.Position.y, z});
				mesh.push_back(Vector3{B.Position.x, B.Position.y, z});
				mesh.push_back(Vector3{C.Position.x, C.Position.y, z});
			} else {
				mesh.push_back(Vector3{A.Position.x, A.Position.y, z});
				mesh.push_back(Vector3{C.Position.x, C.Position.y, z});
				mesh.push_back(Vector3{B.Position.x, B.Position.y, z});
			}
		}
		return mesh;
	}

	inline void MeshGraph::clear() {
		m_Vertices.clear();
		m_Edges.clear();
		m_Triangles.clear();
	}
}
