#ifndef __Util_H__
#define __Util_H__

class Util {
    public:
        static ssize_t WriteN(const void *vptr, size_t n, int fd);
        static int Open(const char *fname, int flag);
        static int Close(int fd);
        static void replaceSubStr( std::string del, 
                                   std::string newstr, 
                                   std::string &line, int startpos = 0 );
        static struct timeval Gettime();
        static double GetTimeDurAB(struct timeval a,
                                   struct timeval b);
};


#endif

