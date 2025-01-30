//
// Created by ianpo on 31/01/2025.
//

#pragma once

#include "Basics.hpp"

namespace TRG::Math {
	template<glm::length_t L, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	class MeshGraph {
	public:
		template<typename TVec>
		using List = std::list<TVec>;

		using Vertice = glm::vec<L,T,Q>;
		using ListVertice = List<Vertice>;

		struct Edge;
		using ListEdge = List<Edge>;

		struct Triangle {
			union {
				struct {typename ListEdge::iterator a; typename ListEdge::iterator b; typename ListEdge::iterator c;};
				typename ListEdge::iterator edges[3];
			};
			bool operator==(const Triangle & other) const {
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
			bool operator!=(const Triangle & other) const {return !(*this == other);}
		};
		using ListTriangle = List<Triangle>;

		struct Edge {
			union {
				struct { typename ListVertice::iterator a; typename ListVertice::iterator b;};
				typename ListVertice::iterator vertices[2];
			};

			typename ListTriangle::iterator leftTriangle;
			typename ListTriangle::iterator rightTriangle;

			bool operator==(const Edge & other) const {return (a == other.a && b == other.b) || (b== other.a && a == other.b);}
			bool operator!=(const Edge & other) const {return !(*this == other);}
		};

	public:
		MeshGraph() = default;
		~MeshGraph() = default;
	public:
		void AddVertex(const Vertice& vertice);
	private:
		ListVertice m_Vertex;
		ListEdge m_Edges;
		ListTriangle m_triangles;
	};

	template<glm::length_t L, typename T, glm::qualifier Q>
	void MeshGraph<L, T, Q>::AddVertex(const Vertice &vertice) {
		m_Vertex.push_back(vertice);
	}
}
