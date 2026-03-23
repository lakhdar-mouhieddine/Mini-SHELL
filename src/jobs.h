#ifndef __JOBS_H
#define __JOBS_H

#include <sys/types.h>
#include <unistd.h>

#define MAXJOBS 10

typedef enum { UNDEF, FG, BG, ST } job_state;

struct job_t {
    int n_job;
    pid_t pid;
    job_state state;
    char cmdline[1024]; 
};

extern struct job_t jobs[MAXJOBS];

void init_jobs();
int add_job(pid_t pid, job_state state, char *cmdline);
int delete_job(pid_t pid);
int get_fg_job_index();
pid_t get_pid_from_job_id(int n_job);
void list_jobs();
void update_job_state(pid_t pid, job_state state);

#endif