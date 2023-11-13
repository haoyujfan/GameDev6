#ifndef ENEMY_H
#define ENEMY_H

#include "body.h"
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>

namespace godot {
class PlayerCamera;

class Enemy : public CharacterBody3D {
    GDCLASS(Enemy, CharacterBody3D)

private:
    Input* input;
    int move;
    Array* player_move_list;
    double gravity;
    Vector3 velocity;
    Body* player;


protected:
    static void _bind_methods();

public:
    Enemy();
    ~Enemy();
    void _ready() override;
    void _process(double delta) override;
    void _physics_process(double delta) override;
};

}

#endif