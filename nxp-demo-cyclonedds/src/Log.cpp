#include "Log.hpp"
#include <iostream>
#include <iomanip>
#include <fmt/chrono.h>
#include <fmt/color.h>



void Log::printVehicleProperty(const VehiclePropValue& propValue) {
    int32_t prop = propValue.prop;
    int32_t zone = propValue.areaId;
    VehiclePropertyType type = getPropType(prop);

    switch (type) {
        case VehiclePropertyType::INT32:
            printf("%s props:%d(%x), zone: %d, int value: %d\n", __func__, prop, prop, zone, propValue.value.int32Values[0]);
            break;
        case VehiclePropertyType::BOOLEAN:
            printf("%s props:%d(%x), zone: %d, boolean value: %s\n", __func__, prop, prop, zone, propValue.value.int32Values[0] ? "true":"false");
            break;
        case VehiclePropertyType::INT64:
            printf("%s props:%d(%x), zone: %d, int64 value: %ld\n", __func__, prop, prop, zone, propValue.value.int64Values[0]);
            break;
        case VehiclePropertyType::FLOAT:
            printf("%s props:%d(%x), zone: %d, float value: %f\n", __func__, prop, prop, zone, propValue.value.floatValues[0]);
            break;
        case VehiclePropertyType::STRING:
            printf("%s props:%d(%x), zone: %d, string value: %s\n", __func__, prop, prop, zone, propValue.value.stringValue.c_str());
            break;
        case VehiclePropertyType::FLOAT_VEC:
            printf("%s props:%d(%x), zone: %d, float vector values: ", __func__, prop, prop, zone);
            for (auto i = propValue.value.floatValues.begin(); i != propValue.value.floatValues.end(); ++i)
                printf("%f ", *i);
            printf("\n");
            break;
        case VehiclePropertyType::INT32_VEC:
            printf("%s props:%d(%x), zone: %d, int vector values: ", __func__, prop, prop, zone);
            for (auto i = propValue.value.int32Values.begin(); i != propValue.value.int32Values.end(); ++i)
                printf("%d ", *i);
            printf("\n");
            break;
        case VehiclePropertyType::INT64_VEC:
            printf("%s props:%d(%x), zone: %d, int64 vector values: ", __func__, prop, prop, zone);
            for (auto i = propValue.value.int64Values.begin(); i != propValue.value.int64Values.end(); ++i)
                printf("%ld ", *i);
            printf("\n");
            break;
        case VehiclePropertyType::BYTES:
            printf("%s props:%d(%x), zone: %d, bytes values: ", __func__, prop, prop, zone);
            for (auto i = propValue.value.bytes.begin(); i != propValue.value.bytes.end(); ++i)
                printf("%02hhx ", *i);
            printf("\n");
            break;
        case VehiclePropertyType::COMPLEX:
            break;
        default:
            break;
    }
}

void Log::printCanFrame(const can_frame_t * frame) {
 //    fmt::printf("canId %04X(%d), data:", canId, canId);
	// fmt::print(fmt::fg(fmt::color::red) | fmt::bg(fmt::color::green), "AppointmentDriveService time : {0} \n", strTime);
    fmt::print(fmt::format("\ncanId: {0:x}({0:d}), data:", frame->can_id));
    
    fmt::print(fmt::format("{0:x}({0:d}), {1:x}({1:d}), {2:x}({2:d}), {3:x}({3:d}), {4:x}({4:d}), {5:x}({5:d}), {6:x}({6:d}), {7:x}({7:d}) \n", frame->data[0], frame->data[1], frame->data[2], frame->data[3], frame->data[4], frame->data[5], frame->data[6], frame->data[7]));

    // for(int i = 0; i < 8; ++i) {
    //     std::cout << static_cast<int>(frame->data[i]);
    //     if(i != 7) {
    //         std::cout << ", ";
    //     } else {
    //         std::cout << "." << std::endl;
    //     }
    // }
    // std::cout << std::dec << std::setw(4) << std::endl;
}
