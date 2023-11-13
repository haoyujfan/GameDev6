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
    double shield;
    double health;
    double gravity;
    bool is_running;
    bool is_fighting;
    Vector3 velocity;
    


protected:
    static void _bind_methods();

public:
    Body();
    ~Body();
    void check_input();
    Vector3 get_location();
    void set_running(bool p_running);
    bool get_running();
    void set_fighting(bool p_fighting);
    bool get_fighting();
    void set_shield(double p_shield);
    double get_shield();
    void set_health(double p_health);
<<<<<<< Updated upstream
    double get_health();
    int get_move();
=======
    bool get_health();
>>>>>>> Stashed changes
    void _ready() override;
    void _process(double delta) override;
    void _physics_process(double delta) override;
};

}

#endif