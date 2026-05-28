#ifndef URL_PARSER_H
#define URL_PARSER_H

typedef struct {
    const char* host_start; // first letter of the domain (e.g., 'e' in example.com)
    size_t host_len; // The exact character length of the domain string

    const char* path_start; // starting slash of the path (e.g., '/ads/banner.js')
    size_t path_len; // The length of the path up until any query parameters (?)
} ParsedURL;

void parse_raw_url(const char* raw_url, ParsedURL* out_parsed);

#endif