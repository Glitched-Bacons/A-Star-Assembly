include Node.inc

.data
NODE_SIZE			equ			32
GRID_SIDE_SIZE		equ			24				; Width of the grid
GRID_SIZE			equ			576				; Size of grid
DISTANCE			equ			10				; Distance from cell to cell in a regular grid
DIAGONAL_DISTANCE	equ			14				; Distance from cell to cell diagonally
open_list			NodeList	<>				; Distance from cell to cell diagonally
node_list			NodeList	<>				; Distance from cell to cell diagonally
neigbour_array		Node		8	dup (<>)

.code

DllEntry proc
	mov	rax, 1
	ret
DllEntry endp

init macro
	xor r11, r11				; counter
	xor r12, r12				; row_index
	xor r13, r13				; col_index

	mov r14, rcx				; store row and col position of source
	mov r15, rdx				; store row and col position of destination

	mov rcx, GRID_SIZE			; initialization of loop L1: counter
	mov rdx, offset open_list
	mov rbx, offset node_list	; alternative is lea rbx, [node_list]
endm

; A function that checks whether given node in 
; format(row, col) is a valid cell or not.
;
; @return true if row and column number is in range
is_valid_node macro ROW, COL
	mov eax, 0 ; Node is not valid

	; Check if row and column values are bigger than zero
	cmp ROW, 0
	jb  not_valid
	cmp COL, 0
	jb  not_valid

	; Check if row and column values are less than width of the grid
	cmp ROW, GRID_SIDE_SIZE
	jae not_valid 
	cmp COL, GRID_SIDE_SIZE
	jae not_valid

	mov eax, 1 ; Node is valid
not_valid::
endm

is_destination_node macro ROW, COL
	mov eax, 0 ; Node is not valid
	
	; Check if destination node is the same as current node
	cmp ROW, [r15 + 12]
	jne not_equal
	cmp COL, [r15 + 8]
	jne not_equal

	mov eax, 1
not_equal::
endm

are_input_points_valid macro SOURCE_REG, DESTINATION_REG
	is_valid_node r10, r11				; check if the source node is not out of range
	is_valid_node r12, r13				; check if the destination node is not out of range
endm

; Stores array's data element offset into REG.
store_dword_offset macro REG, ROW_IDX, COL_IDX
	XOR  REG, REG 
	mov  REG, ROW_IDX		; rdi = row
	imul REG, GRID_SIDE_SIZE		; rdi = width * row
	add  REG, COL_IDX		; rdi = width * row + col
	shl  REG, 2				; rdi = 4 * (width * row + col)
endm

createNodeList proc
L1:
	; Check if the row or column index needs to be reset
	cmp r13, GRID_SIDE_SIZE
	jb no_col_reset
	mov r13, 0
	add r12, 1
no_col_reset:

	cmp r12, GRID_SIDE_SIZE
	jna no_row_reset
	mov r12, 0
no_row_reset:
	
	; Update counter
	mov r11d, [rbx].NodeList.size_list
	shl r11d, 5		; size of the list * NODE_SIZE

	; Fill in the column and row in the structure
	mov [rbx][r11].NodeList.node_array.position.col , r13d
	mov [rbx][r11].NodeList.node_array.position.row , r12d

	; Element_Address = [rcx + (col_index * row_size + row_index) * Element_Size]
	store_dword_offset rdi, r12, r13
	mov eax, [r8 + rdi]
	mov [rbx][r11].NodeList.node_array.symbol, eax


	; Increment col index
	inc r13
	inc [rbx].NodeList.size_list

	loop L1
ret
createNodeList endp

traceFinalPathOnMap proc
ret
traceFinalPathOnMap endp

; On basis of parameter windows "C" Calling Convention I pass
; the first four pramaters as follows
; source		-> RCX (Obj storing row (x) and col (y) coordinates)
; destination	-> RDX (Obj storing row (x) and col (y) coordinates)
; grid			-> R8 (1D Array)
aStarSearch proc
	init
	call createNodeList

	; rdx -> open_list
	; rbx -> node_list

	; Adding first node (source) to open list
	mov eax, [r14 + 8]
	mov [rdx].NodeList.node_array.position.col, eax
	mov eax, [r14 + 12]
	mov [rdx].NodeList.node_array.position.row, eax
	inc [rdx].NodeList.size_list

; While open list is not empty, perform the pathfinding
while_loop:
	cmp [rdx].NodeList.size_list, 0						; The exit condition is when size_list is empty
	je end_pathfinding

	mov [rdx].NodeList.node_array.isVisited, 1			; Mark the node as visited
	sub [rdx].NodeList.size_list, 1						; decrement the size of the list

	; Generating all the 8 successor of this node
	;	{-1,  1} {0,  1} {1,  1}
	;	{-1,  0}         {1,  0}
	;	{-1, -1} {0, -1} {1, -1}

	mov r12d, [rdx].NodeList.node_array.position.row		; initial row_index
	mov r13d, [rdx].NodeList.node_array.position.col		; initial col_index

	; ----------- {-1,  1} ------------
	mov r9, r12		; new row index
	inc r9

	mov r10, r13	; new col index
	sub r10, 1
	
	; If the current node is valid, check if it is a destination
	is_valid_node r9, r10 
	cmp eax, 1
	je step2
	; If it is a destination, mark the current node symbol with 6 and end (EndingPoint)
		is_destination_node r9, r10 
		cmp eax, 1
		call traceFinalPathOnMap

		je step2

step2:

	;store_dword_offset macro REG, ROW_IDX, COL_IDX
	;XOR  REG, REG 
	;mov  REG, ROW_IDX		; rdi = row
	;imul REG, ROW_SIZE		; rdi = width * row
	;add  REG, COL_IDX		; rdi = width * row + col
	;shl  REG, 2			; rdi = 4 * (width * row + col)




end_pathfinding:
quit::
	ret 
aStarSearch endp
end