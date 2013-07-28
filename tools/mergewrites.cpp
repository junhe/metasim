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


class Performance {
    public:
        map<string, vector<string> > _data;
        int _colwidth; // column width
        
        void put(const char *key, const char *val);
        void put(const char *key, int val);
        void put(const char *key, double val);
        void put(const char *key, float val);
        string showColumns();

        Performance(int colwidth);
};

Performance::Performance(int colwidth = 15)
    :_colwidth(colwidth)
{
}

string 
Performance::showColumns()
{
    ostringstream oss;
    map<string, vector<string> >::iterator it;

    // print header
    vector<string>::size_type maxdepth = 0;
    for ( it = _data.begin() ;
          it != _data.end() ;
          ++it )
    {
        oss << setw(15) << it->first << " ";
        if ( maxdepth < it->second.size() ) {
            maxdepth = it->second.size();
        }
    }
    oss << "MYHEADERROWMARKER" << endl;

    // print performance data
    //
    vector<string>::size_type i;
    for ( i = 0 ; i < maxdepth ; i++ ) {
        for ( it = _data.begin() ;
              it != _data.end() ;
              ++it )
        {
            vector<string> &vals = it->second; // for short
            assert(vals.size() == maxdepth);
            oss << setw(15) << vals.at(i) << " ";
        }
    }
    oss << "DATAROWMARKER" << endl;
    return oss.str();
}

void 
Performance::put(const char *key, const char *val)
{
    string keystr = string(key);
    if (_data.count(keystr) == 0) {
        _data[keystr] = vector<string>();
    }
    _data[keystr].push_back( string(val) );
}

void
Performance::put(const char *key, int val)
{
    ostringstream oss;
    oss << val;
    put(key, oss.str().c_str());
}

void
Performance::put(const char *key, double val)
{
    ostringstream oss;
    oss << val;
    put(key, oss.str().c_str());
}

void
Performance::put(const char *key, float val)
{
    ostringstream oss;
    oss << val;
    put(key, oss.str().c_str());
}


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

    Performance perfs;
    perfs.put("cnt", cnt);
    perfs.put("time", result);
    perfs.put("speed", (cnt/result));

    // clean up
    int newcnt = 0;
    map<pid_t, Index*>::iterator it;
    for (it = index_pool.begin() ; it != index_pool.end() ; ++it) {
        newcnt += it->second->_hostIndex.size();
        delete it->second;
    }
    perfs.put("final_size", newcnt);
    perfs.put("merging", iMerge);

    cout << perfs.showColumns();

}



