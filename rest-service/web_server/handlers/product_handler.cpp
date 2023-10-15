#include <iostream>
#include <Poco/Net/HTMLForm.h>

#include "../../database/Product.h"
#include "product_handler.h"

using Poco::Net::HTMLForm;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

void ProductHandler::handleRequest(HTTPServerRequest &request, [[maybe_unused]] HTTPServerResponse &response) {

    try {
        // Create product from JSON body
        if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST && request.getContentType() == "application/json") {
            std::string body(std::istreambuf_iterator<char>(request.stream()), {});
            auto product = database::Product::fromJSON(body);

            std::cout << "[INFO] Inserting " << product << std::endl;

            product.Save();

            std::cout << "[INFO] Product: " << product << std::endl;

            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");

            auto& out = response.send();
            Poco::JSON::Stringifier::stringify(product.toJSON(), out);
        }
        else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            HTMLForm form(request, request.stream());


            std::vector<database::Product> products;
            if (form.has("ownerId")) {
                long owner_id = atol(form.get("ownerId").c_str());
                products = database::Product::SelectByOwnerId(owner_id);
            }
            else {
                products = database::Product::SelectAll();
            }

            auto result = Poco::JSON::Array();
            for (auto product : products)
                result.add(product.toJSON());
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            auto& out = response.send();
            Poco::JSON::Stringifier::stringify(result, out);
        }

    //     if (form.has("id") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
    //     {
    //         long id = atol(form.get("id").c_str());

    //         std::optional<database::User> result = database::User::read_by_id(id);
    //         if (result)
    //         {
    //             response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    //             response.setChunkedTransferEncoding(true);
    //             response.setContentType("application/json");
    //             std::ostream &ostr = response.send();
    //             Poco::JSON::Stringifier::stringify(remove_password(result->toJSON()), ostr);
    //             return;
    //         }
    //         else
    //         {
    //             response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
    //             response.setChunkedTransferEncoding(true);
    //             response.setContentType("application/json");
    //             Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
    //             root->set("type", "/errors/not_found");
    //             root->set("title", "Internal exception");
    //             root->set("status", "404");
    //             root->set("detail", "user ot found");
    //             root->set("instance", "/user");
    //             std::ostream &ostr = response.send();
    //             Poco::JSON::Stringifier::stringify(root, ostr);
    //             return;
    //         }
    //     }
    //     else if (hasSubstr(request.getURI(), "/auth"))
    //     {

    //         std::string scheme;
    //         std::string info;
    //         request.getCredentials(scheme, info);
    //         std::cout << "scheme: " << scheme << " identity: " << info << std::endl;

    //         std::string login, password;
    //         if (scheme == "Basic")
    //         {
    //             get_identity(info, login, password);
    //             if (auto id = database::User::auth(login, password))
    //             {
    //                 response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    //                 response.setChunkedTransferEncoding(true);
    //                 response.setContentType("application/json");
    //                 std::ostream &ostr = response.send();
    //                 ostr << "{ \"id\" : \"" << *id << "\"}" << std::endl;
    //                 return;
    //             }
    //         }

    //         response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED);
    //         response.setChunkedTransferEncoding(true);
    //         response.setContentType("application/json");
    //         Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
    //         root->set("type", "/errors/unauthorized");
    //         root->set("title", "Internal exception");
    //         root->set("status", "401");
    //         root->set("detail", "not authorized");
    //         root->set("instance", "/auth");
    //         std::ostream &ostr = response.send();
    //         Poco::JSON::Stringifier::stringify(root, ostr);
    //         return;
    //     }
    //     else if (hasSubstr(request.getURI(), "/search"))
    //     {

    //         std::string fn = form.get("first_name");
    //         std::string ln = form.get("last_name");
    //         auto results = database::User::search(fn, ln);
    //         Poco::JSON::Array arr;
    //         for (auto s : results)
    //             arr.add(remove_password(s.toJSON()));
    //         response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    //         response.setChunkedTransferEncoding(true);
    //         response.setContentType("application/json");
    //         std::ostream &ostr = response.send();
    //         Poco::JSON::Stringifier::stringify(arr, ostr);

    //         return;
    //     }
    //     else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
    //     {
    //         if (form.has("first_name") && form.has("last_name") && form.has("email") && form.has("title") && form.has("login") && form.has("password"))
    //         {
    //             database::User user;
    //             user.first_name() = form.get("first_name");
    //             user.last_name() = form.get("last_name");
    //             user.email() = form.get("email");
    //             user.title() = form.get("title");
    //             user.login() = form.get("login");
    //             user.password() = form.get("password");

    //             bool check_result = true;
    //             std::string message;
    //             std::string reason;

    //             if (!check_name(user.get_first_name(), reason))
    //             {
    //                 check_result = false;
    //                 message += reason;
    //                 message += "<br>";
    //             }

    //             if (!check_name(user.get_last_name(), reason))
    //             {
    //                 check_result = false;
    //                 message += reason;
    //                 message += "<br>";
    //             }

    //             if (!check_email(user.get_email(), reason))
    //             {
    //                 check_result = false;
    //                 message += reason;
    //                 message += "<br>";
    //             }

    //             if (check_result)
    //             {
    //                 user.save_to_mysql();
    //                 response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    //                 response.setChunkedTransferEncoding(true);
    //                 response.setContentType("application/json");
    //                 std::ostream &ostr = response.send();
    //                 ostr << user.get_id();
    //                 return;
    //             }
    //             else
    //             {
    //                 response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    //                 std::ostream &ostr = response.send();
    //                 ostr << message;
    //                 response.send();
    //                 return;
    //             }
    //         }
    //     }
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

