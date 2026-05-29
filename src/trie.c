#include <stdio.h>
#include <stdlib.h>
#include "../include/trie.h"

TrieNode* create_node(void) {
    TrieNode* new_node = (TrieNode*)malloc(sizeof(TrieNode));
    if (new_node) {
        new_node->is_end_of_rule = false;
        new_node->context_flags = 0;
        for(int i = 0; i < ALPHABET_SIZE; i++) {
            new_node->children[i] = NULL;
        }
    }
    return new_node;
}

void insert_rule(TrieNode* root, const char* rule_str, uint32_t flags) {
    TrieNode* current = root;

    for (int i = 0; rule_str[i] != '\0'; i++) {
        unsigned char index = (unsigned char)rule_str[i];

        if (current->children[index] == NULL) {
            current->children[index] = create_node();
        }
        current = current->children[index];
    }

    current->is_end_of_rule = true;
    current->context_flags = flags;
}

static bool check_exact_match(TrieNode* root, const char* str) {
    TrieNode* current = root;

    while (*str != '\0') {
        unsigned char index = (unsigned char)*str;

        if (current->children[index] == NULL) {
            return false;
        }

        current = current->children[index];

        if (current->is_end_of_rule) {
            return true;
        }

        str++;
    }

    return current->is_end_of_rule;
}

bool search_url(TrieNode* root, const char* url_str) {
    if (!root || !url_str || *url_str == '\0') return false;

    for(size_t i = 0; url_str[i] != '\0'; i++) {
        if (check_exact_match(root, &url_str[i])) {
            return true;
        }        
    }
    return false;
}

void free_trie(TrieNode* root) {
    if (root == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            free_trie(root->children[i]);
        }
    }
    free(root);
}