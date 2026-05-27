#include <stdio.h>
#include "trie.h"
#include "parser.h"

int main() {
    TrieNode* filter_rules_root = create_node();

    // Parse rules from file
    printf("Loading filter engine lists...\n");
    if (!parse_filter_file(filter_rules_root, "rules.txt")) {
        free_trie(filter_rules_root);
        return 1;
    }

    // Matching logic
    const char* test_url = "/ads/banner.js";
    printf("\nEvaluating outboudn request %s\n", test_url);

    if (search_url(filter_rules_root, test_url)) {
        printf("Result: [ BLOCKED ]\n");
    } else {
        printf("Result: [ ALLOWED ]\n");
    }

    free_trie(filter_rules_root);
    return 0;
}