#include "directory.h"
#include <stdlib.h>
#include <string.h>

struct Directory *openDir(struct Ext2File *f, uint32_t iNum) {
    struct Directory *d = malloc(sizeof(struct Directory));
    if (!d) return NULL;

    if (fetchInode(f, iNum, &d->inode) != 0 || (d->inode.i_mode & 0xF000) != 0x4000) {
        free(d);
        return NULL; // Not a valid directory
    }

    d->file = f;
    d->cursor = 0;
    d->blockBuffer = malloc(f->block_size);
    if (!d->blockBuffer) {
        free(d);
        return NULL;
    }
    return d;
}

bool getNextDirect(struct Directory *d, uint32_t *iNum, char *name) {
    while (d->cursor < d->inode.i_size) {
        uint32_t blockIndex = d->cursor / d->file->block_size;
        uint32_t offset = d->cursor % d->file->block_size;

        if (offset == 0 && !fetchBlock(d->file, d->inode.i_block[blockIndex], d->blockBuffer)) {
            return false;
        }

        struct Direct *direct = (struct Direct *)(d->blockBuffer + offset);
        d->cursor += direct->recLen;

        if (direct->iNum != 0) {
            *iNum = direct->iNum;
            strncpy(name, (char *)direct->name, direct->nameLen);
            name[direct->nameLen] = '\0'; // Null-terminate
            return true;
        }
    }
    return false;
}

void rewindDir(struct Directory *d) {
    d->cursor = 0;
}

void closeDir(struct Directory *d) {
    if (d) {
        free(d->blockBuffer);
        free(d);
    }
}

uint32_t searchDir(struct Ext2File *f, uint32_t iNum, char *target) {
    struct Directory *dir = openDir(f, iNum);
    if (!dir) return 0;

    uint32_t iNumFound = 0;
    char name[MAX_NAME_LEN + 1];
    while (getNextDirect(dir, &iNumFound, name)) {
        if (strcmp(name, target) == 0) {
            closeDir(dir);
            return iNumFound;
        }
    }
    closeDir(dir);
    return 0;
}

uint32_t traversePath(struct Ext2File *f, char *path) {
    uint32_t iNum = 2; // Start at root inode (2)
    size_t start = 1, len = strlen(path);

    while (start < len && iNum != 0) {
        size_t end = start;
        while (end < len && path[end] != '/') end++;
        path[end] = '\0';
        iNum = searchDir(f, iNum, path + start);
        start = end + 1;
    }
    return iNum;
}