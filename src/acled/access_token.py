# code directly from acled api documentation: https://acleddata.com/api-documentation/getting-started

import requests

def get_access_token(username, password, token_url):
    headers = {
        'Content-Type': 'application/x-www-form-urlencoded',
    }
    data = {
          'username': username,
          'password': password,
          'grant_type': "password",
          'client_id': "acled",
          'scope': "authenticated"
    }

    response = requests.post(token_url, headers=headers, data=data)

    if response.status_code == 200:
        token_data = response.json()
        return token_data['access_token']
    else:
        raise Exception(f"Failed to get access token: {response.status_code} {response.text}")


# Get an access token
my_token = get_access_token(
    username="username",
    password="password",
    token_url="https://acleddata.com/oauth/token",
)

with open("access_token.txt", "w") as token_file:
    token_file.write(my_token)

print("Access Token:", my_token)