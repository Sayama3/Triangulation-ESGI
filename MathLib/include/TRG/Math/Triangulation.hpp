//
// Created by ianpo on 31/01/2025.
//

#pragma once

#include "Basics.hpp"
#include "Geometry.hpp"
#include "Shells.hpp"
#include "Mesh.hpp"

namespace TRG::Math {



	template<typename Iter, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	std::pair<std::vector<glm::vec<2,T,Q>>, std::vector<uint32_t>> IncrementalTriangulation(Iter cbegin, Iter cend) {
		using Vector2 = glm::vec<2,T,Q>;
		using Vector = std::vector<Vector2>;
		using Iter = Vector::iterator;
		using CIter = Vector::const_iterator;
		MeshGraph<2,Real> meshGraph;

		Vector vertex{cbegin, cend};
		std::vector<uint32_t> indices;
		indices.reserve(vertex.size() * 3);

		std::sort(vertex.begin(), vertex.end(), [](const Vector2& a, const Vector2& b) {
			if (a.x == b.x) {
				return a.y < b.y;
			}
			return a.x < b.x;
		});

		for (const auto& vertex: vertex) {
			meshGraph.AddVertex(vertex);
		}


	}

}