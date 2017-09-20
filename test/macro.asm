
TRIANGULO: EQU 1
T2: EQU 0
SECTION DATA
	B:		SPACE
	H:		SPACE
	R:		SPACE
	DOIS:	CONST		2
SECTION TEXT
M2: MACRO
	OUTPUT B
	END
	ADD B
TROCA: MACRO
		INPUT		B
		INPUT		H
		END
		LOAD		B
		MULT		H
		TROCA
		IF TRIANGULO
		troca
		DIV		DOIS
		IF t2
		TROCA
		STORE		R
		OUTPUT	R
		TROCA
		troca
troca 
m2
TROCA
		troca
		troca
		troca
		troca
		troca
		troca
		troca
		troca
		STOP

ADD 
N2
