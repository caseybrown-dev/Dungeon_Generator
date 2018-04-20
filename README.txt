Dungeon Generator by Casey Brown (caseyb@iastate.edu)

Generates, saves, and loads dungeons stored in HOME/.rlg229/dungeon. To save, add —save flag. to load, add —load flag. Also implements dijkstra’s algorithm.

Colors are enabled. A (red) is smart/psychic. B (Cyan) is smart. C (Green) is dumb/psychic. D (Yellow) is dumb. Player is ‘@‘ (Blue). Walls and floor are default shell color.

USAGE: 
	—save : saves file to ~/.rlg229/dungeon
	—load : loads file from ~/.rlg229/dungeon
	—generate (default) : generates new file

	—default (default) : prints file normally
	—dijkstra : prints with dijkstra algorithm (represented as ‘@‘ symbols)

	—summon [int] : adds specified number of monsters to the dungeon