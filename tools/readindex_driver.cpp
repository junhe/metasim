/*
 * Use this program to drive reading index.
 * This program gives a physical index file path,
 * the Index class reads the index entries to
 * memory and form a global index.
 */

#include <iostream>
#include <stdlib.h>

#include "Index.h"
#include "Util.h"
#include "MapFetcher.h"

using namespace std;

int main(int argc, char **argv)
{
    // Handle the program arguments
    if (argc < 3) {
        cout << "Usage: " << argv[0] 
             << " indexfilepath"
             << " indexdirpath"
             << endl;
        exit(1);
    }

    string iIndexPhyPath = argv[1];
    string iIndexDirPath = argv[2];

    Index index;
    index.readIndex(iIndexPhyPath);
    cout << "global index size:" << index._globalIndex.size() << endl;

    vector<string> index_files = Util::GetIndexFiles(
                  iIndexDirPath.c_str() );
    vector<string>::iterator it;
    for ( it = index_files.begin();
          it != index_files.end();
          ++it ) 
    {
        cout << *it << endl;
    }

}
    

