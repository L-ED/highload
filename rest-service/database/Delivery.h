#pragma once
#include <string>
#include <vector>
#include <optional>

#include <Poco/JSON/Object.h>

namespace database {

    class Delivery {
        private:
            long id_;
            long sender_id_;
            long reciever_id_;
            long product_id_;

        public:
            static void Initialize();

            static Delivery fromJSON(const std::string& str);
            static std::vector<Delivery> SelectAll();
            static std::vector<Delivery> SelectByOwnerId(long owner_id);

            inline long GetId() const { return id_; }
            long GetSenderId() const { return sender_id_; }
            long GetRecieverId() const { return reciever_id_; }

            long& GetId() { return id_; }
            long& GetSenderId() { return sender_id_; }
            long& GetRecieverId() { return reciever_id_; }

            void Save();
            Poco::JSON::Object::Ptr toJSON() const;

            friend std::ostream& operator<< (std::ostream& stream, const Delivery& product);
    };
}