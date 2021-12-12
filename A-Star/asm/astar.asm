include Structures.inc

.data
GRID_SIZE		dd          ?
GRID_WIDTH		dd          ?
CURR_NODES		dd          0
CURR_NEIGHBOR	dd			0

newPos		Pos			<>
int64 dq ?
squareRoot dd ?

distances	dd			14, 10, 14, 10, 10, 14, 10, 14
positions	Pos <-1,1>,  <0,1>, <1,1>, <-1,0>, <1,0>, <-1,-1>, <0,-1>, <1,-1>


.code
DllEntry proc
    mov    rax, 1
    ret
DllEntry endp

init macro
    ; Assign width
    mov rax, [r9]
    mov [GRID_WIDTH], eax
    mov [GRID_SIZE], eax

    ; Calculate size of the grid
    mov rax, [r9 + 4]
    imul eax, [GRID_WIDTH]
    mov [GRID_SIZE], eax
endm

getLowestUnvisitedNode proc
	; Initialize counter
	mov ebx, [CURR_NODES] 
	; Set lowest cost node as nullptr
	mov r10, 0

compare:
	cmp ebx, 0
	je all_nodes_checked

	dec ebx

	;Get index of current node
	mov rax, rbx
	shl rax, 5
	lea r12, [rdx][rax].Node ; Pointer to node

	; Check if the Node was visited
	cmp [r12].Node.isVisited , 1 
	je compare

		; Check if lowest unvisited node is nullptr
		cmp r10, 0 
		jne not_nullptr
		jmp change_smallest

	not_nullptr:
		; Check if the current node has lower value than current lowest
		mov r11d, [r10].Node.fCost
		; Compare current node with lowest cost node
		mov esi, [r12].Node.fCost
		cmp [r12].Node.fCost, r11d 
		jl change_smallest
		jmp compare

	change_smallest:
		;Change lowest cost node to current node
		lea r10, [r12].Node
		mov esi, [r10].Node.fCost
		jmp compare

all_nodes_checked:
ret
getLowestUnvisitedNode endp

getNodeInto macro reg
	; Set counter
	mov eax, -1
	; Set node as nullptr
	mov r11, 0
	; Get both x and y positions of the new node
	mov edi, [rbx].Pos.x
	mov esi, [rbx].Pos.y

checkNextNode::
	cmp [CURR_NODES], eax
	je endSearch

	inc eax
	mov r14d, eax
	shl r14, 5

	cmp [rdx][r14].Node.position.x, edi
	jne checkNextNode
	cmp [rdx][r14].Node.position.y, esi
	jne checkNextNode

	; If the two positions are equal return node
	lea r11, [rdx][r14].Node
	je endSearch

endSearch::
endm

getEuclideanDistance macro posFrom, posTo
	xor rax, rax
	mov eax, [posFrom].Pos.x		; Current node's x
	sub eax, [posTo + 8]			; Current node's x - Destination x
	imul eax, eax					; Current node's x * Current node's x

	mov edi, [posFrom].Pos.y		; Current node's y
	sub edi, [posTo + 12]			; Current node's y - Destination y
	imul edi, edi					; Current node's y * Current node's y

	add eax, edi					; Current node's y + Current node's x

	mov int64, rax

	fild int64
	fsqrt							; sqrt(Current node's y + Current node's x)
	fistp squareRoot  

	mov eax, squareRoot
	imul eax, 10
endm

addNodeInto macro reg, currentNode, posFrom, posTo, distanceToAdd
	; Calculate the offset in the nodes array
	mov esi, CURR_NODES
	shl esi, 5
	;mov
	inc CURR_NODES
	
	; Calculate the distanceTillEnd
	getEuclideanDistance rbx, r8
	mov [rdx + rsi].Node.distanceTillEnd, eax

	; Calculate the new value of distanceSinceBeginning 
	mov edi, [r10].Node.distanceSinceBeginning
	add edi, r13d
	mov [rdx][rsi].Node.distanceSinceBeginning, edi

	inc CURR_NODES
	; Store the value of fCost 
	add eax, edi	; distanceTillEnd + distanceSinceBeginning
	mov [rdx][rsi].Node.fCost, eax

	mov eax, [rbx].Pos.x
	mov [rdx][rsi].Node.position.x, eax

	mov eax, [rbx].Pos.y
	mov [rdx][rsi].Node.position.y, eax

	mov [rdx][rsi].Node.isVisited, 0
	lea rax, [r10].Node
	mov qword ptr [rdx][rsi].Node.parent, rax

	; Mark the proper position in border array as Unvisited
	getIndexInto edi, [rbx].Pos
	mov eax, 3
	mov [rcx + rdi], eax

	; Get the ptr to newly added node
	lea r11, [rdx][rsi].Node
endm

getIndexInto macro reg, position
	local invalid_index
	local valid_index
	local end_macro

	; If x < 0
	cmp position.Pos.x, 0
	jl invalid_index
	; If y < 0
	cmp position.Pos.y, 0
	jl invalid_index

	;If everything is OK calculate index
	jmp valid_index

invalid_index::
	;Indicate index as invalid and end macro
	mov reg, -1
	jmp end_macro

valid_index::
	;Index calculation
	mov		reg, GRID_WIDTH
	imul	reg, position.Pos.y
	add		reg, position.Pos.x
	imul	reg, 4
end_macro::
endm

performBacktracing proc
	lea rdi, [r10].Node
	getIndexInto r13d, [rdi].Node.position

check_next_node:
	; Break when the current node's is equal its parent
	cmp rdi, qword ptr [rdi].Node.parent
	je finished_backtracking

	; Mark the proper position in border array as a path
	getIndexInto esi, [rdi].Node.position
	mov eax, 4
	mov [rcx + rsi], eax

	; As a current node set its parent
	mov rdi, [rdi].Node.parent
	jmp check_next_node

finished_backtracking:
	; Mark the source and destination points
	getIndexInto esi, [rdi].Node.position
	mov eax, 5
	mov [rcx + rsi], eax
	mov eax, 6
	mov [rcx + r13], eax
ret
performBacktracing endp

isDestinationNode macro position
	; Node is not a destination
	mov eax, 0 
	
	; Check if destination node is the same as current node
	mov rdi, [r8 + 8]			; Destination xPos
	cmp position.Pos.x, edi		; Current Node xPos
	jne not_destination

	mov rdi, [r8 + 12]			; Destination yPos
	cmp position.Pos.y, edi		; Current Node yPos
	jne not_destination

	mov eax, 1
not_destination::
endm

; A function that checks whether given node is valid
;
; @return true if row and column number is in range
isInBorder macro nodeIndex
	mov eax, 0 ; Node is not valid

	; Check if node index value is bigger than zero
	cmp nodeIndex, 0
	jb  not_valid


	; Check if node index value is less than grid size
	mov edi, [GRID_SIZE]
	shl edi, 2
	cmp nodeIndex, edi
	jae not_valid 

	mov eax, 1 ; Node is valid
not_valid::
endm

isNotObstacle macro nodeIndex
	mov eax, 0 ; Node is obstacle

	mov edi, 1
	cmp [rcx + nodeIndex], edi
	je is_not_obstacle

	mov eax, 1 ; Node is not an obstacle
is_not_obstacle::
endm

;void findPath(int* board, Node* nodes, Path path, BoardDimension boardDimension);
; board             -> RCX  (Grid)
; nodes             -> RDX  (Array of unvisited nodes)
; path              -> r8   (Structure holding start and end point)
; boardDimension    -> r9   (Width and height of the grid)
aStarSearch proc
    init

    ; Put the starting source node in the array of nodes
    mov rax, [r8].Pos
    mov [rdx][0].Node.position, rax
    mov [rdx][0].Node.distanceTillEnd, 0
    mov [rdx][0].Node.distanceSinceBeginning, 0
    mov [rdx][0].Node.fCost, 0
    mov [rdx][0].Node.isVisited, 0

	; Take your address and 
	lea rax, [rdx][0].Node
	mov qword ptr [rdx][0].Node.parent, rax
	mov rax, [rdx][0].Node.parent

	inc [CURR_NODES]

; Process till all nodes will be visited 
main_loop:
    mov eax, [GRID_SIZE]
    cmp [CURR_NODES], eax
    je finishAlgorithm

	; Find lowest unvisited cost
	call getLowestUnvisitedNode
	; Set the current node with the lowest F cost as visited
	mov [r10].Node.isVisited, 1

	; Mark the proper position in border array as visited
	getIndexInto edi, [r10].Node.position
	mov eax, 2
	mov [rcx + rdi], eax

	; Check if path was found
	isDestinationNode [r10].Node.position
	cmp eax, 0
	je destination_not_found
		; Backtrack until the parent node equals current node
		call performBacktracing
		jmp finishAlgorithm
	
destination_not_found:
	; Iterate until all eight neighbours are checked
	mov [CURR_NEIGHBOR], 0
check_next_neighbour:
	mov eax, [CURR_NEIGHBOR]
	cmp eax, 8
	jae main_loop

	lea r12, positions

	; Calculate new position
	lea rbx, newPos

	; New x
	mov edi, [r10].Node.position.x ; move currentNode's x position
	add edi, [r12 + rax * 8].Pos.x 
	mov [rbx].Pos.x, edi ; add to currentNode's x position a value from position array
	mov esi,[rbx].Pos.x

	; New y
	mov edi, [r10].Node.position.y ; move currentNode's x position
	add edi, [r12 + rax * 8].Pos.y
	mov [rbx].Pos.y, edi
	mov edi,[rbx].Pos.y

	; Store distance to add
	lea r15, distances
	mov r13d, [r15 + rax * 4] ;distance to add
	
	getIndexInto r14d, [rbx].Pos

	inc CURR_NEIGHBOR

	; Check if node is in border range
	isInBorder r14d
	cmp eax, 0
	je check_next_neighbour

	;Check if it is not an obstacle
	isNotObstacle r14
	cmp eax, 0
	je check_next_neighbour

		; If it is not an obstacle nor is visited, continue
		getNodeInto r11
		cmp r11, 0
		jne checkIfVisited
		; If node is a nullptr add it to the nodes array and check next
		addNodeInto r11, r10, rbx, r8, r13d
		jmp check_next_neighbour
	
	checkIfVisited:
		; If node is visited check next neighbor
		cmp [r11].Node.isVisited, 1
		je check_next_neighbour
		; Calculate new distance and compare it with an old one
		mov eax, [r10].Node.distanceSinceBeginning
		add eax, r13d	; distanceSinceBeginning of the current node + distanceToAdd
		cmp eax, [r11].Node.distanceTillEnd
		jae check_next_neighbour
			mov [r11].Node.distanceSinceBeginning, eax
			; Calculate new f Cost
			add eax, [r11].Node.distanceTillEnd
			mov [r11].Node.fCost, eax
			; Set the current node as a parent of node
			lea rax, [r10].Node
			mov qword ptr [r11].Node.parent, rax
			jmp check_next_neighbour

;Algorithm came to end
finishAlgorithm:
ret
aStarSearch endp
end