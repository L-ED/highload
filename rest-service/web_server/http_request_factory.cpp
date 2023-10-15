#include <iostream>

#include "handlers/user_handler.h"
#include "handlers/product_handler.h"
#include "handlers/delivery_handler.h"

#include "http_request_factory.h"

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPRequestHandler;

static bool StartsWith(const std::string &str, const std::string &substr) {
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i) {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}

HTTPRequestHandler* HTTPRequestFactory::createRequestHandler(const HTTPServerRequest& request) {

    std::cout << request.getMethod() << " " << request.getURI()<< std::endl;
    if (StartsWith(request.getURI(),"/api/users") ||
        StartsWith(request.getURI(),"/api/users/search") ||
        StartsWith(request.getURI(),"/api/users/self")) 
        return new UserHandler(format_);

    // Create product => POST /api/products + JSON
    // Get user's products => GET /api/products?&userId=0
    // Create delivery => POST /api/delivery/ + JSON

    // Get delivery info => GET /api/delivery?senderId=0&recieverId=0

    if (StartsWith(request.getURI(), "/api/products"))
        return new AuthRequired<ProductHandler>();

    if (StartsWith(request.getURI(), "/api/deliveries"))
        return new AuthRequired<DeliveryHandler>();

    return 0;
}
