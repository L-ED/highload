cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET (OTHER_SERVICE "other_service")

set(CMAKE_PREFIX_PATH "third_party/poco/cmake-build/package")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")
link_directories("third_party/poco/cmake-build/package/lib")


SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rest-service
    rest-service/main.cpp 
    rest-service/config/config.cpp 
    rest-service/web_server/handlers/product_handler.cpp
    rest-service/web_server/handlers/delivery_handler.cpp
    rest-service/database/database.cpp
    rest-service/database/user.cpp
    rest-service/database/Product.cpp
    rest-service/database/Delivery.cpp
)


target_include_directories(rest-service PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(rest-service PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(rest-service PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(rest-service PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(rest-service PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)




# add_executable(${OTHER_SERVICE} other_main.cpp)
# target_include_directories(${OTHER_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
# target_compile_options(${OTHER_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
# set_target_properties(${OTHER_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
# set_target_properties(${OTHER_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

# target_link_libraries(${OTHER_SERVICE} PRIVATE 
#                              ${CMAKE_THREAD_LIBS_INIT} 
#                              ${Poco_LIBRARIES}
#                              "PocoData"
#                              "PocoDataMySQL"
#                              "mysqlclient"
#                              ZLIB::ZLIB)





