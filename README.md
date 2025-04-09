# TermProj-Step7
## Directory Access
### Goals
#### • Provide functions to traverse a file path and copy a file to the host.
### Details
#### In these steps, we provide the final steps to copy a file from the VDI file to the host system.
### File Paths
#### A file path consists of the sequence of directories to follow from the root directory to the desired file. For example, this document has the path /home/bob/School/Courses/5806.Course/Projects/Ext2Copy/ext2-cp-7.base.tex.
#### The goal in step 7 is to split the path into its component names and then search the sequence of directories for the inode number corresponding to the file at the end of the path.
### Searching a Directory
#### To search a directory, you’ll want to create a function uint32_t searchDir(struct Ext2File *f,uint32_t iNum,char *target) with the following parameters:
##### • f
###### Pointer to an opened Ext2File
##### • iNum
###### Inode number of the directory to be searched
##### • target
###### C-string with the name of the file to look for
#### The function should return the inode number of the target file if found, or 0 otherwise.
### Splitting a Path
#### Create a function uint32_t traversePath(Ext2File *f,char *path) that takes a full path and returns the inode number of the file at the end of the path. The algorithm for the function follows.
##### Algorithm 1 Traversing a directory path
###### procedure TraversePath(Ext2File ∗ƒ, char ∗path)
###### start ← 1
###### len ← strlen(path)
###### iNum ← 2
###### while start < len and iNum ≠ 0 do
###### end ← location of next ’/’
###### path[end] ← 0
###### iNum ← searchDir(ƒ, iNum, path + start)
###### start ← end + 1
###### end while
###### return iNum
###### end procedure
### Copying the file
#### Step 8 is copying the file out of the VDI disk image and onto the host system.
#### Your program should get the names of the VDI file, the file in the virtual disk and the destination file. Open the destination file like a normal file — use the open() system call with O_WRONLY|O_CREAT parameters and mode 0666 — and loop through all of the blocks in the file on the virtual disk and copy the data into the destination file.
