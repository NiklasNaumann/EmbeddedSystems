rmdir /S /Q obj
del program.hex
del program.bin
..\..\riscv64-gcc\bin\make.exe
pause