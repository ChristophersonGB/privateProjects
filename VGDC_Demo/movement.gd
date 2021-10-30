extends Node2D

# Movement Speed
const speed = 250
const limit_for_ghost = 200

var _up
var _down
var _left
var _right

var input_x
var input_y

onready var ghost_node = get_node("../Ghost")

onready var possessionzone_node = get_node("../Ghost/PossessionZone")
onready var possessionzone_collision = get_node("../Ghost/PossessionZone/CollisionShape2D")

# Possessed stores the object we are moving, which starts out
# as the ghost.
onready var possessed = ghost_node
onready var ghost_back = get_node("../Ghost/ghost_back")
onready var ghost_front = get_node("../Ghost/ghost_front")
onready var ghost_right = get_node("../Ghost/ghost_right")
onready var ghost_left = get_node("../Ghost/ghost_left")

# get script that ectoplasm management is attached to
# we will reference this on possession to let the
# timer know to start ticking
onready var ectoplasm_manager = get_node("../UI/Timer/EctoplasmTimer")

# Stores if we are controlling the ghost or an object
var possessing = false

# vertical size of the screen
onready var _screen_size_y = get_viewport_rect().size.y
# horizontal size of the screen
onready var _screen_size_x = get_viewport_rect().size.x


# Called on startup
func _ready():
	set_sprite_direction(ghost_front)
  
	# Here we define the following variables to make
	# dealing with our action inputs easier.
	# Note: Action inputs can be found under 
	#       Project Settings > Input Map
	_up = "ghost_move_up"
	_down = "ghost_move_down"
	_left = "ghost_move_left"
	_right = "ghost_move_right"

func set_sprite_direction(direction_sprite):
	ghost_back.visible = false
	ghost_front.visible = false
	ghost_right.visible = false
	ghost_left.visible = false
	direction_sprite.visible = true

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var limit
	if possessed == ghost_node:
		limit=limit_for_ghost
	else:
		limit=30	# Move up and down based on input.
	
	# FIXME: Clamp is broken on new background
	input_y = Input.get_action_strength(_down) - Input.get_action_strength(_up)
	possessed.position.y =  possessed.position.y + input_y * speed * delta # clamp(possessed.position.y + input_y * speed * delta, limit , _screen_size_y) 
	input_x = Input.get_action_strength(_right) - Input.get_action_strength(_left)
	possessed.position.x =  possessed.position.x + input_x * speed * delta  # clamp(possessed.position.x + input_x * speed * delta, 0, _screen_size_x) 
	
	# only move ghost directino if not possessing
	if !possessing:
		if input_y < 0:
			set_sprite_direction(ghost_back)
		elif input_y > 0:
			set_sprite_direction(ghost_front)
		if input_x < 0:
			set_sprite_direction(ghost_left)
		elif input_x > 0:
			set_sprite_direction(ghost_right)
	else:
		if ectoplasm_manager.ectoplasm_time_left == 0:
			endPossession()
		
	if (Input.is_action_just_released("possession")):
		if (!possessing):
			togglePossessionZoneOn(true)
		else:
			endPossession()
	
	if (Input.is_action_just_released("restart_level")):
		get_tree().change_scene("res://" + get_tree().current_scene.name + ".tscn")

# called by PossessionZone signal of objects that collide
func possession_collision(area):
	# Check colliding object against children. 
	# Only children can be possessed
	if (get_children().has(area)):
		possess(area)
	else:
		togglePossessionZoneOn(false)
	

# Called on possession, with object to be possessed
func possess(object):
	# don't let em possess if they have no ectoplasm
	if (ectoplasm_manager.ectoplasm_time_left == 0):
		return
	possessed = object
	possessing = !possessing
	togglePossessionZoneOn(false)
	ectoplasm_manager.possession_started()

func endPossession():
	possessed = ghost_node
	possessing = !possessing
	ectoplasm_manager.possession_ended()
	
func togglePossessionZoneOn(enable):
	# FIXME: If animation for possession added, remove visiblilty code	
	if (!enable):
		possessionzone_node.visible = false
		possessionzone_collision.set_deferred("disabled", true)
	else:
		possessionzone_node.visible = true
		possessionzone_collision.set_deferred("disabled", false)
	
