# MPLAB IDE generated this makefile for use with GNU make.
# Project: sine Pwm.mcp
# Date: Mon Nov 15 15:44:47 2010

AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = rm

sine\ Pwm.hex : sine\ Pwm.cof
	$(HX) "sine Pwm.cof"

sine\ Pwm.cof : init.o main.o DataEEPROM.o delay.o adc.o
	$(CC) -mcpu=30F6010A "init.o" "main.o" "DataEEPROM.o" "delay.o" "adc.o" -o"sine Pwm.cof" -Wl,-L"C:\Program Files\Microchip\MPLAB C30\lib",--script="p30f6010A.gld",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=__ICD2RAM=1,-Map="sine Pwm.map",--report-mem

init.o : p30f6010A.h init.c
	$(CC) -mcpu=30F6010A -x c -c "init.c" -o"init.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

main.o : adc.h delay.h math.h p30f6010A.h main.h init.h DataEEPROM.h p30f6010A.h main.c
	$(CC) -mcpu=30F6010A -x c -c "main.c" -o"main.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

DataEEPROM.o : p30f6010A.inc DataEEPROM.s
	$(CC) -mcpu=30F6010A -c -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 "DataEEPROM.s" -o"DataEEPROM.o" -Wa,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g

delay.o : p30f6010A.h delay.c
	$(CC) -mcpu=30F6010A -x c -c "delay.c" -o"delay.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

adc.o : p30f6010A.h main.h p30f6010A.h adc.c
	$(CC) -mcpu=30F6010A -x c -c "adc.c" -o"adc.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

clean : 
	$(RM) "init.o" "main.o" "DataEEPROM.o" "delay.o" "adc.o" "sine Pwm.cof" "sine Pwm.hex"

