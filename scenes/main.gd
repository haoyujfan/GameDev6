extends Node3D
@export var ArenaScene: PackedScene
@export var PlayerScene: PackedScene


# Called when the node enters the scene tree for the first time.
func _ready():
	var arena = ArenaScene.instantiate()
	arena.position = Vector3(0,0,0);
	arena.add_to_group("arena1")
	$Arena.add_child(arena,true)
	arena = ArenaScene.instantiate()
	arena.position = Vector3(-300,0,0);
	arena.add_to_group("arena2")
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
	if(p.get_location().x < a.global_position.x-200) :
		var arena = ArenaScene.instantiate()
		arena.position = Vector3(a.global_position.x-600,a.global_position.y,a.global_position.z);
		arena.add_to_group("arena2")
		a2.remove_from_group("arena2")
		a2.add_to_group("arena1")
		$Arena.add_child(arena,true)
		a.queue_free();
		
	pass
