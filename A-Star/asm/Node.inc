; - - - - - - - - - - - - - - - - - - - - - - - - -
; Node structure definition
; - - - - - - - - - - - - - - - - - - - - - - - - -


; A structure that stores two necessary coordinates row and column
Pos struct
	row dd 0
	col dd 0
Pos ends

; A structure that stores the necessary parameters 
; of each cell in the grid.
Node struct
	position	Pos		<0, 0>		; Position in 1D array calculated in format: pos = GRID_SIZE * ROW + COL
	isVisited	db		0			; Boolean value to check if the cell is already visited
	fCost		dd		0			
	parentPos	Pos		<0, 0>		; Position of the parent node that offers the shortest path
	symbol		dd		0			; Numerical symbol appearing in grid

	padd1		dd		0
	padd2		dw		0
	padd3		db		0
Node ends

NodeList struct
	node_array	Node	576	dup	(< <0,0>, 0, 0, <0,0>, 0>)
	size_list	dd		0	
NodeList ends