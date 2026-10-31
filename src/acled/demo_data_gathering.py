import requests

with open("access_token.txt") as f:
    my_token = f.read().strip()

parameters = {
    "_format": "json",
    "country": "Georgia:OR:country=Armenia:OR:country=Azerbaijan",
    "year": 2021,
    "fields": "event_id_cnty|event_date|event_type|country|fatalities",
    "limit": 10,
}

r = requests.get(
    "https://acleddata.com/api/acled/read",
    params=parameters,
    headers={"Authorization": f"Bearer {my_token}"},
)

# if access is denied, you can download the data directly from the ACLED website: https://acleddata.com/conflict-data/data-export-tool

print(r.status_code)
print(r.text[:500])