#include <unistd.h>
#include "DataHub.hpp"

int main(int argc, char** argv) {
    std::cout << " main thread id: " << std::this_thread::get_id() << std::endl;
    dashboard::dds::DataHub hub;
    
    while(1) {
        usleep(1000);
    }
    return 0;
}
