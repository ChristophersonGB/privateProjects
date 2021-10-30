extends Area2D

onready var key_node = get_node("../Possessable/Key")
onready var key_collision = key_node.get_node("CollisionShape2D")

onready var unlocked_door_node = get_node("../UnlockedDoor")
onready var unlocked_door_collision = unlocked_door_node.get_node("CollisionShape2D")

# no need to get the lock node we are on it
onready var lock_collision = get_node("CollisionShape2D")

# we want a reference to the movement script so we can stop possession
onready var movement_script = get_node("../Possessable")


func _on_area_entered(area):
	if (area == key_node):
		# unlock door, make collision box collisionable
		unlocked_door_node.visible = true
		unlocked_door_collision.set_deferred("disabled", false)
		
		# hide key and disable key collisions
		key_node.visible = false
		key_collision.set_deferred("disabled", true)
		
		# hide lock, and for good measures, disable collisions
		self.visible = false
		lock_collision.set_deferred("disabled", true)
		
		# cancel possession and return to ghost
		movement_script.endPossession()
