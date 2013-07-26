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
        void addEntry( const HostEntry &entry );

        string _physical_path; // the physical path of the index file of this index
        int    _index_fd;      // the handle of the index file

        Index(char *physical_path);
};

#endif

