//
// Created by ianpo on 31/01/2025.
//

#pragma once

#include "Basics.hpp"
#include "Geometry.hpp"
#include "Shells.hpp"
#include "Mesh.hpp"

namespace TRG::Math {



	template<typename Iter>
	std::vector<glm::vec<3,Real>> IncrementalTriangulation(Iter cbegin, Iter cend, Real y = 0) {
		using Vector2 = glm::vec<2,Real>;
		MeshGraph meshGraph;
		std::vector<Vector2> vertices{cbegin, cend};

		std::sort(vertices.begin(), vertices.end(), [](const Vector2& a, const Vector2& b) {
			if (a.x == b.x) {
				return a.y < b.y;
			}
			return a.x < b.x;
		});

		for (const auto& vertex: vertices) {
			meshGraph.AddPoint(vertex);
		}

		return meshGraph.ToMesh3DXZ(y);
	}

}