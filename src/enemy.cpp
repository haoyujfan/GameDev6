#include <cassert>

#include "enemy.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp> 
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>


using namespace godot;

Enemy::~Enemy() {
    // Add your cleanup here.
    player_move_list = new Array();
}

void Enemy::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("get_gravity"), &Body::get_gravity);
    // ClassDB::bind_method(D_METHOD("set_gravity", "p_gravity"), &Body::set_gravity);
    // ClassDB::bind_method(D_METHOD("get_jump_velocity"), &Body::get_jump_velocity);
    // ClassDB::bind_method(D_METHOD("set_jump_velocity", "p_jump_velocity"), &Body::set_jump_velocity);
    // ClassDB::bind_method(D_METHOD("get_air_control"), &Body::get_air_control);
    // ClassDB::bind_method(D_METHOD("set_air_control", "p_air_control"), &Body::set_air_control);
    // ClassDB::bind_method(D_METHOD("get_air_resistance"), &Body::get_air_resistance);
    // ClassDB::bind_method(D_METHOD("set_air_resistance", "p_air_resistance"), &Body::set_air_resistance);
    // ClassDB::bind_method(D_METHOD("get_speed"), &Body::get_speed);
    // ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Body::set_speed);
    // ClassDB::add_property("Body", PropertyInfo(Variant::FLOAT, "gravity"), "set_gravity", "get_gravity");
    // ClassDB::add_property("Body", PropertyInfo(Variant::FLOAT, "jump_velocity"), "set_jump_velocity", "get_jump_velocity");
    // ClassDB::add_property("Body", PropertyInfo(Variant::FLOAT, "air_control"), "set_air_control", "get_air_control");
    // ClassDB::add_property("Body", PropertyInfo(Variant::FLOAT, "air_resistance"), "set_air_resistance", "get_air_resistance");
    // ClassDB::add_property("Body", PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");

    // ADD_SIGNAL(MethodInfo("enemy_hit"));
    // ADD_SIGNAL(MethodInfo("ball_hit"));
}

Enemy::Enemy() {
    input = Input::get_singleton();
    InputMap::get_singleton()->load_from_project_settings();
    gravity = 1400.0;
    velocity = Vector3(0, 0, 0);
}


void Enemy::_ready() {
    
}

void Enemy::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
}

void Enemy::_physics_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (!this->is_on_floor()) {
        velocity.y -= gravity * delta;
    }
    set_velocity(velocity);
    move_and_slide();
}