#include "gamemanager.h"
#include "body.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp> 
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>


using namespace godot;

GameManager::~GameManager() {
    // Add your cleanup here.
}

void GameManager::_bind_methods() {
}

GameManager::GameManager() {
    input = Input::get_singleton();
    InputMap::get_singleton()->load_from_project_settings(); 
}

void GameManager::_ready() {
}

void GameManager::_process(double delta) {

}

