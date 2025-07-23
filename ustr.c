#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "pset1.h"

/*
Initializes a new UStr with contents
*/
UStr new_ustr(char* contents) {
	int32_t bytes = strlen(contents);
	int32_t codepoints = utf8_strlen(contents);
	uint8_t all_ascii = bytes == codepoints;
	char* contents_copy = malloc(bytes + 1);
	strcpy(contents_copy, contents);
	UStr s = {
		codepoints,
		bytes,
		all_ascii,
		contents_copy
	};
	return s;
}

/*
Returns length of string
*/
int32_t len(UStr s) {
	return s.codepoints;
}

/*
Returns a substring of str, starting at index start (inclusive) 
and ending at index end (exclusive).

Returns an empty string on invalid range.
*/
UStr substring(UStr s, int32_t start, int32_t end) {
    if(start <0 || start >= end || end> s.codepoints){
    return new_ustr("");
    }

    int start_byte = bi_of_cpi(s.contents, start);
    int end_byte = bi_of_cpi(s.contents, end);

    char* new_contents  = malloc(end_byte- start_byte + 1);
    if(! new_contents){
	 return new_ustr("");
    }

    memcpy(new_contents, s.contents + start_byte, end_byte- start_byte);    
    new_contents[end_byte- start_byte] = '\0';

    UStr result = new_ustr(new_contents);
    free(new_contents);

    return result;
}

/*
Given 2 strings s1 and s2, returns a string that is the result of 
concatenating s1 and s2. 
*/
UStr concat(UStr s1, UStr s2) {
    char* new_contents = malloc(s1.bytes + s2.bytes + 1);
    if(!new_contents){
	    return new_ustr("");
    }

    memcpy(new_contents, s1.contents, s1.bytes);
    memcpy(new_contents + s1.bytes, s2.contents, s2.bytes);
    new_contents[s1.bytes + s2.bytes] = '\0';

    UStr result;

    result.contents = new_contents;
    result.codepoints = s1.codepoints + s2.codepoints;
    result.bytes =  s1.bytes + s2.bytes;
    result.is_ascii = s1.is_ascii && s2.is_ascii;

    return result;
}

/*
Given a string s and an index, return a string with the character at index 
removed from the original string. 

Returns the original string if index is out of bounds.
*/
UStr removeAt(UStr s, int32_t index) {
	 if (index < 0 || index >= s.codepoints) {
        return new_ustr(s.contents); // return a copy
    }

    int32_t start_byte = utf8_index(s.contents, index);
    int32_t end_byte = utf8_index(s.contents, index + 1);
    int32_t removed_bytes = end_byte - start_byte;

    int32_t new_bytes = s.bytes - removed_bytes;
    char* new_str = malloc(new_bytes + 1);

    memcpy(new_str, s.contents, start_byte); // copy before
    memcpy(new_str + start_byte, s.contents + end_byte, s.bytes - end_byte); // copy after
    new_str[new_bytes] = '\0';

    UStr result = new_ustr(new_str);
    free(new_str);
    return result;

}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/
UStr reverse(UStr s) {
	char* reversed = malloc(s.bytes + 1);
    int32_t offset = 0;

    for (int32_t i = s.codepoints - 1; i >= 0; i--) {
        int32_t start = utf8_index(s.contents, i);
        int32_t end = utf8_index(s.contents, i + 1);
        int32_t len = end - start;
        memcpy(reversed + offset, s.contents + start, len);
        offset += len;
    }

    reversed[s.bytes] = '\0';
    UStr result = new_ustr(reversed);
    free(reversed);
    return result;

}


void print_ustr(UStr s) {
	printf("%s [codepoints: %d | bytes: %d]", s.contents, s.codepoints, s.bytes);
}

void free_ustr(UStr s) {
	if (s.contents != NULL) {
		free(s.contents);
		s.contents = NULL;
	}
}

