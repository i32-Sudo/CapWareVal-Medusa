#include "main.h"
#include "icons.h"
#include "fonthelper.h"
#include "custom.hpp"
#include <font.hpp>

const char* healthtype[2] = { "Text", "Bar" };

void TssakliMenu()
{

	ImGui::PopStyleVar();

	ImGuiWindow* window = ImGui::GetCurrentWindow();

	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImVec2 p = ImGui::GetWindowPos();

	ImGui::SetWindowSize({ 650, 450 });

	static bool navbar_collapsed = true;
	static float navbar_width = 0.f; navbar_width = ImLerp(navbar_width, navbar_collapsed ? 0.f : 1.f, 0.04f);
	content_anim = ImLerp(content_anim, 1.f, 0.04f);

	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !navbar_collapsed);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, content_anim * (navbar_width > 0.005f ? 1 / navbar_width / 2 : 1.f));
	ImGui::SetCursorPos({ 81, 25 });
	ImGui::BeginGroup();
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
	ImGui::Text("CapWare");
	ImGui::PopFont();
	ImGui::BeginChild("main", { window->Size.x - 81, window->Size.y - window->DC.CursorPos.y + window->Pos.y }, 0, ImGuiWindowFlags_NoBackground);
	{
		switch (tab) {
		case 0: {
			ImGui::BeginChild("subtabs", { ImGui::GetWindowWidth() - 30, 40 }, 1);
			{
				ImGui::SetCursorPos({ 16, 0 });

				if (custom::subtab("Aimbot##subtab", subtab == 0)) subtab = 0; ImGui::SameLine(0, 20);


			}
			ImGui::EndChild();

			ImGui::BeginChild("main_child", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 55 }, 0, ImGuiWindowFlags_NoBackground);
			{
					custom::begin_child("Aimbot 1", { (ImGui::GetWindowWidth() - 30) / 2 - ImGui::GetStyle().ItemSpacing.x / 2, 0 });
					{
						ImGui::Checkbox("Enabled", &config.aimenable);
						ImGui::Combo("Key", &config.aimbotkey, optKey, IM_ARRAYSIZE(optKey));
						ImGui::SliderFloat("Fov", &config.aimbot_fov, 10.f, 1000.f, "%.f");
						ImGui::SliderFloat("Smooth", &config.aimbot_smooth, 1.f, 20.f, "%.f");
					}
					custom::end_child();
				}
				ImGui::SameLine(0, 15);
				{
					custom::begin_child("Aimbot 2", { (ImGui::GetWindowWidth() - 30) / 2 - ImGui::GetStyle().ItemSpacing.x / 2, 0 });
					{
						ImGui::Combo("Target Bone", &config.aimboness, aimbone, IM_ARRAYSIZE(aimbone));
						ImGui::Checkbox("Dr4w Fov", &config.aimbot_draw_fov);
						ImGui::Checkbox("RCS", &config.rcs);
				//		ImGui::Checkbox("vischeck", &config.visiblecheck);
					}
				}
				custom::end_child();
				ImGui::SameLine(0, 15);
				ImGui::Spacing();
		
			ImGui::EndChild();
		} break;
		case 1:
			ImGui::BeginChild("subtabs", { ImGui::GetWindowWidth() - 30, 40 }, 1);
			{
				ImGui::SetCursorPos({ 16, 0 });

				if (custom::subtab("Visuals##subtab", subtab == 0)) subtab = 0; ImGui::SameLine(0, 20);


			}
			ImGui::EndChild();

			ImGui::BeginChild("main_child", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 55 }, 0, ImGuiWindowFlags_NoBackground);
			{
				custom::begin_child("Visuals 1", { (ImGui::GetWindowWidth() - 30) / 2 - ImGui::GetStyle().ItemSpacing.x / 2, 0 });
				{

					ImGui::Checkbox("Box ESP", &config.player_box);
					ImGui::Combo("ESP Type", &config.esptype, type, IM_ARRAYSIZE(type));
					ImGui::Checkbox("Ignore Dormant", &config.player_ignore_dormant);
					ImGui::Checkbox("Head Circle", &config.circlehead);
					ImGui::Checkbox("Skeleton ESP", &config.player_skeleton);
					ImGui::Checkbox("Distance ESP", &config.player_distance);
					ImGui::Checkbox("Snaplines", &config.player_snapline);

				}
				custom::end_child();
			}
			ImGui::SameLine(0, 15);
			{
				custom::begin_child("Visuals 2", { (ImGui::GetWindowWidth() - 30) / 2 - ImGui::GetStyle().ItemSpacing.x / 2, 0 });
				{
					ImGui::Checkbox("Enemy View Angle", &config.player_view_angle);
					ImGui::Checkbox("Show Health", &config.player_healthbar);
					ImGui::Combo("Health Type", &config.healthhtype, healthtype, IM_ARRAYSIZE(healthtype));
					ImGui::Checkbox("Glow", &config.glow);
					ImGui::Checkbox("Show Agents Names", &config.characterr);
					//ImGui::Checkbox("Ch4ms", &config.chams);
			//		ImGui::Checkbox("Fov Changer", &config.fovchanger);

			//		ImGui::SliderFloat("Fov Value", &config.fovchangervalue, 100.f, 170.f, "%.f"); //i will make this to degrees later
				}
				custom::end_child();
			}
			ImGui::SameLine(0, 15);
			ImGui::Spacing();

			ImGui::EndChild();
			break;
		case 2: {
			ImGui::BeginChild("subtabs", { ImGui::GetWindowWidth() - 30, 40 }, 1);
			{
				ImGui::SetCursorPos({ 16, 0 });

				if (custom::subtab("Color##subtab", subtab == 0)) subtab = 0; ImGui::SameLine(0, 20);


			}
			ImGui::EndChild();

			ImGui::BeginChild("main_child", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 55 }, 0, ImGuiWindowFlags_NoBackground);
			{
				ImGui::BeginGroup();
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 0, 0, 0));
					ImGui::Text("ESP Color");
					ImGui::ColorEdit3("", (float*)&config.espcolor);
					ImGui::Text("Snapline Color");
					ImGui::ColorEdit3("", (float*)&config.snapcolor);
					ImGui::Text("Skeleton Color");
					ImGui::ColorEdit3("", (float*)&config.skeleton);
					ImGui::Text("Head C!rcle Color");
					ImGui::ColorEdit3("", (float*)&config.headboxcolor);

					ImGui::EndGroup();
				}
			}
			ImGui::EndChild();
		} break;
		case 3: {
			ImGui::BeginChild("subtabs", { ImGui::GetWindowWidth() - 30, 40 }, 1);
			{
				ImGui::SetCursorPos({ 16, 0 });

				if (custom::subtab("Config##subtab", subtab == 0)) subtab = 0; ImGui::SameLine(0, 20);


			}
			ImGui::EndChild();

			ImGui::BeginChild("main_child", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 55 }, 0, ImGuiWindowFlags_NoBackground);
			{
				if (ImGui::Button("[  Load  ]"))
				{
					LoadConfig();
				}
				if (ImGui::Button("[  Save  ]"))
				{
					SaveConfig();
				}
			}
			ImGui::EndChild();
		} break;
		}
	}
	ImGui::EndChild();
	ImGui::EndGroup();
	ImGui::PopStyleVar();
	ImGui::PopItemFlag();

	ImGui::SetCursorPos({ 0, 0 });

	ImGui::BeginChild("navbar", { 50 + 100 * navbar_width, window->Size.y }, 0, ImGuiWindowFlags_NoBackground);
	{
		ImGui::GetWindowDrawList()->AddRectFilled(p, p + ImGui::GetWindowSize(), ImGui::GetColorU32(ImGuiCol_ChildBg), ImGui::GetStyle().WindowRounding, ImDrawFlags_RoundCornersLeft);
		ImGui::GetWindowDrawList()->AddRectFilled({ p.x + ImGui::GetWindowWidth() - 1, p.y }, p + ImGui::GetWindowSize(), ImGui::GetColorU32(ImGuiCol_Border));

		ImGui::SetCursorPosY(87);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 16 });
		if (custom::tab("A", "Aimbot", tab == 0)) tab = 0;
		if (custom::tab("B", "Visuals", tab == 1)) tab = 1;
		if (custom::tab("C", "Colors", tab == 2)) tab = 2;
		if (custom::tab("D", "Config", tab == 3)) tab = 3;
		ImGui::PopStyleVar();
	}
	ImGui::EndChild();

	ImGui::SetCursorPos({ 41.5f + 100 * navbar_width, 47 });
	if (custom::collapse_button(navbar_collapsed)) navbar_collapsed = !navbar_collapsed;
}