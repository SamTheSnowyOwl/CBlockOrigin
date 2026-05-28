#ifndef PARSER_H
#define PARSER_H

#include "trie.h"

#define FLAG_NONE 0 
#define FLAG_THIRD_PARTY (1 << 0)
#define FLAG_SCRIPT (1 << 1)
#define FLAG_IMAGE (1 << 2)

typedef enum {
    REQ_OTHER = 0,
    REQ_SCRIPT,
    REQ_IMAGE,
    REQ_XHR,
    NUM_BUCKETS
} RequestType;

bool parse_filter_file_to_buckets(TrieNode* buckets[], const char* filepath);

#endif