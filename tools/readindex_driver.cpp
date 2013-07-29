/*
 * Use this program to drive reading index.
 * This program gives a physical index file path,
 * the Index class reads the index entries to
 * memory and form a global index.
 */

#include <iostream>
#include <stdlib.h>
#include <sstream>

#include "Index.h"
#include "Util.h"
#include "MapFetcher.h"

using namespace std;

int main(int argc, char **argv)
{
    // Handle the program arguments
    if (argc < 3) {
        cout << "Usage: " << argv[0] 
             << " indexdirpath"
             << " DoMerge"
             << endl;
        exit(1);
    }

    // input handling
    string iIndexDirPath = argv[1];
    bool iDoMerge = (bool) atoi(argv[2]);

    struct timeval startt, endt;

    // Get index files names
    startt = Util::Gettime();

    vector<string> index_files;
    index_files = Util::GetIndexFiles(
                  iIndexDirPath.c_str() );

    endt = Util::Gettime();
    double readdir_time = Util::GetTimeDurAB(startt, endt);


    // Read indices from files
    vector<Index *> index_pool; // to hold all indice from files
    vector<string>::iterator it;

    startt = Util::Gettime();
    for ( it = index_files.begin();
          it != index_files.end();
          ++it ) 
    {
        ostringstream filepath;
        filepath << iIndexDirPath << "/" << *it;
        cout << filepath.str() << endl;

        Index *idx = new Index(iDoMerge);
        idx->readIndex(filepath.str());
        cout << "global index size:" << idx->_globalIndex.size() << endl;

        index_pool.push_back(idx);
    }
    endt = Util::Gettime();
    double readindex_time = Util::GetTimeDurAB(startt, endt);

    // Build a global index
    Index gIndex(iDoMerge);

    startt = Util::Gettime();
    // merge and delete merged indices
    vector<Index *>::iterator pool_it;
    for ( pool_it = index_pool.begin() ;
          pool_it != index_pool.end() ;
          ++pool_it )
    {
        gIndex.merge(*pool_it);
        delete *pool_it;
    }
    endt = Util::Gettime();
    double buildglobal_time = Util::GetTimeDurAB(startt, endt);

    cout << "gIndex globalIndex size:" << gIndex._globalIndex.size() << endl;

    // Performance output
    Performance pfs(25);
    pfs.put("readindex_time", readindex_time);
    pfs.put("readdir_time", readdir_time);
    pfs.put("buildglobal_time", buildglobal_time);
    pfs.put("globalindex_size", (int)gIndex._globalIndex.size());
    pfs.put("exefile", argv[0]);
    pfs.put("index_dir", iIndexDirPath.c_str());

    cout << pfs.showColumns() << endl;
    return 0;
}
    

