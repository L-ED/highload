import requests
import json
from base64 import b64encode

BASE_OTHER_SERVICE_URL = 'http://localhost:8081'

def get_user_products(login, password):
    credentials = b64encode(f'{login}:{password}'.encode()).decode()
    res = requests.get(f'{BASE_OTHER_SERVICE_URL}/api/users/self', headers={
        'Authorization': f'Basic {credentials}'
    })
    res.raise_for_status()
    return res.json()

# def test_search():
#     users = user_search('user', 'last')
#     assert len(users) > 0

#     users = user_search('not exists', 'last')
#     assert len(users) == 0

# insert_test_data()

# test_correct_delivery_filter()
# test_correct_user_products()
# test_auth()
# test_search()

a = get_user_products('login-1', 'pass-1')
print('User 1 products:', a)


a = get_user_products('login-2', 'pass-2')
print('User 2 products:', a)

try:
    a = get_user_products('not-exists', 'not-exists')
    print('Not exists user test rejected')
except:
    print('Not exists user test passed')

