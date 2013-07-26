#include "Index.h"

#include <sstream>

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



void
Index::addEntry( const HostEntry &entry )
{
    _hostIndex.push_back( entry );
}


