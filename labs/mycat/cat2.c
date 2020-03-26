#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 5000000

/* filecopy:  copy file ifp to file ofp */
void filecopy(int ifp, FILE *ofp)
{
    char buffer[SIZE];
    ssize_t bytes;

    bytes = read(ifp, buffer, SIZE);

    for(int i = 0; i < bytes; i++){
        putc(buffer[i], ofp);
    }

}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
    int fp;
    void filecopy(int fp, FILE *);
    char *prog = argv[0];   /* program name for errors */

    if (argc == 1){  /* no args; copy standard input */
        fprintf(stderr, "no args passed \n");
    }
    else
        while (--argc > 0)
            if ((fp = open(*++argv, O_RDONLY)) == -1) {
                fprintf(stderr, "%s: can′t open %s\n",
			prog, *argv);
                return 1;
            } else {
                filecopy(fp, stdout);
                close(fp);
            }

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }

    return 0;
}
