#ifndef __Index_H__
#define __Index_H__

#include <string>
#include <vector>

class HostEntry
{
    public:
        off_t  logical_offset;
        off_t  physical_offset;
        size_t length;
        double begin_timestamp;
        double end_timestamp;
        pid_t  id;      // needs to be last so no padding

        std::string show();
};


class Index {
    public:
        std::vector< HostEntry > _hostIndex;

        std::string _physical_path; // the physical path of the index file of this index
        int    _index_fd;      // the handle of the index file

        
        
        Index(const char *physical_path);
        Index(); // use this one if you don't want a file open for this index
        ~Index();
        
        void addEntry( const HostEntry &entry );
        int flush();
};

#endif

