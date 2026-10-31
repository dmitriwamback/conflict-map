import csv

n_files = 20

with open("GEDEvent_v26_1.csv", newline="") as f:
    total_rows = sum(1 for _ in f) - 1  # minus header

base, extra = divmod(total_rows, n_files)

with open("GEDEvent_v26_1.csv", newline="") as f:
    reader = csv.reader(f)
    header = next(reader)

    for i in range(n_files):
        size = base + (1 if i < extra else 0)
        with open(f"GEDEvent_v26_1_{i+1}.csv", "w", newline="") as out:
            writer = csv.writer(out)
            writer.writerow(header)
            for _ in range(size):
                writer.writerow(next(reader))