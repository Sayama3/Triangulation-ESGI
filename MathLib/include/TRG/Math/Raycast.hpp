//
// Created by ianpo on 30/01/2025.
//

#pragma once

#include "Basics.hpp"
#include "Geometry.hpp"

namespace TRG::Math {
	template<typename T, glm::qualifier Q = glm::qualifier::defaultp>
	static inline std::optional<T> Raycast(const Plane<T,Q>& plane, const Ray<3,T,Q>& ray) {

		const auto localPlane = Plane<T,Q>({0,0,0}, plane.Normal);
		const auto localRay = Ray<3,T,Q>(ray.Origin - plane.Origin, ray.Direction);

		float denom = Dot(localPlane.Normal, localRay.Direction);

		if (std::abs(denom) <= static_cast<T>(1e-4f)) {
			return std::nullopt;
		}

		auto t = -(Dot(localPlane.Normal, localRay.Origin)) / denom;

		return t;
	}
}