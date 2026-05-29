#include <stdio.h>
#include <string.h>
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
    const char* traffic_simulation[] = {
        "https://example.com/assets/plugins/doubleclick/ads.js", // Hidden middle rule
        "https://yale.edu/about-us/index.html",                 // Completely clean path
        "https://news-portal.com/pagead/tracking?pixel=1",       // Early path match with parameters
        "https://blog.com/sidebar/ads/banner.js"
    };
    int total_requests = 4;

    printf("\n--- Advanced Substring Traffic Scan ---\n");
    for (int i = 0; i < total_requests; i++) {
        ParsedURL parsed;
        parse_raw_url(traffic_simulation[i], &parsed);

        char clean_path_buffer[512] = {0};
        if (parsed.path_len < sizeof(clean_path_buffer)) {
            strncpy(clean_path_buffer, parsed.path_start, parsed.path_len);
        }

        printf("Evaluating: %-50s -> ", clean_path_buffer);
        if (search_url(buckets[REQ_SCRIPT], clean_path_buffer)) {
            printf("[ BLOCKED ]\n");
        } else {
            printf("[ ALLOWED ]\n");
        }
    }

    for (int i = 0; i < NUM_BUCKETS; i++) {
        free_trie(buckets[i]);
    }
    return 0;
}