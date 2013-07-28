/* 
 *   This program is for driving and testing inserting entries
 *   to a global index. 
 *   A global index can hold entries from many different PIDs.
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
#include <iomanip>

#include "Index.h"
#include "Util.h"
#include "MapFetcher.h"

using namespace std;

int main(int argc, char **argv)
{
    // Handle the program arguments
    if (argc < 5) {
        cout << "Usage: " << argv[0] 
             << " mapfilename" 
             << " FetchBuffer" 
             << " DoFlush" // save indices to disk
             << " FlushDir" // where to save indices files
             << endl;
        exit(1);
    }

    string mapfilename = argv[1];
    int iFetchSize = atoi(argv[2]);
    bool iDoFlush = (bool)atoi(argv[3]);
    string iFlushDir = argv[4];
    
    // Initial MapFetcher
    MapFetcher mf(iFetchSize,  mapfilename.c_str());
    HostEntry hentry; // a temp entry holder
    Index *index;
    if ( iDoFlush ) {
        ostringstream fpath;
        fpath << iFlushDir << "/" 
              << "dropping.index";
        index = new Index( fpath.str().c_str() );
    } else {
        index = new Index(); // hold the indice that we have
    }

    mf.fillBuffer(); // fill it first, since sometimes I want all entries
                     // are in memory before timing.
   
  
    // Add map entries to index
    struct timeval start, end;
    int cnt = 0;
    
    start = Util::Gettime();
   
    GlobalEntry gentry;
    while ( mf.fetchEntry(hentry) != EOF ) {
        //cout << hentry.show() << endl;
        gentry.logical_offset = hentry.logical_offset;
        gentry.length = hentry.length;
        gentry.id = hentry.id;
        gentry.begin_timestamp = hentry.begin_timestamp;
        gentry.end_timestamp = hentry.end_timestamp;
        gentry.original_chunk = hentry.id;

        index->insertGlobalEntry(&gentry);
        cnt++;
    }


    end = Util::Gettime();
    double add_time = Util::GetTimeDurAB(start, end);


    // cleanup
    delete index;

    // Print performance results
    Performance perfs(30);
    perfs.put("add_time", add_time);
    perfs.put("cnt", cnt);
    perfs.put("prog_name", argv[0]);
    perfs.put("speed", (cnt/add_time));
    cout << perfs.showColumns();
}



