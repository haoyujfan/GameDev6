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
    ClassDB::bind_method(D_METHOD("get_location"), &Body::get_location);
    ClassDB::bind_method(D_METHOD("get_running"), &Body::get_running);
    ClassDB::bind_method(D_METHOD("set_running", "p_running"), &Body::set_running);
    ClassDB::bind_method(D_METHOD("get_fighting"), &Body::get_fighting);
    ClassDB::bind_method(D_METHOD("set_fighting", "p_fighting"), &Body::set_fighting);
    ClassDB::bind_method(D_METHOD("get_shield"), &Body::get_shield);
    ClassDB::bind_method(D_METHOD("set_shield", "p_shield"), &Body::set_shield);
    ClassDB::bind_method(D_METHOD("get_health"), &Body::get_health);
    ClassDB::bind_method(D_METHOD("set_health", "p_health"), &Body::set_health);
    ClassDB::bind_method(D_METHOD("get_move"), &Body::get_move);
    ClassDB::bind_method(D_METHOD("set_got_blocked", "p_got_blocked"), &Body::set_got_blocked);
    ADD_SIGNAL(MethodInfo("player_chop"));
    ADD_SIGNAL(MethodInfo("player_slice"));
    ADD_SIGNAL(MethodInfo("player_stab"));
    ADD_SIGNAL(MethodInfo("player_dodge"));
    ADD_SIGNAL(MethodInfo("player_jump"));
    ADD_SIGNAL(MethodInfo("player_block"));
    ADD_SIGNAL(MethodInfo("blocking",PropertyInfo(Variant::FLOAT,"shield")));
}

Body::Body() {
    input = Input::get_singleton();
    InputMap::get_singleton()->load_from_project_settings();
    gravity = 1400.0;
    shield = 100;
    health = 100;
    velocity = Vector3(0, 0, 0);
    is_running = true;
    got_blocked = false;
}

void Body::_ready() {
}

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
            animation->play("Dodge_Right");
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

Vector3 Body::get_location() {
    return get_position();
}

void Body::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Knight/AnimationPlayer"));
    if(is_running) {
        set_velocity(Vector3(-30,0,0));
        move_and_slide();
        animation->play("Running_A");
        return;
    }
    if(got_blocked) {
        UtilityFunctions::print("got blocked");
        if(animation->get_current_animation() == "Cheer") {
                return;
        }
        got_blocked = false;
    }

    if(move != Moves::BLOCK) {
        shield += delta * 10;
        emit_signal("blocking", shield);
    }
    if(shield > 100)
            shield = 100;
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
            if(animation->get_current_animation() == "Dodge_Right") {
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
            if(shield < 0) {
                shield = 0;
                move = Moves::IDLE;
                animation->play("Idle");
                return;
            }
            shield -= delta*20;
            emit_signal("blocking", shield);
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

void Body::set_running(bool p_running) {
    is_running = p_running;
}

bool Body::get_running() {
    return is_running;
}

void Body::set_fighting(bool p_fighting) {
    is_fighting = p_fighting;
}

bool Body::get_fighting() {
    return is_fighting;
}

void Body::set_shield(double p_shield) {
    shield = p_shield;
}

double Body::get_shield() {
    return shield;
}

void Body::set_health(double p_health) {
    health = p_health;
}

double Body::get_health() {
    return health;
}

int Body::get_move() {
    return move;
}

void Body::on_got_blocked() {
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Knight/AnimationPlayer"));
    animation->play("Cheer");
}

void Body::set_got_blocked(bool p_got_blocked) {
    got_blocked = p_got_blocked;
    on_got_blocked();
}