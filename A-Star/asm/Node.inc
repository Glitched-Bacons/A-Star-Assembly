; - - - - - - - - - - - - - - - - - - - - - - - - -
; Node structure definition
; - - - - - - - - - - - - - - - - - - - - - - - - -


; A structure that stores two necessary coordinates row and column
Pos struct
	row dd ?
	col dd ?
Pos ends

; A structure that stores the necessary parameters 
; of each cell in the grid.
Node struct
	position	Pos		<>		; Position in 1D array calculated in format: pos = GRID_SIZE * ROW + COL
	parentPos	Pos		<>		; Position of the parent node that offers the shortest path
	nType		dd		?		; Numerical symbol appearing in grid
	fCost		dd		?
	isVisited	db		?		; Boolean value to check if the cell is already visited
Node ends

;TraversableNode struct
;	posInNodeArr	dd	?
;	fValue			dd	?
;	isVisited		db	?		; Boolean value to check if the cell is already visited
;TraversableNode ends