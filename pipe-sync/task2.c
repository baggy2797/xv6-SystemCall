#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define print_usage(cmd) printf("Usage: %s 1|2|3\n", cmd)

int main(int argc, char * argv[]) 
{
    int option = 0;

    setbuf(stdout, NULL);

    if (argc < 2)
    {
        print_usage(argv[0]);
        return 1;
    }

    option = atoi(argv[1]);
    if (option != 1 && option != 2 && option != 3)
    {
        print_usage(argv[0]);
        return 1;        
    }



    return 0;
}
