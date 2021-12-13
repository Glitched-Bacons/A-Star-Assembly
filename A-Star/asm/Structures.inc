; - - - - - - - - - - - - - - - - - - - - - - - - -
; Structure definitions
; - - - - - - - - - - - - - - - - - - - - - - - - -

; A structure that stores two coordinates of each node.
; These are used in the calculation of the indexes in the array.
Pos struct
	x dd 0
	y dd 0
Pos ends

; A structure that stores the necessary parameters 
; of each cell in the grid.
Node struct
	position                Pos         <>	; Position in 1D array calculated in format: pos = GRID_SIZE * ROW + COL
	distanceTillEnd         dd		    0
	distanceSinceBeginning  dd          0
	fCost                   dd          0
	isVisited				db			0	; Boolean value to check if the cell is already visited
	parent					qword ptr   ?		
Node ends
