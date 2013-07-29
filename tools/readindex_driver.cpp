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
    if (argc < 2) {
        cout << "Usage: " << argv[0] 
             << " indexfilepath"
             << endl;
        exit(1);
    }

    string iIndexPhyPath = argv[1];

    Index index;
    index.readIndex(iIndexPhyPath);

}
    

