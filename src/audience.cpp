#include <cassert>

#include "audience.h"
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

Audience::~Audience() {
    // Add your cleanup here.
}

void Audience::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_location"), &Audience::get_location);
    // ClassDB::bind_method(D_METHOD("get_running"), &Body::get_running);
    // ClassDB::bind_method(D_METHOD("set_running", "p_running"), &Body::set_running);
    // ClassDB::bind_method(D_METHOD("get_fighting"), &Body::get_fighting);
    // ClassDB::bind_method(D_METHOD("set_fighting", "p_fighting"), &Body::set_fighting);
    // ADD_SIGNAL(MethodInfo("player_chop"));
    // ADD_SIGNAL(MethodInfo("player_slice"));
    // ADD_SIGNAL(MethodInfo("player_stab"));
    // ADD_SIGNAL(MethodInfo("player_dodge"));
    // ADD_SIGNAL(MethodInfo("player_jump"));
    // ADD_SIGNAL(MethodInfo("player_block"));
}

Audience::Audience() {
    input = Input::get_singleton();
    InputMap::get_singleton()->load_from_project_settings();
    gravity = 1400.0;
    velocity = Vector3(0, 0, 0);
    is_running = true;
}

void Audience::_ready() {
}


Vector3 Audience::get_location() {
    return get_position();
}

void Audience::_process(double delta) {
    if(Engine::get_singleton()->is_editor_hint()) {
        return;
    }
    AnimationPlayer* animation = get_node<AnimationPlayer>(NodePath("Mage/AnimationPlayer"));
    animation->play("Cheer");
    
}

