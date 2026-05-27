#ifndef PARSER_H
#define PARSER_H

#include "trie.h"

#define FLAG_NONE 0 
#define FLAG_THIRD_PARTY (1 << 0)
#define FLAG_SCRIPT (1 << 1)
#define FLAG_IMAGE (1 << 2)

bool parse_filter_file(TrieNode* root, const char* filepath);

#endif