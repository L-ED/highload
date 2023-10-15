export REST_SERVICE_HOST=localhost
export REST_SERVICE_PORT=8080
export LD_LIBRARY_PATH=$(pwd)/third_party/poco/cmake-build/package/lib

./build/other-service
