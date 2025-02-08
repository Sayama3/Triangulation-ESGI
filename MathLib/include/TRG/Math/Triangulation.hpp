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
		// using Vector2 = glm::vec<2,T,Q>;
		// MeshGraph2D<T> meshGraph;
		// std::vector<Vector2> vertices{cbegin, cend};
		// std::vector<uint32_t> indices;
		// indices.reserve(vertices.size() * 3);
		//
		// std::sort(vertices.begin(), vertices.end(), [](const Vector2& a, const Vector2& b) {
		// 	if (a.x == b.x) {
		// 		return a.y < b.y;
		// 	}
		// 	return a.x < b.x;
		// });
		//
		// for (const auto& vertex: vertices) {
		// 	meshGraph.AddVertex(vertex);
		// }
		//
		// return meshGraph.GenerateMesh();
		return {};
	}

}