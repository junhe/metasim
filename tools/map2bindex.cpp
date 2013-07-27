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
#include <map>

#include "Index.h"
#include "MapFetcher.h"

using namespace std;

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
    
    MapFetcher mf(1000,  mapfilename.c_str());
    HostEntry hentry; // a temp entry holder

    // This map is very tricky. If you don't use pointer to Index,
    // you will need to copy the index object to map. since
    // i close the file when destructing the object, then the
    // fd passed in is no long valid.
    map< pid_t, Index* > index_pool; // hold the indice that we have
    
    while ( mf.fetchEntry(hentry) != EOF ) {
        //cout << hentry.show() << endl;
        if ( index_pool.count(hentry.id) == 0 ) {
            // index for this pid is not in pool
            ostringstream fname;
            fname << "dropping.index." << hentry.id;

            index_pool[hentry.id] = new Index(fname.str().c_str());
        } else {
            index_pool[hentry.id]->addEntry(hentry);
        }
    }

    // explicitly flush index
    map<pid_t, Index*>::iterator it;
    for (it = index_pool.begin() ; it != index_pool.end() ; ++it) {
        //cout << it->first << ": " << it->second->_hostIndex.size() << endl;
        it->second->flush();

        delete it->second;
    }



}



