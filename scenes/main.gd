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
	arena = ArenaScene.instantiate()
	arena.position = Vector3(-300,0,0);
	arena.add_to_group("arena2")
	enemy = arena.get_node("Enemy");
	enemy.add_to_group("enemy2");
	enemy.connect("enemy_chop", enemy_chop);
	enemy.connect("enemy_slice", enemy_slice);
	enemy.connect("enemy_stab", enemy_stab);
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
	
	if(p.get_running() && p.get_location().x < a.global_position.x+10 && p.get_location().x > a.global_position.x+5) :
		p.set_running(false);
		e.set_approaching(true);
		print("approaching")
		return;
	if(e.get_approaching() && e.get_location().x > p.get_location().x-10) :
		e.set_approaching(false);
		e.set_fighting(true);
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
		$Arena.add_child(arena,true)
		a.queue_free();
		
	pass


<<<<<<< Updated upstream
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
	
func update_health(health):
	get_node("GUI/GridContainer/HBoxContainer/Health").set_value(health)
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
		p.set_health(p.get_health() - 5)
		update_health(p.get_health())
	pass


func _on_body_blocking(shield):
	get_node("GUI/GridContainer/HBoxContainer/Shield").set_value(shield)
=======
func _on_body_blocking(shield):
>>>>>>> Stashed changes
	pass # Replace with function body.
