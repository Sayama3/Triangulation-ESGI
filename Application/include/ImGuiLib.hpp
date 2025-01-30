//
// Created by Iannis on 19/12/2024.
//

#pragma once

#include <imgui.h>
#include <raylib.h>
#include "TRG/Math.hpp"

namespace ImGuiLib
{
#ifdef TRG_DOUBLE
	inline static constexpr ImGuiDataType_ ImGuiRealDataType = ImGuiDataType_Double;
#else
	inline static constexpr ImGuiDataType_ ImGuiRealDataType = ImGuiDataType_Float;
#endif

	inline static bool DragReal(const char* label, TRG::Real* v, float v_speed = 1.0f, TRG::Real v_min = 0.0f, TRG::Real v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
		return ImGui::DragScalar(label, ImGuiRealDataType, v, v_speed, &v_min, &v_max, format, flags);
    }
	inline static bool DragReal2(const char* label, TRG::Real v[2], float v_speed = 1.0f, TRG::Real v_min = 0.0f, TRG::Real v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
		return ImGui::DragScalarN(label, ImGuiRealDataType, v, 2, v_speed, &v_min, &v_max, format, flags);
	}
	inline static bool DragReal3(const char* label, TRG::Real v[3], float v_speed = 1.0f, TRG::Real v_min = 0.0f, TRG::Real v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
		return ImGui::DragScalarN(label, ImGuiRealDataType, v, 3, v_speed, &v_min, &v_max, format, flags);
	}
	inline static bool DragReal4(const char* label, TRG::Real v[4], float v_speed = 1.0f, TRG::Real v_min = 0.0f, TRG::Real v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
		return ImGui::DragScalarN(label, ImGuiRealDataType, v, 4, v_speed, &v_min, &v_max, format, flags);
	}
	inline static bool DragQuat(const char* label, TRG::Quat* quat, float v_speed = 1.0f, TRG::Real v_min = 0.0f, TRG::Real v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
		if (!quat) return false;
		TRG::Vec3 euler = glm::eulerAngles(*quat) * TRG::rad2deg;
		const bool result =  DragReal3(label, &euler.x, v_speed, v_min, v_max, format, flags);
		if (result) *quat = TRG::Quat(euler * TRG::deg2rad);
		return result;
	}

	inline static bool SliderReal(const char* label, TRG::Real* v, TRG::Real v_min = 0.0f, TRG::Real v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
    	return ImGui::SliderScalar(label, ImGuiRealDataType, v, &v_min, &v_max, format, flags);
	}
	inline static bool SliderReal2(const char* label, TRG::Real v[2], TRG::Real v_min = 0.0f, TRG::Real v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
    	return ImGui::SliderScalarN(label, ImGuiRealDataType, v, 2, &v_min, &v_max, format, flags);
	}
	inline static bool SliderReal3(const char* label, TRG::Real v[3], TRG::Real v_min = 0.0f, TRG::Real v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
    	return ImGui::SliderScalarN(label, ImGuiRealDataType, v, 3, &v_min, &v_max, format, flags);
	}
	inline static bool SliderReal4(const char* label, TRG::Real v[4], TRG::Real v_min = 0.0f, TRG::Real v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
		return ImGui::SliderScalarN(label, ImGuiRealDataType, v, 4, &v_min, &v_max, format, flags);
	}

	inline static bool InputReal(const char* label, TRG::Real* v, TRG::Real step = 0.0f, TRG::Real step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
    	return ImGui::InputScalar(label, ImGuiRealDataType, (void*)v, (void*)(step > 0.0f ? &step : NULL), (void*)(step_fast > 0.0f ? &step_fast : NULL), format, flags);
	}
	inline static bool InputReal2(const char* label, TRG::Real v[2], const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
    	return ImGui::InputScalarN(label, ImGuiRealDataType, v, 2, NULL, NULL, format, flags);
	}
	inline static bool InputReal3(const char* label, TRG::Real v[3], const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
    	return ImGui::InputScalarN(label, ImGuiRealDataType, v, 3, NULL, NULL, format, flags);
	}
	inline static bool InputReal4(const char* label, TRG::Real v[4], const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
		return ImGui::InputScalarN(label, ImGuiRealDataType, v, 4, NULL, NULL, format, flags);
	}

	bool ComboMouseButton(const char* label, MouseButton* mouseButton);
	bool ComboKeyboardKey(const char* label, KeyboardKey* keyboardKey);

} // ImGuiLib
