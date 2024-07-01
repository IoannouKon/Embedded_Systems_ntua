// check_all_pseudoterminals.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void check_pseudoterminal(const char *device_path) {
    // Check if the path corresponds to a character device
    struct stat stat_info;
    if (stat(device_path, &stat_info) == -1) {
        perror("Error getting file information");
        return;
    }

    if (!S_ISCHR(stat_info.st_mode)) {
        printf("Path '%s' is not a character device.\n", device_path);
        return;
    }

    // Open the device for reading and writing
    int fd = open(device_path, O_RDWR);

    if (fd == -1) {
        perror("Error opening device");
        return;
    }

    // Check if it is a pseudoterminal
    if (isatty(fd)) {
        printf("Pseudoterminal found at path: %s\n", device_path);
    } else {
        printf("Device found at path '%s', but it is not a pseudoterminal.\n", device_path);
    }

    // Close the device
    close(fd);
}

int main() {
    DIR *dir;
    struct dirent *ent;
    const char *dev_pts_path = "/dev/pts/";

    if ((dir = opendir(dev_pts_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strncmp(ent->d_name, ".", 1) != 0 && strncmp(ent->d_name, "..", 2) != 0) {
                char device_path[256];
                snprintf(device_path, sizeof(device_path), "%s%s", dev_pts_path, ent->d_name);
                check_pseudoterminal(device_path);
            }
        }
        closedir(dir);
    } else {
        perror("Error opening /dev/pts/");
        exit(EXIT_FAILURE);
    }

    return 0;
}

