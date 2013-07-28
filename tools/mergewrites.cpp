/* 
 * This filel was copied from map2index.cpp for timing:
 *   merging contiguous writes when addWrite() in memory.
 *
 *   The procedure is like this:
 *   1. read all map entries from PLFS map file into MapFetcher
 *      memory
 *   2. record time A
 *   3. loop { fetch entry from MapFetcher buffer and index.addWrite(entry) }
 *   4. record time B
 *   5. print B-A
 *
 *   Note that the index is not written to disk. 
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
    if (argc < 4) {
        cout << "Usage: " << argv[0] 
             << " mapfilename" 
             << " FetchBuffer" 
             << " Merge"
             << endl;
        exit(1);
    }

    string mapfilename = argv[1];
    int iFetchSize = atoi(argv[2]);
    bool iMerge = (bool)atoi(argv[3]);
    
    // Initial MapFetcher
    MapFetcher mf(iFetchSize,  mapfilename.c_str());
    HostEntry hentry; // a temp entry holder
    map< pid_t, Index* > index_pool; // hold the indice that we have

    mf.fillBuffer(); // fill it first, since sometimes I want all entries
                     // are in memory before timing.
   
  
    // Add map entries to index
    struct timeval start, end;
    double result;
    int cnt = 0;
    
    start = Util::Gettime();
    
    while ( mf.fetchEntry(hentry) != EOF ) {
        //cout << hentry.show() << endl;
        if ( index_pool.count(hentry.id) == 0 ) {
            // index for this pid is not in pool
            index_pool[hentry.id] = new Index(iMerge);
        }
        //cout << hentry.show() << endl;
        index_pool[hentry.id]->addWrite(
                hentry.logical_offset,
                hentry.length,
                hentry.id,
                hentry.begin_timestamp,
                hentry.end_timestamp
                );
        cnt++;
    }


    end = Util::Gettime();
    result = Util::GetTimeDurAB(start, end);

    // clean up
    int newcnt = 0;
    map<pid_t, Index*>::iterator it;
    for (it = index_pool.begin() ; it != index_pool.end() ; ++it) {
        newcnt += it->second->_hostIndex.size();
        delete it->second;
    }

    // Print performance results
    Performance perfs;
    perfs.put("cnt", cnt);
    perfs.put("time", result);
    perfs.put("speed", (cnt/result));
    perfs.put("final_size", newcnt);
    perfs.put("merging", iMerge);

    cout << perfs.showColumns();

}



