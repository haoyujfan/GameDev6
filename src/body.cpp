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
}


void Body::_ready() {
    
}

void Body::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Knight/AnimationPlayer"));
    switch(move) {
        case Moves::IDLE:
            // code block
            if(input->is_action_just_pressed("chop")) {
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
            }
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
                return;
            }
            // code block
            emit_signal("player_block");
            move = Moves::IDLE;
            break;
        default:
            // code block
            break;
    }

}