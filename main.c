#include "partition.h"
#include "ext2.h"
#include "directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <ctype.h>

// Function prototypes
void displayBufferPage(uint8_t *buf, uint32_t count, uint32_t skip, uint64_t offset);
void displayBuffer(uint8_t *buf, uint32_t count, uint64_t offset);

int main() {
    Ext2File *ext2 = openExt2File("./good-fixed-1k.vdi");
    if (!ext2) return 1;

    printf("Root directory contents\n");
    struct Directory *d = openDir(ext2, 2); // Open root directory
    uint32_t iNum;
    char name[256];
    while (getNextDirect(d, &iNum, name)) {
        printf("Inode: %u   name: [%s]\n", iNum, name);
    }
    closeDir(d);

    printf("\nlost+found directory contents\n");
    d = openDir(ext2, 11); // Open lost+found directory
    while (getNextDirect(d, &iNum, name)) {
        printf("Inode: %u   name: [%s]\n", iNum, name);
    }
    closeDir(d);
    return 0;
}

// Function Definitions
void displayBufferPage(uint8_t *buf, uint32_t count, uint32_t skip, uint64_t offset) {
    printf("Offset: 0x%lx\n", offset);
    for (uint32_t i = skip; i < count; i += 16) {
        printf("%04x | ", i);
        for (uint32_t j = 0; j < 16 && i + j < count; j++) {
            printf("%02x ", buf[i + j]);
        }
        printf("| ");
        for (uint32_t j = 0; j < 16 && i + j < count; j++) {
            printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
        }
        printf("\n");
    }
}

void displayBuffer(uint8_t *buf, uint32_t count, uint64_t offset) {
    uint32_t step = 256;
    for (uint32_t i = 0; i < count; i += step) {
        displayBufferPage(buf, count, i, offset + i);
    }
}