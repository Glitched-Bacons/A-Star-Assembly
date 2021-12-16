include Structures.inc

.data
GRID_SIZE         dd          ?     ; Size of the board
GRID_WIDTH        dd          ?     ; Width of the board
CURR_NODES        dd          0     ; Counts number of neighboring nodes
CURR_NEIGHBOR     dd          0     ; Counts number of current nodes

newPos            Pos         <>    ; Temporary stores new position of the neighbouring node
squareRoot        dd          ?     ; Used as memory operand during sqrt calculations
ZERO              dd          0     ; Used as memory operand in cmov instrcutions

; Array conatining distances based on position of the node in a grid
distancesToAdd    dd          14, 10, 14, 10, 10, 14, 10, 14

; Array conatining x and y values ready to use during calculations 
; of neighboring nodes positions
positionsToAdd    Pos         <-1,1>,  <0,1>, <1,1>, <-1,0>, <1,0>, <-1,-1>, <0,-1>, <1,-1>

.code
DllEntry proc
    mov    rax, 1
    ret
DllEntry endp

; At the beginning of the algorithm call, 
; initialize the necessary variables
init macro
    mov CURR_NODES, 0

    ; Assign width
    mov rax, [r9]
    mov [GRID_WIDTH], eax
    mov [GRID_SIZE], eax

    ; Calculate size of the grid
    mov  rax, [r9 + 4]
    imul eax, [GRID_WIDTH]
    mov  [GRID_SIZE], eax

    ; Store array of distances
    lea r15, distancesToAdd
endm

; A function that search for the lowest f cost in an array of nodes.
; It does this by simply iterating the array of nodes
; comparing the cost of the selected current node
; with the node with the lowest f cost.
;
; @param reg Register in which the pointer to the node is stored
; @return The pointer to the node with the lowest f cost
getLowestUnvisitedNodeInto macro reg
    ; Initialize counter
    mov ebx, -1
    ; Set lowest cost node as nullptr
    mov reg, 0

compare:
    inc ebx
    cmp ebx, CURR_NODES
    je all_nodes_checked

    ;Get index of the current node
    mov rax, rbx
    shl rax, 5
    lea r12, [rdx][rax].Node

    ; Check if the Node was visited
    cmp [r12].Node.isVisited , 1 
    je compare

        ; Check if lowest unvisited node is nullptr
        cmp reg, 0 
        jne not_nullptr
        jmp change_smallest

    not_nullptr:
        ; Check if the current node has lower value than current lowest
        mov r11d, [reg].Node.fCost
        ; Compare current node with lowest cost node
        cmp [r12].Node.fCost, r11d 
        jl change_smallest
        jmp compare

    change_smallest:
        ;Change lowest cost node to current node
        lea reg, [r12].Node
        jmp compare

all_nodes_checked:
endm

; Saves the node into a given register. The node 
; is serached in nodes array based on its position.
; If the node has not yet been added to the 
; array then nullptr is returned.
;
; @param reg Register in which the pointer to the node is stored
; @return The pointer to the node if it exists, otherwise nullptr
getNodeInto macro reg
    ; Set counter
    mov eax, 0
    ; Set node as nullptr
    mov reg, 0
    ; Get both x and y positions of the new node
    mov edi, [rbx].Pos.x
    mov esi, [rbx].Pos.y

checkNextNode::
    cmp [CURR_NODES], eax
    je endSearch

    ; Calculate node's offset
    mov r14d, eax
    shl r14, 5

    inc eax

    cmp [rdx][r14].Node.position.x, edi
    jne checkNextNode
    cmp [rdx][r14].Node.position.y, esi
    jne checkNextNode

    ; If the two positions are equal return node
    lea reg, [rdx][r14].Node
endSearch::
endm

; Calculates Euclidean distance between two 
; points in our case current node's position
; and destination point.
;
; @param reg Register in which the distance is stored
; @param posFrom Current node's position
; @param posTo Destination node's position
getEuclideanDistanceInto macro reg, posFrom, posTo
    mov  reg, [rbx].Pos.x       ; xPosFrom
    sub  reg, [posTo + 8]       ; xPosFrom - xPosTo
    imul reg, reg               ; xPosFrom * xPosFrom

    mov  edi, [rbx].Pos.y       ; yPosFrom
    sub  edi, [posTo + 12]      ; yPosFrom - yPosTo
    imul edi, edi               ; yPosFrom * yPosFrom

    add  reg, edi               ; yPosFrom + xPosFrom

    mov   [squareRoot], reg
    fild  [squareRoot]
    fsqrt                       ; sqrt( yPosFrom + xPosFrom )
    fistp squareRoot  

    mov  reg, squareRoot
    imul reg, 10
endm

; Adds current node into the unvisited nodes array.
; Before that it performs distance calculations
; The distance between current and destination node is
; calculated using euclidean distance formula.
;
; @param reg Pointer to the newly added node
; @param currentNode Current node used in distance calculations
; @param posFrom Current node's position
; @param posTo Current node's position
; @param distanceToAdd Array conatining distances
addNodeInto macro reg, currentNode, posFrom, posTo, distanceToAdd
    ; Calculate the offset in the nodes array
    mov esi, CURR_NODES
    shl esi, 5
    
    ; Calculate the distanceTillEnd
    getEuclideanDistanceInto eax, posFrom, posTo

    mov [rdx][rsi].Node.distanceTillEnd, eax

    ; Calculate the new value of distanceSinceBeginning 
    mov edi, [currentNode].Node.distanceSinceBeginning
    add edi, distanceToAdd
    mov [rdx][rsi].Node.distanceSinceBeginning, edi

    ; Store the value of fCost 
    add eax, edi
    mov [rdx][rsi].Node.fCost, eax

    ; Set new positions
    mov eax, [posFrom].Pos.x
    mov [rdx][rsi].Node.position.x, eax
    mov eax, [posFrom].Pos.y
    mov [rdx][rsi].Node.position.y, eax

    mov [rdx][rsi].Node.isVisited, 0
    lea rax, [currentNode].Node
    mov qword ptr [rdx][rsi].Node.parent, rax

    ; Mark the proper position in border array as Unvisited
    getIndexInto edi, [posFrom].Pos
    mov eax, 3
    mov [rcx + rdi], eax

    ; Get the ptr to newly added node
    lea reg, [rdx][rsi].Node

    inc CURR_NODES
endm

; Calculates the index of the node based on 
; its position.
;
; @param position Current node's position
; @param reg Register in which the index is stored
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

    ; If everything is OK calculate index
    jmp valid_index

invalid_index::
    ; Indicate index as invalid and end macro
    mov reg, -1
    jmp end_macro

valid_index::
    ; Begin index calculation
    mov     reg, GRID_WIDTH
    imul    reg, position.Pos.x
    add     reg, position.Pos.y
    imul    reg, 4
end_macro::
endm

; Performs backtracking from the destination
; to source point. The tracing of the path
; is considered finished when the node's parent 
; points to itself.
performBacktracing macro
    lea rdi, [r10].Node
    ; Store destination index for 
    getIndexInto r13d, [rdi].Node.position

check_next_node:
    ; Break when the current node's is equal its parent
    cmp rdi, qword ptr [rdi].Node.parent
    je finished_backtracking

    ; Mark the proper position in border array as a path (4)
    getIndexInto esi, [rdi].Node.position
    mov eax, 4
    mov [rcx + rsi], eax

    ; As a current node set its parent
    mov rdi, [rdi].Node.parent
    jmp check_next_node

finished_backtracking:
    ; Mark in a board array source (5) and destination (6) points
    getIndexInto esi, [rdi].Node.position
    mov eax, 5
    mov [rcx + rsi], eax
    mov eax, 6
    mov [rcx + r13], eax
endm

; Function to check whether destination node 
; has been reached or not.
;
; @param Current node's position
; @return One in eax if node is a destination, otherwise zero
isDestinationNode macro position
    ; Node is a destination
    mov eax, 1
    
    ; Check if destination node is the same as current node
    mov rdi, [r8 + 8]            ; Destination xPos
    cmp position.Pos.x, edi      ; Cmp current node's xPos with destination's xPos
    cmovne eax, [ZERO]

    mov rdi, [r8 + 12]           ; Destination yPos
    cmp position.Pos.y, edi      ; Cmp current node's yPos with destination's yPos
    cmovne eax, [ZERO]
endm

; A macro that checks whether given node 
; index is a valid one.
;
; @param  Index of the node 
; @return One in eax if node index is in range, otherwise zero
isInBorder macro nodeIndex
    ; Set node as valid
    mov eax, 1 

    ; Check if node's index value is bigger than zero
    cmp nodeIndex, 0
    cmovb eax, [ZERO]

    ; Check if node index value is less than grid size
    mov edi, [GRID_SIZE]
    shl edi, 2
    cmp nodeIndex, edi
    cmovae eax, [ZERO]
endm

; Checks if a node is marked as an obstacle 
; at a given index in the grid.
;
; @param  Index of the node 
; @return Zero in eax if node is an obstacke, otherwise one
isNotObstacle macro nodeIndex
    ; Node is not an obstacle
    mov eax, 1 

    ; Check if there is an obstacle under the nodeIndex
    mov edi, 1
    cmp [rcx + nodeIndex], edi
    cmove eax, [ZERO]
endm

; Calculates the new x and y position of 
; each neighboring node
;
; @param reg Register in which the new position is saved
calculateNewPositionInto macro reg
    ; Set a pointer to postions array
    lea r12, positionsToAdd
    ; Store new x and y position
    lea reg, newPos

    ; Calculate new x
    mov edi, [r10].Node.position.x
    add edi, [r12 + rax * 8].Pos.x 
    mov [reg].Pos.x, edi

    ; Calculate new y
    mov edi, [r10].Node.position.y
    add edi, [r12 + rax * 8].Pos.y
    mov [reg].Pos.y, edi
endm

; void findPath(int* board, Node* nodes, Path path, BoardDimension boardDimension);
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

    ; Take address of the node and set it as parent
    lea rax, [rdx][0].Node
    mov qword ptr [rdx][0].Node.parent, rax
    mov rax, [rdx][0].Node.parent

    inc [CURR_NODES]

; Process until all nodes will be visited
main_loop:
    ; Check that the node count fits within the board size
    mov eax, [GRID_SIZE]
    cmp [CURR_NODES], eax
    je finishAlgorithm

    ; Find lowest unvisited node cost, 
    ; then return the pointer to that node in r10
    getLowestUnvisitedNodeInto r10
    ; Set the current node as visited (1)
    mov [r10].Node.isVisited, 1

    ; Mark the proper position in border array as visited
    getIndexInto edi, [r10].Node.position
    mov eax, 2
    mov [rcx + rdi], eax
    
    ; Check if path was found
    isDestinationNode [r10].Node.position
    cmp eax, 0
    je destination_not_found
        ; If path was found backtrack until the parent node equals itself
        performBacktracing
        jmp finishAlgorithm
    
destination_not_found:
    ; Iterate until all eight neighbours are checked
    mov [CURR_NEIGHBOR], 0
check_next_neighbour:
    mov eax, [CURR_NEIGHBOR]
    cmp eax, 8
    jae main_loop
    
    ; Perform calculations of new x and y positions of node
    calculateNewPositionInto rbx

    ; Calculate offset and store distance to add
    mov r13d, [r15 + rax * 4] 
    getIndexInto r14d, [rbx].Pos

    inc CURR_NEIGHBOR

    ; Check if node's position is in border range
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
        ; distanceSinceBeginning of the current node + distanceToAdd
        add eax, r13d    
        cmp eax, [r11].Node.distanceSinceBeginning
        jae check_next_neighbour
            mov [r11].Node.distanceSinceBeginning, eax
            ; Calculate new f Cost
            add eax, [r11].Node.distanceTillEnd
            mov [r11].Node.fCost, eax
            ; Set the current node as a parent of node
            lea rax, [r10].Node
            mov qword ptr [r11].Node.parent, rax
            jmp check_next_neighbour

finishAlgorithm:
ret
aStarSearch endp
end