extends Area2D


onready var ghost_node = get_node("../Ghost")



# called when ghost leaves level
func level_exit():
	get_tree().change_scene("res://" + str(int(get_tree().current_scene.name) + 1) + ".tscn")

# called on any collision
func onCollision(area):
	# only call level exit if ghost is colliding
	if area == ghost_node:
		level_exit()
	
