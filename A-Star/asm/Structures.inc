; - - - - - - - - - - - - - - - - - - - - - - - - -
; Structure definitions
; - - - - - - - - - - - - - - - - - - - - - - - - -

; Structure that stores all available types taken into account 
; when outputting a graphic grid page
TypeNode struct
	Nothing			dd 0
	Obstacle		dd 1
	Visited			dd 2
	Unvisited		dd 3
	Path			dd 4
TypeNode ends

; A structure that stores the two necessary coordinates of each node
Pos struct
	x dd 0
	y dd 0
Pos ends

; A structure that stores the necessary parameters 
; of each cell in the grid.

Node struct
align 8
	position				Pos			<>	; Position in 1D array calculated in format: pos = GRID_SIZE * ROW + COL
	distanceTillEnd			dd			0
	distanceSinceBeginning	dd			0
	fCost					dd			0
	isVisited				db			0	; Boolean value to check if the cell is already visited
	parent					qword ptr	?
Node ends
