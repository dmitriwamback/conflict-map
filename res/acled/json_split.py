
import json
import math

for i in range(2000, 2026):
    input_file = f"acled_data_full_{i}.json"

    with open(input_file, "r", encoding="utf-8") as f:
        root = json.load(f)

    response = root[0]
    events = response["data"]

    chunk_size = math.ceil(len(events) / 3)

    for j in range(3):
        chunk = events[j * chunk_size:(j + 1) * chunk_size]

        output = response.copy()
        output["data"] = chunk

        filename = f"acled_data_{i}_{j + 1}.json"

        with open(filename, "w", encoding="utf-8") as f:
            json.dump(output, f, separators=(",", ":"), ensure_ascii=False)

        print(f"{filename}: {len(chunk)} events")
