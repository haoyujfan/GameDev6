#ifndef BODY_H
#define BODY_H

#include "moves.h"
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>

namespace godot {
class PlayerCamera;

class Body : public CharacterBody3D {
    GDCLASS(Body, CharacterBody3D)

private:
    Input* input;
    int move;


protected:
    static void _bind_methods();

public:
    Body();
    ~Body();
    void _ready() override;
    void _process(double delta) override;
};

}

#endif