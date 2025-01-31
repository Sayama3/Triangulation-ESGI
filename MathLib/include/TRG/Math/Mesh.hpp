//
// Created by ianpo on 31/01/2025.
//

#pragma once

#include "Basics.hpp"

namespace TRG::Math {
	template<typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	class MeshGraph2D {
	public:
		template<typename TVec>
		using List = std::list<TVec>;

		using Vertice = glm::vec<2, T, Q>;
		using ListVertice = List<Vertice>;
		using VerticeIter = typename ListVertice::iterator;

		struct Edge {
			Edge() : leftTriangle{}, rightTriangle{} {}
			~Edge() {}
			Edge(VerticeIter a, VerticeIter b) : a(a), b(b), leftTriangle{}, rightTriangle{}  {}
			Edge(const Edge& other) : a(other.a),  b(other.b), leftTriangle(other.leftTriangle), rightTriangle(other.rightTriangle) {}
			Edge& operator=(const Edge& other) {a = other.a;  b = other.b; leftTriangle = other.leftTriangle; rightTriangle = other.rightTriangle; return *this;}

			union {
				struct {
					VerticeIter a;
					VerticeIter b;
				};

				VerticeIter vertices[2];
			};

			union {
				struct {
					std::any leftTriangle;
					std::any rightTriangle;
				};

				std::any triangles[2];
			};

			bool operator==(const Edge &other) const {
				return (a == other.a && b == other.b) || (b == other.a && a == other.b);
			}

			bool operator!=(const Edge &other) const { return !(*this == other); }
		};
		using ListEdge = List<Edge>;
		using EdgeIter = typename ListEdge::iterator;

		struct Triangle {
			Triangle() {};
			~Triangle() {};
			Triangle(EdgeIter ab, EdgeIter bc, EdgeIter ca) : ab(ab), bc(bc), ca(ca) {}
			Triangle(const Triangle& other) : ab(other.ab),  bc(other.bc),  ca(other.ca) {}
			Triangle& operator=(const Triangle& other) {ab = other.ab;  bc = other.bc;  ca = other.ca; return *this;}
			union {
				struct {
					EdgeIter ab;
					EdgeIter bc;
					EdgeIter ca;
				};

				EdgeIter edges[3];
			};

			bool operator==(const Triangle &other) const {
				bool same = true;
				for (int i = 0; i < 3; ++i) {
					bool found = false;
					for (int j = 0; j < 3; ++j) {
						if (edges[i] == other.edges[j]) {
							found = true;
							break;
						}
					}
					if (!found) {
						same = false;
						break;
					}
				}
				return same;
			}
			bool operator!=(const Triangle &other) const { return !(*this == other); }
		};

		using ListTriangle = List<Triangle>;
		using TriangleIter = typename ListTriangle::iterator;


	public:
		MeshGraph2D() = default;
		~MeshGraph2D() = default;
	public:
		void AddVertex(const Vertice &vertice);
		std::pair<std::vector<glm::vec<2,T,Q>>, std::vector<uint32_t>> GenerateMesh() const;
	private:
		ListVertice m_Vertices;
		ListEdge m_Edges;
		ListTriangle m_Triangles;
		bool m_Initialized = false;
	};

	template<typename T, glm::qualifier Q>
	void MeshGraph2D<T, Q>::AddVertex(const Vertice &vertice) {
		VerticeIter vertIt = m_Vertices.insert(m_Vertices.end(), vertice);
		const auto verticesCount = m_Vertices.size();
		if (verticesCount == 2) {
			// Inserting the first edge
			m_Edges.insert(m_Edges.cend(), Edge{m_Vertices.begin(), vertIt});
		}
		else if (verticesCount > 2 && !m_Initialized) {
			// Add the new Edge
			VerticeIter prevIt = vertIt;
			--prevIt;

			// Prepare the loop from the second vertices to check if everything is collinear
			VerticeIter firstVertIt = m_Vertices.begin();
			const T dotProd = Math::Dot(Math::Normalize(*prevIt - *firstVertIt),
			                            Math::Normalize(*vertIt - *firstVertIt));
			const bool isCollinear = std::abs(dotProd) >= 1-std::numeric_limits<T>::epsilon();
			if (isCollinear) {
				m_Edges.insert(m_Edges.begin(), Edge{prevIt, vertIt});
			}
			else {
				// Initilizing all the triangles
				m_Initialized = true;
				for (EdgeIter abIt = m_Edges.begin(); abIt != m_Edges.end(); ++abIt) {
					if (abIt->a == vertIt || abIt->b == vertIt) continue;
					const bool isOriented = Math::IsTriangleOriented(*abIt->a, *abIt->b, vertice);
					EdgeIter bcIt = m_Edges.insert(m_Edges.cend(), Edge{abIt->b, vertIt});
					EdgeIter caIt = m_Edges.insert(m_Edges.cend(), Edge{vertIt, abIt->a});

					TriangleIter abc = m_Triangles.insert(m_Triangles.cend(), Triangle{abIt, bcIt, caIt});
					if (isOriented) {
						abIt->leftTriangle = abc;
						bcIt->leftTriangle = abc;
						caIt->leftTriangle = abc;
					} else {
						abIt->rightTriangle = abc;
						bcIt->rightTriangle = abc;
						caIt->rightTriangle = abc;
					}
				}
			}
		}
		else if (verticesCount > 2 && m_Initialized) {
			// TODO: Take in account internal triangles
			for (auto abIt = m_Edges.begin(); abIt != m_Edges.end(); ++abIt) {
				// Check Self
				if (abIt->a == vertIt || abIt->b == vertIt) continue;
				// Check collinear
				if (std::abs(Math::Dot(Math::Normalize(*abIt->b - *abIt->a), Math::Normalize(*vertIt - *abIt->a)))  >= static_cast<T>(1)-std::numeric_limits<T>::epsilon()) continue;

				bool isOriented = Math::IsTriangleOriented(*abIt->a, *abIt->b, vertice);

				// Check triangle not taken
				if (isOriented && !abIt->leftTriangle.has_value()) continue;
				if (!isOriented && !abIt->rightTriangle.has_value()) continue;

				EdgeIter bcIt = m_Edges.insert(m_Edges.cend(), Edge{abIt->b, vertIt});
				EdgeIter caIt = m_Edges.insert(m_Edges.cend(), Edge{vertIt, abIt->a});

				TriangleIter abc = m_Triangles.insert(m_Triangles.cend(), Triangle{abIt, bcIt, caIt});
				if (isOriented) {
					abIt->leftTriangle = abc;
					bcIt->leftTriangle = abc;
					caIt->leftTriangle = abc;
				} else {
					abIt->rightTriangle = abc;
					bcIt->rightTriangle = abc;
					caIt->rightTriangle = abc;
				}
			}
		}
	}

	template<typename T, glm::qualifier Q>
	std::pair<std::vector<glm::vec<2, T, Q>>, std::vector<uint32_t>> MeshGraph2D<T, Q>::GenerateMesh() const {
		std::unordered_map<glm::vec<2, T, Q>, uint32_t> verticesIndices;
		std::vector<glm::vec<2, T, Q>> vertices;
		verticesIndices.reserve(m_Vertices.size());
		vertices.reserve(m_Vertices.size());

		for (auto vertIt = m_Vertices.cbegin(); vertIt != m_Vertices.cend(); ++vertIt) {
			verticesIndices[*vertIt] = vertices.size();
			// verticesIndices.insert({vertIt, vertices.size()});
			vertices.push_back(*vertIt);
		}

		std::vector<uint32_t> indices;
		indices.reserve(m_Triangles.size() * 3);

		for (auto triangleIt = m_Triangles.cbegin(); triangleIt != m_Triangles.cend(); ++triangleIt) {
			indices.push_back(verticesIndices.at(*triangleIt->ab->a)); // Insert A from ABC
			TriangleIter leftTriangleIterPtr = std::any_cast<TriangleIter>(triangleIt->bc->leftTriangle);

			if (leftTriangleIterPtr == triangleIt) {
				indices.push_back(verticesIndices.at(*triangleIt->ab->b));  // Insert B from ABC
				indices.push_back(verticesIndices.at(*triangleIt->bc->b));  // Insert C from ABC
			} else {
				indices.push_back(verticesIndices.at(*triangleIt->bc->b));  // Insert C from ABC
				indices.push_back(verticesIndices.at(*triangleIt->ab->b));  // Insert B from ABC
			}
		}

		return {std::move(vertices), std::move(indices)};
	}
}
