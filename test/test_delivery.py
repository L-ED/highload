import requests
import json
from base64 import b64encode

BASE_DELIVERY_SERVICE_URL = 'http://localhost:8081'


def make_auth_header(login, password):
    credentials = b64encode(f'{login}:{password}'.encode()).decode()
    return f'Basic {credentials}'

def get_user_products(login, password, session):
    res = session.get(f'{BASE_DELIVERY_SERVICE_URL}/api/users/self', headers={
        'Authorization': make_auth_header(login, password)
    })
    res.raise_for_status()
    return res.json()

def insert_product(product, session):
    res = session.post(f'{BASE_DELIVERY_SERVICE_URL}/api/products', json={
        'ownerId': product['ownerId'],
        'name': product['name']
    })

    res.raise_for_status()
    return json.loads(res.text)

def get_products(owner_id, session):
    res = session.get(f'{BASE_DELIVERY_SERVICE_URL}/api/products', params={
        'ownerId': owner_id,
    })
    res.raise_for_status()
    return res.json()

def insert_delivery(delivery, session):
    res = session.post(f'{BASE_DELIVERY_SERVICE_URL}/api/deliveries', json={
        'senderId': delivery['senderId'],
        'recieverId': delivery['recieverId'],
        'productId': delivery['productId']
    })

    res.raise_for_status()
    return res.json()

def get_deliveries(session, sender_id=None, reciever_id=None):
    res = session.get(f'{BASE_DELIVERY_SERVICE_URL}/api/deliveries', params={
        'senderId': sender_id,
        'recieverId': reciever_id,
    })
    res.raise_for_status()
    return res.json()


def insert_test_data(session):
    with open('test/data.json') as file:
        data = json.load(file)

    products = data['Product']
    for product in products:
        insert_product(product, session)

    deliveries = data['Delivery']
    for delivery in deliveries:
        insert_delivery(delivery, session)


def test_correct_delivery_filter(session):
    deliveries = get_deliveries(session, 1, 2)
    for d in deliveries:
        assert d['senderId'] == 1 and d['recieverId'] == 2
    print(deliveries)

    deliveries = get_deliveries(session, sender_id=1)
    for d in deliveries:
        assert d['senderId'] == 1 and d['recieverId'] == 2

    deliveries = get_deliveries(session, reciever_id=1)
    for d in deliveries:
        assert d['recieverId'] == 1

def test_correct_user_products(session):
    products = get_products(1, session)
    print(products)
    for p in products:
        assert p['ownerId'] == 1


session = requests.Session()
session.headers = {
    'Authorization': make_auth_header('admin', 'admin')
}

if __name__ == '__main__':
    # insert_test_data(session)

    test_correct_delivery_filter(session)
    test_correct_user_products(session)
