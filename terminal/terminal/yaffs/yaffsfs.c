
#include "internal.h"

#ifdef NCSC
#define	WIN32
#endif

#ifdef WIN32

int yaffs_open(const char *path, int oflag, int mode)
{
	oflag = mode;
	path = 0;
	return 0;
}

int yaffs_read(int fd, void *buf, unsigned int nbyte)
{
	fd = nbyte;
	buf = 0;
	return 0;
}

int yaffs_write(int fd, const void *buf, unsigned int nbyte)
{
	fd = nbyte;
	memcpy(&fd, buf, 1);
	return 0;
}

int yaffs_close(int fd)
{
	fd = 0;
	return 0;
}

long yaffs_lseek(int fd, long offset, int whence)
{
	fd = offset = whence;
	return 0;
}

int yaffs_unlink(const char *path)
{
	path = 0;
	return 0;
}

int yaffs_rename(const char *oldPath, const char *newPath)
{
	oldPath = newPath;
	return 0;
}

int yaffs_stat(const char *path, struct yaffs_stat *buf)
{
	path = (char *)buf;
	return 0;
}

int yaffs_lstat(const char *path, struct yaffs_stat *buf)
{
	path = (char *)buf;
	return 0;
}

int yaffs_fstat(int fd, struct yaffs_stat *buf)
{
	fd = buf->st_atime;
	return 0;
}

int yaffs_chmod(const char *path, unsigned long mode)
{
	mode = path[0];
	return 0;
}

int yaffs_fchmod(int fd, unsigned long mode)
{
	fd = mode;
	return 0;
}

int yaffs_mkdir(const char *path, unsigned long mode)
{
	mode = path[0];
	return 0;
}

int yaffs_rmdir(const char *path)
{
	path = 0;
	return 0;
}

yaffs_DIR *yaffs_opendir(const char *dirname)
{
	return (yaffs_DIR *)dirname;
}

struct yaffs_dirent *yaffs_readdir(yaffs_DIR *dirp)
{
	dirp = 0;
	return NULL;
}

void yaffs_rewinddir(yaffs_DIR *dirp)
{
	dirp = 0;
}

int yaffs_closedir(yaffs_DIR *dirp)
{
	dirp = 0;
	return 0;
}

int yaffs_mount(const char *path)
{
	return path[0];
}

int yaffs_unmount(const char *path)
{
	return path[0];
}

int yaffs_symlink(const char *oldpath, const char *newpath)
{
	return newpath[0] - oldpath[0];
}

int yaffs_readlink(const char *path, char *buf, int bufsiz)
{
	buf = (char *)path;
	bufsiz = 0;
	return 0;
}


int yaffs_link(const char *oldpath, const char *newpath)
{
	return oldpath[0] - newpath[0];
}

int yaffs_mknod(const char *pathname, unsigned long mode, long dev)
{
	mode = dev = pathname[0];
	return 0;
}


long yaffs_freespace(const char *path)
{
	return (long)path[0];
}


//void yaffs_initialise(yaffsfs_DeviceConfiguration *configList);

int yaffs_StartUp(void)
{
	return 0;
}

void DEV_FD_Reset(void)
{
}

unsigned char DEV_FD_Erase_Block(unsigned int block)
{
	return (unsigned char)block;
}



#endif


