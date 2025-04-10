#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "ext2.h"
#include "inode.h"

#define MAX_NAME_LEN 255

struct Direct {
    uint32_t iNum;        // Inode number
    uint16_t recLen;      // Length of this record
    uint8_t nameLen;      // Length of the name
    char name[MAX_NAME_LEN]; // File name
};

// Definition of struct Directory
struct Directory {
    Ext2File *file;       // Pointer to the file structure
    Inode inode;   // Inode structure for the directory
    uint32_t cursor;      // Current position in the directory
    void *blockBuffer;    // Buffer for reading directory blocks
};

struct Directory *openDir(Ext2File *f, uint32_t iNum);
bool getNextDirect(struct Directory *d, uint32_t *iNum, char *name);
void rewindDir(struct Directory *d);
void closeDir(struct Directory *d);
uint32_t searchDir(Ext2File *f, uint32_t iNum, char *target);
uint32_t traversePath(Ext2File *f, char *path);

#endif