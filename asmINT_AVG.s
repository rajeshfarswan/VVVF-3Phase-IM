/*Function for averaging of values stored in a array, argument1 is pointer to
  start addres of array, argument2 is no. of array elements, argument3
  is power of 2 for no. of array elements */
/* power of 2 is used for easy division of sum of all samples by number of samples */
/* for example if number of elements 2 power of 2 is 1 */
/* for number of elements is 4, power of 2 is 2 */
/* for number of elements is 8 power of 2 is 3 */
/* for number of samples is 16 power of 2 is 4 */
 

#include "p30f6010A.inc"

.global _asmINT_AVG

_asmINT_AVG:
disi #0x3FFF

CLR A
MOV W1, W4     ;copy number of array elements 
MOV W2, W6     ;copy division factor for averaging in terms of base of 2
LOOP_1: 
MOV [W0++], W3 ;calculate sum of array elements
MOV W3, ACCB
ADD A
DEC W4, W5     ;check array element no is > 0 
MOV W5, W4
CP0 W4
BRA NZ,LOOP_1  ;if sum of all elements done exit

SFTAC A,W6     ;calculate average = sum of all elements/number of elements

MOV ACCA, W0   ;return average value of array elements

disi #0x0000
return 
.end

