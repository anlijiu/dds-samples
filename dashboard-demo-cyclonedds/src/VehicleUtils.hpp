#ifndef __VEHICLE_UTILS_HPP__
#define __VEHICLE_UTILS_HPP__

/** Returns underlying (integer) value for given enum. */
template<typename ENUM, typename U = typename std::underlying_type<ENUM>::type>
inline constexpr U toInt(ENUM const value) {
    return static_cast<U>(value);
}

inline constexpr VehiclePropertyType getPropType(int32_t prop) {
    return static_cast<VehiclePropertyType>(
            prop & toInt(VehiclePropertyType::MASK));
}

inline constexpr int32_t getProp(uint64_t propzone) {
    return static_cast<int32_t>(propzone);
}
inline constexpr int32_t getZone(uint64_t propzone) {
    return static_cast<int32_t>(propzone >> 32);
}
inline constexpr uint64_t getPropZone(int32_t prop, int32_t zone) {
    return (static_cast<uint64_t>(zone) << 32) | static_cast<uint64_t>(prop);
}

inline constexpr VehicleArea getPropArea(int32_t prop) {
    return static_cast<VehicleArea>(prop & toInt(VehicleArea::MASK));
}

inline constexpr bool isGlobalProp(int32_t prop) {
    return getPropArea(prop) == VehicleArea::GLOBAL;
}


#endif //__VEHICLE_UTILS_HPP__
