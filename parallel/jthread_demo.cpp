#include <thread>
#include <stop_token>
#include <iostream>
#include <future>

void readConfig(std::promise<std::string> p) {
    p.set_value("hello promisze.");
}

int main() {
    // if(__has_cpp_attribute(cpp_lib_jthread)){
    //     std::cout << "valid\n";
    // } else {
    //     std::cout << "invalid\n";
    // }
    // std::stop_source source_;
    // std::jthread th;

    // int v = 0;
    // std::future<std::string> fu = std::async([&](){
    //     if (v == 0) {
    //         std::string rett{""};
    //         return rett;
    //     }

    //     std::string ret = "hello world";
    //     return ret;
    // });
    std::promise<std::string> prom;
    std::future<std::string> ret_future = prom.get_future();
    std::thread th(readConfig, std::move(prom));
    // readConfig(std::move(prom));
    std::cout << ret_future.get() << "\n";
    return 0;
}