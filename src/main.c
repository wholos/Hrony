// *Nikita Shyshatskyi (Holos)* //
// *RePKG* //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#define REPO_URL ""
#define REPO_PATH ""
#define INSTALL_DIR "/usr/bin/"

// HTTP client
void download_file(const char *host, const char *path, const char *output_file) {
    int sock;
    struct sockaddr_in server_addr;
    struct hostent *he;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error!");
        exit(1);
    }

    if ((he = gethostbyname(host)) == NULL) {
        perror("Error!");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    memcpy(&server_addr.sin_addr, he->h_addr_list[0], he->h_length);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error!");
        exit(1);
    }

    char request[512];
    snprintf(request, sizeof(request),
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n\r\n",
        path, host);

    if (send(sock, request, strlen(request), 0) < 0) {
        perror("Error!");
        close(sock);
        exit(1);
    }

    FILE *output = fopen(output_file, "wb");
    if (!output) {
        perror("Error!");
        close(sock);
        exit(1);
    }

    char buffer[1024];
    int headers_ended = 0;
    ssize_t bytes_read;

    while ((bytes_read = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        if (!headers_ended) {
            char *header_end = strstr(buffer, "\r\n\r\n");
            if (header_end) {
                headers_ended = 1;
                fwrite(header_end + 4, 1, bytes_read - (header_end - buffer) - 4, output);
            }
        } else {
            fwrite(buffer, 1, bytes_read, output);
        }
    }

    fclose(output);
    close(sock);
}

// Install packages
void install_pkg(const char *pkg_name) {
    printf("Install %s...\n", pkg_name);

    char url_path[256];
    snprintf(url_path, sizeof(url_path), "%s%s", REPO_PATH, pkg_name);

    char tmp_path[256];
    snprintf(tmp_path, sizeof(tmp_path), "/tmp/%s", pkg_name);

    download_file(REPO_URL, url_path, tmp_path);

    chmod(tmp_path, 0755);

    char install_path[256];
    snprintf(install_path, sizeof(install_path), "%s%s", INSTALL_DIR, pkg_name);

    char move_cmd[512];
    snprintf(move_cmd, sizeof(move_cmd), "mv %s %s", tmp_path, install_path);
    system(move_cmd);

    printf("%s installed %s\n", pkg_name, INSTALL_DIR);
}

// Remove packages
void remove_pkg(const char *pkg_name) {
    printf("Remove %s...\n", pkg_name);

    char path[256];
    snprintf(path, sizeof(path), "%s%s", INSTALL_DIR, pkg_name);

    if (access(path, F_OK) == 0) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "rm -f %s", path);
        system(cmd);
        printf("%s removed.\n", pkg_name);
    } else {
        printf("Error: %s not installed.\n", pkg_name);
    }
}

// Update repos
void update_repo() {
    printf("Update repos...\n");
    printf("Repos updated.\n");
}

// Help
void print_help() {
    printf("Use:\n");
    printf("  repkg -install <PKG>  - Install package\n");
    printf("  repkg -remove <PKG>   - Remove package\n");
    printf("  repkg -update         - Update repos\n");
}

// Main function
int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'install': // Install flag
			        install_pkg(argv[2]); return 0;
                case 'remove': // Remove flag
			        remove_pkg(argv[2]); return 0;
		        case 'update': // Update flag
		            update_repo(); return 0;
		        case 'help': // Help Flag
			        print_help(); return 0;
		        default:  printf("No flag: '%s'\n", argv[i]); return 0;
            }
        }
    }
    printf("Use repkg -help.");
    return 0;
}
