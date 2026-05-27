#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/parser.h"

static uint32_t parse_options(const char* options_str) {
    uint32_t flags = FLAG_NONE;
    if (!options_str) return flags;

    if (strstr(options_str, "third-party")) {
        flags |= FLAG_THIRD_PARTY;
    }
    if (strstr(options_str, "script")) {
        flags |= FLAG_SCRIPT;
    }
    if (strstr(options_str, "image")) {
        flags |= FLAG_IMAGE;
    }
    return flags;
}

bool parse_filter_file_to_buckets(TrieNode* buckets[], const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Error: Could not open filter file %s\n", filepath);
        return false;
    }

    char line[1024];
    int rules_loaded = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '\0' || line[0] == '!' || line[0] == '#' || (line[0] == '@' && line[1] == '@')) {
            continue;
        }

        char url_pattern[1024] = {0};
        uint32_t flags = FLAG_NONE;

        char* dollar_sign = strchr(line, '$');
        if (dollar_sign != NULL) {
            flags = parse_options(dollar_sign + 1);

            size_t pattern_length = dollar_sign - line;
            strncpy(url_pattern, line, pattern_length);
            url_pattern[pattern_length] = '\0';
        } else {
            strcpy(url_pattern, line);
        }

        if (url_pattern[0] != '\0') {
            if (flags & FLAG_SCRIPT) {
                insert_rule(buckets[REQ_SCRIPT], url_pattern, flags);
            } else if (flags & FLAG_IMAGE) {
                insert_rule(buckets[REQ_IMAGE], url_pattern, flags);
            } else {
                insert_rule(buckets[REQ_OTHER], url_pattern, flags);
            }            
            rules_loaded++;
        }
    }

    fclose(file);
    printf("Parser complete. Successfully compiled %d rules into memory buckets.\n", rules_loaded);
    return true;
}