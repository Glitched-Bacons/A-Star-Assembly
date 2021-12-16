; - - - - - - - - - - - - - - - - - - - - - - - - -
; Structure definitions
; - - - - - - - - - - - - - - - - - - - - - - - - -

; A structure that stores two coordinates x and y of each node.
; These are used in the calculation of the indexes in the array.
Pos struct
	x dd 0 
	y dd 0
Pos ends

; A structure that stores the necessary parameters 
; of each cell in the grid.
Node struct
	position                Pos         <>	; Position of the node used in calculations of index
	distanceTillEnd         dd          0	; The distance calculated using heuristic function
	distanceSinceBeginning  dd          0	; The distance to travel from the starting point to this node
	fCost                   dd          0	; distanceSinceBeginning + distanceTillEnd
	isVisited               db          0	; Boolean value to check if the cell is already visited
	parent                  qword ptr   ?   ; Pointer to the parent of the current node
Node ends
