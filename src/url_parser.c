#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "../include/url_parser.h"

void parse_raw_url(const char* raw_url, ParsedURL* out_parsed) {
    out_parsed->host_start = NULL;
    out_parsed->host_len = 0;
    out_parsed->path_start = NULL;
    out_parsed->path_len = 0;

    if (!raw_url) return;

    const char* current = raw_url;

    // Remove protocol
    const char* protocol_marker = strstr(current, "://");
    if (protocol_marker != NULL) {
        current = protocol_marker + 3;
    }

    out_parsed->host_start = current;

    // Get host length w/o port
    while (*current != '/' && *current != ':' && *current != '\0') {
        current++;
    } 
    out_parsed->host_len = current - out_parsed->host_start;
    if (*current == ':') {
        while (*current != '/' && *current != '\0') {
            current++;
        }
    }

    if (*current == '/') {
        out_parsed->path_start = current;
        
        const char* qm = strchr(current, '?');
        const char* ht = strchr(current, '#');
        const char* eol = strchr(current, '\0');

        if (qm && ht) {
            current = (qm < ht) ? qm : ht;
        } else if (qm) {
            current = qm;
        } else if (ht) {
            current = ht;
        } else {
            current = eol;
        }

        out_parsed->path_len = current - out_parsed->path_start;
    }

} 