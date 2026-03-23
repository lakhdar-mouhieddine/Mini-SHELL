#include "jobs.h"
#include <stdio.h>
#include <string.h>

struct job_t jobs[MAXJOBS];

void init_jobs() {
    for (int i = 0; i < MAXJOBS; i++) {
        jobs[i].state = UNDEF;
        jobs[i].pid = 0;
        jobs[i].n_job = 0;
    }
}

int add_job(pid_t pid, job_state state, char *cmdline) {
    if (pid < 1) return 0;
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].state == UNDEF) {
            jobs[i].n_job = i + 1;
            jobs[i].pid = pid;
            jobs[i].state = state;
            strncpy(jobs[i].cmdline, cmdline, 1023);
            return 1;
        }
    }
    return 0;
}

int delete_job(pid_t pid) {
    if (pid < 1) return 0;
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid == pid) {
            jobs[i].state = UNDEF;
            jobs[i].pid = 0;
            jobs[i].n_job = 0;
            return 1;
        }
    }
    return 0;
}

int get_fg_job_index() {
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].state == FG) return i;
    }
    return -1;
}

pid_t get_pid_from_job_id(int n_job) {
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].n_job == n_job && jobs[i].state != UNDEF) return jobs[i].pid;
    }
    return -1;
}

void update_job_state(pid_t pid, job_state state) {
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].pid == pid) {
            jobs[i].state = state;
            return;
        }
    }
}

void list_jobs() {
    for (int i = 0; i < MAXJOBS; i++) {
        if (jobs[i].state != UNDEF) {
            char *s = (jobs[i].state == ST) ? "Stopped" : "Running";
            printf("[%d] %d %s %s\n", jobs[i].n_job, jobs[i].pid, s, jobs[i].cmdline);
        }
    }
}