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
    if (argc < 7) {
        cout << "Usage: " << argv[0] 
             << " mapfilename" 
             << " FetchBuffer" 
             << " DoMerge"
             << " DoSort"
             << " DoFlush" // save indices to disk
             << " FlushDir" // where to save indices files
             << endl;
        exit(1);
    }

    string mapfilename = argv[1];
    int iFetchSize = atoi(argv[2]);
    bool iDoMerge = (bool)atoi(argv[3]);
    bool iDoSort = (bool)atoi(argv[4]);
    bool iDoFlush = (bool)atoi(argv[5]);
    string iFlushDir = argv[6];
    
    // Initial MapFetcher
    MapFetcher mf(iFetchSize,  mapfilename.c_str());
    HostEntry hentry; // a temp entry holder
    map< pid_t, Index* > index_pool; // hold the indice that we have

    mf.fillBuffer(); // fill it first, since sometimes I want all entries
                     // are in memory before timing.
   
  
    // Add map entries to index
    struct timeval start, end;
    int cnt = 0;
    
    start = Util::Gettime();
    
    while ( mf.fetchEntry(hentry) != EOF ) {
        //cout << hentry.show() << endl;
        if ( index_pool.count(hentry.id) == 0 ) {
            // index for this pid is not in pool
            if ( iDoFlush ) {
                ostringstream fpath;
                fpath << iFlushDir << "/" 
                      << "dropping.index." << hentry.id;
                index_pool[hentry.id] = new Index( 
                        fpath.str().c_str(), iDoMerge );
            } else {
                index_pool[hentry.id] = new Index(iDoMerge);
            }
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
    double add_time = Util::GetTimeDurAB(start, end);


    // cleanup
    int newcnt = 0;
    map<pid_t, Index*>::iterator it;

    start = Util::Gettime();
    for (it = index_pool.begin() ; it != index_pool.end() ; ++it) {
        newcnt += it->second->_hostIndex.size();
        if (iDoSort) {
            it->second->sortEntries(); // sort entries
        }
        if (iDoFlush) {
            it->second->flush();
        }
        //cout << it->second->show();
        delete it->second;
    }
    end = Util::Gettime();
    double sort_flush_time = Util::GetTimeDurAB(start, end);

    // Print performance results
    Performance perfs(20);
    perfs.put("prog_name", argv[0]);
    perfs.put("cnt", cnt);
    perfs.put("add_time", add_time);
    perfs.put("speed", (cnt/add_time));
    perfs.put("final_size", newcnt);
    perfs.put("do_merging", iDoMerge);
    perfs.put("do_sort", iDoSort);
    perfs.put("sort_flush_time", sort_flush_time);
    perfs.put("mapfile", mapfilename.c_str()); 
    perfs.put("do_flush", iDoFlush);
    perfs.put("fetchbufsize", iFetchSize);

    cout << perfs.showColumns();

}



