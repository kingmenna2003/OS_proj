#include "user.h"

int main() {
    struct datetime dt;
    memset(&dt, 0, sizeof(dt));

    if(datetime(&dt) < 0) {
        fprintf(2, "Error getting time\n");
        exit(1);
    }

    // Use basic format specifiers without padding
    fprintf(1, "%d-%d-%d %d:%d:%d\n",
           dt.year, dt.month, dt.day,
           dt.hour, dt.minute, dt.second);
    exit(0);
}
