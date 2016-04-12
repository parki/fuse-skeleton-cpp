#include <iostream>

#define FUSE_USE_VERSION 26
#include <fuse.h>

using namespace std;

static const char *dash_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char *dash_path= "/hello";

static int dash_getattr(const char *path, struct stat *stbuf) {
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, dash_path) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(dash_str);
	} else
		res = -ENOENT;

	return res;	
}

static int dash_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, dash_path + 1, NULL, 0);

	return 0;
}

static int dash_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path, dash_path) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int dash_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	if(strcmp(path, dash_path) != 0)
		return -ENOENT;

	len = strlen(dash_str);
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, dash_str + offset, size);
	} else
		size = 0;

	return size;
}

static struct fuse_operations dash_oper = {
	.getattr	= dash_getattr,
	.readdir	= dash_readdir,
	.open		= dash_open,
	.read		= dash_read,
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &dash_oper, NULL);
}
