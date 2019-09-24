/*division of two unsigned integers*/
/*Returns value in unsigned Q15*/

#include "p30f6010A.inc"

.global _asmINT_USDIV

_asmINT_USDIV:
disi #0x3FFF

MOV W0, W4  ;copy input argument 1
MOV W1, W5  ;copy input argument 2

REPEAT #17  ;div argument 1/argument2
DIV.U W4, W5

disi #0x0000
return 
.end


