import sys

if __name__ == "__main__":
    wlen = 1
    wstride = 2
    local_nwrites = 1000
    npid = 6
    
    for wpid in range(0, npid):
        for i in range(0, local_nwrites):
            woff = wpid * local_nwrites * wstride \
                   + wstride * i
            wphyoff = wlen * i
            begin_t = i
            end_t = i + 0.1

            print wpid, "w", woff, wlen, begin_t, end_t, \
                    woff+wlen-1, wpid, wphyoff

