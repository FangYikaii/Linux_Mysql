#include <stdio.h>


//Compile:gcc iterate.c -o iterate           (-o for output)

//Compile:gcc iterate.c -g -o iterate        (-o for output    -g  for add debug information)
//Disassembly: objdump -S -d ./iterate

int iterate(int value)
{
    if(value == 1)
    {
        return 1;
    }
    return iterate(value-1)+value;
}

int main()
{
    printf("%d\n",iterate(10));
    return 1;
}

