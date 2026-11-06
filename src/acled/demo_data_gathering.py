import requests

with open("access_token.txt") as f:
    my_token = f.read().strip()

for i in range(2000, 2026):

    parameters = {
        "_format": "json",
        "year": i,
        "fields": "event_id_cnty|event_date|disorder_type|event_type|country|fatalities|longitude|latitude",
        "limit": 500000,
    }

    r = requests.get(
        "https://acleddata.com/api/acled/read",
        params=parameters,
        headers={"Authorization": f"Bearer {my_token}"},
    )

    # if access is denied, you can download the data directly from the ACLED website: https://acleddata.com/conflict-data/data-export-tool

    print(r.status_code)
    with open(f"acled_data_full_{i}.json", "w") as f:
        f.write(f'[{r.text}]')