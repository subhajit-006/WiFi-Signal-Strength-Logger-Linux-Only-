#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void log_signal_strength(const char *interface, const char *logfile, int interval, int times) {
    FILE *fp;
    char command[128], buffer[256];

    snprintf(command, sizeof(command), "iwconfig %s", interface);

    for (int i = 0; i < times; i++) {
        fp = popen(command, "r");
        if (fp == NULL) {
            perror("Failed to run command");
            exit(1);
        }

        FILE *log = fopen(logfile, "a");
        if (log == NULL) {
            perror("Log file error");
            exit(1);
        }

        fprintf(log, "Log #%d:\n", i + 1);
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            if (strstr(buffer, "Signal level=")) {
                fprintf(log, "%s", buffer);
            }
        }
        fprintf(log, "\n");
        fclose(log);
        pclose(fp);

        sleep(interval);
    }
}

int main() {
    char interface[20] = "wlan0";  // Change if your WiFi device has a different name
    char logfile[50] = "wifi_log.txt";
    int interval = 5;  // in seconds
    int times = 5;

    log_signal_strength(interface, logfile, interval, times);
    printf("Logging complete. Check %s\n", logfile);
    return 0;
}
