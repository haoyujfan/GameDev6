#ifndef ENEMY_H
#define ENEMY_H

#include "body.h"
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>

namespace godot {
class PlayerCamera;

class Enemy : public CharacterBody3D {
    GDCLASS(Enemy, CharacterBody3D)

private:
    Input* input;
    int move;
    // Array* player_move_list;
    int player_move_list[7] = {0, 0, 0, 0, 0, 0, 0};
    int total_moves;
    double chop_probability;
    double slice_probability;
    double stab_probability;
    double dodge_probability;
    double jump_probability;
    double block_probability;

    double gravity;
    Vector3 velocity;
    Body* player;
    bool is_approaching;
    bool is_fighting;
    bool dead;
    bool dying;
    bool got_blocked;
    double health;
    RandomNumberGenerator rand;
    bool damage_done;

    // Fuzzy variables
    // Aggressiveness of 0.0 means no attacks; 2.0 means only attacks
    double aggressiveness;
    double default_agg = 1.0;


protected:
    static void _bind_methods();

public:
    Enemy();
    ~Enemy();
    Vector3 get_location();
    void _ready() override;
    void _process(double delta) override;
    void _physics_process(double delta) override;
    void set_approaching(bool p_approaching);
    bool get_approaching();
    void set_fighting(bool p_fighting);
    bool get_fighting();
    int get_move();
    void set_health(double p_health);
    double get_health();

    void set_got_blocked(bool p_got_blocked);
    void on_got_blocked();

    void pick_move();
    void predict();
    void move_response(int m);
    void add_move_list(int m);
};

}

#endif