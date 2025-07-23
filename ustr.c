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
	// TODO: implement this

}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/
UStr reverse(UStr s) {
	// TODO: implement this

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

