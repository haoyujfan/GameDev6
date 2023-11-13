#include <cassert>

#include "body.h"
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

Body::~Body() {
    // Add your cleanup here.
}

void Body::_bind_methods() {
    ADD_SIGNAL(MethodInfo("player_chop"));
    ADD_SIGNAL(MethodInfo("player_slice"));
    ADD_SIGNAL(MethodInfo("player_stab"));
    ADD_SIGNAL(MethodInfo("player_dodge"));
    ADD_SIGNAL(MethodInfo("player_jump"));
    ADD_SIGNAL(MethodInfo("player_block"));
}

Body::Body() {
    input = Input::get_singleton();
    InputMap::get_singleton()->load_from_project_settings();
    gravity = 1400.0;
    velocity = Vector3(0, 0, 0);
}

void Body::_ready() {}

void Body::check_input() {
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Knight/AnimationPlayer"));
    if(input->is_action_just_pressed("chop")) {
            UtilityFunctions::print("chopping");
            move = Moves::CHOP;
            animation->play("1H_Melee_Attack_Chop");
        } else if(input->is_action_just_pressed("slice")) {
            move = Moves::SLICE;
            animation->play("1H_Melee_Attack_Slice_Horizontal");
        } else if(input->is_action_just_pressed("stab")) {
            move = Moves::STAB;
            animation->play("1H_Melee_Attack_Stab");
        } else if(input->is_action_just_pressed("dodge")) {
            move = Moves::DODGE;
            animation->play("Dodge_Left");
        } else if(input->is_action_just_pressed("jump")) {
            move = Moves::JUMP;
            animation->play("Jump_Full_Short");
        } else if(input->is_action_just_pressed("block")) {
            move = Moves::BLOCK;
            animation->play("Blocking");
        } else {
            move = Moves::IDLE;
            animation->play("Idle");
        }
}

void Body::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Knight/AnimationPlayer"));
    switch(move) {
        case Moves::IDLE:
            // code block
            check_input();
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

void Body::_physics_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    if (!this->is_on_floor()) {
            velocity.y -= gravity * delta;
    }
    set_velocity(velocity);
    move_and_slide();
}