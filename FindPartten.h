#pragma once

#include <string.h>
#include<windows.h>
// Boyer-Moore skip table
int skip_table[256];

void build_skip_table(unsigned char* pattern, int pattern_length) {
    for (int i = 0; i < 256; i++) {
        skip_table[i] = pattern_length;
    }
    for (int i = 0; i < pattern_length - 1; i++) {
        skip_table[pattern[i]] = pattern_length - i - 1;
    }
}

unsigned char* search_memory(unsigned char* memory, int memory_length, unsigned char* pattern, unsigned char* mask, int pattern_length) {
    build_skip_table(pattern, pattern_length);
    for (int i = pattern_length - 1; i < memory_length;) {
        int j = pattern_length - 1;
        while (j >= 0 && (memory[i] == pattern[j] || mask[j] == 0x00)) {
            i--;
            j--;
        }
        if (j < 0) {
            return &memory[i + 1];
        }
        i += skip_table[memory[i]] > pattern_length - j ? skip_table[memory[i]] : pattern_length - j;
    }
    return NULL;
}

unsigned char* search_pe_code_segment(void* base, unsigned char* pattern, unsigned char* mask, int pattern_length) {
    IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)base;
    IMAGE_NT_HEADERS* nt_headers = (IMAGE_NT_HEADERS*)((char*)base + dos_header->e_lfanew);
    IMAGE_SECTION_HEADER* section_headers = (IMAGE_SECTION_HEADER*)((char*)&nt_headers->OptionalHeader + nt_headers->FileHeader.SizeOfOptionalHeader);

    for (int i = 0; i < nt_headers->FileHeader.NumberOfSections; i++) {
        if (true) {
            unsigned char* code_segment = (unsigned char*)((char*)base + section_headers[i].VirtualAddress);
            int code_segment_size = section_headers[i].Misc.VirtualSize;

            return search_memory(code_segment, code_segment_size, pattern, mask, pattern_length);
        }
    }

    return NULL;
}