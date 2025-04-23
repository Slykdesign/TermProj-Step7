#ifndef COPY_FILE_H
#define COPY_FILE_H

#include "ext2.h"

void copyFile(struct Ext2File *f, char *srcPath, char *destPath);

#endif