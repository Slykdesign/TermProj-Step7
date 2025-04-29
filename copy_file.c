#include "copy_file.h"
#include "directory.h"
#include "file_access.h"
#include "inode.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void copyFile(struct Ext2File *f, char *srcPath, char *destPath) {
    uint32_t iNum = traversePath(f, srcPath);
    if (iNum == 0) {
        printf("File not found: %s\n", srcPath);
        return;
    }

    struct Inode fileInode;
    if (fetchInode(f, iNum, &fileInode) != 0) {
        printf("Failed to fetch inode for: %s\n", srcPath);
        return;
    }

    int destFd = open(destPath, O_WRONLY | O_CREAT, 0666);
    if (destFd == -1) {
        printf("Failed to open destination file");
        return;
    }

    uint8_t *buffer = malloc(f->blockSize);
    if (!buffer) {
        close(destFd);
        printf("Memory allocation failed\n");
        return;
    }

    uint32_t numBlocks = (fileInode.i_size + f->blockSize - 1) / f->blockSize;
    for (uint32_t bNum = 0; bNum < numBlocks; bNum++) {
        if (fetchBlockFromFile(f, &fileInode, bNum, buffer) != 0) {
            printf("Failed to read block %u\n", bNum);
            free(buffer);
            close(destFd);
            return;
        }
        write(destFd, buffer, (bNum == numBlocks - 1) ? (fileInode.i_size % f->blockSize) : f->blockSize);
    }

    free(buffer);
    close(destFd);
}