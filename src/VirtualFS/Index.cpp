#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

#include "Index.h"
#include "Util.h"

using namespace std;

string
HostEntry::show()
{
    ostringstream oss;

    oss.precision(26);
    oss << id 
        << " "
        << logical_offset
        << " "
        << length
        << " "
        << physical_offset
        << " "
        << begin_timestamp
        << " "
        << end_timestamp
        << " ";
    return oss.str();
}

Index::Index(const char *physical_path)
    : _physical_path(physical_path),
      _index_fd(-1)
{
    // open physical file for index
    _index_fd = Util::Open(_physical_path.c_str(), O_CREAT|O_WRONLY);
    assert( _index_fd != -1 );
}


Index::Index()
{
}

Index::~Index()
{
    if ( _index_fd != -1 ) {
        Util::Close(_index_fd);
    }
}


void
Index::addEntry( const HostEntry &entry )
{
    _hostIndex.push_back( entry );
}


int
Index::flush()
{
    // ok, vectors are guaranteed to be contiguous
    // so just dump it in one fell swoop
    size_t  len = _hostIndex.size() * sizeof(HostEntry);
    
    if ( len == 0 ) {
        return 0;    // could be 0 if we weren't buffering
    }
    
    void *start = &_hostIndex[0];
    int ret     = Util::WriteN(start, len, _index_fd);
    if ( (size_t)ret != (size_t)len ) {
        cerr << "Failed to write to Index file" << endl;
        exit(-1);
    }
    _hostIndex.clear();
    return((ret < 0) ? ret : 0);
}


