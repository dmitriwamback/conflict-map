import pandas as pd

df = pd.read_csv("ACLED Data_2026-10-31_event_date_from_2026-01-01_event_date_to_2026-10-25.csv")
df.to_json("ACLED Data_2026-10-31_event_date_from_2026-01-01_event_date_to_2026-10-25.json", orient="records", indent=2)