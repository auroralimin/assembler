um: equ 1
dois: equ 2
zero: equ 0
hex: equ 0xff

SECTION TEXT
IF zero
add aux
copy aux, n + 0XFA
add n + um
add n + hex

STOP
SECTION DATA
AUX: SPACE
N: SPACE dois
ONE: CONST -10
