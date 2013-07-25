/* 
 * Usage: mpirun -np num-of-proc-in-map-file ./xmain mapfile outputfile
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <assert.h>

class HostEntry
{
    public:
        off_t  logical_offset;
        off_t  physical_offset;
        size_t length;
        double begin_timestamp;
        double end_timestamp;
        pid_t  id;      // needs to be last so no padding
};

using namespace std;

class MapFetcher
{
    public:
        ifstream _mapStream;
        queue <HostEntry> _entryBuf;
        int _bufSize; // if _bufSize is 0, then read
                      // from file every time. 
                      // if not, read from buffer if buffer
                      // is no empty
        
        int readEntryFromStream(HostEntry &entry);
        int fetchEntry(HostEntry &entry);

        MapFetcher(int bsize, const char *mapfilename);
        ~MapFetcher();
};

MapFetcher::MapFetcher(int bsize, const char *mapfilename)
    :_bufSize(bsize)
{ 
    _mapStream.open(mapfilename);
    if (_mapStream.is_open()) {
        cout << "Good, map file (" << mapfilename << ") is open." << endl;
    } else {
        cout << "ERROR, map file (" << mapfilename << ") cannot be open." << endl;
        exit(1);
    }
}

MapFetcher::~MapFetcher()
{
    _mapStream.close();
}

// Return 1: got the entry
// Return EOF: reach the eof of file, entry may have
//             random data
int 
MapFetcher::readEntryFromStream(HostEntry &entry)
{
    string line;
    if (getline(_mapStream, line)) {
        istringstream iss(line);
        cout << l 
        return 1;
    } else {
        return EOF;
    }
}

// fetch one entry from buffer, if buffer is
// empty, read it from map file.
// Note that when buffer size is set to 0, 
// it always reads from file.
//
// Return number of entries fetched, or EOF (indicating
// nothing more can be fetched)
int 
MapFetcher::fetchEntry(HostEntry &entry)
{
    if ( _entryBuf.empty() ) {

    }
}


int main(int argc, char **argv)
{
    // Open the map txt file
    // 1. Get an entry from it,
    // 2. Convert to HostEntry
    // 3. Write HostEntry to a file named by its pid
    
    if (argc < 2) {
        cout << "Usage: " << argv[0] << "mapfilename" << endl;
        exit(1);
    }

    string mapfilename = argv[1];
    
    MapFetcher mf(1,  mapfilename.c_str());
}
