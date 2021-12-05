include Node.inc

.data
NODE_SIZE           equ		28

GRID_SIZE			dd		?
GRID_WIDTH			dq		?
UNVISITED_SIZE		dd		0

source				Pos		<>
destination			Pos		<>

distanceArr			dd		14, 10, 14, 7, 10, 14, 10, 14, 8
neigbourArray       Node	8    dup (<>)
fCostValue			dd		8    dup (-1)

.code

DllEntry proc
    mov    rax, 1
    ret
DllEntry endp

init macro
    xor r11, r11                ; Counter
    xor r12, r12                ; row_index
    xor r13, r13                ; col_index
    mov r14, rcx                ; Store row and col position of source and destination points

	;lea r14, source.Pos.row
	;mov [r14].Pos.row , ecx

	lea r14, source

	;Initialization of source and destination
	;=======================
	mov rax, [rcx]
	mov [r14].Pos.row , eax

	mov rax, [rcx + 4]
	mov [r14].Pos.col , eax

	lea r14, destination

	mov rax, [rcx + 8]
	mov [r14].Pos.row , eax

	mov rax, [rcx + 12]
	mov [r14].Pos.col , eax
	;=======================

    mov GRID_WIDTH, rdx
    imul rdx, rdx
    mov [GRID_SIZE], edx

    lea rbx, [neigbourArray]    ; Pointer to neigbourArray
endm

; Stores array's data element offset into REG.
store_dword_offset macro REG, ROW_IDX, COL_IDX
    xor  REG, REG
	mov  REG, ROW_IDX		    ; rdi = row
	imul REG, GRID_WIDTH	    ; rdi = width * row
	add  REG, COL_IDX		    ; rdi = width * row + col
    imul REG, NODE_SIZE
endm

traceFinalPathOnMap proc
ret
traceFinalPathOnMap endp

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
	cmp ROW, GRID_WIDTH
	jae not_valid 
	cmp COL, GRID_WIDTH
	jae not_valid

	mov eax, 1 ; Node is valid
not_valid::
endm

is_destination_node proc
	mov eax, 0 ; Node is not valid
	
	; Check if destination node is the same as current node
	lea r15, destination

	mov edx, [r15].Pos.row
	cmp r10d, edx
	jne not_equal

	mov edx, [r15].Pos.row
	cmp r11d, edx
	jne not_equal

	mov eax, 1
not_equal:
	ret
is_destination_node endp

is_visited proc 
	mov eax, 0 ; node is visited
	cmp [r8][rsi].Node.isVisited, 1
	je visited

	mov eax, 1 ;node is not visited
	visited:
	ret
is_visited endp

is_blocked proc
	mov eax, 0 ; Node is blocked
	cmp [r8][rsi].Node.nType, 6
	je blocked

	mov eax, 1 ; Node is unblocked
	blocked:
	ret
is_blocked endp

get_minimum proc
	xor rcx, rcx
	xor rsi, rsi
	xor rax, rax
	xor r10, r10
	xor r11, r11

	mov ecx, UNVISITED_SIZE				; Counter

	mov eax, [r9 + rcx * 4]
	imul eax, NODE_SIZE
	mov r10d, [r8][rax]					; The last element as the current smallest node
	mov r11d, [r8][rax].Node.nType		; The smallest value 

compare:
	dec ecx
	je exit

	mov eax, [r9 + rcx * 4]
	imul eax, NODE_SIZE

	cmp [r8][rax].Node.isVisited, 1		; Check if the Node was visited
	je compare							; If yes jump to next node

	cmp [r8][rax].Node.nType, r11d		; Value to compare
	jl change_smallest
	jmp compare
	
change_smallest:
	mov r10d, [r8][rax]
	mov r11d, [r8][rax].Node.nType 
	jmp compare

exit:
	ret
get_minimum endp

; On basis of parameter windows "C" Calling Convention I pass
; the first four pramaters as follows
; source        -> RCX		(Obj storing row (x) and col (y) coordinates)
; GRID_WIDTH    -> RDX		Width of one side of the input grid 
; grid          -> R8		(Array of nodes)
; openList      -> R9		(Array containning traversable nodes)

aStarSearch proc
	xor rax, rax
    init

    ; Calcute the index of the node in the array
	mov eax, source.Pos.row	; store row of source
	mov edx, source.Pos.row	; store col of source
    store_dword_offset rdi, rax, rdx

    ;Add to open list
    mov [r9], rdi	; r9 -> openList
	

    inc UNVISITED_SIZE
; While open list is not empty, perform the pathfinding
while_loop:
	;Take a minimum from open list
	call get_minimum

    cmp UNVISITED_SIZE, 0               ; The exit occurs when open list is empty
    je end_pathfinding

    sub UNVISITED_SIZE, 1               ; Decrement the size of the unvisited nodes list
    mov [r8][rdi].Node.isVisited, 1     ; Set the node as visited
	
    ; Generating all the 8 successor of this node
	;	{-1,  1} {0,  1} {1,  1}
	;	{-1,  0}         {1,  0}
	;	{-1, -1} {0, -1} {1, -1}

   ; ----------- {-1,  1} ------------
	mov r10d, [r8][rdi].Node.position.row 		; new row index
	sub r10, 1

	mov r11d, [r8][rdi].Node.position.col    	; new col index
	inc r11
	store_dword_offset rsi, r10, r11			; store new neighbour node offset in rsi

	; Move the Node to neighbour array  
    ;mov eax, [r8][rdi]
    ;mov [rbx], eax		; rbx -> pointer to neigbourArray					

	; Check if the current node is valid
	is_valid_node r10, r11 
	cmp eax, 0
	je next_node

		; If it is a destination, mark the current node symbol with 6 and end (EndingPoint)
		call is_destination_node 
		cmp eax, 0
		je check_if_visited
			;Backtrack to find a path from source -> destination
			call traceFinalPathOnMap


check_if_visited:
	;Check if the node is already visited or if it is untraversable
	call is_visited
	cmp eax, 0
	je next_node

	call is_blocked
	cmp eax, 0
	je next_node

	


    


next_node:
	; ----------- {0,  1} ------------

end_pathfinding:
	ret
aStarSearch endp
end