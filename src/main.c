#include <stdio.h>
#include "trie.h"
#include "parser.h"
#include "url_parser.h"

int main() {
    TrieNode* buckets[NUM_BUCKETS];
    for (int i = 0; i < NUM_BUCKETS; i++) {
        buckets[i] = create_node();
    }

    // Parse rules from file
    printf("Loading filter engine lists into buckets...\n");
    if (!parse_filter_file_to_buckets(buckets, "rules.txt")) {
        for (int i = 0; i < NUM_BUCKETS; i++) free_trie(buckets[i]);        
        return 1;
    }

    // Matching logic
    const char* raw_browser_request = "https://subdomain.doubleclick.net:443/ads/banner.js?trackerid=999&user=sam";
    printf("\nIntercepting live raw string: %s\n", raw_browser_request);

    ParsedURL parsed;
    parse_raw_url(raw_browser_request, &parsed);

    printf("-> Extracted Host: %.*s\n", (int)parsed.host_len, parsed.host_start);
    printf("-> Extracted Path: %.*s\n", (int)parsed.path_len, parsed.path_start);

    char clean_path_buffer[512] = {0};
    if (parsed.path_len < sizeof(clean_path_buffer)) {
        strncpy(clean_path_buffer, parsed.path_start, parsed.path_len);
    }

    if (search_url(buckets[REQ_SCRIPT], clean_path_buffer)) {
        printf("Result: [ BLOCKED ] Request matched an active blocking signature.\n");
    } else {
        printf("Result: [ ALLOWED ] Request clean.\n");
    }

    for (int i = 0; i < NUM_BUCKETS; i++) {
        free_trie(buckets[i]);
    }
    return 0;
}