#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

namespace godot {

class GameManager : public Node3D {
    GDCLASS(GameManager, Node3D)

private:
    Input* input;

protected:
    static void _bind_methods();

public:
    GameManager();
    ~GameManager();
    void _ready() override;
    void _process(double delta) override;
};

}

#endif