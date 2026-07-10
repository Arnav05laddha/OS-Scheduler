import csv
import random
import argparse
import os

def gen(n, max_arr, max_burst, max_prio, seed):
    random.seed(seed)
    rows = []
    for i in range(1, n + 1):
        arr   = random.randint(0, max_arr)
        burst = random.randint(1, max_burst)
        prio  = random.randint(1, max_prio)
        rows.append((i, arr, burst, prio))
    rows.sort(key=lambda r: r[1])
    return rows

def main():
    p = argparse.ArgumentParser(description="Generate scheduling workload CSV")
    p.add_argument("-n",   type=int, default=20,   help="number of processes")
    p.add_argument("-a",   type=int, default=20,   help="max arrival time")
    p.add_argument("-b",   type=int, default=10,   help="max burst time")
    p.add_argument("-pr",  type=int, default=10,   help="max priority")
    p.add_argument("-s",   type=int, default=0,    help="random seed")
    p.add_argument("-o",   type=str, default="data.csv", help="output file")
    args = p.parse_args()

    rows = gen(args.n, args.a, args.b, args.pr, args.s)
    with open(args.o, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["pid", "arrival", "burst", "priority"])
        w.writerows(rows)
    print(f"Written {len(rows)} processes -> {os.path.abspath(args.o)}")
    print("Header: pid, arrival, burst, priority")
    for r in rows[:5]:
        print(f"  {r}")
    if len(rows) > 5:
        print(f"  ... ({len(rows)-5} more)")

if __name__ == "__main__":
    main()
