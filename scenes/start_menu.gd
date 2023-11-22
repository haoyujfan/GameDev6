extends CanvasLayer


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	get_node("AnimationPlayer").play("CameraAnimation")
	pass


func _on_start_button_button_down():
	get_tree().change_scene_to_file("res://main.tscn");
	pass # Replace with function body.


func _on_exit_button_button_down():
	get_tree().quit();
	pass # Replace with function body.
