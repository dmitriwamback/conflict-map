import pandas as pd

for i in range(17):
    df = pd.read_csv(f"GEDEvent_v26_1_{i+1}.csv")
    df.to_json(f"GEDEvent_v26_1_{i+1}.json", orient="records", indent=2)