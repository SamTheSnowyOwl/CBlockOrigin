#include <stdio.h>
#include "trie.h"
#include "parser.h"

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
    const char* test_url = "/ads/banner.js";
    RequestType type = REQ_SCRIPT;

    printf("\nEvaluating inbound request %s\n", test_url);
    if (search_url(buckets[type], test_url)) {
        printf("Result: [ BLOCKED ]\n");
    } else {
        printf("Result: [ ALLOWED ]\n");
    }

    for (int i = 0; i < NUM_BUCKETS; i++) {
        free_trie(buckets[i]);
    }
    return 0;
}