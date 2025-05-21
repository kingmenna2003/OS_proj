#include "kernel/types.h"
#include "user/user.h"
#include "kernel/uproc.h"

int main() {
    struct uproc table[64];

    if (getptable(64, (char *)table)) {
        for (int i = 0; i < 64; i++) {
            if (table[i].pid == 0) continue;
            printf("PID: %d PPID: %d STATE: %s NAME: %s SIZE: %lu\n",
                   table[i].pid, table[i].ppid, table[i].state, table[i].name, table[i].sz);
        }
    } else {
        printf("Failed to retrieve process table.\n");
    }

    exit(0);
}
