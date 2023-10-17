import test_user
import test_delivery

test_user.insert_test_data()
test_delivery.insert_test_data(test_delivery.session)
