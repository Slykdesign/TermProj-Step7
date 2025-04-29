#ifndef FILE_ACCESS_H
#define FILE_ACCESS_H

#include "ext2.h"
#include "inode.h"

typedef struct {
    struct Inode inode;
    uint64_t cursor;
    uint32_t inodeNum;
    struct Ext2File *ext2File;
} FileHandle;

int32_t fetchBlockFromFile(struct Ext2File *f, struct Inode *i, uint32_t bNum, void *buf);
int32_t writeBlockToFile(struct Ext2File *f, struct Inode *i, uint32_t bNum, void *buf);

#endif