#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string>

#include "Util.h"

using namespace std;

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


void
Util::replaceSubStr( string del, string newstr, string &line, int startpos) 
{
    size_t found;
    
    found = line.find(del, startpos);
    while (found != string::npos) 
    {
        line.replace(found, del.size(), newstr);  
        found = line.find(del, startpos);
    }
}


