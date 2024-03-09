#include "UI.hpp"
#include <imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace a_game_engine
{
	void UI::draw()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
