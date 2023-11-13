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
    ClassDB::bind_method(D_METHOD("get_location"), &Enemy::get_location);
    ClassDB::bind_method(D_METHOD("get_approaching"), &Enemy::get_approaching);
    ClassDB::bind_method(D_METHOD("set_approaching", "p_approaching"), &Enemy::set_approaching);
}

Enemy::Enemy() {
    input = Input::get_singleton();
    InputMap::get_singleton()->load_from_project_settings();
    gravity = 1400.0;
    velocity = Vector3(0, 0, 0);
    is_approaching = false;
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
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Barbarian/AnimationPlayer"));
    if(is_approaching) {
        velocity.x = 30;
        set_velocity(velocity);
        move_and_slide();
        animation->play("Running_A");
        return;
    }
    set_velocity(velocity);
    move_and_slide();
    animation->play("Idle");
    switch(move) {
        case Moves::IDLE:
            // code block
            // check_input();
            break;
        case Moves::CHOP:
            if(animation->get_current_animation() == "1H_Melee_Attack_Chop") {
                return;
            }
            emit_signal("player_chop");
            move = Moves::IDLE;
            // code block
            break;
        case Moves::SLICE:
            if(animation->get_current_animation() == "1H_Melee_Attack_Slice_Horizontal") {
                return;
            }
            emit_signal("player_slice");
            // code block
            move = Moves::IDLE;
            break;
        case Moves::STAB:
            if(animation->get_current_animation() == "1H_Melee_Attack_Stab") {
                return;
            }
            emit_signal("player_stab");
            // code block
            move = Moves::IDLE;
            break;
        case Moves::DODGE:
            if(animation->get_current_animation() == "Dodge_Left") {
                return;
            }
            // code block
            emit_signal("player_dodge");
            move = Moves::IDLE;
            break;
        case Moves::JUMP:
            if(animation->get_current_animation() == "Jump_Full_Short") {
                return;
            }
            // code block
            emit_signal("player_jump");
            move = Moves::IDLE;
            break;
        case Moves::BLOCK:
            if(animation->get_current_animation() == "Blocking") {
                if(input->is_action_pressed("block")) {
                    animation->play("Blocking");
                }
                return;
            }
            if(input->is_action_pressed("block")) {
                    animation->play("Blocking");
                    return;
            }
            // code block
            emit_signal("player_block");
            move = Moves::IDLE;
            break;
        default:
            // code block
            break;
        animation->play("Idle");
    }
}

void Enemy::set_approaching(bool p_approaching) {
    is_approaching = p_approaching;
}

bool Enemy::get_approaching() {
    return is_approaching;
}

Vector3 Enemy::get_location() {
    return get_position();
}