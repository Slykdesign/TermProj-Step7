#include "copy_file.h"
#include "file_access.h"
#include "inode.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void copyFile(Ext2File *f, char *srcPath, char *destPath) {
    uint32_t iNum = traversePath(f, srcPath);
    if (iNum == 0) {
        fprintf(stderr, "File not found: %s\n", srcPath);
        return;
    }

    Inode fileInode;
    if (fetchInode(f, iNum, &fileInode) != 0) {
        fprintf(stderr, "Failed to fetch inode for: %s\n", srcPath);
        return;
    }

    int destFd = open(destPath, O_WRONLY | O_CREAT, 0666);
    if (destFd == -1) {
        perror("Failed to open destination file");
        return;
    }

    uint8_t *buffer = malloc(f->block_size);
    if (!buffer) {
        close(destFd);
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    uint32_t numBlocks = (fileInode.i_size + f->block_size - 1) / f->block_size;
    for (uint32_t bNum = 0; bNum < numBlocks; bNum++) {
        if (fetchBlockFromFile(f, &fileInode, bNum, buffer) != 0) {
            fprintf(stderr, "Failed to read block %u\n", bNum);
            free(buffer);
            close(destFd);
            return;
        }
        write(destFd, buffer, (bNum == numBlocks - 1) ? (fileInode.i_size % f->block_size) : f->block_size);
    }

    free(buffer);
    close(destFd);
}