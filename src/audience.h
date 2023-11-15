#ifndef AUDIENCE_H
#define AUDIENCE_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>

namespace godot {
class PlayerCamera;

class Audience : public CharacterBody3D {
    GDCLASS(Audience, CharacterBody3D)

private:
    Input* input;
    int move;
    double gravity;
    bool is_running;
    bool is_fighting;
    Vector3 velocity;
    


protected:
    static void _bind_methods();

public:
    Audience();
    ~Audience();
    // void check_input();
    Vector3 get_location();
    // void set_running(bool p_running);
    // bool get_running();
    // void set_fighting(bool p_fighting);
    // bool get_fighting();
    void _ready() override;
    void _process(double delta) override;
    // void _physics_process(double delta) override;
};

}

#endif