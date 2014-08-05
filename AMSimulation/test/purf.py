#!/usr/bin/env python

from subprocess import Popen, PIPE

prog = "./bin/test_lean_table3"
perf = ["perf","stat","EVENTS", "--"]

# See https://perf.wiki.kernel.org/index.php/Tutorial
events = [
"task-clock",
"context-switches",
"cpu-migrations",
"page-faults",
"cpu-cycles",
"instructions",
"cache-references",
"cache-misses",
"branches",
"branch-misses",
"stalled-cycles-frontend",
"stalled-cycles-backend",
"L1-dcache-loads",
"L1-dcache-load-misses",
"L1-dcache-stores",
"L1-dcache-store-misses",
"L1-icache-loads",
"L1-icache-load-misses",
"LLC-loads",
"LLC-load-misses",
"LLC-stores",
"LLC-store-misses",
"dTLB-loads",
"dTLB-load-misses",
"dTLB-stores",
"dTLB-store-misses",
]

perf[perf.index("EVENTS")] = "-e %s" % (",".join(events))
perf.append(prog)

# Debug string
str0 = "# patterns: 8347588 size: 8003632"
str1 = "5 -> 31ab0206 6b6e4019 754c0000 7f428202 89398207 93300009 [  10]"


## MAIN ##
if __name__ == "__main__":

    # Run perf, catching output
    p1 = Popen(perf, stdout=PIPE, stderr=PIPE)
    output = p1.communicate()

    print "stdout\n", output[0]
    print "stderr\n", output[1]


    # -------------------------------------------------------------------------
    # Parse perf output
    parsed = {}

    i = 0
    for line in output[1].split("\n"):
        # Skip certain lines
        if i < 3: 
            i += 1
            continue

        line = line.strip()
        if line == "" or line[0] == "#":
            continue

        # Reverse iteration to avoid overwriting already-parsed events
        for e in reversed(events):
            if e in line:
                cnt = line.split(" ")[0]
                #print line, "~~>", cnt
                parsed[e] = float(cnt.replace(",",""))
                break


    # -------------------------------------------------------------------------
    # Check correctness
    found = 0
    for line in output[0].split("\n"):
        if str0 in line:  found += 1
        if str1 in line:  found += 1


    # -------------------------------------------------------------------------
    # Print comma-separated-value (csv) string
    #for e in events:
    #    print e, parsed[e]

    print "%7.3f,%6.0f,%5.2f,%6.0f,%5.2f,%6.0f,%5.2f,%6.0f,%5.2f,%6.0f,%5.2f,%6.0f,%5.2f,%6.0f,%5.2f,%6.0f,%5.2f,%6.0f,%5.2f" % (
       parsed["task-clock"] / 1000,
       parsed["cpu-cycles"] / 1000000.,
       parsed["stalled-cycles-frontend"] / parsed["cpu-cycles"] * 100,
       parsed["cache-references"] / 1000000.,
       parsed["cache-misses"] / parsed["cache-references"] * 100,
       parsed["branches"] / 1000000.,
       parsed["branch-misses"] / parsed["branches"] * 100,
       parsed["L1-dcache-loads"] / 1000000.,
       parsed["L1-dcache-load-misses"] / parsed["L1-dcache-loads"] * 100,
       parsed["L1-dcache-stores"] / 1000000.,
       parsed["L1-dcache-store-misses"] / parsed["L1-dcache-stores"] * 100,
       parsed["LLC-loads"] / 1000000.,
       parsed["LLC-load-misses"] / parsed["LLC-loads"] * 100,
       parsed["LLC-stores"] / 1000000.,
       parsed["LLC-store-misses"] / parsed["LLC-stores"] * 100,
       parsed["dTLB-loads"] / 1000000., 
       parsed["dTLB-load-misses"] / parsed["dTLB-loads"] * 100,
       parsed["dTLB-stores"] / 1000000.,
       parsed["dTLB-store-misses"] / parsed["dTLB-stores"] * 100
    )

    print "Result: ", "CORRECT" if found == 2 else "NOT CORRECT"

