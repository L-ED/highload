#pragma once

#include <iostream>
#include <iostream>
#include <fstream>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class DeliveryHandler : public Poco::Net::HTTPRequestHandler {
private:
    std::string format_;

public:
    DeliveryHandler(const std::string& format) : format_(format) { }
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};
