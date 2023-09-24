#pragma once
#include "Panel.h"
class MovePanel :
    public Panel
{
public:
    MovePanel(std::string name, Scene* scene)
        : Panel(name, scene), moves() {}

    void Render() override;
    void OnResize() override {}

private:
    std::string moves;
};

