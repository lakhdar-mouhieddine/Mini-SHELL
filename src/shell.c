#include <stdio.h>
#include <stdlib.h>
#include <wordexp.h>
#include "readcmd.h"
#include "jobs.h"
#include "csapp.h"

void handler_sigchld(int sig)
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
    {
        if (WIFSTOPPED(status))
        {
            update_job_state(pid, ST);
        }
        else
        {
            delete_job(pid);
        }
    }
}

void handler_sigint_tstp(int sig)
{
    int idx = get_fg_job_index();
    if (idx != -1)
    {
        kill(-jobs[idx].pid, sig);
    }
}

void do_bgfg(char **argv)
{
    if (argv[1] == NULL)
        return;
    pid_t pid;
    if (argv[1][0] == '%')
    {
        pid = get_pid_from_job_id(atoi(&argv[1][1]));
    }
    else
    {
        pid = atoi(argv[1]);
    }

    if (pid <= 0)
        return;

    kill(-pid, SIGCONT);
    if (strcmp(argv[0], "fg") == 0)
    {
        update_job_state(pid, FG);
        while (get_fg_job_index() != -1)
            sleep(1);
    }
    else
    {
        update_job_state(pid, BG);
    }
}

int main()
{
    init_jobs();
    Signal(SIGCHLD, handler_sigchld);
    Signal(SIGINT, handler_sigint_tstp);
    Signal(SIGTSTP, handler_sigint_tstp);

    while (1)
    {
        struct cmdline *l;
        int fd_in = 0;
        sigset_t mask, prev;

        printf("shell> ");
        l = readcmd();
        if (!l || !l->seq[0])
            continue;

        if (strcmp(l->seq[0][0], "exit") == 0)
        {
            Kill(getpid(), SIGCHLD);
            exit(0);
        }
       if (strcmp(l->seq[0][0], "quit") == 0)
{
    for (int i = 0; i < MAXJOBS; i++)
    {
        pid_t pid = jobs[i].pid;         
        if (pid > 1)                      
        {
            kill(-pid, SIGKILL);          
            delete_job(pid);
        }
    }
    exit(0);
}

        if (strcmp(l->seq[0][0], "jobs") == 0)
        {
            list_jobs();
            continue;
        }
        if (strcmp(l->seq[0][0], "fg") == 0 || strcmp(l->seq[0][0], "bg") == 0)
        {
            do_bgfg(l->seq[0]);
            continue;
        }

        sigemptyset(&mask);
        sigaddset(&mask, SIGCHLD);

        for (int i = 0; l->seq[i] != NULL; i++)
        {
            int p[2];
            if (l->seq[i + 1] != NULL)
                pipe(p);

            sigprocmask(SIG_BLOCK, &mask, &prev);

            pid_t pid = fork();
            if (pid == 0)
            {
                sigprocmask(SIG_SETMASK, &prev, NULL);
                setpgid(0, 0);

                if (i == 0 && l->in)
                {
                    int fd = Open(l->in, O_RDONLY, 0);
                    dup2(fd, 0);
                    close(fd);
                }
                else if (fd_in != 0)
                {
                    dup2(fd_in, 0);
                    close(fd_in);
                }

                if (l->seq[i + 1])
                {
                    close(p[0]);
                    dup2(p[1], 1);
                    close(p[1]);
                }
                else if (l->out)
                {
                    int fd = Open(l->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    dup2(fd, 1);
                    close(fd);
                }

                wordexp_t we;
                if (wordexp(l->seq[i][0], &we, 0) == 0) // dans le code soumis on a pas regler ce probléme mais j'ai mieux adapter le code
                {
                    execvp(we.we_wordv[0], l->seq[i]);
                    wordfree(&we);
                }
                else
                {
                    execvp(l->seq[i][0], l->seq[i]);
                }
                exit(1);
            }

            if (fd_in != 0)
                close(fd_in);
            if (l->seq[i + 1])
            {
                close(p[1]);
                fd_in = p[0];
            }

            if (l->background)
            {
                add_job(pid, BG, l->seq[i][0]);
                sigprocmask(SIG_SETMASK, &prev, NULL);
            }
            else
            {
                add_job(pid, FG, l->seq[i][0]);
                sigprocmask(SIG_SETMASK, &prev, NULL);
            }
        }

        if (!l->background)
        {
            while (get_fg_job_index() != -1)
                sleep(1);
        }
    }
    return 0;
}