//
// Created by ianpo on 30/01/2025.
//

#include "ImGuiLib.hpp"
#include <raylib.h>

namespace ImGuiLib {
	inline static const std::map<MouseButton, std::string> s_MouseButtonStr {
		{MOUSE_BUTTON_LEFT, "Button Left"},
		{MOUSE_BUTTON_RIGHT, "Button Right"},
		{MOUSE_BUTTON_MIDDLE, "Button Middle"},
		{MOUSE_BUTTON_SIDE, "Button Side"},
		{MOUSE_BUTTON_EXTRA, "Button Extra"},
		{MOUSE_BUTTON_FORWARD, "Button Forward"},
		{MOUSE_BUTTON_BACK, "Button Back"},
	};

	inline static const std::map<KeyboardKey, std::string> s_KeyboardKeyStr {
		{KEY_NULL, "None"},
		// Alphanumeric keys
		{KEY_APOSTROPHE, "Apostrophe"},
		{KEY_COMMA, "Comma"},
		{KEY_MINUS, "Minus"},
		{KEY_PERIOD, "Period"},
		{KEY_SLASH, "Slash"},
		{KEY_ZERO, "Zero"},
		{KEY_ONE, "One"},
		{KEY_TWO, "Two"},
		{KEY_THREE, "Three"},
		{KEY_FOUR, "Four"},
		{KEY_FIVE, "Five"},
		{KEY_SIX, "Six"},
		{KEY_SEVEN, "Seven"},
		{KEY_EIGHT, "Eight"},
		{KEY_NINE, "Nine"},
		{KEY_SEMICOLON, "Semicolon"},
		{KEY_EQUAL, "Equal"},
		{KEY_A, "A"},
		{KEY_B, "B"},
		{KEY_C, "C"},
		{KEY_D, "D"},
		{KEY_E, "E"},
		{KEY_F, "F"},
		{KEY_G, "G"},
		{KEY_H, "H"},
		{KEY_I, "I"},
		{KEY_J, "J"},
		{KEY_K, "K"},
		{KEY_L, "L"},
		{KEY_M, "M"},
		{KEY_N, "N"},
		{KEY_O, "O"},
		{KEY_P, "P"},
		{KEY_Q, "Q"},
		{KEY_R, "R"},
		{KEY_S, "S"},
		{KEY_T, "T"},
		{KEY_U, "U"},
		{KEY_V, "V"},
		{KEY_W, "W"},
		{KEY_X, "X"},
		{KEY_Y, "Y"},
		{KEY_Z, "Z"},
		{KEY_LEFT_BRACKET, "Left Bracket"},
		{KEY_BACKSLASH, "Backslash"},
		{KEY_RIGHT_BRACKET, "Right Bracket"},
		{KEY_GRAVE, "Grave"},
		// Function keys
		{KEY_SPACE, "Space"},
		{KEY_ESCAPE, "Escape"},
		{KEY_ENTER, "Enter"},
		{KEY_TAB, "Tab"},
		{KEY_BACKSPACE, "Backspace"},
		{KEY_INSERT, "Insert"},
		{KEY_DELETE, "Delete"},
		{KEY_RIGHT, "Right"},
		{KEY_LEFT, "Left"},
		{KEY_DOWN, "Down"},
		{KEY_UP, "Up"},
		{KEY_PAGE_UP, "Page Up"},
		{KEY_PAGE_DOWN, "Page Down"},
		{KEY_HOME, "Home"},
		{KEY_END, "End"},
		{KEY_CAPS_LOCK, "Caps Lock"},
		{KEY_SCROLL_LOCK, "Scroll Lock"},
		{KEY_NUM_LOCK, "Num Lock"},
		{KEY_PRINT_SCREEN, "Print Screen"},
		{KEY_PAUSE, "Pause"},
		{KEY_F1, "F1"},
		{KEY_F2, "F2"},
		{KEY_F3, "F3"},
		{KEY_F4, "F4"},
		{KEY_F5, "F5"},
		{KEY_F6, "F6"},
		{KEY_F7, "F7"},
		{KEY_F8, "F8"},
		{KEY_F9, "F9"},
		{KEY_F10, "F10"},
		{KEY_F11, "F11"},
		{KEY_F12, "F12"},
		{KEY_LEFT_SHIFT, "Left Shift"},
		{KEY_LEFT_CONTROL, "Left Control"},
		{KEY_LEFT_ALT, "Left Alt"},
		{KEY_LEFT_SUPER, "Left Super"},
		{KEY_RIGHT_SHIFT, "Right Shift"},
		{KEY_RIGHT_CONTROL, "Right Control"},
		{KEY_RIGHT_ALT, "Right Alt"},
		{KEY_RIGHT_SUPER, "Right Super"},
		{KEY_KB_MENU, "Kb Menu"},
		// Keypad keys
		{KEY_KP_0, "Keypad 0"},
		{KEY_KP_1, "Keypad 1"},
		{KEY_KP_2, "Keypad 2"},
		{KEY_KP_3, "Keypad 3"},
		{KEY_KP_4, "Keypad 4"},
		{KEY_KP_5, "Keypad 5"},
		{KEY_KP_6, "Keypad 6"},
		{KEY_KP_7, "Keypad 7"},
		{KEY_KP_8, "Keypad 8"},
		{KEY_KP_9, "Keypad 9"},
		{KEY_KP_DECIMAL, "Keypad Decimal"},
		{KEY_KP_DIVIDE, "Keypad Divide"},
		{KEY_KP_MULTIPLY, "Keypad Multiply"},
		{KEY_KP_SUBTRACT, "Keypad Subtract"},
		{KEY_KP_ADD, "Keypad Add"},
		{KEY_KP_ENTER, "Keypad Enter"},
		{KEY_KP_EQUAL, "Keypad Equal"},
		// Android key buttons
		{KEY_BACK, "Back"},
		{KEY_MENU, "Menu"},
		{KEY_VOLUME_UP, "Volume Up"},
		{KEY_VOLUME_DOWN, "Volume Down"},
	};

	bool ComboMouseButton(const char *label, MouseButton*mouseButton) {
		bool changed = false;
		const std::string &value = s_MouseButtonStr.at(*mouseButton);
		if (ImGui::BeginCombo(label, value.c_str(), ImGuiComboFlags_None)) {
			for (const auto &[value, name]: s_MouseButtonStr) {
				const bool is_selected = (*mouseButton == value);
				if (ImGui::Selectable(name.c_str(), is_selected)) {
					*mouseButton = value;
					changed = true;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		return changed;
	}


	bool ComboKeyboardKey(const char *label, KeyboardKey *keyboardKey) {
		bool changed = false;
		const std::string &value = s_KeyboardKeyStr.at(*keyboardKey);
		if (ImGui::BeginCombo(label, value.c_str(), ImGuiComboFlags_None)) {
			for (const auto &[value, name]: s_KeyboardKeyStr) {
				const bool is_selected = (*keyboardKey == value);
				if (ImGui::Selectable(name.c_str(), is_selected)) {
					*keyboardKey = value;
					changed = true;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		return changed;
	}
}
