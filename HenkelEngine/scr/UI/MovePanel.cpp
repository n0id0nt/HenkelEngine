#include "MovePanel.h"
#include "imgui_stdlib.h"

void MovePanel::Render()
{
	StartPanelRender();

	ImGui::InputText("Moves", &moves);

	if (ImGui::Button("Make move"))
	{
		moves.clear();
	}

	EndPanelRender();
}
