extends Light2D

# local y coordinates for mask on empty and full
var drained_ectoplasm_y = 385
var full_ectoplasm_y = 280

# checked by possesion to see if possession is still possible
var ectoplasm_time_left = 105

var possessing = false

onready var timer = get_node("../")

# ectoplasm_warning is a sprite telling the player to restart
onready var ectoplasm_warning = get_node("../../out_of_plasm_warning")

# upon start up, a timer waits one second then calls _on_Timer_timeout
func possession_started():
	possessing = true
	timer.start(1)

func possession_ended():
	possessing = false

# increases the y value of the mask and waits another second (loop)
func _on_Timer_timeout():
	if !possessing:
		return
	if position.y < drained_ectoplasm_y:
		position.y = position.y + 1
		# FIXME: we might want to look at this in the refactor
		#        to consider what we need to do for ectoplasm
		#        like maybe using the timer so we can tweak it
		#        instead of focusing on the screen position
		timer.start(0.1)
	# display retry message if ectoplasm is empty
	if position.y >= drained_ectoplasm_y:
		ectoplasm_warning.visible = true
		ectoplasm_time_left = 0
