#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>e
#include <stdlib.h>

int read_from_pipe (int file)
{
    FILE *stream;
    int c;
    stream = fdopen (file, "r");
    fscanf(stream, "%d", &c);
    fclose (stream);
    return c;
}


void write_to_pipe (int file, int  counter)
{
    FILE *stream;
    stream = fdopen (file, "w");
    fprintf (stream, "%d", counter);
    fclose (stream);
}

int main (void)
{
    pid_t pid;
    int ABpipe[2];
    int BApipe[2];

    /* Create the pipe. */
    if (pipe (ABpipe))
    {
        fprintf (stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }
    if (pipe (BApipe)){
        fprintf (stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    /* Create B child process. */
    pid = fork ();
    if (pid == (pid_t) 0)
    {
        /* This is the child process.*/
        close (ABpipe[1]);
        int answer = read_from_pipe (ABpipe[0]);
        close (BApipe[0]);
        write_to_pipe(BApipe[1], answer + 1);
        return EXIT_SUCCESS;
    }
    else if (pid < (pid_t) 0)
    {
        /* The fork failed. */
        fprintf (stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }
    else
    {
        /* This is the parent process.*/
        close (ABpipe[0]);
        write_to_pipe (ABpipe[1], 1);

        close (BApipe[1]);
        int BChildrenPlusA = read_from_pipe(BApipe[0]);
        int ACpipe[2];
        int CApipe[2];

        if (pipe (ACpipe)){
            fprintf (stderr, "Pipe failed.\n");
            return EXIT_FAILURE;
        }
        if (pipe (CApipe)){
            fprintf (stderr, "Pipe failed.\n");
            return EXIT_FAILURE;
        }

        /* Create C child process. */
        pid = fork ();
        if (pid == (pid_t) 0)
        {
            /* This is the child process.*/
            close (ACpipe[1]);
            int answer = read_from_pipe (ACpipe[0]);
            close (CApipe[0]);
            write_to_pipe(CApipe[1], answer + 1);
            return EXIT_SUCCESS;
        }
        else if (pid < (pid_t) 0)
        {
            /* The fork failed. */
            fprintf (stderr, "Fork failed.\n");
            return EXIT_FAILURE;
        }
        else {
            close(ACpipe[0]);
            write_to_pipe(ACpipe[1], BChildrenPlusA);

            close(CApipe[1]);
            int CChildrenPlusAB = read_from_pipe(CApipe[0]);

            int CDpipe[2];
            int DCpipe[2];

            if (pipe (DCpipe)){
                fprintf (stderr, "Pipe failed.\n");
                return EXIT_FAILURE;
            }
            if (pipe (CDpipe)){
                fprintf (stderr, "Pipe failed.\n");
                return EXIT_FAILURE;
            }

            /* Create D child process. */
            pid = fork ();
            if (pid == (pid_t) 0)
            {
                /* This is the child process.*/
                close (CDpipe[1]);
                int answer = read_from_pipe (CDpipe[0]);
                close (DCpipe[0]);
                write_to_pipe(DCpipe[1], answer + 1);
                return EXIT_SUCCESS;
            }
            else if (pid < (pid_t) 0)
            {
                /* The fork failed. */
                fprintf (stderr, "Fork failed.\n");
                return EXIT_FAILURE;
            }
            else {
                close(CDpipe[0]);
                write_to_pipe(CDpipe[1], CChildrenPlusAB);

                close(DCpipe[1]);
                int DChildrenPlusABC = read_from_pipe(DCpipe[0]);

                int CEpipe[2];
                int ECpipe[2];

                if (pipe (ECpipe)){
                    fprintf (stderr, "Pipe failed.\n");
                    return EXIT_FAILURE;
                }
                if (pipe (CEpipe)){
                    fprintf (stderr, "Pipe failed.\n");
                    return EXIT_FAILURE;
                }

                /* Create E child process. */
                pid = fork ();
                if (pid == (pid_t) 0)
                {
                    /* This is the child process.*/
                    close (CEpipe[1]);
                    int answer = read_from_pipe (CEpipe[0]);
                    close (ECpipe[0]);
                    write_to_pipe(ECpipe[1], answer + 1);
                    return EXIT_SUCCESS;
                }
                else if (pid < (pid_t) 0)
                {
                    /* The fork failed. */
                    fprintf (stderr, "Fork failed.\n");
                    return EXIT_FAILURE;
                }
                else {
                    close(CEpipe[0]);
                    write_to_pipe(CEpipe[1], DChildrenPlusABC);

                    close(ECpipe[1]);
                    int EChildrenPlusABCD = read_from_pipe(ECpipe[0]);
                    printf("%d", EChildrenPlusABCD);
                }
                return EXIT_SUCCESS;
            }
            return EXIT_SUCCESS;
        }
    }
}