#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "Util.h"

// Write n bytes from vptr to fd
// returns n or returns -err ( if < n is written, it is an error )
ssize_t
Util::WriteN(const void *vptr, size_t n, int fd)
{
    size_t      nleft;
    ssize_t     nwritten;
    const char  *ptr;
    ptr = (const char *)vptr;
    nleft = n;
    int ret   = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && nwritten != -EINTR) {
                ret = nwritten;   /* error! */
                break;
            }
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return ret;
}


int
Util::Open(const char *fname, int flag)
{
    return open(fname, flag, 0666);
}

int
Util::Close(int fd)
{
    return close(fd);
}


