#ifndef __Util_H__
#define __Util_H__

class Util {
    public:
        static ssize_t WriteN(const void *vptr, size_t n, int fd);
        static int Open(const char *fname, int flag);
        static int Close(int fd);
};






#endif

