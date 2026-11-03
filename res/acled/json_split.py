
import json
import math

input_file = "acled_data.json"

with open(input_file, "r", encoding="utf-8") as f:
    root = json.load(f)

response = root[0]
events = response["data"]

chunk_size = math.ceil(len(events) / 4)

for i in range(4):
    chunk = events[i * chunk_size:(i + 1) * chunk_size]

    output = response.copy()
    output["data"] = chunk

    filename = f"acled_data_{i + 8}.json"

    with open(filename, "w", encoding="utf-8") as f:
        json.dump(output, f, separators=(",", ":"), ensure_ascii=False)

    print(f"{filename}: {len(chunk)} events")
