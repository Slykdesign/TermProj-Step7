#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "ext2.h"
#include "inode.h"

#define MAX_NAME_LEN 256

struct Direct {
    uint32_t iNum;        // Inode number
    uint16_t recLen;      // Length of this record
    uint8_t nameLen, fileType, name[1];      // Length of the name
};

// Definition of struct Directory
struct Directory {
    struct Ext2File *file;       // Pointer to the file structure
    struct Inode inode;   // Inode structure for the directory
    uint32_t cursor;      // Current position in the directory
    void *blockBuffer;    // Buffer for reading directory blocks
};

struct Directory *openDir(struct Ext2File *f, uint32_t iNum);
bool getNextDirect(struct Directory *d, uint32_t *iNum, char *name);
void rewindDir(struct Directory *d);
void closeDir(struct Directory *d);
uint32_t searchDir(struct Ext2File *f, uint32_t iNum, char *target);
uint32_t traversePath(struct Ext2File *f, char *path);

#endif