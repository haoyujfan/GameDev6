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
    ClassDB::bind_method(D_METHOD("add_move_list", "m"), &Enemy::add_move_list);
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
    health = 50;
    move = Moves::IDLE;
    total_moves = 0;

    aggressiveness = 0.5;
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
    switch(move) {
        case Moves::IDLE:
            animation->play("Idle");
            if (is_fighting) {
                pick_move();
            }
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
    // move = rand.randi_range(1, 6); // inclusive
    predict();
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Skin/AnimationPlayer"));
    switch(move) {
        case Moves::CHOP:
            animation->play("1H_Melee_Attack_Chop");
            break;
        case Moves::SLICE:
            animation->play("1H_Melee_Attack_Slice_Horizontal");
            break;
        case Moves::STAB:
            animation->play("1H_Melee_Attack_Stab");
            break;
        case Moves::DODGE:
            animation->play("Dodge_Right");
            break;
        case Moves::JUMP:
            animation->play("Jump_Full_Short");
            break;
        case Moves::BLOCK:
            animation->play("Blocking");
            break;
        default:
            animation->play("Idle");
            break;
    }
}

void Enemy::predict() {
    UtilityFunctions::print("Probabilities:");
    UtilityFunctions::print((double)(block_probability + jump_probability + dodge_probability + stab_probability + slice_probability + chop_probability)); 

    double prediction = rand.randf_range(0, 1);
    if (prediction <= chop_probability) {
        move_response(Moves::CHOP);
    } else if (prediction <= slice_probability + chop_probability) {
        move_response(Moves::SLICE);
    } else if (prediction <= stab_probability + slice_probability + chop_probability) {
        move_response(Moves::STAB);
    } else if (prediction <= dodge_probability + stab_probability + slice_probability + chop_probability) {
        move_response(Moves::DODGE);
    } else if (prediction <= jump_probability + dodge_probability + stab_probability + slice_probability + chop_probability) {
        move_response(Moves::JUMP);
    } else if (prediction <= block_probability + jump_probability + dodge_probability + stab_probability + slice_probability + chop_probability) {
        move_response(Moves::BLOCK);
    } else {
        move_response(Moves::IDLE);
    }
}

void Enemy::move_response(int m) {
    double decision[7] = {0, 0, 0, 0, 0, 0, 0};
    // m being the predicted next move of player
    // modifiable probabilities
    switch(m) {
        case Moves::CHOP:
            decision[Moves::CHOP] = 0.1;
            decision[Moves::SLICE] = 0.1;
            decision[Moves::STAB] = 0.2;
            decision[Moves::DODGE] = 0.5;
            decision[Moves::JUMP] = 0;
            decision[Moves::BLOCK] = 0.1;
            break;
        case Moves::SLICE:
            decision[Moves::CHOP] = 0.1;
            decision[Moves::SLICE] = 0.1;
            decision[Moves::STAB] = 0.2;
            decision[Moves::DODGE] = 0;
            decision[Moves::JUMP] = 0.5;
            decision[Moves::BLOCK] = 0.1;            
            break;
        case Moves::STAB:
            decision[Moves::CHOP] = 0;
            decision[Moves::SLICE] = 0;
            decision[Moves::STAB] = 0.1;
            decision[Moves::DODGE] = 0.2;
            decision[Moves::JUMP] = 0.2;
            decision[Moves::BLOCK] = 0.5;
            break;
        case Moves::DODGE:
            decision[Moves::CHOP] = 0;
            decision[Moves::SLICE] = 0.6;
            decision[Moves::STAB] = 0;
            decision[Moves::DODGE] = 0.1;
            decision[Moves::JUMP] = 0.1;
            decision[Moves::BLOCK] = 0.2;
            break;
        case Moves::JUMP:
            decision[Moves::CHOP] = 0.6;
            decision[Moves::SLICE] = 0;
            decision[Moves::STAB] = 0;
            decision[Moves::DODGE] = 0.1;
            decision[Moves::JUMP] = 0.1;
            decision[Moves::BLOCK] = 0.2;
            break;
        case Moves::BLOCK:
            decision[Moves::CHOP] = 0.3;
            decision[Moves::SLICE] = 0.3;
            decision[Moves::STAB] = 0;
            decision[Moves::DODGE] = 0;
            decision[Moves::JUMP] = 0;
            decision[Moves::BLOCK] = 0.4;
            break;
        default:
            // If player idle (supposedly after getting a stab blocked)
            decision[Moves::CHOP] = 0.1;
            decision[Moves::SLICE] = 0.1;
            decision[Moves::STAB] = 0.8;
            decision[Moves::DODGE] = 0;
            decision[Moves::JUMP] = 0;
            decision[Moves::BLOCK] = 0;
            break;
    }

    // Aggression algorithm
    // When less aggressive than default, take away from attacking and give to defending
    // When more aggressive than default, take away from defending and give to attacking
    double temp = aggressiveness - default_agg;
    double aggression_factor = 1.0 - temp;
    if (temp > 0) {
        double p_total = decision[Moves::DODGE] + decision[Moves::JUMP] + decision[Moves::BLOCK];
        double modifier = (aggression_factor * p_total) / 3.0;
        for (int i = 1; i < 4; i++) {
            decision[i] += modifier;
        }
        for (int i = 4; i < 7; i++) {
            decision[i] -= modifier;
        }
    } else if (temp < 0) {
        double a_total = decision[Moves::CHOP] + decision[Moves::SLICE] + decision[Moves::STAB];
        double modifier = (aggression_factor * a_total) / 3.0;
        for (int i = 1; i < 4; i++) {
            decision[i] -= modifier;
        }
        for (int i = 4; i < 7; i++) {
            decision[i] += modifier;
        }
    }

    // Ensure that decisions add up to 1
    UtilityFunctions::print("Decisions after Aggressiveness:");
    UtilityFunctions::print((double)(decision[1] + decision[2] + decision[3] + decision[4] + decision[5] + decision[6]));

    double choice = rand.randf_range(0, 1);
    if (choice <= decision[1]) {
        move = Moves::CHOP;
    } else if (choice <= decision[1] + decision[2]) {
        move = Moves::SLICE;
    } else if (choice <= decision[1] + decision[2] + decision[3]) {
        move = Moves::STAB;
    } else if (choice <= decision[1] + decision[2] + decision[3] + decision[4]) {
        move = Moves::DODGE;
    } else if (choice <= decision[1] + decision[2] + decision[3] + decision[4] + decision[5]) {
        move = Moves::JUMP;
    } else if (choice <= decision[1] + decision[2] + decision[3] + decision[4] + decision[5] + decision[6]) {
        move = Moves::BLOCK;
    } else {
        move = Moves::IDLE;
    }
}

void Enemy::add_move_list(int m) {
    player_move_list[m] += 1;
    total_moves += 1;

    chop_probability = (double)player_move_list[1] / (double)total_moves;
    slice_probability = (double)player_move_list[2] / (double)total_moves;
    stab_probability = (double)player_move_list[3] / (double)total_moves;
    dodge_probability = (double)player_move_list[4] / (double)total_moves;
    jump_probability = (double)player_move_list[5] / (double)total_moves;
    block_probability = (double)player_move_list[6] / (double)total_moves;
    // for (int i = 0; i < 7; i++) {
    //     UtilityFunctions::print(player_move_list[i]);
    // }
}