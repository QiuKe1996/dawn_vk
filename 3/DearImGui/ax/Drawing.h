# pragma once
#include "imgui.h"

namespace ax {
namespace Drawing {

enum class IconType { Flow, Circle /*Բ��*/, Square/*����*/, Grid /*���߾���*/, RoundSquare/*Բ�Ǿ���*/, Diamond/*����*/ };

void DrawIcon(ImDrawList* drawList, const ImVec2& a, const ImVec2& b, IconType type, bool filled, ImU32 color, ImU32 innerColor);

} // namespace Drawing
} // namespace ax