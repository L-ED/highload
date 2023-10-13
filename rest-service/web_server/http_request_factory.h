#ifndef HTTPREQUESTFACTORY_H
#define HTTPREQUESTFACTORY_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>

using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

#include "handlers/user_handler.h"
#include "handlers/product_handler.h"
#include "handlers/delivery_handler.h"


static bool StartsWith(const std::string &str, const std::string &substr) {
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i)
    {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}

class HTTPRequestFactory: public HTTPRequestHandlerFactory {
private:
    std::string format_;

public:
    HTTPRequestFactory(const std::string& format) : format_(format) { }

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {

        std::cout << request.getMethod() << " " << request.getURI()<< std::endl;
        if (hasSubstr(request.getURI(),"/api/users") ||
            hasSubstr(request.getURI(),"/search") ||
            hasSubstr(request.getURI(),"/auth")) 
            return new UserHandler(format_);

        // Create product => POST /api/products + JSON
        // Get user's products => GET /api/products?&userId=0
        // Create delivery => POST /api/delivery/ + JSON

        // Get delivery info => GET /api/delivery?senderId=0&recieverId=0

        if (StartsWith(request.getURI(), "/api/products")) {
            return new ProductHandler(format_);
        }

        if (StartsWith(request.getURI(), "/api/deliveries")) {
            return new DeliveryHandler(format_);
        }

        return 0;
    }
};

#endif