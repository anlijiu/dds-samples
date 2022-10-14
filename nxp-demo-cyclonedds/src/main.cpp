#include <sstream>
#include <iostream>
#include <string>
#include <unistd.h>

#include "vhal_compatible.hpp"
#include "DataHub.hpp"

using namespace headunit::dds;

// bool printVehicleProperty(VehiclePropValuePtr ptr) {
//     int32_t prop = ptr->prop;
//     int32_t zone = ptr->areaId;
//     VehiclePropertyType type = getPropType(prop);
// 
//     switch (type) {
//         case VehiclePropertyType::INT32:
//             printf("%s props:%d(%x), zone: %d, int value: %d\n", __func__, prop, prop, zone, ptr->value.int32Values[0]);
//             break;
//         case VehiclePropertyType::BOOLEAN:
//             printf("%s props:%d(%x), zone: %d, boolean value: %s\n", __func__, prop, prop, zone, ptr->value.int32Values[0] ? "true":"false");
//             break;
//         case VehiclePropertyType::INT64:
//             printf("%s props:%d(%x), zone: %d, int64 value: %ld\n", __func__, prop, prop, zone, ptr->value.int64Values[0]);
//             break;
//         case VehiclePropertyType::FLOAT:
//             printf("%s props:%d(%x), zone: %d, float value: %f\n", __func__, prop, prop, zone, ptr->value.floatValues[0]);
//             break;
//         case VehiclePropertyType::STRING:
//             printf("%s props:%d(%x), zone: %d, string value: %s\n", __func__, prop, prop, zone, ptr->value.stringValue.c_str());
//             break;
//         case VehiclePropertyType::FLOAT_VEC:
//             printf("%s props:%d(%x), zone: %d, float vector values: ", __func__, prop, prop, zone);
//             for (auto i = ptr->value.floatValues.begin(); i != ptr->value.floatValues.end(); ++i)
//                 printf("%f ", *i);
//             printf("\n");
//             break;
//         case VehiclePropertyType::INT32_VEC:
//             printf("%s props:%d(%x), zone: %d, int vector values: ", __func__, prop, prop, zone);
//             for (auto i = ptr->value.int32Values.begin(); i != ptr->value.int32Values.end(); ++i)
//                 printf("%d ", *i);
//             printf("\n");
//             break;
//         case VehiclePropertyType::INT64_VEC:
//             printf("%s props:%d(%x), zone: %d, int64 vector values: ", __func__, prop, prop, zone);
//             for (auto i = ptr->value.int64Values.begin(); i != ptr->value.int64Values.end(); ++i)
//                 printf("%ld ", *i);
//             printf("\n");
//             break;
//         case VehiclePropertyType::BYTES:
//             printf("%s props:%d(%x), zone: %d, bytes values: ", __func__, prop, prop, zone);
//             for (auto i = ptr->value.bytes.begin(); i != ptr->value.bytes.end(); ++i)
//                 printf("%02hhx ", *i);
//             printf("\n");
//             break;
//         case VehiclePropertyType::COMPLEX:
//             break;
//         default:
//             break;
//     }
//     return true;
// }
// 
int main(int argc, char *argv[])
{
    std::cout << " main thread id: " << std::this_thread::get_id() << std::endl;

    std::unique_ptr<DataHub> dataHub = std::make_unique<DataHub>();

    std::cin.get();

    // auto adapter = std::make_unique<DDSMessageAdapter>();
    // adapter->bind(std::bind(printVehicleProperty, std::placeholders::_1));
    // adapter->start();
    // 
    // for(std::string line; std::getline(std::cin, line);) {
    //     std::vector<std::string> strings;
    //     std::istringstream f(line);
    //     std::string s;
    //     while (getline(f, s, ':')) {
    //         std::cout << "s:" << s << std::endl;
    //         strings.push_back(s);
    //     }
    //     if(strings.size() > 1) {
    //         VehiclePropValue acValue;
    //         acValue.prop = toInt(VehicleProperty::HVAC_AC_ON);
    //         acValue.areaId = 0;
    //         acValue.value.int32Values.resize(1);
    //         if(strings[1] == "0") {
    //             std::cout<< "00000 main strings[1]:" << strings[1] << std::endl;
    //             acValue.value.int32Values[0] = 0;
    //         } else {
    //             std::cout<< "else main strings[1]:" << strings[1] << std::endl;
    //             acValue.value.int32Values[0] = 1;
    //         }
    // 
    //         StatusCode s = adapter->writeValue(acValue);
    //         std::cout << "main.cpp  writeValue return code: " << s << std::endl;
    //     }
    //     std::cout << line << std::endl;
    // }


    return 0;
}

