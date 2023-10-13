import requests
import json

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

def insert_product(product):
    res = requests.post(f'{BASE_API_URL}/api/products', json={
        'ownerId': product['ownerId'],
        'name': product['name']
    })

    res.raise_for_status()
    return json.loads(res.text)

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
    return res.json();

# insert_product(1, 'wheel')
# insert_product(1, 'car')


# insert_delivery(1, 2, 1)

with open('test/data.json') as file:
    data = json.load(file)

# users = data['User']
# for user in users:
#     insert_user(user)

# products = data['Product']
# for product in products:
#     insert_product(product)

deliveries = data['Delivery']
for delivery in deliveries:
    insert_delivery(delivery)
