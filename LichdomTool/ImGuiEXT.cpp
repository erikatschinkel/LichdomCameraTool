#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGuiEXT.h"
#include "../imgui/imgui_internal.h"

#define IM_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f)))   // Unsaturated, for display purpose 
bool ImGui::Checkbox(const char* label, bool* v, const ImVec2& subtractSize)
{
  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID id = window->GetID(label);
  const ImVec2 label_size = CalcTextSize(label, NULL, true);

  const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2)); // We want a square shape to we use Y twice
  ItemSize(check_bb, style.FramePadding.y);

  ImRect total_bb = check_bb;
  if (label_size.x > 0)
    SameLine(0, style.ItemInnerSpacing.x);
  const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
  if (label_size.x > 0)
  {
    ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
    total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
  }

  if (!ItemAdd(total_bb, id))
    return false;

  bool hovered, held;
  bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
  if (pressed)
    *v = !(*v);

  RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
  if (*v)
  {
    const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
    const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
    RenderCheckMark(check_bb.Min + ImVec2(pad, pad), GetColorU32(ImGuiCol_CheckMark), check_bb.GetWidth() - pad * 2.0f);
  }

  if (label_size.x > 0.0f)
    RenderText(text_bb.Min, label);

  return pressed;
}

bool ImGui::ToggleButton(const char* label, const ImVec2& size_arg, bool toggled, bool canUntoggle)
{
  ImGuiButtonFlags flags = (toggled && !canUntoggle) ? ImGuiButtonFlags_Disabled : 0;
  int pushedStyles = 0;
  if (toggled)
  {
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 1)); pushedStyles++;
    PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.9f, 0.9f, 1)); pushedStyles++;
    PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1)); pushedStyles++;
  }

  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems)
  {
    PopStyleColor(pushedStyles);
    return false;
  }

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID id = window->GetID(label);
  const ImVec2 label_size = CalcTextSize(label, NULL, true);

  ImVec2 pos = window->DC.CursorPos;
  if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
    pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
  ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

  const ImRect bb(pos, pos + size);
  ItemSize(bb, style.FramePadding.y);
  if (!ItemAdd(bb, id))
  {
    PopStyleColor(pushedStyles);
    return false;
  }

  if (g.CurrentItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
  bool hovered, held;
  bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

  if (pressed && !toggled)
  {
    PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.9f, 0.9f, 1)); pushedStyles++;
    PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1)); pushedStyles++;
  }

  if (held)
  {
    PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1)); pushedStyles++;
  }

  // Render
  ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
  if (pressed) col = ImU32(0xFFFFFFFF);
  RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
  RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

  // Automatically close popups
  //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
  //    CloseCurrentPopup();

  PopStyleColor(pushedStyles);

  return pressed;
}

void ImGui::Separator(const ImVec2& size, const ImVec4& color)
{
  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems)
    return;
  ImGuiContext& g = *GImGui;

  ImGuiSeparatorFlags flags = 0;
  if ((flags & (ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical)) == 0)
    flags |= (window->DC.LayoutType == ImGuiLayoutType_Horizontal) ? ImGuiSeparatorFlags_Vertical : ImGuiSeparatorFlags_Horizontal;
  IM_ASSERT(ImIsPowerOfTwo((int)(flags & (ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical))));   // Check that only 1 option is selected
  if (flags & ImGuiSeparatorFlags_Vertical)
  {
    SeparatorEx(ImGuiSeparatorFlags_Vertical);
    return;
  }

  // Horizontal Separator
  if (window->DC.CurrentColumns)
    PopClipRect();

  float x1 = window->Pos.x;
  float x2 = window->Pos.x + window->Size.x;
  if (!g.GroupStack.empty())
    x1 += window->DC.Indent.x;

  const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2 + size.x, window->DC.CursorPos.y + size.y));
  ItemSize(ImVec2(0.0f, 0.0f)); // NB: we don't provide our width so that it doesn't get feed back into AutoFit, we don't provide height to not alter layout.
  if (!ItemAdd(bb, 0))
  {
    if (window->DC.CurrentColumns)
      PushColumnClipRect(-1);
    return;
  }

  window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), GetColorU32(ImGuiCol_Separator));

  if (window->DC.CurrentColumns)
  {
    PushColumnClipRect(-1);
    window->DC.CurrentColumns->LineMinY = window->DC.CursorPos.y;
  }
}

void ImGui::DrawWithBorders(std::function<void()> drawCall)
{
  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
  drawCall();
  ImGui::PopStyleColor();
  ImGui::PopStyleVar();
}