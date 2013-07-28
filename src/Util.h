#ifndef __Util_H__
#define __Util_H__

#include <vector>
#include <map>

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

class Performance {
    public:
        std::map<std::string, std::vector<std::string> > _data;
        int _colwidth; // column width
        
        void put(const char *key, const char *val);
        void put(const char *key, int val);
        void put(const char *key, double val);
        void put(const char *key, float val);
        std::string showColumns();

        Performance(int colwidth=15);
};


#endif

