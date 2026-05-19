#ifndef TRIE_H
#define TRIE_H

#include <stdint.h>
#include <stdbool.h>

#define ALPHABET_SIZE 128

typedef struct TrieNode {
    bool is_end_of_rule;
    uint32_t context_flags;
    struct TrieNode* children[ALPHABET_SIZE];
} TrieNode;

TrieNode* create_node(void);
void insert_rule(TrieNode* root, const char* rule_str, uint32_t flags);
bool search_url(TrieNode* root, const char* url_str);
void free_trie(TrieNode* root);

#endif