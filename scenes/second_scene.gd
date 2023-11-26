extends Node3D

var time = 0;
var dying = false;

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	
	get_node("AnimationPlayer").play("second_scene")
	
	time += delta;
	if(time > 3):
		if(!dying):
			get_node("Barbarian/AnimationPlayer").play("Death_A");
			dying = true
		else:
			if(!get_node("Barbarian/AnimationPlayer").is_playing()):
				get_node("Barbarian/AnimationPlayer").play("Death_A_Pose");
		get_node("Rogue/AnimationPlayer").play("Cheer");
	else:
		get_node("Barbarian/AnimationPlayer").play("2H_Melee_Attack_Spin");
		get_node("Rogue/AnimationPlayer").play("2H_Melee_Attack_Stab");
		
	pass
