extends Node3D

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	get_node("Prison/Knight/AnimationPlayer").play("Sit_Floor_Pose")
	get_node("AnimationPlayer").play("start_scene")
	pass
