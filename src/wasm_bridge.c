#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h> // Emscripten core header
#include "../include/trie.h"
#include "../include/parser.h"
#include "../include/url_parser.h"

static TrieNode* global_buckets[NUM_BUCKETS];

static void process_single_rule(char* line) {
    line[strcspn(line, "\r\n")] = '\0';
    
    if (line[0] == '\0' || line[0] == '!' || strstr(line, "##") != NULL || (line[0] == '@' && line[1] == '@')) {
        return;
    }

    char url_pattern[1024] = {0};
    uint32_t flags = FLAG_NONE;

    char* dollar_sign = strchr(line, '$');
    if (dollar_sign != NULL) {
        if (strstr(dollar_sign + 1, "third-party")) flags |= FLAG_THIRD_PARTY;
        if(strstr(dollar_sign + 1, "script")) flags |= FLAG_SCRIPT;
        if(strstr(dollar_sign + 1, "image")) flags |= FLAG_IMAGE;
        if(strstr(dollar_sign + 1, "xmlhttprequest")) flags |= FLAG_NONE;

        *dollar_sign = '\0';
    } 

    char* src = line;
    int dest_idx = 0;

    while (*src != '\0' && dest_idx < 1023) {
        if (*src == '|' && *(src + 1) != '\0' && *(src + 1) == '|') {
            src += 2;
            continue;
        }

        if (*src == '^' || *src == '*') {
            src++;
            continue;
        }

        url_pattern[dest_idx++] = *src;
        src++;
    }
    url_pattern[dest_idx] = '\0';

    if (dest_idx < 4) {
        return;
    }

    for (int i = 0; i < NUM_BUCKETS; i++) {
        if (!global_buckets[i]) return;
    }

    if (flags & FLAG_SCRIPT) {
        insert_rule(global_buckets[REQ_SCRIPT], url_pattern, flags);
    } else if (flags & FLAG_IMAGE) {
        insert_rule(global_buckets[REQ_IMAGE], url_pattern, flags);
    } else {
        insert_rule(global_buckets[REQ_OTHER], url_pattern, flags);
    }
}

EMSCRIPTEN_KEEPALIVE
bool init_adblocker(const char* raw_rules_data) {
    for (int i = 0; i < NUM_BUCKETS; i++) {
        global_buckets[i] = create_node();
    }

    if (!raw_rules_data || raw_rules_data[0] == '\0') {
        return true;
    }

    char* rules_copy = strdup(raw_rules_data);
    if (!rules_copy) return false;

    char* saveptr;
    char* line = strtok_r(rules_copy, "\n", &saveptr);

    while (line != NULL) {
        process_single_rule(line);
        line = strtok_r(NULL, "\n", &saveptr);
    }

    free(rules_copy);
    return true;
}

EMSCRIPTEN_KEEPALIVE
bool should_block_url(const char* raw_url, int request_type_bucket) {
    if (request_type_bucket < 0 || request_type_bucket >= NUM_BUCKETS) {
        return false;
    }

    if (!global_buckets[request_type_bucket]) {
        return false;
    }

    ParsedURL parsed;
    parse_raw_url(raw_url, &parsed);

    if (parsed.path_len == 0) return false;

    char clean_path_buffer[512] = {0};
    if (parsed.path_len < sizeof(clean_path_buffer)) {
        strncpy(clean_path_buffer, parsed.path_start, parsed.path_len);
    } else {
        return false;
    }

    return search_url(global_buckets[request_type_bucket], clean_path_buffer);
}

EMSCRIPTEN_KEEPALIVE
void teardown_adblocker(void) {
    for (int i = 0; i < NUM_BUCKETS; i++) {
        if (global_buckets[i]) {            
            free_trie(global_buckets[i]);
            global_buckets[i] = NULL;
        }
    }
}