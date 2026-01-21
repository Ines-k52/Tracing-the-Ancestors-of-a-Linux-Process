#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    pid_t pid;

    if (argc > 1) {
        pid = atoi(argv[1]);
    } else {
        pid = getpid();
    }

    char path[256];
    char name[256];
    FILE *file;
    FILE *stat_file;
    int ppid;

    while (pid != 1 && pid != 0) {
        sprintf(path, "/proc/%d/comm", pid);
        file = fopen(path, "r");
        if (file == NULL) {
            perror("Could not open comm file");
            return 1;
        }

        if (fgets(name, sizeof(name), file) == NULL) {
            perror("Could not read process name");
            fclose(file);
            return 1;
        }

        name[strcspn(name, "\n")] = '\0';
        printf("%d %s\n", pid, name);
        fclose(file);

        sprintf(path, "/proc/%d/stat", pid);
        stat_file = fopen(path, "r");
        if (stat_file == NULL) {
            perror("Could not open stat file");
            return 1;
        }

        fscanf(stat_file, "%*d %*s %*c %d", &ppid);
        fclose(stat_file);

        pid = ppid;
    }
    return 0;
}
