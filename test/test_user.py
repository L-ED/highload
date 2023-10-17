import requests
import json
from base64 import b64encode

BASE_USER_SERVICE_URL = 'http://localhost:8080'

def insert_user(user):
    res = requests.post(f'{BASE_USER_SERVICE_URL}/api/users', data={
        'first_name':   user['firstName'],
        'last_name':    user['lastName'],
        'email':        user['email'],
        'title':        user['title'],
        'login':        user['login'],
        'password':     user['password']
    })

    res.raise_for_status()
    return res.json()

def user_self(login, password):
    credentials = b64encode(f'{login}:{password}'.encode()).decode()
    res = requests.get(f'{BASE_USER_SERVICE_URL}/api/users/self', headers={
        'Authorization': f'Basic {credentials}'
    })
    res.raise_for_status()
    return res.json()

def user_search(first_name, last_name):
    res = requests.get(f'{BASE_USER_SERVICE_URL}/api/users/search', params={
        'first_name': first_name,
        'last_name': last_name
    })
    res.raise_for_status()
    return res.json()

def insert_test_data():
    with open('test/data.json') as file:
        data = json.load(file)

    users = data['User']
    for user in users:
        insert_user(user)


def test_auth():
    user = user_self('login-2', 'pass-2')
    assert user['login'] == 'login-2'

    try:
        user = user_self('login-2', 'pass-3')
        print('Auth test failed')
    except:
        pass

def test_search():
    users = user_search('user', 'last')
    assert len(users) > 0

    users = user_search('not exists', 'last')
    assert len(users) == 0

if __name__ == '__main__':
    # insert_test_data()

    test_auth()
    test_search()
