#ifndef __vhal_compatible__hpp__
#define __vhal_compatible__hpp__

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <mutex>
#include <atomic>
#include <functional>
#include <memory>
#include <type_traits>

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];  // NOLINT(readability/casting)

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

constexpr int32_t kAllSupportedAreas = 0;

enum StatusCode : int32_t {
    OK = 0,

    /** Try again. */
    TRY_AGAIN = 1,

    /** Invalid argument provided. */
    INVALID_ARG = 2,

    /**
     * This code must be returned when device that associated with the vehicle
     * property is not available. For example, when client tries to set HVAC
     * temperature when the whole HVAC unit is turned OFF.
     */
    NOT_AVAILABLE = 3,

    /** Access denied */
    ACCESS_DENIED = 4,

    /** Something unexpected has happened in Vehicle HAL */
    INTERNAL_ERROR = 5,

    TIMEOUT = 6,
};

enum class VehicleArea : int32_t {
    GLOBAL = 16777216, // 0x01000000
    ZONE = 33554432, // 0x02000000
    WINDOW = 50331648, // 0x03000000
    MIRROR = 67108864, // 0x04000000
    SEAT = 83886080, // 0x05000000
    DOOR = 100663296, // 0x06000000
    WHEEL = 117440512, // 0x07000000
    BATTERY = 134217728, // 0x08000000
    MASK = 251658240, // 0x0f000000
};

enum class VehiclePropertyType : int32_t {
    STRING = 1048576, // 0x00100000
    BOOLEAN = 2097152, // 0x00200000
    INT32 = 4194304, // 0x00400000
    INT32_VEC = 4259840, // 0x00410000
    INT64 = 5242880, // 0x00500000
    INT64_VEC = 5308416, // 0x00510000
    FLOAT = 6291456, // 0x00600000
    FLOAT_VEC = 6356992, // 0x00610000
    BYTES = 7340032, // 0x00700000
    MIXED = 14745600, // 0x00e10000
    COMPLEX = 14680064, // 0x00e00000
    MASK = 16711680, // 0x00ff0000
};

enum VehiclePropertyUpdateBy {
    CAR = 0x00,
    USER = 0x01,
};

struct VehiclePropValue {
    int64_t timestamp;
    int32_t areaId;
    int32_t prop;
    struct RawValue {
        std::vector<int32_t> int32Values;

        /**
         * This is used for properties of types VehiclePropertyType#FLOAT
         * and VehiclePropertyType#FLOAT_VEC
         */
        std::vector<float> floatValues;

        /** This is used for properties of type VehiclePropertyType#INT64 */
        std::vector<int64_t> int64Values;

        /** This is used for properties of type VehiclePropertyType#BYTES */
        std::vector<uint8_t> bytes;

        /** This is used for properties of type VehiclePropertyType#STRING */
        std::string stringValue;
    };

    RawValue value;

    VehiclePropertyUpdateBy updateBy;
};

enum VehiclePropertyAccess : int32_t {
    NONE = 0x00,

    READ = 0x01,
    WRITE = 0x02,
    READ_WRITE = 0x03,
};

enum VehiclePropertyChangeMode : int32_t {
  /**
   * Property of this type must never be changed. Subscription is not supported
   * for these properties.
   */
  STATIC = 0x00,

  /**
   * Property of this type must be reported when there is a change.
   * IVehicle#get call must return the current value.
   * Set operation for this property is assumed to be asynchronous. When the
   * property is read (using IVehicle#get) after IVehicle#set, it may still
   * return old value until underlying H/W backing this property has actually
   * changed the state. Once state is changed, the property must dispatch
   * changed value as event.
   */
  ON_CHANGE = 0x01,

  /**
   * Property of this type change continuously and requires fixed rate of
   * sampling to retrieve the data.
   */
  CONTINUOUS = 0x02,

  /**
   * Property of this type may be polled to get the current value.
   */
  POLL = 0x03,

  /**
   * This is for property where change event must be sent only when the
   * value is set from external component. Normal value change must not trigger
   * event. For example, clock property can send change event only when it is
   * set, outside android, for case like user setting time or time getting
   * update. There is no need to send it per every value change.
   */
  ON_SET = 0x04,
};

struct VehicleAreaConfig {
    /**
     * Area id is ignored for VehiclePropertyGroup:GLOBAL properties.
     */
    int32_t areaId;

    int32_t minInt32Value;
    int32_t maxInt32Value;

    int64_t minInt64Value;
    int64_t maxInt64Value;

    float minFloatValue;
    float maxFloatValue;
};

/** property 协议类型 */
enum PropertyChannelType : int32_t {
    /** CAN */
    CAN = 0x00,
    /** DDS */
    DDS = 0x01,
};

struct VehiclePropConfig {
    /** Property identifier */
    int32_t prop;

    /**
     * Defines if the property is read or write or both.
     */
    VehiclePropertyAccess access;

    /**
     * Defines the change mode of the property.
     */
    VehiclePropertyChangeMode changeMode;

    /**
     * Some of the properties may have associated areas (for example, some hvac
     * properties are associated with VehicleAreaZone), in these
     * cases the config may contain an ORed value for the associated areas.
     */
    int32_t supportedAreas;

    /**
     * Contains per-area configuration.
     */
    std::vector<VehicleAreaConfig> areaConfigs;

    /**
     * Configuration flags for this property.
     *
     * For example, it may store the number of presets that are stored by the
     * radio module.
     */
    int32_t configFlags;

    /** Contains additional configuration parameters */
    std::vector<int32_t> configArray;

    /**
     * Some properties may require additional information passed over this
     * string. Most properties do not need to set this.
     */
    std::string configString;

    /**
     * Min sample rate in Hz.
     * Must be defined for VehiclePropertyChangeMode::CONTINUOUS
     */
    float minSampleRate;

    /**
     * Must be defined for VehiclePropertyChangeMode::CONTINUOUS
     * Max sample rate in Hz.
     */
    float maxSampleRate;

    PropertyChannelType channelType;
};

enum class VehiclePropertyGroup : int32_t {
    SYSTEM = 268435456, // 0x10000000
    VENDOR = 536870912, // 0x20000000
    MASK = -268435456, // 0xf0000000
};


enum class VehicleProperty : int32_t;





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

inline constexpr VehiclePropertyGroup getPropGroup(int32_t prop) {
    return static_cast<VehiclePropertyGroup>(
            prop & toInt(VehiclePropertyGroup::MASK));
}

inline constexpr VehicleArea getPropArea(int32_t prop) {
    return static_cast<VehicleArea>(prop & toInt(VehicleArea::MASK));
}

inline constexpr bool isGlobalProp(int32_t prop) {
    return getPropArea(prop) == VehicleArea::GLOBAL;
}

inline constexpr bool isSystemProperty(int32_t prop) {
    return VehiclePropertyGroup::SYSTEM == getPropGroup(prop);
}

std::unique_ptr<VehiclePropValue> createVehiclePropValue(
    VehiclePropertyType type, size_t vecSize);

size_t getVehicleRawValueVectorSize(
    const VehiclePropValue::RawValue& value, VehiclePropertyType type);

void copyVehicleRawValue(VehiclePropValue::RawValue* dest,
                                const VehiclePropValue::RawValue& src);

template<typename T>
void shallowCopyHidlVec(std::vector<T>* dest, const std::vector<T>& src);

void shallowCopyHidlStr(std::string* dest, const std::string& src);

void shallowCopy(VehiclePropValue* dest, const VehiclePropValue& src);

bool isEqual(const VehiclePropValue::RawValue& value1, const VehiclePropValue::RawValue& value2, VehiclePropertyType type);


uint64_t elapsedRealtimeNano();
std::chrono::time_point<std::chrono::high_resolution_clock> get_time_point(uint64_t timestamp);














enum class VehicleProperty : int32_t {
    /**
     * Undefined property.
     */
    INVALID = 0 /* 0x00000000 */,
    /**
     * VIN of vehicle
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    INFO_VIN = 286261504 /* (0x0100 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:STRING | VehicleArea:GLOBAL) */,
    /**
     * Manufacturer of vehicle
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    INFO_MAKE = 286261505 /* (0x0101 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:STRING | VehicleArea:GLOBAL) */,
    /**
     * Model of vehicle
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    INFO_MODEL = 286261506 /* (0x0102 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:STRING | VehicleArea:GLOBAL) */,
    /**
     * Model year of vehicle.
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:YEAR
     */
    INFO_MODEL_YEAR = 289407235 /* (0x0103 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Fuel capacity of the vehicle in milliliters
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:MILLILITER
     */
    INFO_FUEL_CAPACITY = 291504388 /* (0x0104 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * List of fuels the vehicle may use
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @data_enum FuelType
     */
    INFO_FUEL_TYPE = 289472773 /* (0x0105 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32_VEC | VehicleArea:GLOBAL) */,
    /**
     * Battery capacity of the vehicle, if EV or hybrid.  This is the nominal
     * battery capacity when the vehicle is new.
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:WH
     */
    INFO_EV_BATTERY_CAPACITY = 291504390 /* (0x0106 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * List of connectors this EV may use
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @data_enum EvConnectorType
     * @access VehiclePropertyAccess:READ
     */
    INFO_EV_CONNECTOR_TYPE = 289472775 /* (0x0107 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32_VEC | VehicleArea:GLOBAL) */,
    /**
     * Fuel door location
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @data_enum PortLocationType
     * @access VehiclePropertyAccess:READ
     */
    INFO_FUEL_DOOR_LOCATION = 289407240 /* (0x0108 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * EV port location
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @data_enum PortLocationType
     */
    INFO_EV_PORT_LOCATION = 289407241 /* (0x0109 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Driver's seat location
     * VHAL implementations must ignore the areaId. Use VehicleArea:GLOBAL.
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @data_enum VehicleAreaSeat
     * @access VehiclePropertyAccess:READ
     */
    INFO_DRIVER_SEAT = 356516106 /* (0x010A | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Exterior dimensions of vehicle.
     *
     *  int32Values[0] = height
     *  int32Values[1] = length
     *  int32Values[2] = width
     *  int32Values[3] = width including mirrors
     *  int32Values[4] = wheel base
     *  int32Values[5] = track width front
     *  int32Values[6] = track width rear
     *  int32Values[7] = curb to curb turning radius
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:MILLIMETER
     */
    INFO_EXTERIOR_DIMENSIONS = 289472779 /* (0x010B | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32_VEC | VehicleArea:GLOBAL) */,
    /**
     * Multiple EV port locations
     *
     * Implement this property if the vehicle has multiple EV ports.
     * Port locations are defined in PortLocationType.
     * For example, a car has one port in front left and one port in rear left:
     *   int32Values[0] = PortLocationType::FRONT_LEFT
     *   int32Values[0] = PortLocationType::REAR_LEFT
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @data_enum PortLocationType
     */
    INFO_MULTI_EV_PORT_LOCATIONS = 289472780 /* (0x010C | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32_VEC | VehicleArea:GLOBAL) */,
    /**
     * Current odometer value of the vehicle
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:KILOMETER
     */
    PERF_ODOMETER = 291504644 /* (0x0204 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Speed of the vehicle
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:METER_PER_SEC
     */
    PERF_VEHICLE_SPEED = 291504647 /* (0x0207 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Speed of the vehicle for displays
     *
     * Some cars display a slightly slower speed than the actual speed.  This is
     * usually displayed on the speedometer.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:METER_PER_SEC
     */
    PERF_VEHICLE_SPEED_DISPLAY = 291504648 /* (0x0208 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Front bicycle model steering angle for vehicle
     *
     * Angle is in degrees.  Left is negative.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:DEGREES
     */
    PERF_STEERING_ANGLE = 291504649 /* (0x0209 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Rear bicycle model steering angle for vehicle
     *
     * Angle is in degrees.  Left is negative.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:DEGREES
     */
    PERF_REAR_STEERING_ANGLE = 291504656 /* (0x0210 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Temperature of engine coolant
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:CELSIUS
     */
    ENGINE_COOLANT_TEMP = 291504897 /* (0x0301 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Engine oil level
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleOilLevel
     */
    ENGINE_OIL_LEVEL = 289407747 /* (0x0303 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Temperature of engine oil
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:CELSIUS
     */
    ENGINE_OIL_TEMP = 291504900 /* (0x0304 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Engine rpm
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:RPM
     */
    ENGINE_RPM = 291504901 /* (0x0305 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Reports wheel ticks
     *
     * The first element in the vector is a reset count.  A reset indicates
     * previous tick counts are not comparable with this and future ones.  Some
     * sort of discontinuity in tick counting has occurred.
     *
     * The next four elements represent ticks for individual wheels in the
     * following order: front left, front right, rear right, rear left.  All
     * tick counts are cumulative.  Tick counts increment when the vehicle
     * moves forward, and decrement when vehicles moves in reverse.  The ticks
     * should be reset to 0 when the vehicle is started by the user.
     *
     *  int64Values[0] = reset count
     *  int64Values[1] = front left ticks
     *  int64Values[2] = front right ticks
     *  int64Values[3] = rear right ticks
     *  int64Values[4] = rear left ticks
     *
     * configArray is used to indicate the micrometers-per-wheel-tick value and
     * which wheels are supported.  configArray is set as follows:
     *
     *  configArray[0], bits [0:3] = supported wheels.  Uses enum Wheel.
     *  configArray[1] = micrometers per front left wheel tick
     *  configArray[2] = micrometers per front right wheel tick
     *  configArray[3] = micrometers per rear right wheel tick
     *  configArray[4] = micrometers per rear left wheel tick
     *
     * NOTE:  If a wheel is not supported, its value shall always be set to 0.
     *
     * VehiclePropValue.timestamp must be correctly filled in.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     */
    WHEEL_TICK = 290521862 /* (0x0306 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT64_VEC | VehicleArea:GLOBAL) */,
    /**
     * Fuel remaining in the the vehicle, in milliliters
     *
     * Value may not exceed INFO_FUEL_CAPACITY
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:MILLILITER
     */
    FUEL_LEVEL = 291504903 /* (0x0307 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Fuel door open
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    FUEL_DOOR_OPEN = 287310600 /* (0x0308 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * EV battery level in WH, if EV or hybrid
     *
     * Value may not exceed INFO_EV_BATTERY_CAPACITY
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:WH
     */
    EV_BATTERY_LEVEL = 291504905 /* (0x0309 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * EV charge port open
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    EV_CHARGE_PORT_OPEN = 287310602 /* (0x030A | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * EV charge port connected
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    EV_CHARGE_PORT_CONNECTED = 287310603 /* (0x030B | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * EV instantaneous charge rate in milliwatts
     *
     * Positive value indicates battery is being charged.
     * Negative value indicates battery being discharged.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:MW
     */
    EV_BATTERY_INSTANTANEOUS_CHARGE_RATE = 291504908 /* (0x030C | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Range remaining
     *
     * Meters remaining of fuel and charge.  Range remaining shall account for
     * all energy sources in a vehicle.  For example, a hybrid car's range will
     * be the sum of the ranges based on fuel and battery.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ_WRITE
     * @unit VehicleUnit:METER
     */
    RANGE_REMAINING = 291504904 /* (0x0308 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,
    /**
     * Tire pressure
     *
     * Each tires is identified by its areaConfig.areaId config and their
     * minFloatValue/maxFloatValue are used to store OEM recommended pressure
     * range.
     * The Min value in the areaConfig data represents the lower bound of
     * the recommended tire pressure.
     * The Max value in the areaConfig data represents the upper bound of
     * the recommended tire pressure.
     * For example:
     * The following areaConfig indicates the recommended tire pressure
     * of left_front tire is from 200.0 KILOPASCAL to 240.0 KILOPASCAL.
     * .areaConfigs = {
     *      VehicleAreaConfig {
     *          .areaId = VehicleAreaWheel::LEFT_FRONT,
     *          .minFloatValue = 200.0,
     *          .maxFloatValue = 240.0,
     *      }
     * },
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:KILOPASCAL
     */
    TIRE_PRESSURE = 392168201 /* (0x0309 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:WHEEL) */,
    /**
     * Currently selected gear
     *
     * This is the gear selected by the user.
     *
     * Values in the config data must represent the list of supported gears
     * for this vehicle.  For example, config data for an automatic transmission
     * must contain {GEAR_NEUTRAL, GEAR_REVERSE, GEAR_PARK, GEAR_DRIVE,
     * GEAR_1, GEAR_2,...} and for manual transmission the list must be
     * {GEAR_NEUTRAL, GEAR_REVERSE, GEAR_1, GEAR_2,...}
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleGear
     */
    GEAR_SELECTION = 289408000 /* (0x0400 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Current gear. In non-manual case, selected gear may not
     * match the current gear. For example, if the selected gear is GEAR_DRIVE,
     * the current gear will be one of GEAR_1, GEAR_2 etc, which reflects
     * the actual gear the transmission is currently running in.
     *
     * Values in the config data must represent the list of supported gears
     * for this vehicle.  For example, config data for an automatic transmission
     * must contain {GEAR_NEUTRAL, GEAR_REVERSE, GEAR_PARK, GEAR_1, GEAR_2,...}
     * and for manual transmission the list must be
     * {GEAR_NEUTRAL, GEAR_REVERSE, GEAR_1, GEAR_2,...}. This list need not be the
     * same as that of the supported gears reported in GEAR_SELECTION.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleGear
     */
    CURRENT_GEAR = 289408001 /* (0x0401 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Parking brake state.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    PARKING_BRAKE_ON = 287310850 /* (0x0402 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * Auto-apply parking brake.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    PARKING_BRAKE_AUTO_APPLY = 287310851 /* (0x0403 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * Warning for fuel low level.
     *
     * This property corresponds to the low fuel warning on the dashboard.
     * Once FUEL_LEVEL_LOW is set, it should not be cleared until more fuel is
     * added to the vehicle.  This property may take into account all fuel
     * sources for a vehicle - for example:
     *
     *   For a gas powered vehicle, this property is based soley on gas level.
     *   For a battery powered vehicle, this property is based solely on battery level.
     *   For a hybrid vehicle, this property may be based on the combination of gas and battery
     *      levels, at the OEM's discretion.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    FUEL_LEVEL_LOW = 287310853 /* (0x0405 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * Night mode
     *
     * True indicates that the night mode sensor has detected that the car cabin environment has
     * low light. The platform could use this, for example, to enable appropriate UI for
     * better viewing in dark or low light environments.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    NIGHT_MODE = 287310855 /* (0x0407 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * State of the vehicles turn signals
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleTurnSignal
     */
    TURN_SIGNAL_STATE = 289408008 /* (0x0408 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Represents ignition state
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleIgnitionState
     */
    IGNITION_STATE = 289408009 /* (0x0409 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * ABS is active
     *
     * Set to true when ABS is active.  Reset to false when ABS is off.  This
     * property may be intermittently set (pulsing) based on the real-time
     * state of the ABS system.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    ABS_ACTIVE = 287310858 /* (0x040A | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * Traction Control is active
     *
     * Set to true when traction control (TC) is active.  Reset to false when
     * TC is off.  This property may be intermittently set (pulsing) based on
     * the real-time state of the TC system.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    TRACTION_CONTROL_ACTIVE = 287310859 /* (0x040B | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /*
     * HVAC Properties
     *
     * Additional rules for mapping a zoned HVAC property (except
     * HVAC_MAX_DEFROST_ON) to AreaIDs:
     *  - Every seat in VehicleAreaSeat that is available in the car, must be
     *    part of an AreaID in the AreaID array.
     *
     * Example 1: A car has two front seats (ROW_1_LEFT, ROW_1_RIGHT) and three
     *  back seats (ROW_2_LEFT, ROW_2_CENTER, ROW_2_RIGHT). There are two
     *  temperature control units -- driver side and passenger side.
     *   - A valid mapping set of AreaIDs for HVAC_TEMPERATURE_SET would be a
     *     two element array:
     *      - ROW_1_LEFT  | ROW_2_LEFT
     *      - ROW_1_RIGHT | ROW_2_CENTER | ROW_2_RIGHT
     *   - An alternative mapping for the same hardware configuration would be:
     *      - ROW_1_LEFT  | ROW_2_CENTER | ROW_2_LEFT
     *      - ROW_1_RIGHT | ROW_2_RIGHT
     *  The temperature controllers are assigned to the seats which they
     *  "most influence", but every seat must be included exactly once. The
     *  assignment of the center rear seat to the left or right AreaID may seem
     *  arbitrary, but the inclusion of every seat in exactly one AreaID ensures
     *  that the seats in the car are all expressed and that a "reasonable" way
     *  to affect each seat is available.
     *
     * Example 2: A car has three seat rows with two seats in the front row (ROW_1_LEFT,
     *  ROW_1_RIGHT) and three seats in the second (ROW_2_LEFT, ROW_2_CENTER,
     *  ROW_2_RIGHT) and third rows (ROW_3_LEFT, ROW_3_CENTER, ROW_3_RIGHT). There
     *  are three temperature control units -- driver side, passenger side, and rear.
     *   - A reasonable way to map HVAC_TEMPERATURE_SET to AreaIDs is a three
     *     element array:
     *     - ROW_1_LEFT
     *     - ROW_1_RIGHT
     *     - ROW_2_LEFT | ROW_2_CENTER | ROW_2_RIGHT | ROW_3_LEFT | ROW_3_CENTER | ROW_3_RIGHT
     *
     *
     * Fan speed setting
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_FAN_SPEED = 356517120 /* (0x0500 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Fan direction setting
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleHvacFanDirection
     */
    HVAC_FAN_DIRECTION = 356517121 /* (0x0501 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * HVAC current temperature.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:CELSIUS
     */
    // HVAC_TEMPERATURE_CURRENT = 358614274 /* (0x0502 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:SEAT) */,
    /**
     * HVAC, target temperature set.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @unit VehicleUnit:CELSIUS
     */
    // HVAC_TEMPERATURE_SET = 358614275 /* (0x0503 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:FLOAT | VehicleArea:SEAT) */,
    /**
     * Fan-based defrost for designated window.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_DEFROSTER = 320865540 /* (0x0504 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:WINDOW) */,
    /**
     * On/off AC for designated areaId
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @config_flags Supported areaIds
     */
    // HVAC_AC_ON = 354419973 /* (0x0505 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,
    /**
     * On/off max AC
     *
     * When MAX AC is on, the ECU may adjust the vent position, fan speed,
     * temperature, etc as necessary to cool the vehicle as quickly as possible.
     * Any parameters modified as a side effect of turning on/off the MAX AC
     * parameter shall generate onPropertyEvent() callbacks to the VHAL.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_MAX_AC_ON = 354419974 /* (0x0506 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,
    /**
     * On/off max defrost
     *
     * When MAX DEFROST is on, the ECU may adjust the vent position, fan speed,
     * temperature, etc as necessary to defrost the windows as quickly as
     * possible.  Any parameters modified as a side effect of turning on/off
     * the MAX DEFROST parameter shall generate onPropertyEvent() callbacks to
     * the VHAL.
     * The AreaIDs for HVAC_MAX_DEFROST_ON indicate MAX DEFROST can be controlled
     * in the area.
     * For example:
     * areaConfig.areaId = {ROW_1_LEFT | ROW_1_RIGHT} indicates HVAC_MAX_DEFROST_ON
     * only can be controlled for the front rows.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_MAX_DEFROST_ON = 354419975 /* (0x0507 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,
    /**
     * Recirculation on/off
     *
     * Controls the supply of exterior air to the cabin.  Recirc “on” means the
     * majority of the airflow into the cabin is originating in the cabin.
     * Recirc “off” means the majority of the airflow into the cabin is coming
     * from outside the car.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_RECIRC_ON = 354419976 /* (0x0508 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,
    /**
     * Enable temperature coupling between areas.
     *
     * The AreaIDs for HVAC_DUAL_ON property shall contain a combination of
     * HVAC_TEMPERATURE_SET AreaIDs that can be coupled together. If
     * HVAC_TEMPERATURE_SET is mapped to AreaIDs [a_1, a_2, ..., a_n], and if
     * HVAC_DUAL_ON can be enabled to couple a_i and a_j, then HVAC_DUAL_ON
     * property must be mapped to [a_i | a_j]. Further, if a_k and a_l can also
     * be coupled together separately then HVAC_DUAL_ON must be mapped to
     * [a_i | a_j, a_k | a_l].
     *
     * Example: A car has two front seats (ROW_1_LEFT, ROW_1_RIGHT) and three
     *  back seats (ROW_2_LEFT, ROW_2_CENTER, ROW_2_RIGHT). There are two
     *  temperature control units -- driver side and passenger side -- which can
     *  be optionally synchronized. This may be expressed in the AreaIDs this way:
     *  - HVAC_TEMPERATURE_SET->[ROW_1_LEFT | ROW_2_LEFT, ROW_1_RIGHT | ROW_2_CENTER | ROW_2_RIGHT]
     *  - HVAC_DUAL_ON->[ROW_1_LEFT | ROW_2_LEFT | ROW_1_RIGHT | ROW_2_CENTER | ROW_2_RIGHT]
     *
     * When the property is enabled, the ECU must synchronize the temperature
     * for the affected areas. Any parameters modified as a side effect
     * of turning on/off the DUAL_ON parameter shall generate
     * onPropertyEvent() callbacks to the VHAL. In addition, if setting
     * a temperature (i.e. driver's temperature) changes another temperature
     * (i.e. front passenger's temperature), then the appropriate
     * onPropertyEvent() callbacks must be generated.  If a user changes a
     * temperature that breaks the coupling (e.g. setting the passenger
     * temperature independently) then the VHAL must send the appropriate
     * onPropertyEvent() callbacks (i.e. HVAC_DUAL_ON = false,
     * HVAC_TEMPERATURE_SET[AreaID] = xxx, etc).
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_DUAL_ON = 354419977 /* (0x0509 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,
    /**
     * On/off automatic mode
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_AUTO_ON = 354419978 /* (0x050A | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,
    /**
     * Seat heating/cooling
     *
     * Negative values indicate cooling.
     * 0 indicates off.
     * Positive values indicate heating.
     *
     * Some vehicles may have multiple levels of heating and cooling. The
     * min/max range defines the allowable range and number of steps in each
     * direction.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_SEAT_TEMPERATURE = 356517131 /* (0x050B | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Side Mirror Heat
     *
     * Increasing values denote higher heating levels for side mirrors.
     * The Max value in the config data represents the highest heating level.
     * The Min value in the config data MUST be zero and indicates no heating.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_SIDE_MIRROR_HEAT = 339739916 /* (0x050C | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:MIRROR) */,
    /**
     * Temperature units for display
     *
     * Indicates whether the vehicle is displaying temperature to the user as
     * Celsius or Fahrenheit.
     * VehiclePropConfig.configArray is used to indicate the supported temperature display units.
     * For example: configArray[0] = CELSIUS
     *              configArray[1] = FAHRENHEIT
     *
     * This parameter MAY be used for displaying any HVAC temperature in the system.
     * Values must be one of VehicleUnit::CELSIUS or VehicleUnit::FAHRENHEIT
     * Note that internally, all temperatures are represented in floating point Celsius.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleUnit
     */
    HVAC_TEMPERATURE_DISPLAY_UNITS = 289408270 /* (0x050E | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Actual fan speed
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    HVAC_ACTUAL_FAN_SPEED_RPM = 356517135 /* (0x050F | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Represents global power state for HVAC. Setting this property to false
     * MAY mark some properties that control individual HVAC features/subsystems
     * to UNAVAILABLE state. Setting this property to true MAY mark some
     * properties that control individual HVAC features/subsystems to AVAILABLE
     * state (unless any/all of them are UNAVAILABLE on their own individual
     * merits).
     *
     * [Definition] HvacPower_DependentProperties: Properties that need HVAC to be
     *   powered on in order to enable their functionality. For example, in some cars,
     *   in order to turn on the AC, HVAC must be powered on first.
     *
     * HvacPower_DependentProperties list must be set in the
     * VehiclePropConfig.configArray. HvacPower_DependentProperties must only contain
     * properties that are associated with VehicleArea:SEAT. Properties that are not
     * associated with VehicleArea:SEAT, for example, HVAC_DEFROSTER, must never
     * depend on HVAC_POWER_ON property and must never be part of
     * HvacPower_DependentProperties list.
     *
     * AreaID mapping for HVAC_POWER_ON property must contain all AreaIDs that
     * HvacPower_DependentProperties are mapped to.
     *
     * Example 1: A car has two front seats (ROW_1_LEFT, ROW_1_RIGHT) and three back
     *  seats (ROW_2_LEFT, ROW_2_CENTER, ROW_2_RIGHT). If the HVAC features (AC,
     *  Temperature etc.) throughout the car are dependent on a single HVAC power
     *  controller then HVAC_POWER_ON must be mapped to
     *  [ROW_1_LEFT | ROW_1_RIGHT | ROW_2_LEFT | ROW_2_CENTER | ROW_2_RIGHT].
     *
     * Example 2: A car has two seats in the front row (ROW_1_LEFT, ROW_1_RIGHT) and
     *   three seats in the second (ROW_2_LEFT, ROW_2_CENTER, ROW_2_RIGHT) and third
     *   rows (ROW_3_LEFT, ROW_3_CENTER, ROW_3_RIGHT). If the car has temperature
     *   controllers in the front row which can operate entirely independently of
     *   temperature controllers in the back of the vehicle, then HVAC_POWER_ON
     *   must be mapped to a two element array:
     *   - ROW_1_LEFT | ROW_1_RIGHT
     *   - ROW_2_LEFT | ROW_2_CENTER | ROW_2_RIGHT | ROW_3_LEFT | ROW_3_CENTER | ROW_3_RIGHT
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_POWER_ON = 354419984 /* (0x0510 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,
    /**
     * Fan Positions Available
     *
     * This is a bit mask of fan positions available for the zone.  Each
     * available fan direction is denoted by a separate entry in the vector.  A
     * fan direction may have multiple bits from vehicle_hvac_fan_direction set.
     * For instance, a typical car may have the following fan positions:
     *   - FAN_DIRECTION_FACE (0x1)
     *   - FAN_DIRECTION_FLOOR (0x2)
     *   - FAN_DIRECTION_FACE | FAN_DIRECTION_FLOOR (0x3)
     *   - FAN_DIRECTION_DEFROST (0x4)
     *   - FAN_DIRECTION_FLOOR | FAN_DIRECTION_DEFROST (0x6)
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleHvacFanDirection
     */
    HVAC_FAN_DIRECTION_AVAILABLE = 356582673 /* (0x0511 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32_VEC | VehicleArea:SEAT) */,
    /**
     * Automatic recirculation on/off
     *
     * When automatic recirculation is ON, the HVAC system may automatically
     * switch to recirculation mode if the vehicle detects poor incoming air
     * quality.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_AUTO_RECIRC_ON = 354419986 /* (0x0512 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,
    /**
     * Seat ventilation
     *
     * 0 indicates off.
     * Positive values indicates ventilation level.
     *
     * Used by HVAC apps and Assistant to enable, change, or read state of seat
     * ventilation.  This is different than seating cooling. It can be on at the
     * same time as cooling, or not.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_SEAT_VENTILATION = 356517139 /* (0x0513 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Electric defrosters' status
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_ELECTRIC_DEFROSTER_ON = 320865556 /* (0x0514 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:WINDOW) */,
    /**
     * Distance units for display
     *
     * Indicates which units the car is using to display distances to the user. Eg. Mile, Meter
     * Kilometer.
     *
     * Distance units are defined in VehicleUnit.
     * VehiclePropConfig.configArray is used to indicate the supported distance display units.
     * For example: configArray[0] = METER
     *              configArray[1] = KILOMETER
     *              configArray[2] = MILE
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleUnit
     */
    DISTANCE_DISPLAY_UNITS = 289408512 /* (0x0600 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Fuel volume units for display
     *
     * Indicates which units the car is using to display fuel volume to the user. Eg. Liter or
     * Gallon.
     *
     * VehiclePropConfig.configArray is used to indicate the supported fuel volume display units.
     * Volume units are defined in VehicleUnit.
     * For example: configArray[0] = LITER
     *              configArray[1] = GALLON
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleUnit
     */
    FUEL_VOLUME_DISPLAY_UNITS = 289408513 /* (0x0601 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Tire pressure units for display
     *
     * Indicates which units the car is using to display tire pressure to the user. Eg. PSI, Bar or
     * Kilopascal.
     *
     * VehiclePropConfig.configArray is used to indicate the supported pressure display units.
     * Pressure units are defined in VehicleUnit.
     * For example: configArray[0] = KILOPASCAL
     *              configArray[1] = PSI
     *              configArray[2] = BAR
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleUnit
     */
    TIRE_PRESSURE_DISPLAY_UNITS = 289408514 /* (0x0602 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * EV battery units for display
     *
     * Indicates which units the car is using to display EV battery information to the user. Eg.
     * watt-hours(Wh), kilowatt-hours(kWh) or ampere-hours(Ah).
     *
     * VehiclePropConfig.configArray is used to indicate the supported electrical energy units.
     * Electrical energy units are defined in VehicleUnit.
     * For example: configArray[0] = WATT_HOUR
     *              configArray[1] = AMPERE_HOURS
     *              configArray[2] = KILOWATT_HOUR
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleUnit
     */
    EV_BATTERY_DISPLAY_UNITS = 289408515 /* (0x0603 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Fuel consumption units for display
     *
     * Indicates type of units the car is using to display fuel consumption information to user
     * True indicates units are distance over volume such as MPG.
     * False indicates units are volume over distance such as L/100KM.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    FUEL_CONSUMPTION_UNITS_DISTANCE_OVER_VOLUME = 287311364 /* (0x0604 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * Speed units for display
     *
     * Indicates type of units the car is using to display speed to user. Eg. m/s, km/h, or mph.
     *
     * VehiclePropConfig.configArray is used to indicate the supported speed display units.
     * Pressure units are defined in VehicleUnit.
     * For example: configArray[0] = METER_PER_SEC
     *              configArray[1] = MILES_PER_HOUR
     *              configArray[2] = KILOMETERS_PER_HOUR
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    VEHICLE_SPEED_DISPLAY_UNITS = 289408517 /* (0x0605 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Property to control power state of application processor
     *
     * It is assumed that AP's power state is controlled by a separate power
     * controller.
     *
     * For configuration information, VehiclePropConfig.configArray can have bit flag combining
     * values in VehicleApPowerStateConfigFlag.
     *
     *   int32Values[0] : VehicleApPowerStateReq enum value
     *   int32Values[1] : additional parameter relevant for each state,
     *                    0 if not used.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    AP_POWER_STATE_REQ = 289475072 /* (0x0A00 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32_VEC | VehicleArea:GLOBAL) */,
    /**
     * Property to report power state of application processor
     *
     * It is assumed that AP's power state is controller by separate power
     * controller.
     *
     *   int32Values[0] : VehicleApPowerStateReport enum value
     *   int32Values[1] : Time in ms to wake up, if necessary.  Otherwise 0.
     *
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    AP_POWER_STATE_REPORT = 289475073 /* (0x0A01 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32_VEC | VehicleArea:GLOBAL) */,
    /**
     * Property to report bootup reason for the current power on. This is a
     * static property that will not change for the whole duration until power
     * off. For example, even if user presses power on button after automatic
     * power on with door unlock, bootup reason must stay with
     * VehicleApPowerBootupReason#USER_UNLOCK.
     *
     * int32Values[0] must be VehicleApPowerBootupReason.
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    AP_POWER_BOOTUP_REASON = 289409538 /* (0x0A02 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Property to represent brightness of the display. Some cars have single
     * control for the brightness of all displays and this property is to share
     * change in that control.
     *
     * If this is writable, android side can set this value when user changes
     * display brightness from Settings. If this is read only, user may still
     * change display brightness from Settings, but that must not be reflected
     * to other displays.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    DISPLAY_BRIGHTNESS = 289409539 /* (0x0A03 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Property to feed H/W input events to android
     *
     * int32Values[0] : action defined by VehicleHwKeyInputAction
     * int32Values[1] : key code, must use standard android key code
     * int32Values[2] : target display defined in VehicleDisplay. Events not
     *                  tied to specific display must be sent to
     *                  VehicleDisplay#MAIN.
     * int32Values[3] : [optional] Number of ticks. The value must be equal or
     *                  greater than 1. When omitted, Android will default to 1.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @config_flags
     */
    HW_KEY_INPUT = 289475088 /* (0x0A10 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32_VEC | VehicleArea:GLOBAL) */,
    /**
     * Property to feed H/W rotary events to android
     *
     * int32Values[0] : RotaryInputType identifying which rotary knob rotated
     * int32Values[1] : number of detents (clicks), positive for clockwise,
     *                  negative for counterclockwise
     * int32Values[2] : target display defined in VehicleDisplay. Events not
     *                  tied to specific display must be sent to
     *                  VehicleDisplay#MAIN.
     * int32values[3 .. 3 + abs(number of detents) - 2]:
     *                  nanosecond deltas between pairs of consecutive detents,
     *                  if the number of detents is > 1 or < -1
     *
     * VehiclePropValue.timestamp: when the rotation occurred. If the number of
     *                             detents is > 1 or < -1, this is when the
     *                             first detent of rotation occurred.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @data_enum RotaryInputType
     * @access VehiclePropertyAccess:READ
     */
    HW_ROTARY_INPUT = 289475104 /* (0x0A20 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32_VEC | VehicleArea:GLOBAL) */,
    /**
     * Most Car Cabin properties have both a POSition and MOVE parameter.  These
     * are used to control the various movements for seats, doors, and windows
     * in a vehicle.
     *
     * A POS parameter allows the user to set the absolution position.  For
     * instance, for a door, 0 indicates fully closed and max value indicates
     * fully open.  Thus, a value halfway between min and max must indicate
     * the door is halfway open.
     *
     * A MOVE parameter moves the device in a particular direction.  The sign
     * indicates direction, and the magnitude indicates speed (if multiple
     * speeds are available).  For a door, a move of -1 will close the door, and
     * a move of +1 will open it.  Once a door reaches the limit of open/close,
     * the door should automatically stop moving.  The user must NOT need to
     * send a MOVE(0) command to stop the door at the end of its range.
     *
     *
     * Door position
     *
     * This is an integer in case a door may be set to a particular position.
     * Max value indicates fully open, min value (0) indicates fully closed.
     *
     * Some vehicles (minivans) can open the door electronically.  Hence, the
     * ability to write this property.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    DOOR_POS = 373295872 /* (0x0B00 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:DOOR) */,
    /**
     * Door move
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    DOOR_MOVE = 373295873 /* (0x0B01 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:DOOR) */,
    /**
     * Door lock
     *
     * 'true' indicates door is locked
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    DOOR_LOCK = 371198722 /* (0x0B02 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:DOOR) */,
    /**
     * Mirror Z Position
     *
     * Positive value indicates tilt upwards, negative value is downwards
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_Z_POS = 339741504 /* (0x0B40 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:MIRROR) */,
    /**
     * Mirror Z Move
     *
     * Positive value indicates tilt upwards, negative value is downwards
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_Z_MOVE = 339741505 /* (0x0B41 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:MIRROR) */,
    /**
     * Mirror Y Position
     *
     * Positive value indicate tilt right, negative value is left
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_Y_POS = 339741506 /* (0x0B42 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:MIRROR) */,
    /**
     * Mirror Y Move
     *
     * Positive value indicate tilt right, negative value is left
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_Y_MOVE = 339741507 /* (0x0B43 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:MIRROR) */,
    /**
     * Mirror Lock
     *
     * True indicates mirror positions are locked and not changeable
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_LOCK = 287312708 /* (0x0B44 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * Mirror Fold
     *
     * True indicates mirrors are folded
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_FOLD = 287312709 /* (0x0B45 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * Seat memory select
     *
     * This parameter selects the memory preset to use to select the seat
     * position. The minValue is always 0, and the maxValue determines the
     * number of seat positions available.
     *
     * For instance, if the driver's seat has 3 memory presets, the maxValue
     * will be 3. When the user wants to select a preset, the desired preset
     * number (1, 2, or 3) is set.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:WRITE
     */
    SEAT_MEMORY_SELECT = 356518784 /* (0x0B80 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat memory set
     *
     * This setting allows the user to save the current seat position settings
     * into the selected preset slot.  The maxValue for each seat position
     * must match the maxValue for SEAT_MEMORY_SELECT.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:WRITE
     */
    SEAT_MEMORY_SET = 356518785 /* (0x0B81 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seatbelt buckled
     *
     * True indicates belt is buckled.
     *
     * Write access indicates automatic seat buckling capabilities.  There are
     * no known cars at this time, but you never know...
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BELT_BUCKLED = 354421634 /* (0x0B82 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,
    /**
     * Seatbelt height position
     *
     * Adjusts the shoulder belt anchor point.
     * Max value indicates highest position
     * Min value indicates lowest position
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BELT_HEIGHT_POS = 356518787 /* (0x0B83 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seatbelt height move
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BELT_HEIGHT_MOVE = 356518788 /* (0x0B84 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat fore/aft position
     *
     * Sets the seat position forward (closer to steering wheel) and backwards.
     * Max value indicates closest to wheel, min value indicates most rearward
     * position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_FORE_AFT_POS = 356518789 /* (0x0B85 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat fore/aft move
     *
     * Moves the seat position forward and aft.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_FORE_AFT_MOVE = 356518790 /* (0x0B86 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat backrest angle 1 position
     *
     * Backrest angle 1 is the actuator closest to the bottom of the seat.
     * Max value indicates angling forward towards the steering wheel.
     * Min value indicates full recline.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BACKREST_ANGLE_1_POS = 356518791 /* (0x0B87 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat backrest angle 1 move
     *
     * Moves the backrest forward or recline.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BACKREST_ANGLE_1_MOVE = 356518792 /* (0x0B88 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat backrest angle 2 position
     *
     * Backrest angle 2 is the next actuator up from the bottom of the seat.
     * Max value indicates angling forward towards the steering wheel.
     * Min value indicates full recline.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BACKREST_ANGLE_2_POS = 356518793 /* (0x0B89 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat backrest angle 2 move
     *
     * Moves the backrest forward or recline.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BACKREST_ANGLE_2_MOVE = 356518794 /* (0x0B8A | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat height position
     *
     * Sets the seat height.
     * Max value indicates highest position.
     * Min value indicates lowest position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEIGHT_POS = 356518795 /* (0x0B8B | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat height move
     *
     * Moves the seat height.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEIGHT_MOVE = 356518796 /* (0x0B8C | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat depth position
     *
     * Sets the seat depth, distance from back rest to front edge of seat.
     * Max value indicates longest depth position.
     * Min value indicates shortest position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_DEPTH_POS = 356518797 /* (0x0B8D | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat depth move
     *
     * Adjusts the seat depth.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_DEPTH_MOVE = 356518798 /* (0x0B8E | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat tilt position
     *
     * Sets the seat tilt.
     * Max value indicates front edge of seat higher than back edge.
     * Min value indicates front edge of seat lower than back edge.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_TILT_POS = 356518799 /* (0x0B8F | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Seat tilt move
     *
     * Tilts the seat.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_TILT_MOVE = 356518800 /* (0x0B90 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Lumber fore/aft position
     *
     * Pushes the lumbar support forward and backwards
     * Max value indicates most forward position.
     * Min value indicates most rearward position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_LUMBAR_FORE_AFT_POS = 356518801 /* (0x0B91 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Lumbar fore/aft move
     *
     * Adjusts the lumbar support.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_LUMBAR_FORE_AFT_MOVE = 356518802 /* (0x0B92 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Lumbar side support position
     *
     * Sets the amount of lateral lumbar support.
     * Max value indicates widest lumbar setting (i.e. least support)
     * Min value indicates thinnest lumbar setting.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_LUMBAR_SIDE_SUPPORT_POS = 356518803 /* (0x0B93 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Lumbar side support move
     *
     * Adjusts the amount of lateral lumbar support.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_LUMBAR_SIDE_SUPPORT_MOVE = 356518804 /* (0x0B94 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Headrest height position
     *
     * Sets the headrest height.
     * Max value indicates tallest setting.
     * Min value indicates shortest setting.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_HEIGHT_POS = 289409941 /* (0x0B95 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Headrest height move
     *
     * Moves the headrest up and down.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_HEIGHT_MOVE = 356518806 /* (0x0B96 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Headrest angle position
     *
     * Sets the angle of the headrest.
     * Max value indicates most upright angle.
     * Min value indicates shallowest headrest angle.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_ANGLE_POS = 356518807 /* (0x0B97 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Headrest angle move
     *
     * Adjusts the angle of the headrest
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_ANGLE_MOVE = 356518808 /* (0x0B98 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Headrest fore/aft position
     *
     * Adjusts the headrest forwards and backwards.
     * Max value indicates position closest to front of car.
     * Min value indicates position closest to rear of car.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_FORE_AFT_POS = 356518809 /* (0x0B99 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Headrest fore/aft move
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_FORE_AFT_MOVE = 356518810 /* (0x0B9A | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Window Position
     *
     * Min = window up / closed
     * Max = window down / open
     *
     * For a window that may open out of plane (i.e. vent mode of sunroof) this
     * parameter will work with negative values as follows:
     *  Max = sunroof completely open
     *  0 = sunroof closed.
     *  Min = sunroof vent completely open
     *
     *  Note that in this mode, 0 indicates the window is closed.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    WINDOW_POS = 322964416 /* (0x0BC0 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:WINDOW) */,
    /**
     * Window Move
     *
     * Max = Open the window as fast as possible
     * Min = Close the window as fast as possible
     * Magnitude denotes relative speed.  I.e. +2 is faster than +1 in closing
     * the window.
     *
     * For a window that may open out of plane (i.e. vent mode of sunroof) this
     * parameter will work as follows:
     *
     * If sunroof is open:
     *   Max = open the sunroof further, automatically stop when fully open.
     *   Min = close the sunroof, automatically stop when sunroof is closed.
     *
     * If vent is open:
     *   Max = close the vent, automatically stop when vent is closed.
     *   Min = open the vent further, automatically stop when vent is fully open.
     *
     * If sunroof is in the closed position:
     *   Max = open the sunroof, automatically stop when sunroof is fully open.
     *   Min = open the vent, automatically stop when vent is fully open.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    WINDOW_MOVE = 322964417 /* (0x0BC1 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:WINDOW) */,
    /**
     * Window Lock
     *
     * True indicates windows are locked and can't be moved.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    WINDOW_LOCK = 320867268 /* (0x0BC4 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:WINDOW) */,
    /**
     * Vehicle Maps Service (VMS) message
     *
     * This property uses MIXED data to communicate vms messages.
     *
     * Its contents are to be interpreted as follows:
     * the indices defined in VmsMessageIntegerValuesIndex are to be used to
     * read from int32Values;
     * bytes is a serialized VMS message as defined in the vms protocol
     * which is opaque to the framework;
     *
     * IVehicle#get must always return StatusCode::NOT_AVAILABLE.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    VEHICLE_MAP_SERVICE = 299895808 /* (0x0C00 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,
    /**
     * OBD2 Live Sensor Data
     *
     * Reports a snapshot of the current (live) values of the OBD2 sensors available.
     *
     * The configArray is set as follows:
     *   configArray[0] = number of vendor-specific integer-valued sensors
     *   configArray[1] = number of vendor-specific float-valued sensors
     *
     * The values of this property are to be interpreted as in the following example.
     * Considering a configArray = {2,3}
     * int32Values must be a vector containing Obd2IntegerSensorIndex.LAST_SYSTEM_INDEX + 2
     * elements (that is, 33 elements);
     * floatValues must be a vector containing Obd2FloatSensorIndex.LAST_SYSTEM_INDEX + 3
     * elements (that is, 73 elements);
     *
     * It is possible for each frame to contain a different subset of sensor values, both system
     * provided sensors, and vendor-specific ones. In order to support that, the bytes element
     * of the property value is used as a bitmask,.
     *
     * bytes must have a sufficient number of bytes to represent the total number of possible
     * sensors (in this case, 14 bytes to represent 106 possible values); it is to be read as
     * a contiguous bitmask such that each bit indicates the presence or absence of a sensor
     * from the frame, starting with as many bits as the size of int32Values, immediately
     * followed by as many bits as the size of floatValues.
     *
     * For example, should bytes[0] = 0x4C (0b01001100) it would mean that:
     *   int32Values[0 and 1] are not valid sensor values
     *   int32Values[2 and 3] are valid sensor values
     *   int32Values[4 and 5] are not valid sensor values
     *   int32Values[6] is a valid sensor value
     *   int32Values[7] is not a valid sensor value
     * Should bytes[5] = 0x61 (0b01100001) it would mean that:
     *   int32Values[32] is a valid sensor value
     *   floatValues[0 thru 3] are not valid sensor values
     *   floatValues[4 and 5] are valid sensor values
     *   floatValues[6] is not a valid sensor value
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    OBD2_LIVE_FRAME = 299896064 /* (0x0D00 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,
    /**
     * OBD2 Freeze Frame Sensor Data
     *
     * Reports a snapshot of the value of the OBD2 sensors available at the time that a fault
     * occurred and was detected.
     *
     * A configArray must be provided with the same meaning as defined for OBD2_LIVE_FRAME.
     *
     * The values of this property are to be interpreted in a similar fashion as those for
     * OBD2_LIVE_FRAME, with the exception that the stringValue field may contain a non-empty
     * diagnostic troubleshooting code (DTC).
     *
     * A IVehicle#get request of this property must provide a value for int64Values[0].
     * This will be interpreted as the timestamp of the freeze frame to retrieve. A list of
     * timestamps can be obtained by a IVehicle#get of OBD2_FREEZE_FRAME_INFO.
     *
     * Should no freeze frame be available at the given timestamp, a response of NOT_AVAILABLE
     * must be returned by the implementation. Because vehicles may have limited storage for
     * freeze frames, it is possible for a frame request to respond with NOT_AVAILABLE even if
     * the associated timestamp has been recently obtained via OBD2_FREEZE_FRAME_INFO.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    OBD2_FREEZE_FRAME = 299896065 /* (0x0D01 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,
    /**
     * OBD2 Freeze Frame Information
     *
     * This property describes the current freeze frames stored in vehicle
     * memory and available for retrieval via OBD2_FREEZE_FRAME.
     *
     * The values are to be interpreted as follows:
     * each element of int64Values must be the timestamp at which a a fault code
     * has been detected and the corresponding freeze frame stored, and each
     * such element can be used as the key to OBD2_FREEZE_FRAME to retrieve
     * the corresponding freeze frame.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    OBD2_FREEZE_FRAME_INFO = 299896066 /* (0x0D02 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,
    /**
     * OBD2 Freeze Frame Clear
     *
     * This property allows deletion of any of the freeze frames stored in
     * vehicle memory, as described by OBD2_FREEZE_FRAME_INFO.
     *
     * The configArray is set as follows:
     *  configArray[0] = 1 if the implementation is able to clear individual freeze frames
     *                   by timestamp, 0 otherwise
     *
     * IVehicle#set of this property is to be interpreted as follows:
     *   if int64Values contains no elements, then all frames stored must be cleared;
     *   if int64Values contains one or more elements, then frames at the timestamps
     *   stored in int64Values must be cleared, and the others not cleared. Should the
     *   vehicle not support selective clearing of freeze frames, this latter mode must
     *   return NOT_AVAILABLE.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:WRITE
     */
    OBD2_FREEZE_FRAME_CLEAR = 299896067 /* (0x0D03 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,
    /**
     * Headlights State
     *
     * Return the current state of headlights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    HEADLIGHTS_STATE = 289410560 /* (0x0E00 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * High beam lights state
     *
     * Return the current state of high beam lights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    HIGH_BEAM_LIGHTS_STATE = 289410561 /* (0x0E01 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Fog light state
     *
     * Return the current state of fog lights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    FOG_LIGHTS_STATE = 289410562 /* (0x0E02 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Hazard light status
     *
     * Return the current status of hazard lights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    HAZARD_LIGHTS_STATE = 289410563 /* (0x0E03 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Headlight switch
     *
     * The setting that the user wants.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    HEADLIGHTS_SWITCH = 289410576 /* (0x0E10 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * High beam light switch
     *
     * The setting that the user wants.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    HIGH_BEAM_LIGHTS_SWITCH = 289410577 /* (0x0E11 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Fog light switch
     *
     * The setting that the user wants.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    FOG_LIGHTS_SWITCH = 289410578 /* (0x0E12 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Hazard light switch
     *
     * The setting that the user wants.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    HAZARD_LIGHTS_SWITCH = 289410579 /* (0x0E13 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Cabin lights
     *
     * Return current status of cabin lights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    CABIN_LIGHTS_STATE = 289410817 /* (0x0F01 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Cabin lights switch
     *
     * The position of the physical switch which controls the cabin lights.
     * This might be different than the CABIN_LIGHTS_STATE if the lights are on because a door
     * is open or because of a voice command.
     * For example, while the switch is in the "off" or "automatic" position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    CABIN_LIGHTS_SWITCH = 289410818 /* (0x0F02 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,
    /**
     * Reading lights
     *
     * Return current status of reading lights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    READING_LIGHTS_STATE = 356519683 /* (0x0F03 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Reading lights switch
     *
     * The position of the physical switch which controls the reading lights.
     * This might be different than the READING_LIGHTS_STATE if the lights are on because a door
     * is open or because of a voice command.
     * For example, while the switch is in the "off" or "automatic" position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    READING_LIGHTS_SWITCH = 356519684 /* (0x0F04 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * Support customize permissions for vendor properties
     *
     * Implement this property if vehicle hal support customize vendor permissions feature.
     * VehiclePropConfig.configArray is used to indicate vendor properties and permissions
     * which selected for this vendor property. The permission must be one of enum in
     * VehicleVendorPermission.
     * The configArray is set as follows:
     *      configArray[n] = propId : property ID for the vendor property
     *      configArray[n+1] = one of enums in VehicleVendorPermission. It indicates the permission
     *      for reading value of the property.
     *      configArray[n+2] = one of enums in VehicleVendorPermission. It indicates the permission
     *      for writing value of the property.
     *
     * For example:
     * configArray = {
     *      vendor_prop_1, PERMISSION_VENDOR_SEAT_READ, PERMISSION_VENDOR_SEAT_WRITE,
     *      vendor_prop_2, PERMISSION_VENDOR_INFO, PERMISSION_NOT_ACCESSIBLE,
     * }
     * If vendor properties are not in this array, they will have the default vendor permission.
     * If vendor chose PERMISSION_NOT_ACCESSIBLE, android will not have access to the property. In
     * the example, Android can not write value for vendor_prop_2.
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    SUPPORT_CUSTOMIZE_VENDOR_PERMISSION = 287313669 /* (0x0F05 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:BOOLEAN | VehicleArea:GLOBAL) */,
    /**
     * Allow disabling optional featurs from vhal.
     *
     * This property reports optional features that should be disabled.
     * All allowed optional features for the system is declared in Car service overlay,
     * config_allowed_optional_car_features.
     * This property allows disabling features defined in the overlay. Without this property,
     * all the features declared in the overlay will be enabled.
     *
     * Value read should include all features disabled with ',' separation.
     * ex) "com.android.car.user.CarUserNoticeService,storage_monitoring"
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    DISABLED_OPTIONAL_FEATURES = 286265094 /* (0x0F06 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:STRING | VehicleArea:GLOBAL) */,
    /**
     * Defines the initial Android user to be used during initialization.
     *
     * This property is called by the Android system when it initializes and it lets the HAL
     * define which Android user should be started.
     *
     * This request is made by setting a VehiclePropValue (defined by InitialUserInfoRequest),
     * and the HAL must respond with a property change event (defined by InitialUserInfoResponse).
     * If the HAL doesn't respond after some time (defined by the Android system), the Android
     * system will proceed as if HAL returned a response of action
     * InitialUserInfoResponseAction:DEFAULT.
     *
     * For example, on first boot, the request could be:
     *
     * int32[0]: 42  // request id (arbitrary number set by Android system)
     * int32[1]: 1   // InitialUserInfoRequestType::FIRST_BOOT
     * int32[2]: 0   // id of current user (usersInfo.currentUser.userId)
     * int32[3]: 1   // flag of current user (usersInfo.currentUser.flags = SYSTEM)
     * int32[4]: 1   // number of existing users (usersInfo.numberUsers);
     * int32[5]: 0   // user #0  (usersInfo.existingUsers[0].userId)
     * int32[6]: 1   // flags of user #0  (usersInfo.existingUsers[0].flags)
     *
     * And if the HAL want to respond with the creation of an admin user called "Owner", the
     * response would be:
     *
     * int32[0]: 42      // must match the request id from the request
     * int32[1]:  2      // action = InitialUserInfoResponseAction::CREATE
     * int32[2]: -10000  // userToSwitchOrCreate.userId (not used as user will be created)
     * int32[3]:  8      // userToSwitchOrCreate.flags = ADMIN
     * string: "||Owner" // userLocales + separator + userNameToCreate
     *
     * Notice the string value represents multiple values, separated by ||. The first value is the
     * (optional) system locales for the user to be created (in this case, it's empty, meaning it
     * will use Android's default value), while the second value is the (also optional) name of the
     * to user to be created (when the type of response is InitialUserInfoResponseAction:CREATE).
     * For example, to create the same "Owner" user with "en-US" and "pt-BR" locales, the string
     * value of the response would be "en-US,pt-BR||Owner". As such, neither the locale nor the
     * name can have || on it, although a single | is fine.
     *
     * NOTE: if the HAL doesn't support user management, then it should not define this property,
     * which in turn would disable the other user-related properties (for example, the Android
     * system would never issue them and user-related requests from the HAL layer would be ignored
     * by the Android System). But if it supports user management, then it must support all core
     * user-related properties (INITIAL_USER_INFO, SWITCH_USER, CREATE_USER, and REMOVE_USER).
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    INITIAL_USER_INFO = 299896583 /* (0x0F07 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,
    /**
     * Defines a request to switch the foreground Android user.
     *
     * This property is used primarily by the Android System to inform the HAL that the
     * current foreground Android user is switching, but it could also be used by the HAL to request
     * the Android system to switch users - the
     *
     * When the request is made by Android, it sets a VehiclePropValue and the HAL must responde
     * with a property change event; when the HAL is making the request, it must also do it through
     * a property change event (the main difference is that the request id will be positive in the
     * former case, and negative in the latter; the SwitchUserMessageType will also be different).
     *
     * The format of both request is defined by SwitchUserRequest and the format of the response
     * (when needed) is defined by SwitchUserResponse. How the HAL (or Android System) should
     * proceed depends on the message type (which is defined by the SwitchUserMessageType
     * parameter), as defined below.
     *
     * 1.LEGACY_ANDROID_SWITCH
     * -----------------------
     *
     * Called by the Android System to indicate the Android user is about to change, when the change
     * request was made in a way that is not integrated with the HAL (for example, through
     * adb shell am switch-user).
     *
     * The HAL can switch its internal user once it receives this request, but it doesn't need to
     * reply back to the Android System. If its internal user cannot be changed for some reason,
     * then it must wait for the SWITCH_USER(type=ANDROID_POST_SWITCH) call to recover
     * (for example, it could issue a SWITCH_USER(type=VEHICLE_REQUEST) to switch back to
     * the previous user), but ideally it should never fail (as switching back could result in a
     * confusing experience for the end user).
     *
     * For example, if the system have users (0, 10, 11) and it's switching from 0 to 11 (where none
     * of them have any special flag), the request would be:
     *
     * int32[0]:  42  // request id
     * int32[1]:  1   // SwitchUserMessageType::LEGACY_ANDROID_SWITCH
     * int32[2]:  11  // target user id
     * int32[3]:  0   // target user flags (none)
     * int32[4]:  10  // current user
     * int32[5]:  0   // current user flags (none)
     * int32[6]:  3   // number of users
     * int32[7]:  0   // user #0 (Android user id 0)
     * int32[8]:  0   // flags of user #0 (none)
     * int32[9]:  10  // user #1 (Android user id 10)
     * int32[10]: 0   // flags of user #1 (none)
     * int32[11]: 11  // user #2 (Android user id 11)
     * int32[12]: 0   // flags of user #2 (none)
     *
     * 2.ANDROID_SWITCH
     * ----------------
     * Called by the Android System to indicate the Android user is about to change, but Android
     * will wait for the HAL's response (up to some time) before proceeding.
     *
     * The HAL must switch its internal user once it receives this request, then respond back to
     * Android with a SWITCH_USER(type=VEHICLE_RESPONSE) indicating whether its internal
     * user was switched or not (through the SwitchUserStatus enum).
     *
     * For example, if Android has users (0, 10, 11) and it's switching from 10 to 11 (where
     * none of them have any special flag), the request would be:
     *
     * int32[0]:  42  // request id
     * int32[1]:  2   // SwitchUserMessageType::ANDROID_SWITCH
     * int32[2]:  11  // target user id
     * int32[3]:  0   // target user flags (none)
     * int32[4]:  10  // current user
     * int32[5]:  0   // current user flags (none)
     * int32[6]:  3   // number of users
     * int32[7]:  0   // 1st user (user 0)
     * int32[8]:  1   // 1st user flags (SYSTEM)
     * int32[9]:  10  // 2nd user (user 10)
     * int32[10]: 0   // 2nd user flags (none)
     * int32[11]: 11  // 3rd user (user 11)
     * int32[12]: 0   // 3rd user flags (none)
     *
     * If the request succeeded, the HAL must update the propery with:
     *
     * int32[0]: 42  // request id
     * int32[1]: 3   // messageType = SwitchUserMessageType::VEHICLE_RESPONSE
     * int32[2]: 1   // status = SwitchUserStatus::SUCCESS
     *
     * But if it failed, the response would be something like:
     *
     * int32[0]: 42   // request id
     * int32[1]: 3    // messageType = SwitchUserMessageType::VEHICLE_RESPONSE
     * int32[2]: 2    // status = SwitchUserStatus::FAILURE
     * string: "108-D'OH!" // OEM-spefic error message
     *
     * 3.VEHICLE_RESPONSE
     * ------------------
     * Called by the HAL to indicate whether a request of type ANDROID_SWITCH should proceed or
     * abort - see the ANDROID_SWITCH section above for more info.
     *
     * 4.VEHICLE_REQUEST
     * ------------------
     * Called by the HAL to request that the current foreground Android user is switched.
     *
     * This is useful in situations where Android started as one user, but the vehicle identified
     * the driver as another user. For example, user A unlocked the car using the key fob of user B;
     * the INITIAL_USER_INFO request returned user B, but then a face recognition subsubsystem
     * identified the user as A.
     *
     * The HAL makes this request by a property change event (passing a negative request id), and
     * the Android system will response by issue an ANDROID_POST_SWITCH call which the same
     * request id.
     *
     * For example, if the current foreground Android user is 10 and the HAL asked it to switch to
     * 11, the request would be:
     *
     * int32[0]: -108  // request id
     * int32[1]: 4     // messageType = SwitchUserMessageType::VEHICLE_REQUEST
     * int32[2]: 11    // Android user id
     *
     * If the request succeeded and Android has 3 users (0, 10, 11), the response would be:
     *
     * int32[0]: -108 // request id
     * int32[1]:  5   // messageType = SwitchUserMessageType::ANDROID_POST_SWITCH
     * int32[2]:  11  // target user id
     * int32[3]:  0   // target user id flags (none)
     * int32[4]:  11  // current user
     * int32[5]:  0   // current user flags (none)
     * int32[6]:  3   // number of users
     * int32[7]:  0   // 1st user (user 0)
     * int32[8]:  0   // 1st user flags (none)
     * int32[9]:  10  // 2nd user (user 10)
     * int32[10]: 4   // 2nd user flags (none)
     * int32[11]: 11  // 3rd user (user 11)
     * int32[12]: 3   // 3rd user flags (none)
     *
     * Notice that both the current and target user ids are the same - if the request failed, then
     * they would be different (i.e, target user would be 11, but current user would still be 10).
     *
     * 5.ANDROID_POST_SWITCH
     * ---------------------
     * Called by the Android System after a request to switch a user was made.
     *
     * This property is called after switch requests of any type (i.e., LEGACY_ANDROID_SWITCH,
     * ANDROID_SWITCH, or VEHICLE_REQUEST) and can be used to determine if the request succeeded or
     * failed:
     *
     * 1. When it succeeded, it's called when the Android user is in the unlocked state and the
     *    value of the current and target users ids in the response are the same. This would be
     *    equivalent to receiving an Intent.ACTION_USER_UNLOCKED in an Android app.
     * 2. When it failed it's called right away and the value of the current and target users ids
     *    in the response are different (as the current user didn't change to the target).
     * 3. If a new switch request is made before the HAL responded to the previous one or before
     *    the user was unlocked, then the ANDROID_POST_SWITCH request is not made. For example,
     *    the driver could accidentally switch to the wrong user which has lock credentials, then
     *    switch to the right one before entering the credentials.
     *
     * The HAL can update its internal state once it receives this request, but it doesn't need to
     * reply back to the Android System.
     *
     * Request: the first N values as defined by INITIAL_USER_INFO (where the request-specific
     * value at index 1 is SwitchUserMessageType::ANDROID_POST_SWITCH), then 2 more values for the
     * target user id (i.e., the Android user id that was requested to be switched to) and its flags
     * (as defined by  UserFlags).
     *
     * Response: none.
     *
     * Example: see VEHICLE_REQUEST section above.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SWITCH_USER = 299896584 /* (0x0F08 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,
    /**
     * Called by the Android System after an Android user was created.
     *
     * The HAL can use this property to create its equivalent user.
     *
     * This is an async request: Android makes the request by setting a VehiclePropValue, and HAL
     * must respond with a property change indicating whether the request succeeded or failed. If
     * it failed, the Android system will remove the user.
     *
     * The format of the request is defined by CreateUserRequest and the format of the response by
     * CreateUserResponse.
     *
     * For example, if system had 2 users (0 and 10) and a 3rd one (which is an ephemeral guest) was
     * created, the request would be:
     *
     * int32[0]: 42  // request id
     * int32[1]: 11  // Android id of the created user
     * int32[2]: 3   // Android flags (ephemeral guest) of the created user
     * int32[3]: 10  // current user
     * int32[4]: 0   // current user flags (none)
     * int32[5]: 3   // number of users
     * int32[6]: 0   // 1st user (user 0)
     * int32[7]: 0   // 1st user flags (none)
     * int32[8]: 10  // 2nd user (user 10)
     * int32[9]: 0   // 2nd user flags (none)
     * int32[19]: 11 // 3rd user (user 11)
     * int32[11]: 3  // 3rd user flags (ephemeral guest)
     * string: "ElGuesto" // name of the new user
     *
     * Then if the request succeeded, the HAL would return:
     *
     * int32[0]: 42  // request id
     * int32[1]: 1   // CreateUserStatus::SUCCESS
     *
     * But if it failed:
     *
     * int32[0]: 42  // request id
     * int32[1]: 2   // CreateUserStatus::FAILURE
     * string: "D'OH!" // The meaning is a blackbox - it's passed to the caller (like Settings UI),
     *                 // which in turn can take the proper action.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    CREATE_USER = 299896585 /* (0x0F09 | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,
    /**
     * Called by the Android System after an Android user was removed.
     *
     * The HAL can use this property to remove its equivalent user.
     *
     * This is write-only call - the Android System is not expecting a reply from the HAL. Hence,
     * this request should not fail - if the equivalent HAL user cannot be removed, then HAL should
     * mark it as inactive or recover in some other way.
     *
     * The request is made by setting the VehiclePropValue with the contents defined by
     * RemoveUserRequest.
     *
     * For example, if system had 3 users (0, 10, and 11) and user 11 was removed, the request
     * would be:
     *
     * int32[0]: 42  // request id
     * int32[1]: 11  // (Android user id of the removed user)
     * int32[2]: 0   // (Android user flags of the removed user)
     * int32[3]: 10  // current user
     * int32[4]: 0   // current user flags (none)
     * int32[5]: 2   // number of users
     * int32[6]: 0   // 1st user (user 0)
     * int32[7]: 0   // 1st user flags (none)
     * int32[8]: 10  // 2nd user (user 10)
     * int32[9]: 0   // 2nd user flags (none)
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:WRITE
     */
    REMOVE_USER = 299896586 /* (0x0F0A | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,
    /**
     * Property used to associate (or query the association) the current user with vehicle-specific
     * identification mechanisms (such as key FOB).
     *
     * This is an optional user management property - the OEM could still support user management
     * without defining it. In fact, this property could be used without supporting the core
     * user-related functions described on INITIAL_USER_INFO.
     *
     * To query the association, the Android system gets the property, passing a VehiclePropValue
     * containing the types of associations are being queried, as defined by
     * UserIdentificationGetRequest. The HAL must return right away, returning a VehiclePropValue
     * with a UserIdentificationResponse. Notice that user identification should have already
     * happened while system is booting up and the VHAL implementation should only return the
     * already identified association (like the key FOB used to unlock the car), instead of starting
     * a new association from the get call.
     *
     * To associate types, the Android system sets the property, passing a VehiclePropValue
     * containing the types and values of associations being set, as defined by the
     * UserIdentificationSetRequest. The HAL will then use a property change event (whose
     * VehiclePropValue is defined by UserIdentificationResponse) indicating the current status of
     * the types after the request.
     *
     * For example, to query if the current user (10) is associated with the FOB that unlocked the
     * car and a custom mechanism provided by the OEM, the request would be:
     *
     * int32[0]: 42  // request id
     * int32[1]: 10  (Android user id)
     * int32[2]: 0   (Android user flags)
     * int32[3]: 2   (number of types queried)
     * int32[4]: 1   (1st type queried, UserIdentificationAssociationType::KEY_FOB)
     * int32[5]: 101 (2nd type queried, UserIdentificationAssociationType::CUSTOM_1)
     *
     * If the user is associated with the FOB but not with the custom mechanism, the response would
     * be:
     *
     * int32[0]: 42  // request id
     * int32[1]: 2   (number of associations in the response)
     * int32[2]: 1   (1st type: UserIdentificationAssociationType::KEY_FOB)
     * int32[3]: 2   (1st value: UserIdentificationAssociationValue::ASSOCIATED_CURRENT_USER)
     * int32[4]: 101 (2st type: UserIdentificationAssociationType::CUSTOM_1)
     * int32[5]: 4   (2nd value: UserIdentificationAssociationValue::NOT_ASSOCIATED_ANY_USER)
     *
     * Then to associate the user with the custom mechanism, a set request would be made:
     *
     * int32[0]: 43  // request id
     * int32[1]: 10  (Android user id)
     * int32[2]: 0   (Android user flags)
     * int32[3]: 1   (number of associations being set)
     * int32[4]: 101 (1st type: UserIdentificationAssociationType::CUSTOM_1)
     * int32[5]: 1   (1st value: UserIdentificationAssociationSetValue::ASSOCIATE_CURRENT_USER)
     *
     * If the request succeeded, the response would be simply:
     *
     * int32[0]: 43  // request id
     * int32[1]: 1   (number of associations in the response)
     * int32[2]: 101 (1st type: UserIdentificationAssociationType::CUSTOM_1)
     * int32[3]: 1   (1st value: UserIdentificationAssociationValue::ASSOCIATED_CURRENT_USER)
     *
     * Notice that the set request adds associations, but doesn't remove the existing ones. In the
     * example above, the end state would be 2 associations (FOB and CUSTOM_1). If we wanted to
     * associate the user with just CUSTOM_1 but not FOB, then the request should have been:
     *
     * int32[0]: 43  // request id
     * int32[1]: 10  (Android user id)
     * int32[2]: 2   (number of types set)
     * int32[3]: 1   (1st type: UserIdentificationAssociationType::KEY_FOB)
     * int32[4]: 2   (1st value: UserIdentificationAssociationValue::DISASSOCIATE_CURRENT_USER)
     * int32[5]: 101 (2nd type: UserIdentificationAssociationType::CUSTOM_1)
     * int32[6]: 1   (2nd value: UserIdentificationAssociationValue::ASSOCIATE_CURRENT_USER)
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    USER_IDENTIFICATION_ASSOCIATION = 299896587 /* (0x0F0B | VehiclePropertyGroup:SYSTEM | VehiclePropertyType:MIXED | VehicleArea:GLOBAL) */,



    /*
     * floatValues[0]: 今天最低气温 第0天
     * floatValues[1]: 今天最高气温 第0天
     * floatValues[2]: 明天最低气温 第1天
     * floatValues[3]: 明天最高气温 第1天
     * floatValues[4]: 后天最低气温 第2天
     * floatValues[5]: 后天最高气温 第2天
     * floatValues[6]: 大后天最低气温 第3天
     * floatValues[7]: 大后天最高气温 第3天
     * floatValues[8]: 第4天最低气温 
     * floatValues[9]: 第4天最高气温 
     */
    WEATHER_FORCAST_TEMPERATURE = 560008972/* (0x0F0C | VehiclePropertyGroup:VENDOR | VehiclePropertyType:FLOAT_VEC | VehicleArea:GLOBAL ) */,


    /*
     * floatValues[0]: 今天最低气温 第0天
     * floatValues[1]: 明天最低气温 第1天
     * floatValues[2]: 后天最低气温 第2天
     * floatValues[3]: 大后天最低气温 第3天
     * floatValues[4]: 第4天最低气温 
     */
    BATTERY_FORCAST_MIN_TEMPERATURE = 560008973/* (0x0F0D | VehiclePropertyGroup:VENDOR | VehiclePropertyType:FLOAT_VEC | VehicleArea:GLOBAL ) */,

    /**
     * 座椅通风状态
     */
    SEAT_VENTILATE = 624955150/* (0x0F0E | VehiclePropertyGroup:VENDOR| VehiclePropertyType:INT32 | VehicleArea:SEAT) */,

    /**
     * 座椅通风请求
     */
    SEAT_SET_VENTILATE = 624955151/* (0x0F0F | VehiclePropertyGroup:VENDOR | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,

    /**
     * 方向盘加热 
     * 2 很热
     * 1 一般热
     * 0 停止
     * -1 制冷 (如果可能的话)
     * -2 很冷
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    HVAC_STEERING_WHEEL_HEAT = 557846288/* (0xF10 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,

    HVAC_SET_STEERING_WHEEL_HEAT = 557846289/* (0x0F11 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:INT32 | VehicleArea:GLOBAL) */,

    /**
     * 座椅使用状态 ， 压力传感器
     */
    SEAT_OCCUPANCY = 624955154/* (0x0F12 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,
    /**
     * 座舱温度
     */
    CABIN_TEMPERATURE = 559943443/* (0x0F13 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,

    /**
     * 环境温度
     */
    ENV_OUTSIDE_TEMPERATURE = 559943444/* (0x0F14 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:FLOAT | VehicleArea:GLOBAL) */,

    /**
     * 空调AC
     */
    HVAC_AC_ON = 622858005/* (0x0F15 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,

    /**
     * 设置空调AC
     */
    HVAC_SET_AC_ON = 622858006/* (0x0F16 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:BOOLEAN | VehicleArea:SEAT) */,

    /**
     * 空调当前温度
     */
    HVAC_TEMPERATURE_CURRENT = 627052311/* (0x0F17 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:FLOAT | VehicleArea:SEAT) */,

    /**
     * 设置空调目标温度
     */
    HVAC_TEMPERATURE_SET = 627052312/* (0x0F18 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:FLOAT | VehicleArea:SEAT) */,

    HU_TIME_SET  = 558894873/* (0x0F19 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:INT64 | VehicleArea:GLOBAL) */,

    /**
     * 电池剩余电量 百分比
     */
    BATTERY_REMAINING_POWER = 675286810/* (0x0F19 | VehiclePropertyGroup:VENDOR | VehiclePropertyType:INT32 | VehicleArea:BATTERY ) */,

    /**
     * 预约驾驶请求
     * unix时间戳  秒
     */
    APPOINTMENT_DRIVE = 558894875 /* (0x0F1A | VehiclePropertyGroup:VENDOR | VehiclePropertyType:INT64 | VehicleArea:GLOBAL) */,

    /**
     * 座椅加热状态
     */
    SEAT_HEAT = 624955163/* (0x0F1B | VehiclePropertyGroup:VENDOR| VehiclePropertyType:INT32 | VehicleArea:SEAT) */,

    /**
     * 座椅加热请求
     */
    SEAT_SET_HEAT = 624955164/* (0x0F1C | VehiclePropertyGroup:VENDOR | VehiclePropertyType:INT32 | VehicleArea:SEAT) */,

    /**
     * 电池预热请求
     */
    BATTERY_SET_PREHEAT = 675286813/* (0x0F1D | VehiclePropertyGroup:VENDOR | VehiclePropertyType:INT32 | VehicleArea:BATTERY) */,
    
};

enum class VehicleGear : int32_t {
    GEAR_UNKNOWN = 0 /* 0x0000 */,
    GEAR_NEUTRAL = 1 /* 0x0001 */,
    GEAR_REVERSE = 2 /* 0x0002 */,
    GEAR_PARK = 4 /* 0x0004 */,
    GEAR_DRIVE = 8 /* 0x0008 */,
    GEAR_1 = 16 /* 0x0010 */,
    GEAR_2 = 32 /* 0x0020 */,
    GEAR_3 = 64 /* 0x0040 */,
    GEAR_4 = 128 /* 0x0080 */,
    GEAR_5 = 256 /* 0x0100 */,
    GEAR_6 = 512 /* 0x0200 */,
    GEAR_7 = 1024 /* 0x0400 */,
    GEAR_8 = 2048 /* 0x0800 */,
    GEAR_9 = 4096 /* 0x1000 */,
};

/**
 * Various Seats in the car.
 */
enum class VehicleAreaSeat : int32_t {
    ROW_1_LEFT = 1 /* 0x0001 */,
    ROW_1_CENTER = 2 /* 0x0002 */,
    ROW_1_RIGHT = 4 /* 0x0004 */,
    ROW_2_LEFT = 16 /* 0x0010 */,
    ROW_2_CENTER = 32 /* 0x0020 */,
    ROW_2_RIGHT = 64 /* 0x0040 */,
    ROW_3_LEFT = 256 /* 0x0100 */,
    ROW_3_CENTER = 512 /* 0x0200 */,
    ROW_3_RIGHT = 1024 /* 0x0400 */,
};

/**
 * Various windshields/windows in the car.
 */
enum class VehicleAreaWindow : int32_t {
    FRONT_WINDSHIELD = 1 /* 0x00000001 */,
    REAR_WINDSHIELD = 2 /* 0x00000002 */,
    ROW_1_LEFT = 16 /* 0x00000010 */,
    ROW_1_RIGHT = 64 /* 0x00000040 */,
    ROW_2_LEFT = 256 /* 0x00000100 */,
    ROW_2_RIGHT = 1024 /* 0x00000400 */,
    ROW_3_LEFT = 4096 /* 0x00001000 */,
    ROW_3_RIGHT = 16384 /* 0x00004000 */,
    ROOF_TOP_1 = 65536 /* 0x00010000 */,
    ROOF_TOP_2 = 131072 /* 0x00020000 */,
};

enum class VehicleAreaDoor : int32_t {
    ROW_1_LEFT = 1 /* 0x00000001 */,
    ROW_1_RIGHT = 4 /* 0x00000004 */,
    ROW_2_LEFT = 16 /* 0x00000010 */,
    ROW_2_RIGHT = 64 /* 0x00000040 */,
    ROW_3_LEFT = 256 /* 0x00000100 */,
    ROW_3_RIGHT = 1024 /* 0x00000400 */,
    HOOD = 268435456 /* 0x10000000 */,
    REAR = 536870912 /* 0x20000000 */,
};

enum class VehicleAreaMirror : int32_t {
    DRIVER_LEFT = 1 /* 0x00000001 */,
    DRIVER_RIGHT = 2 /* 0x00000002 */,
    DRIVER_CENTER = 4 /* 0x00000004 */,
};


enum class VehicleAreaBattery: int32_t {
    ELECTRIC_VEHICLE_BATTERY = 1,    /*  0x00000001 */
    STARTING_LIGHTING_IGNITION = 2,    /*  0x00000002 */
};

enum class VehicleTurnSignal : int32_t {
    NONE = 0 /* 0x00 */,
    RIGHT = 1 /* 0x01 */,
    LEFT = 2 /* 0x02 */,
};







#endif //__vhal_compatible__hpp__
