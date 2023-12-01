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
    ClassDB::bind_method(D_METHOD("set_got_blocked", "p_got_blocked"), &Enemy::set_got_blocked);
    ClassDB::bind_method(D_METHOD("set_difficulty", "p_difficulty"), &Enemy::set_difficulty);
    ClassDB::bind_method(D_METHOD("add_move_list", "m"), &Enemy::add_move_list);

    ClassDB::bind_method(D_METHOD("get_total_moves"), &Enemy::get_total_moves);
    ClassDB::bind_method(D_METHOD("set_total_moves", "moves"), &Enemy::set_total_moves);
    ClassDB::bind_method(D_METHOD("get_move_0"), &Enemy::get_move_0);
    ClassDB::bind_method(D_METHOD("get_move_1"), &Enemy::get_move_1);
    ClassDB::bind_method(D_METHOD("get_move_2"), &Enemy::get_move_2);
    ClassDB::bind_method(D_METHOD("get_move_3"), &Enemy::get_move_3);
    ClassDB::bind_method(D_METHOD("get_move_4"), &Enemy::get_move_4);
    ClassDB::bind_method(D_METHOD("get_move_5"), &Enemy::get_move_5);
    ClassDB::bind_method(D_METHOD("get_move_6"), &Enemy::get_move_6);

    ClassDB::bind_method(D_METHOD("set_move_0", "v"), &Enemy::set_move_0);
    ClassDB::bind_method(D_METHOD("set_move_1", "v"), &Enemy::set_move_1);
    ClassDB::bind_method(D_METHOD("set_move_2", "v"), &Enemy::set_move_2);
    ClassDB::bind_method(D_METHOD("set_move_3", "v"), &Enemy::set_move_3);
    ClassDB::bind_method(D_METHOD("set_move_4", "v"), &Enemy::set_move_4);
    ClassDB::bind_method(D_METHOD("set_move_5", "v"), &Enemy::set_move_5);
    ClassDB::bind_method(D_METHOD("set_move_6", "v"), &Enemy::set_move_6);
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
    health = 40;
    move = Moves::IDLE;
    total_moves = 0;
    got_blocked = false;
    damage_done = true;
    timer = 0;
}


void Enemy::_ready() {
    rand.randomize();

    aggressiveness = rand.randf_range(0.3, 1.7);
    // aggressiveness = 0.6;
    UtilityFunctions::print("Aggression:");
    UtilityFunctions::print(aggressiveness);
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

    if(got_blocked) {
        UtilityFunctions::print("got blocked");
        if(animation->get_current_animation() == "Cheer") {
                return;
        }
        got_blocked = false;
    }
    timer += 1;
    switch(move) {
        case Moves::IDLE:
            animation->play("Idle");
            // about 2 second delay
            if (timer % 100 == 0){
                UtilityFunctions::print(timer);
                if (is_fighting) {
                    pick_move();
                }
                timer = 0;
            }
            break;
        case Moves::CHOP:
            if (timer % 10 == 0) {
                flash = Object::cast_to<ColorRect>(get_node_or_null("../../../ColorRect"));
                if (flash) {
                    flash->set_visible(false);
                }
            }
            if(animation->get_current_animation() == "1H_Melee_Attack_Chop") {
                if (!damage_done && animation->get_current_animation_position() > animation->get_current_animation_length() / 2) {
                    emit_signal("enemy_chop");
                    damage_done = true;
                }
                return;  
            }
            move = Moves::IDLE;
            break;
        case Moves::SLICE:
            if (timer % 10 == 0) {
                flash = Object::cast_to<ColorRect>(get_node_or_null("../../../ColorRect"));
                if (flash) {
                    flash->set_visible(false);
                }
            }
            if(animation->get_current_animation() == "1H_Melee_Attack_Slice_Horizontal") {
                if (!damage_done && animation->get_current_animation_position() > animation->get_current_animation_length() / 2) {
                    emit_signal("enemy_slice");
                    damage_done = true;
                }
                return;  
            }
            move = Moves::IDLE;
            break;
        case Moves::STAB:
            if (timer % 10 == 0) {
                flash = Object::cast_to<ColorRect>(get_node_or_null("../../../ColorRect"));
                if (flash) {
                    flash->set_visible(false);
                }
            }
            if(animation->get_current_animation() == "1H_Melee_Attack_Stab") {
                if (!damage_done && animation->get_current_animation_position() > animation->get_current_animation_length() / 3) {
                    emit_signal("enemy_stab");
                    damage_done = true;
                }
                return;  
            }
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
            flash = Object::cast_to<ColorRect>(get_node_or_null("../../../ColorRect"));
            if (flash) {
                flash->set_visible(true);
            }
            animation->play("1H_Melee_Attack_Chop",-1,difficulty,false);
            damage_done = false;
            break;
        case Moves::SLICE:
            flash = Object::cast_to<ColorRect>(get_node_or_null("../../../ColorRect"));
            if (flash) {
                flash->set_visible(true);
            }
            animation->play("1H_Melee_Attack_Slice_Horizontal",-1,difficulty,false);
            damage_done = false;
            break;
        case Moves::STAB:
            flash = Object::cast_to<ColorRect>(get_node_or_null("../../../ColorRect"));
            if (flash) {
                flash->set_visible(true);
            }
            animation->play("1H_Melee_Attack_Stab",-1,difficulty,false);
            damage_done = false;
            break;
        case Moves::DODGE:
            animation->play("Dodge_Left");
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

/* 
I think predict() is currently a Mean of Max algorithm by deciding on the player's
next move. Could change to Center of Mass in the future by just taking an average
of responses to all of player's moves.

Aggressiveness just factors into the final move decision, but probably isn't
considered fuzzy logic. If we implement movement, it could definitely be made
to do so. 
*/
void Enemy::predict() {
    UtilityFunctions::print("Probabilities:");
    UtilityFunctions::print((double)(block_probability + jump_probability + dodge_probability + stab_probability + slice_probability + chop_probability)); 

    // Predict player's next move
    // Weighted range of a move determined by the number of times a move has been done
    // The range where the float lands determines the predicted next move
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
        // May want to change default behavior
        move_response(-1);
    }
}

/* 
Take in player's predicted next move.
Set up an array to handle response to the predicted move.
Modify values in array using aggressiveness variable.
Choose random action using weighted probabilities inside of decision array.
*/
void Enemy::move_response(int m) {
    double decision[7] = {0, 0, 0, 0, 0, 0, 0};
    // m being the predicted next move of player
    // Populate probabilities of enemy's next move
    int num_nonzero_a;
    int num_nonzero_d;
    switch(m) {
        case Moves::CHOP:
            decision[Moves::CHOP] = 0.05;
            decision[Moves::SLICE] = 0.05;
            decision[Moves::STAB] = 0.35;
            decision[Moves::DODGE] = 0.5;
            decision[Moves::JUMP] = 0;
            decision[Moves::BLOCK] = 0.05;
            num_nonzero_a = 3;
            num_nonzero_d = 2;
            break;
        case Moves::SLICE:
            decision[Moves::CHOP] = 0.05;
            decision[Moves::SLICE] = 0.05;
            decision[Moves::STAB] = 0.35;
            decision[Moves::DODGE] = 0;
            decision[Moves::JUMP] = 0.5;
            decision[Moves::BLOCK] = 0.05;
            num_nonzero_a = 3;
            num_nonzero_d = 2;         
            break;
        case Moves::STAB:
            decision[Moves::CHOP] = 0;
            decision[Moves::SLICE] = 0;
            decision[Moves::STAB] = 0.1;
            decision[Moves::DODGE] = 0;
            decision[Moves::JUMP] = 0;
            decision[Moves::BLOCK] = 0.9;
            num_nonzero_a = 1;
            num_nonzero_d = 1;
            break;
        case Moves::DODGE:
            decision[Moves::CHOP] = 0;
            decision[Moves::SLICE] = 0.85;
            decision[Moves::STAB] = 0;
            decision[Moves::DODGE] = 0;
            decision[Moves::JUMP] = 0;
            decision[Moves::BLOCK] = 0.15;
            num_nonzero_a = 1;
            num_nonzero_d = 1;
            break;
        case Moves::JUMP:
            decision[Moves::CHOP] = 0.85;
            decision[Moves::SLICE] = 0;
            decision[Moves::STAB] = 0;
            decision[Moves::DODGE] = 0;
            decision[Moves::JUMP] = 0;
            decision[Moves::BLOCK] = 0.15;
            num_nonzero_a = 1;
            num_nonzero_d = 1;
            break;
        case Moves::BLOCK:
            decision[Moves::CHOP] = 0.5;
            decision[Moves::SLICE] = 0.5;
            decision[Moves::STAB] = 0;
            decision[Moves::DODGE] = 0;
            decision[Moves::JUMP] = 0;
            decision[Moves::BLOCK] = 0;
            num_nonzero_a = 2;
            num_nonzero_d = 0;
            break;
        default:
            // No moves registered; default behavior
            decision[Moves::CHOP] = 0.1;
            decision[Moves::SLICE] = 0.1;
            decision[Moves::STAB] = 0.1;
            decision[Moves::DODGE] = 0;
            decision[Moves::JUMP] = 0;
            decision[Moves::BLOCK] = 0.7;
            num_nonzero_a = 3;
            num_nonzero_d = 1;
            break;
    }

    /* 
    Aggression algorithm modifies probabilities inside of decision array.
    Should change probabilities only for actions with non-zero probabilites
    in order to ensure that actions which should never be done in response
    to a player move are indeed never done. Makes spamming one move more
    more punishing for the player.
    */
    double temp = aggressiveness - default_agg;
    double aggression_factor = abs(temp);
    if (temp > 0) { 
        // If more aggressive than default: take from defending, give to attacking
        double total_d = decision[Moves::DODGE] + decision[Moves::JUMP] + decision[Moves::BLOCK];
        double modifier_a;
        double modifier_d;
        if (num_nonzero_a == 0 || num_nonzero_d == 0) {
            modifier_a = 0.0;
            modifier_d = 0.0;
        } else {
            modifier_a = (aggression_factor * total_d) / num_nonzero_a;
            modifier_d = (aggression_factor * total_d) / num_nonzero_d;
        }
        
        for (int i = 1; i < 4; i++) {
            if (decision[i] > 0.0) {
                decision[i] += modifier_a;
            } 
        }
        for (int i = 4; i < 7; i++) {
            if (decision[i] > 0.0) {
                decision[i] -= modifier_d;
            }
        }
    } else if (temp < 0) { 
        // If less aggressive than default: take from attacking, give to defending
        double total_a = decision[Moves::CHOP] + decision[Moves::SLICE] + decision[Moves::STAB];
        double modifier_a;
        double modifier_d;
        if (num_nonzero_a == 0 || num_nonzero_d == 0) {
            modifier_a = 0.0;
            modifier_d = 0.0;
        } else {
            modifier_a = (aggression_factor * total_a) / num_nonzero_a;
            modifier_d = (aggression_factor * total_a) / num_nonzero_d;
        }

        for (int i = 1; i < 4; i++) {
            decision[i] -= modifier_a;
        }
        for (int i = 4; i < 7; i++) {
            decision[i] += modifier_d;
        }
    }

    // Ensure that decisions add up to 1
    UtilityFunctions::print("Decisions after Aggressiveness:");
    UtilityFunctions::print((double)(decision[1] + decision[2] + decision[3] + decision[4] + decision[5] + decision[6]));

    // Choose move based on random float
    // The range where the float lands determines the move
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

void Enemy::on_got_blocked() {
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Skin/AnimationPlayer"));
    animation->play("Cheer");
}

void Enemy::set_got_blocked(bool p_got_blocked) {
    got_blocked = p_got_blocked;
    on_got_blocked();
}

void Enemy::set_difficulty(double p_difficulty) {
    difficulty = p_difficulty;
}

int Enemy::get_move_0() {
    return player_move_list[0];
}

int Enemy::get_move_1() {
    return player_move_list[1];
}

int Enemy::get_move_2() {
    return player_move_list[2];
}

int Enemy::get_move_3() {
    return player_move_list[3];
}

int Enemy::get_move_4() {
    return player_move_list[4];
}

int Enemy::get_move_5() {
    return player_move_list[5];
}

int Enemy::get_move_6() {
    return player_move_list[6];
}

void Enemy::set_move_0(int v) {
    player_move_list[0] = v;
}

void Enemy::set_move_1(int v) {
    player_move_list[1] = v;
}

void Enemy::set_move_2(int v) {
    player_move_list[2] = v;
}

void Enemy::set_move_3(int v) {
    player_move_list[3] = v;
}

void Enemy::set_move_4(int v) {
    player_move_list[4] = v;
}

void Enemy::set_move_5(int v) {
    player_move_list[5] = v;
}

void Enemy::set_move_6(int v) {
    player_move_list[6] = v;
}


int Enemy::get_total_moves() {
    return total_moves;
}

void Enemy::set_total_moves(int moves) {
    total_moves = moves;
}
