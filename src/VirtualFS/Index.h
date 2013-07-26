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
    std::vector< HostEntry > _hostIndex;
    void addEntry( const HostEntry &entry );
};

#endif

