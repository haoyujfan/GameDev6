extends Node3D
@export var ArenaScene: PackedScene
@export var PlayerScene: PackedScene


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
		p.set_health(p.get_health() - 1)
		update_health(p.get_health())
	else :
		p.set_health(p.get_health() - 5)
		update_health(p.get_health())
	pass

func enemy_slice() :
	var p = get_tree().get_first_node_in_group("player")
	var move = p.get_move()
	# jump
	if move == 5 :
		return
	# block
	elif move == 6 :
		p.set_health(p.get_health() - 1)
		update_health(p.get_health())
	else :
		p.set_health(p.get_health() - 5)
		update_health(p.get_health())
	pass

func enemy_stab() :
	var p = get_tree().get_first_node_in_group("player")
	var move = p.get_move()
	# block
	if move == 6 :
		return
	else :
		p.set_health(p.get_health() - 10)
		update_health(p.get_health())
	pass
	
func enemy_death() :
	var p = get_tree().get_first_node_in_group("player")
	p.set_running(true);
	p.set_fighting(false);
	
	

func _on_body_blocking(shield):
	get_node("GUI/GridContainer/HBoxContainer/Shield").set_value(shield)
	
func update_health(health):
	get_node("GUI/GridContainer/HBoxContainer/Health").set_value(health)
	pass

func _on_player_chop():
	var e = get_tree().get_first_node_in_group("enemy1")
	var move = e.get_move()
	# dodge
	if move == 4 :
		return
	# block
	elif move == 6 :
		e.set_health(e.get_health() - 1)
	else :
		e.set_health(e.get_health() - 5)
	pass # Replace with function body.

func _on_player_slice():
	var e = get_tree().get_first_node_in_group("enemy1")
	var move = e.get_move()
	# jump
	if move == 5 :
		return
	# block
	elif move == 6 :
		e.set_health(e.get_health() - 1)
	else :
		e.set_health(e.get_health() - 5)
	pass # Replace with function body.


func _on_player_stab():
	var e = get_tree().get_first_node_in_group("enemy1")
	var move = e.get_move()
	# block
	if move == 6 :
		get_node("Body").set_got_blocked(true);
		return
	else :
		e.set_health(e.get_health() - 10)
	pass # Replace with function body.
