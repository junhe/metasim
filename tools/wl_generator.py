import sys

if __name__ == "__main__":
    choice = "sortandmerge_bad"

    if choice == "dumbmerge_good":
        wlen = 2
        wstride = 2
        local_nwrites = 100000
        npid = 64
        
        for wpid in range(0, npid):
            for i in range(0, local_nwrites):
                woff = wpid * local_nwrites * wstride \
                       + wstride * i
                wphyoff = wlen * i
                begin_t = i
                end_t = i + 0.1

                print wpid, "w", woff, wlen, begin_t, end_t, \
                        woff+wlen-1, wpid, wphyoff
    elif choice == "sortandmerge_good":
        wlen = 2
        wstride = -2
        local_nwrites = 100000
        npid = 64
        
        for wpid in range(0, npid):
            for i in range(0, local_nwrites):
                woff = (wpid+1) * local_nwrites * abs(wstride) \
                       + wstride * (i+1)
                wphyoff = wlen * i
                begin_t = i
                end_t = i + 0.1

                print wpid, "w", woff, wlen, begin_t, end_t, \
                        woff+wlen-1, wpid, wphyoff
    elif choice == "sortandmerge_bad":
        wlen = 1 
        wstride = -2
        local_nwrites = 100000
        npid = 64
        
        for wpid in range(0, npid):
            for i in range(0, local_nwrites):
                woff = (wpid+1) * local_nwrites * abs(wstride) \
                       + wstride * (i+1)
                wphyoff = wlen * i
                begin_t = i
                end_t = i + 0.1

                print wpid, "w", woff, wlen, begin_t, end_t, \
                        woff+wlen-1, wpid, wphyoff

