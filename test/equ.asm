TRIANGULO: EQU 1
T2: equ 0
t3: equ 1
t4: equ 1
SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
SECTION TEXT
		if T2
		INPUT		B
		if t2
		INPUT		H
		if t3
		LOAD		B
		MULT		H
		IF TRIANGULO
		DIV		DOIS
		STORE		R
		OUTPUT	R
		STOP
