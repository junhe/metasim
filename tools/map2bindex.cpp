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

#include "Index.h"

using namespace std;


void replaceSubStr( string del, string newstr, string &line, int startpos = 0 ) 
{
    size_t found;
    
    found = line.find(del, startpos);
    while (found != string::npos) 
    {
        line.replace(found, del.size(), newstr);  
        found = line.find(del, startpos);
    }
}


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
    
    private:
        int line2entry(string line, HostEntry &hentry);
};

MapFetcher::MapFetcher(int bsize, const char *mapfilename)
    :_bufSize(bsize)
{ 
    assert(_bufSize > 0);
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
        
        if ( !line2entry( line, entry ) == 0 ) {
            cerr << "failed to line2entry()" << endl;
            exit(-1);
        } else {
            return 1;
        }
    } else {
        return EOF;
    }
}

// convert a text line to a HostEntry
// return -1: not a valid plfs map entry
// return 0: success
int
MapFetcher::line2entry(string line, HostEntry &hentry)
{
    if ( line.length() > 0 ) {
        if ( line[0] == '#' ) {
            // it is a comment line
            cerr << "it is a comment line" << endl;
            return -1;
        } else {
            // it is a valid plfs map row

            // remove the weird symbols
            replaceSubStr( "[", " ", line );
            replaceSubStr( "]", " ", line );
            replaceSubStr( ".", " ", line, 107 ); //107 is the byte # where chunk info starts

            vector<string> tokens;
            vector<string>::iterator iter;
            istringstream iss(line);
            copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter<vector<string> >(tokens));
           
            // pid
            hentry.id = atoi( tokens[7].c_str() );

            // logical offset
            stringstream convert(tokens[2]);
            if ( !(convert >> hentry.logical_offset) ) {
                cout << "error on converting" << endl;
                exit(-1);
            }
            
            // length
            convert.clear();
            convert.str(tokens[3]);
            if ( !(convert >> hentry.length) ) {
                cout << "error on converting" << endl;
                exit(-1);
            }

            // begin_timestamp
            convert.clear();
            convert.str(tokens[4]);
            if ( !(convert >> hentry.begin_timestamp) ) {
                cout << "error on converting" << endl;
                exit(-1);
            }
            
            // end_timestamp
            convert.clear();
            convert.str(tokens[5]);
            if ( !(convert >> hentry.end_timestamp) ) {
                cout << "error on converting" << endl;
                exit(-1);
            }

            // physical offset
            convert.clear();
            convert.str(tokens[8]);
            if ( !(convert >> hentry.physical_offset) ) {
                cout << "error on converting" << endl;
                exit(-1);
            }
            
            return 0;
        }
    } else {
        // line has zero size
        cerr << "line has zero size" << endl;
        return -1;
    }
}

// fetch one entry from buffer, if buffer is
// empty, read it from map file.
// Note that when buffer size should be at least 1
//
// Return number of entries fetched, or EOF (indicating
// nothing more can be fetched (EOF and empty buffer))
int 
MapFetcher::fetchEntry(HostEntry &entry)
{
    // Fill buffer if it is empty
    if ( _entryBuf.empty() ) {
        int cnt = 0;
        while ( cnt < _bufSize ) {
            HostEntry lp_entry;
            int ret;
            ret = readEntryFromStream(lp_entry);
            if ( ret == 1 ) {
                _entryBuf.push(lp_entry);
                cnt++;
            } else {
                // ret == EOF
                if ( cnt == 0 ) {
                    // empty buffer and empty file
                    return EOF;
                } else {
                    break;
                }
            }
        } // while
    }
    
    assert(!_entryBuf.empty());
    entry = _entryBuf.front(); // the oldest entry
    _entryBuf.pop();
    return 1;
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
    
    MapFetcher mf(10,  mapfilename.c_str());
    HostEntry hentry;
    Index index("dropping.index");

    while ( mf.fetchEntry(hentry) != EOF ) {
        cout << hentry.show() << endl;
        index.addEntry(hentry);
    }

    cout << "index size:" << index._hostIndex.size() << endl;
    cout << "flushing to file" << endl;
    index.flush();
    cout << sizeof(off_t) <<endl;
}



