# Pokémon Dream Radar Pocket Save Editor
by suloku 2015

This is a very simple save editor for Pokémon Dream Radar, based on fylesystem.c and filesystem.h from meladroit's svdt, which is in turn based on ironhax installer by smea (aka sploit_installer).

This editor allows to modify current orbs (the that can be used to buy), total collected orbs (shown in the records screen).
Also allows to modify the "current orb count for reward", which is the value that stores how many orbs were gathered since the last reward was received:

Retrieval Extension:			Obtain a total of 200 Dream Orbs
Eureka Extension A (Tornadus):	Obtain a total of 400 Dream Orbs

Dowsing Extension:				Obtain a total of 700 Dream Orbs, catch Tornadus
Eureka Extension B (Thundurus):	Obtain a total of 1500 Dream Orbs, catch Tornadus

Eureka Extension G (Landorus):	Obtain a total of 3000 Dream Orbs, catch Thundurus

After the player catches Tornadus, Thundurs or Landorus, the "current orb count for reward" is reset to 0. This means that currently, to unlock all game content, one has to do the following:
1.- Set "current orb count for reward" to at least 400
2.- Enter the game, Retrieval and Eureka A extensions are received
3.- Catch Tornadus, "current orb count for reward" is reset to 0.

4.- Set "current orb count for reward" to at least 1500
5.- Enter the game, Dowsing and Eureka B extensions are received
6.- Catch Thundurus, "current orb count for reward" is reset to 0.

7.- Set "current orb count for reward" to at least 3000
8.- Enter the game, Eureka G extensions is received
9.- After catching Landorus, the credits will roll and all left game content will be unlocked (simulator A, B and G will be received and gen IV extensions, if unlocked, will be available).

It also allows to unlock the geneartion IV extensions, which can normally only be unlocked by inserting a gen IV DS cartridge in the 3DS console, and to re-catch those legendaries so they can be transfered to another Black2/White2 cartridge.

See more information about the game at: http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_Dream_Radar
