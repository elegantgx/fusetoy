//
// Created by gx on 2026/11/12.
//

#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <string.h>
#include <errno.h>

static const char *fusetoy_contents = "Just a toy.\n";
static const char *fusetoy_filename = "toy.txt";

// NOLINTNEXTLINE(readability-non-const-parameter)
static int fusetoy_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    (void) fi;
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if (strcmp(path + 1, fusetoy_filename) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = (off_t)strlen(fusetoy_contents);
    }
    else {
        return -ENOENT;
    }
    return 0;
}

// NOLINTNEXTLINE(readability-non-const-parameter)
static int fusetoy_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
    (void) offset; (void) fi; (void) flags;

    if (strcmp(path, "/") != 0) return -ENOENT;

    filler(buf, ".", nullptr, 0, 0);
    filler(buf, "..", nullptr, 0, 0);
    filler(buf, fusetoy_filename, nullptr, 0, 0);

    return 0;
}

// NOLINTNEXTLINE(readability-non-const-parameter)
static int fusetoy_open(const char *path, struct fuse_file_info *fi) {
    if (strcmp(path + 1, fusetoy_filename) != 0) return -ENOENT;

    if ((fi->flags & O_ACCMODE) != O_RDONLY) return -EACCES;

    return 0;
}

// NOLINTNEXTLINE(readability-non-const-parameter)
static int fusetoy_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    (void) fi;

    const size_t len = strlen(fusetoy_contents);

    if (strcmp(path + 1, fusetoy_filename) != 0) return -ENOENT;

    if ((size_t) offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, fusetoy_contents + offset, size);
    } else {
        size = 0;
    }
    return (int)size;
}

static const struct fuse_operations fusetoy_oper = {
    .getattr = fusetoy_getattr,
    .readdir = fusetoy_readdir,
    .open    = fusetoy_open,
    .read    = fusetoy_read,
};

int main(const int argc, char *argv[]) {
    return fuse_main(argc, argv, &fusetoy_oper, nullptr);
}
