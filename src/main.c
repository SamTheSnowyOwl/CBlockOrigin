#include <stdio.h>
#include "trie.h"

int main() {
    TrieNode* filter_rules_root = create_node();

    // Mock ad-delivery patterns
    insert_rule(filter_rules_root, "/doubleclick/ads/", 0);
    insert_rule(filter_rules_root, "/pagead/js/", 0);

    const char* user_request_1 = "https://example.com/doubleclick/ads/banner.png";
    const char* user_request_2 = "https://example.com/assets/images/logo.png";

    printf("Testing Request 1: %s\n", user_request_1);
    if (search_url(filter_rules_root, "/doubleclick/ads/")) {
        printf("Result: [ BLOCKED ]\n\n");
    } else {
        printf("Result: [ ALLOWED ]\n\n");
    }

    printf("Testing Request 2: %s\n", user_request_2);
    if (search_url(filter_rules_root, "/assets/images/logo.png")) {
        printf("Result: [ BLOCKED ]\n");
    } else {
        printf("Result: [ ALLOWED ]\n");
    }

    free_trie(filter_rules_root);
    return 0;
}