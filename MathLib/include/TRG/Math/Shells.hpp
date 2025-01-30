//
// Created by ianpo on 30/01/2025.
//

#pragma once


#include "Basics.hpp"
#include "Geometry.hpp"

namespace TRG::Math {
	template<typename Iter, typename T = Real, glm::qualifier Q = glm::qualifier::defaultp>
	std::vector<glm::vec<2, T, Q> > JarvisConvexShell(Iter begin, Iter end) {
		if (begin == end) return {};
		std::vector<glm::vec<2, T, Q> > shell;
		uint64_t count{0};
		glm::vec<2, T, Q> first = *begin;
		for (auto it = begin; it != end; ++it) {
			const glm::vec<2, T, Q> &p = *it;
			++count;
			if (p.x < first.x) {
				first = p;
			} else if (p.x == first.x && p.y < first.y) {
				first = p;
			}
		}

		glm::vec<2, T, Q> current = first;
		glm::vec<2, T, Q> next = first;
		glm::vec<2, T, Q> direction = {0, 1};
		shell.push_back(first);

		for (int i = 0; i < count*count; ++i) {
			next = *std::min_element(
				begin, end, [direction, current](const glm::vec<2, T, Q> &a, const glm::vec<2, T, Q> &b) {
					if (a == current) return false;
					if (b == current) return true;
					const auto cToA = a - current;
					const auto cToB = b - current;
					const auto angleA = Math::SignedAngle(direction, cToA);
					const auto angleB = Math::SignedAngle(direction, cToB);
					if (angleA == angleB) {
						return Math::Magnitude(cToA) > Math::Magnitude(cToB);
					}
					return angleA < angleB;
				});
			if (next != first) {
				shell.push_back(next);
				direction = next - current;
				current = next;
			} else {
				break;
			}
		}

		return shell;
	}
}
