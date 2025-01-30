//
// Created by Sayama on 05/11/2024.
//

#pragma once

namespace TRG {
	template<typename T2, typename T1>
	static inline T2& reinterpret(T1& value)
	{
		static_assert(sizeof(T1) == sizeof(T2));
		return *(T2*)&value;
	}

	template<typename T2, typename T1>
	static inline const T2& reinterpret(const T1& value)
	{
		static_assert(sizeof(T1) == sizeof(T2));
		return *(T2*)&value;
	}
}
