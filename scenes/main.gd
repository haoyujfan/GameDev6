extends Node3D
@export var ArenaScene: PackedScene
@export var PlayerScene: PackedScene
@export var RANDOM_SHAKE_STRENGTH = 5.0;
@export var SHAKE_DECAY_RATE = 5.0;
var kills = 0;
var total_moves = 0;
var player_moves = [0,0,0,0,0,0,0];
var rand = RandomNumberGenerator.new();
var shake_strength = 0.0;

# Called when the node enters the scene tree for the first time.
func _ready():
	var arena = get_node("Arena/Env_Main_Arena")
	arena.position = Vector3(0,0,0);
	arena.add_to_group("arena1")
	var enemy = arena.get_node("Enemy");
	enemy.add_to_group("enemy1");
	enemy.connect("enemy_chop", enemy_chop);
	enemy.connect("enemy_slice", enemy_slice);
	enemy.connect("enemy_stab", enemy_stab);
	enemy.connect("enemy_death", enemy_death);
	arena = ArenaScene.instantiate()
	arena.position = Vector3(-300,0,0);
	arena.add_to_group("arena2")
	enemy = arena.get_node("Enemy");
	enemy.add_to_group("enemy2");
	enemy.connect("enemy_chop", enemy_chop);
	enemy.connect("enemy_slice", enemy_slice);
	enemy.connect("enemy_stab", enemy_stab);
	enemy.connect("enemy_death", enemy_death);
	$Arena.add_child(arena,true)
	rand.randomize();

	# var player = PlayerScene.instantiate()
	# player.position = Vector3(0,5,0);
	#add_child(player)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	var p = get_tree().get_first_node_in_group("player")
	var a = get_tree().get_first_node_in_group("arena1")
	var a2 = get_tree().get_first_node_in_group("arena2")
	var e = get_tree().get_first_node_in_group("enemy1")
	var e2 = get_tree().get_first_node_in_group("enemy2")
	
	shake_strength = lerp(shake_strength, 0.0, SHAKE_DECAY_RATE * _delta);
	var camera = get_node("Body/Camera3D");
	var offset = get_random_offset();
	camera.h_offset = offset.x;
	camera.v_offset = offset.y;
	
	if(e.get_health() > 0 && p.get_running() && abs(p.get_location().x - a.global_position.x) < 7) :
		p.set_running(false);
		e.set_approaching(true);
		print("approaching")
		return;
	if(e.get_approaching() && abs(e.global_position.x - p.global_position.x) < 6) :
		e.set_approaching(false);
		e.set_fighting(true);
		p.set_fighting(true);
		print("next to them")
		return;
	if(p.get_location().x < a.global_position.x-200) :
		var arena = ArenaScene.instantiate()
		arena.position = Vector3(a.global_position.x-600,a.global_position.y,a.global_position.z);
		arena.add_to_group("arena2")
		arena.get_node("Enemy").add_to_group("enemy2")
		a2.remove_from_group("arena2")
		a2.add_to_group("arena1")
		e2.remove_from_group("enemy2")
		e2.add_to_group("enemy1")
		e2.connect("enemy_chop", enemy_chop);
		e2.connect("enemy_slice", enemy_slice);
		e2.connect("enemy_stab", enemy_stab);
		e2.connect("enemy_death", enemy_death);
		e2.set_total_moves(total_moves);
		e2.set_move_0(player_moves[0]);
		e2.set_move_1(player_moves[1]);
		e2.set_move_2(player_moves[2]);
		e2.set_move_3(player_moves[3]);
		e2.set_move_4(player_moves[4]);
		e2.set_move_5(player_moves[5]);
		e2.set_move_6(player_moves[6]);
		var difficulty = 0.5 + (kills % 3)/10;
		e2.set_difficulty(difficulty);
		$Arena.add_child(arena,true)
		a.queue_free();
		
	pass


func enemy_chop() :
	var p = get_tree().get_first_node_in_group("player")
	var move = p.get_move()
	# dodge
	if move == 4 :
		return
	# block
	elif move == 6 :
		p.set_health(p.get_health() - 10)
		update_health(p.get_health(),true)
	else :
		p.set_health(p.get_health() - 20)
		update_health(p.get_health(),true)
	pass

func enemy_slice() :
	var p = get_tree().get_first_node_in_group("player")
	var move = p.get_move()
	# jump
	if move == 5 :
		return
	# block
	elif move == 6 :
		p.set_health(p.get_health() - 10)
		update_health(p.get_health(),true)
	else :
		p.set_health(p.get_health() - 20)
		update_health(p.get_health(),true)
	pass

func enemy_stab() :
	var p = get_tree().get_first_node_in_group("player")
	var move = p.get_move()
	# block
	if move == 6 :
		var e = get_tree().get_first_node_in_group("enemy1")
		e.set_got_blocked(true);
		return
	else :
		p.set_health(p.get_health() - 40)
		update_health(p.get_health(), true)
	pass
	
func enemy_death() :
	var p = get_tree().get_first_node_in_group("player")
	p.set_running(true);
	p.set_fighting(false);
	if(p.get_health() == 100):
		kills += 3
		
	else:
		kills += 1
	p.set_health(p.get_health() + 40);
	update_health(p.get_health(), false);
	get_node("GUI/GridContainer/ScoreBox/ScoreLabel").set_text(str(kills))
	var e = get_tree().get_first_node_in_group("enemy1")
	player_moves[0] = e.get_move_0();
	player_moves[1] = e.get_move_1();
	player_moves[2] = e.get_move_2();
	player_moves[3] = e.get_move_3();
	player_moves[4] = e.get_move_4();
	player_moves[5] = e.get_move_5();
	player_moves[6] = e.get_move_6();
	total_moves = e.get_total_moves();
	
	
	

func _on_body_blocking(shield):
	get_node("GUI/GridContainer/HBoxContainer/Shield").set_value(shield)

func shake_camera():
	shake_strength = RANDOM_SHAKE_STRENGTH;

func get_random_offset():
	return Vector2(
		rand.randf_range(-shake_strength,shake_strength),
		rand.randf_range(-shake_strength,shake_strength)
	)

func update_health(health, should_shake):
	if(should_shake):
		shake_camera();		
	get_node("GUI/GridContainer/HBoxContainer/Health").set_value(health)
	if(health <= 0):
		var p = get_tree().get_first_node_in_group("player")
		p.set_dead();
	pass

func _on_player_chop():
	var e = get_tree().get_first_node_in_group("enemy1")
	e.add_move_list(1)
	var move = e.get_move()
	# dodge
	if move == 4 :
		return
	# block
	elif move == 6 :
		e.set_health(e.get_health() - 3)
	else :
		e.set_health(e.get_health() - 5)
	pass # Replace with function body.

func _on_player_slice():
	var e = get_tree().get_first_node_in_group("enemy1")
	e.add_move_list(2)
	var move = e.get_move()
	# jump
	if move == 5 :
		return
	# block
	elif move == 6 :
		e.set_health(e.get_health() - 3)
	else :
		e.set_health(e.get_health() - 5)
	pass # Replace with function body.


func _on_player_stab():
	var e = get_tree().get_first_node_in_group("enemy1")
	e.add_move_list(3)
	var move = e.get_move()
	# block
	if move == 6 :
		get_node("Body").set_got_blocked(true);
		return
	else :
		e.set_health(e.get_health() - 10)
	pass # Replace with function body.


func _on_player_dodge():
	var e = get_tree().get_first_node_in_group("enemy1")
	e.add_move_list(4)
	pass # Replace with function body.


func _on_player_jump():
	var e = get_tree().get_first_node_in_group("enemy1")
	e.add_move_list(5)
	pass # Replace with function body.


func _on_player_block():
	var e = get_tree().get_first_node_in_group("enemy1")
	e.add_move_list(6)
	pass # Replace with function body.


func _on_body_player_death():
	get_tree().change_scene_to_file("res://game_over.tscn")
	pass # Replace with function body.
