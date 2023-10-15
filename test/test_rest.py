import requests
import json
from base64 import b64encode

BASE_API_URL = 'http://localhost:8080'

def insert_user(user):
    res = requests.post(f'{BASE_API_URL}/api/users', data={
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
    res = requests.get(f'{BASE_API_URL}/api/users/self', headers={
        'Authorization': f'Basic {credentials}'
    })
    res.raise_for_status()
    return res.json()

def user_search(first_name, last_name):
    res = requests.get(f'{BASE_API_URL}/api/users/search', params={
        'first_name': first_name,
        'last_name': last_name
    })
    res.raise_for_status()
    return res.json()

def insert_product(product):
    res = requests.post(f'{BASE_API_URL}/api/products', json={
        'ownerId': product['ownerId'],
        'name': product['name']
    })

    res.raise_for_status()
    return json.loads(res.text)

def get_products(owner_id):
    res = requests.get(f'{BASE_API_URL}/api/products', params={
        'ownerId': owner_id,
    })
    res.raise_for_status()
    return res.json()

def insert_delivery(delivery):
    res = requests.post(f'{BASE_API_URL}/api/deliveries', json={
        'senderId': delivery['senderId'],
        'recieverId': delivery['recieverId'],
        'productId': delivery['productId']
    })

    res.raise_for_status()
    return res.json()

def get_deliveries(sender_id=None, reciever_id=None):
    res = requests.get(f'{BASE_API_URL}/api/deliveries', params={
        'senderId': sender_id,
        'recieverId': reciever_id,
    })
    res.raise_for_status()
    return res.json()

def insert_test_data():
    with open('test/data.json') as file:
        data = json.load(file)

    users = data['User']
    for user in users:
        insert_user(user)

    products = data['Product']
    for product in products:
        insert_product(product)

    deliveries = data['Delivery']
    for delivery in deliveries:
        insert_delivery(delivery)



def test_correct_delivery_filter():
    deliveries = get_deliveries(1, 2)
    for d in deliveries:
        assert d['senderId'] == 1 and d['recieverId'] == 2

    deliveries = get_deliveries(sender_id=1)
    for d in deliveries:
        assert d['senderId'] == 1 and d['recieverId'] == 2

    deliveries = get_deliveries(reciever_id=1)
    for d in deliveries:
        assert d['recieverId'] == 1

def test_correct_user_products():
    products = get_products(1)
    for p in products:
        assert p['ownerId'] == 1

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

# insert_test_data()

test_correct_delivery_filter()
test_correct_user_products()
test_auth()
test_search()

