# TrollArea
Purely meant as a joke.

## Behaviors

Obj_arg0 = 0: Crashes with an SRR0 of DEADBEEC

Obj_arg0 = 1: Shuts down your Wii

Obj_arg0 = 2: Exits to the Home Menu

Obj_arg0 = 3: Very strongly rumbles the Wiimote (because yes)

Obj_arg0 = 4: Plays random sound effects every frame

Obj_arg0 = 5: Stop rendering the game (Blue Screen of Death)

Obj_arg0 = 6: Teleport player to a random position
    --> Obj_arg1: Sets maximum distance (default 10000.0f)

Obj_arg0 = 7: Teleport a random object to the player's position

Obj_arg0 = 8: Change screen width
    --> Obj_arg1 (required): The width to set it to. *Normal 16:9 width is 812, normal 4:3 width is 608*.

Obj_arg0 = 9: Change framerate
    --> Obj_arg1: Framerate to set the game to. By default, a random framerate will be picked.

Obj_arg0 = 10: Display Bitcoin ransom message
    --> Obj_arg1: The red value of the background color (default 0).
    --> Obj_arg2: The green value of the background color (default 0).
    --> Obj_arg3: The blue value of the background color (default 0).
    --> Obj_arg4: The red value of the text color (default 255).
    --> Obj_arg5: The green value of the text color (default 0).
    --> Obj_arg6: The blue value of the text color (default 0).

Obj_arg0 = 11: Fake Game Over

Obj_arg0 = 12: Constant Battery warning

Obj_arg0 = 13: Swap sin(x) and cos(x)

Obj_arg0 = 14: Delete Save file
    --> Obj_arg1: What save file to delete (number from 1 to 3). By default, it will delete *all* save files.

Obj_arg0 = 15: Set amount of Damage the player takes
    --> Obj_arg1 (required): Amount of the damage the player takes.

Obj_arg0 = 16: Eject Game Disc

Obj_arg0 = 17: Set player scale
    --> Obj_arg1: The x value of the new player's scale. By default, a random number will be picked.
    --> Obj_arg2: The y value of the new player's scale. By default, a random number will be picked.
    --> Obj_arg3: The z value of the new player's scale. By default, a random number will be picked.

**Additionally, for every behavior, you can set obj_arg7 to -1 to execute it every frame the player is in the TrollArea. If you set it to 0, the behavior will only be executed once.**