#ifndef UPROC_H
#define UPROC_H

#define NAMELEN 16
#define STATE_LEN 16

struct uproc
{
    int pid;
    int ppid;
    char state[STATE_LEN];  // e.g., "RUNNING", "SLEEPING"
    char name[NAMELEN];
    uint64 sz;
};

#endif // UPROC_H
