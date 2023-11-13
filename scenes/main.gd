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
	arena = ArenaScene.instantiate()
	arena.position = Vector3(-300,0,0);
	arena.add_to_group("arena2")
	enemy = arena.get_node("Enemy");
	enemy.add_to_group("enemy2");
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
	if(e.get_location().x == p.get_location().x-10) :
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
