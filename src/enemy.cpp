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
#include <godot_cpp/classes/progress_bar.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>


using namespace godot;

Enemy::~Enemy() {
    // Add your cleanup here.
    player_move_list = new Array();
}

void Enemy::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_location"), &Enemy::get_location);
    ClassDB::bind_method(D_METHOD("get_approaching"), &Enemy::get_approaching);
    ClassDB::bind_method(D_METHOD("set_approaching", "p_approaching"), &Enemy::set_approaching);
    ClassDB::bind_method(D_METHOD("get_fighting"), &Enemy::get_fighting);
    ClassDB::bind_method(D_METHOD("set_fighting", "p_fighting"), &Enemy::set_fighting);
    ClassDB::bind_method(D_METHOD("get_move"), &Enemy::get_move);
    ClassDB::bind_method(D_METHOD("get_health"), &Enemy::get_health);
    ClassDB::bind_method(D_METHOD("set_health", "p_health"), &Enemy::set_health);
    ADD_SIGNAL(MethodInfo("enemy_chop"));
    ADD_SIGNAL(MethodInfo("enemy_slice"));
    ADD_SIGNAL(MethodInfo("enemy_stab"));
    ADD_SIGNAL(MethodInfo("enemy_death"));
}

Enemy::Enemy() {
    input = Input::get_singleton();
    InputMap::get_singleton()->load_from_project_settings();
    gravity = 0.0;
    velocity = Vector3(0, 0, 0);
    is_approaching = false;
    is_fighting = false;
    dying = false;
    dead = false;
    health = 1;
    move = Moves::IDLE;
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
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Skin/AnimationPlayer"));
    if(dying) {
        if(animation->get_current_animation() == "Death_A") {
                return;
        }
        dying = false;
        dead = true;
        emit_signal("enemy_death");
        if (get_node_or_null("CollisionShape3D"))
            get_node<CollisionShape3D>("CollisionShape3D")->queue_free();
        return;
    }
    if(dead) {
        animation->play("Death_A_Pose");
        return;
    }
    velocity.x = 0;
    if (!this->is_on_floor()) {
            velocity.y -= gravity * delta;
    }
    if(is_approaching) {
        velocity.x = 30;
        set_velocity(velocity);
        move_and_slide();
        animation->play("Running_A");
        return;
    }
    set_velocity(velocity);
    move_and_slide();
    if (is_fighting) {
        pick_move();
    }
    switch(move) {
        case Moves::IDLE:
            animation->play("Idle");
            break;
        case Moves::CHOP:
            if(animation->get_current_animation() == "1H_Melee_Attack_Chop") {
                return;
            }
            emit_signal("enemy_chop");
            move = Moves::IDLE;
            break;
        case Moves::SLICE:
            if(animation->get_current_animation() == "1H_Melee_Attack_Slice_Horizontal") {
                return;
            }
            emit_signal("enemy_slice");
            move = Moves::IDLE;
            break;
        case Moves::STAB:
            if(animation->get_current_animation() == "1H_Melee_Attack_Stab") {
                return;
            }
            emit_signal("enemy_stab");
            move = Moves::IDLE;
            break;
        case Moves::DODGE:
            if(animation->get_current_animation() == "Dodge_Left") {
                return;
            }
            move = Moves::IDLE;
            break;
        case Moves::JUMP:
            if(animation->get_current_animation() == "Jump_Full_Short") {
                return;
            }
            move = Moves::IDLE;
            break;
        case Moves::BLOCK:
            if(animation->get_current_animation() == "Blocking") {
                return;
            }
            move = Moves::IDLE;
            break;
        default:
            move = Moves::IDLE;
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

void Enemy::set_fighting(bool p_fighting) {
    is_fighting = p_fighting;
}

bool Enemy::get_fighting() {
    return is_fighting;
}

int Enemy::get_move() {
    return move;
}

void Enemy::set_health(double p_health) {
    health = p_health;
    get_node<ProgressBar>("SubViewport/ProgressBar")->set_value(health);
    if (health <= 0) {
        dying = true;
        AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Skin/AnimationPlayer"));
        animation->play("Death_A");
    }
}

double Enemy::get_health() {
    return health;
}

void Enemy::pick_move() {

    move = Moves::SLICE;
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Skin/AnimationPlayer"));
    animation->play("1H_Melee_Attack_Slice_Horizontal");
}