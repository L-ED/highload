

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
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
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "delivery_handler.h"
#include "../../database/Delivery.h"


void DeliveryHandler::handleRequest(HTTPServerRequest &request, [[maybe_unused]] HTTPServerResponse &response) {

    try {
        // Create delivery from JSON body
        if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST && request.getContentType() == "application/json") {
            std::string body(std::istreambuf_iterator<char>(request.stream()), {});
            auto delivery = database::Delivery::fromJSON(body);

            std::cout << "[INFO] Inserting " << delivery << std::endl;

            delivery.Save();

            std::cout << "[INFO] Delivery: " << delivery << std::endl;

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");

            auto& out = response.send();
            Poco::JSON::Stringifier::stringify(delivery.toJSON(), out);
        }
        else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            // Select deliveries using filters from query string
            HTMLForm form(request, request.stream());

            std::vector<database::Delivery> deliveries;
            long sender_id = std::numeric_limits<long>::max();
            long reciever_id = std::numeric_limits<long>::max();
            if (form.has("senderId"))
                sender_id = atol(form.get("senderId").c_str());
            if (form.has("recieverId"))
                reciever_id = atol(form.get("recieverId").c_str());

            deliveries = database::Delivery::Select(sender_id, reciever_id);

            auto result = Poco::JSON::Array();
            for (auto delivery : deliveries)
                result.add(delivery.toJSON());
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            auto& out = response.send();
            Poco::JSON::Stringifier::stringify(result, out);
        }

    }
    catch (const std::exception& e) {
        std::cout << "[ERROR] " << e.what() << std::endl;
        response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        response.send();
        return;
    }

    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_FORBIDDEN);
    response.setChunkedTransferEncoding(true);
    response.setContentType("application/json");
    response.send();
}

