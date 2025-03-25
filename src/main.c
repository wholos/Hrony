/*Package Manager, 2000-2025 Holos*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

#define MAX_URL_LENGTH 1024

struct file {
    char name[128];
    char url[MAX_URL_LENGTH];
};

struct repository {
    char name[128];
    char url[MAX_URL_LENGTH];
    struct file files[10];
    int file_count;
};

struct repository repos[10];
int repo_count = 0;

void load_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error in opening repository config!");
        exit(1);
    }

    char buffer[4096];
    size_t len = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    struct json_object *parsed_json = json_tokener_parse(buffer);
    struct json_object *repos_json;
    json_object_object_get_ex(parsed_json, "repos", &repos_json);

    repo_count = json_object_array_length(repos_json);
    for (int i = 0; i < repo_count; i++) {
        struct json_object *repo_json = json_object_array_get_idx(repos_json, i);
        struct json_object *name_json = json_object_object_get(repo_json, "name");
        struct json_object *url_json = json_object_object_get(repo_json, "url");
        struct json_object *files_json = json_object_object_get(repo_json, "files");

        strcpy(repos[i].name, json_object_get_string(name_json));
        strcpy(repos[i].url, json_object_get_string(url_json));

        repos[i].file_count = json_object_array_length(files_json);
        for (int j = 0; j < repos[i].file_count; j++) {
            struct json_object *file_json = json_object_array_get_idx(files_json, j);
            struct json_object *file_name_json = json_object_object_get(file_json, "name");
            struct json_object *file_url_json = json_object_object_get(file_json, "url");

            strcpy(repos[i].files[j].name, json_object_get_string(file_name_json));
            strcpy(repos[i].files[j].url, json_object_get_string(file_url_json));
        }
    }

    json_object_put(parsed_json);
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

int download_package(const char *url, const char *output_filename) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        FILE *fp = fopen(output_filename, "wb");
        if (!fp) {
            perror("Error in opening file to write!");
            return -1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "Error in download: %s\n", curl_easy_strerror(res));
            fclose(fp);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return -1;
        }

        fclose(fp);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [PACKAGE]\n", argv[0]);
        return 1;
    }

    const char *package_name = argv[1];

    load_config("repo.json");

    int found = 0;
    for (int i = 0; i < repo_count; i++) {
        for (int j = 0; j < repos[i].file_count; j++) {
            if (strcmp(repos[i].files[j].name, package_name) == 0) {
                printf("Download file: %s\n", repos[i].files[j].name);
                if (download_package(repos[i].files[j].url, repos[i].files[j].name) != 0) {
                    return 1;
                }
                found = 1;
                break;
            }
        }
        if (found) break;
    }

    if (!found) {
        fprintf(stderr, "Package %s no found in repositories.\n", package_name);
        return 1;
    }

    printf("Package %s download.\n", package_name);
    return 0;
}
