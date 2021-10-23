;-------------------------------------------------------------------------

.data

.code

DllEntry PROC

mov	rax, 1 	;TRUE
ret

DllEntry ENDP

;-------------------------------------------------------------------------
; This is an example function. It's here to show
; where to put your own functions in the DLL
;-------------------------------------------------------------------------

MyProc1 PROC

xor	eax,eax
mov eax, 1
ret

MyProc1 endp

END