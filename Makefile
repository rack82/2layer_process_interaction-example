all:
	gcc -v ProcessA.c -o ProcessA.exe
	gcc -v ProcessB.c -o ProcessB.exe
	gcc -v ProcessC.c -o ProcessC.exe
	gcc DProcess.c
