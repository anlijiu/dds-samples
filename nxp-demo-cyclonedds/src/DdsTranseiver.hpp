#ifndef __DdsTranseiver_hpp__
#define __DdsTranseiver_hpp__

#include <dds/core/ddscore.hpp>
#include <dds/pub/ddspub.hpp>
#include <dds/sub/ddssub.hpp>
#include <std_msgs/msg/Temperature.hpp>
#include <hvac_msgs/msg/Ac.hpp>
#include <hvac_msgs/srv/SetAc.hpp>
#include <hvac_msgs/msg/Hvac.hpp>
#include <hvac_msgs/srv/HvacSrv.hpp>
#include <hvac_msgs/srv/SetTemperature.hpp>
#include <habit_msgs/srv/AppointmentDrive.hpp>
#include <cabin_msgs/msg/Cabin.hpp>
#include <cloud_msgs/msg/Cloud.hpp>
#include <seat_msgs/msg/Seat.hpp>
#include <seat_msgs/srv/SeatSrv.hpp>
#include "ObjectPool.hpp"
#include "AsyncWaitSet.hpp"

namespace headunit {
namespace dds {


/**
 * DDS收发单元 , 每一个dds message  都对应一个收发单元
 * 针对每一个dds topic 的 Receiver / Sender
 * 基于以太网的更高层 dds 协议 与 CAN总线有着本质的不同
 * 不同于CAN总线的设计， CAN总线可以面向总线 , more generic
 * 而本 receiver/sender 则是面向一个个独立的 dds topic
 */
class DdsReceiver {
public:
    using OnHalEvent = std::function<bool(VehiclePropValuePtr event)>;
    DdsReceiver(const OnHalEvent&, VehiclePropValuePool*, AsyncWaitSet &);
    virtual ~DdsReceiver() {}; 
    virtual void handle() = 0;
protected:
    void onNewProperty(VehiclePropValuePtr propValue) {
        mOnHalEvent(std::move(propValue));
    }

    void attach_condition(const ::dds::core::cond::Condition& cond) {
        std::cout<<"DdsReceiver::attach_condition 111" << std::endl;
        mAsyncWaitSet.attach_condition(cond);
        std::cout<<"DdsReceiver::attach_condition 222" << std::endl;
    }
    void detach_condition(const ::dds::core::cond::Condition& cond) {
        std::cout<<"DdsReceiver::detach_condition 111" << std::endl;
        mAsyncWaitSet.detach_condition(cond);
        std::cout<<"DdsReceiver::detach_condition 222" << std::endl;
    }

    VehiclePropValuePool* getValuePool() {
        return mVehiclePropValuePool;
    }

private:
    AsyncWaitSet& mAsyncWaitSet;
    OnHalEvent mOnHalEvent;
    VehiclePropValuePool* mVehiclePropValuePool;
};

class DdsSender {
public:
    DdsSender() {}; 
    virtual ~DdsSender() {}; 
    virtual void send(const VehiclePropValue& propValue) = 0;
private:
};

class DdsTranseiver {
public:
    using OnHalEvent = std::function<bool(VehiclePropValuePtr event)>;
    DdsTranseiver(const OnHalEvent&, VehiclePropValuePool*, AsyncWaitSet& asyncWaitSet, int domainId = 0);
    void send(const VehiclePropValue& propValue);

protected:
    const ::dds::domain::DomainParticipant & getDomainParticipant() {
        return mParticipant;
    }
private:
    void init();
private:
    const OnHalEvent& mOnHalEvent;
    AsyncWaitSet& mAsyncWaitSet;
    VehiclePropValuePool* mPropValuePool;
    ::dds::domain::DomainParticipant mParticipant;
    std::unordered_map<int, std::unique_ptr<DdsSender>> senderMap;
    std::unordered_map<int, std::unique_ptr<DdsReceiver>> receiverMap;
};

/**
 * 空调AC状态
 * topic: hvac_msgs::msg::TOPIC_AC
 * prop: HVAC_AC_ON
 */
class DdsHvacAcReceiver : public DdsReceiver {
public:
    DdsHvacAcReceiver(const OnHalEvent&, VehiclePropValuePool*, const ::dds::domain::DomainParticipant&, AsyncWaitSet& asyncWaitSet);
    ~DdsHvacAcReceiver();
    virtual void handle();
private:
    virtual VehiclePropValuePtr toVehicleProperty(const hvac_msgs::msg::Ac& value);
private:
    ::dds::sub::DataReader<hvac_msgs::msg::Ac> mReader;
    ::dds::core::cond::StatusCondition mReaderStatusCondition;
};


/**
 * 设置空调AC开关
 * topic: hvac_msgs::msg::TOPIC_AC
 * prop: HVAC_AC_ON
 */
class DdsHvacSetAcSender : public DdsSender {
public:
    DdsHvacSetAcSender(const ::dds::domain::DomainParticipant& participant);
    virtual void send(const VehiclePropValue& propValue);
private:
    ::dds::pub::DataWriter<hvac_msgs::srv::SetAcRequest> mWriter;
};

/**
 * 舱内温度
 * topic: cabin_msgs::msg::TOPIC_CABIN_TEMPERATURE
 * struct: std_msgs::msg::Temperature
 * prop: CABIN_TEMPERATURE
 */
class DdsCabinTemperatureReceiver : public DdsReceiver {
public:
    DdsCabinTemperatureReceiver(const OnHalEvent&, VehiclePropValuePool*, const ::dds::domain::DomainParticipant&, AsyncWaitSet& asyncWaitSet);
    ~DdsCabinTemperatureReceiver();
    virtual void handle();
private:
    virtual VehiclePropValuePtr toVehicleProperty(const std_msgs::msg::Temperature& value);
private:
    ::dds::sub::DataReader<std_msgs::msg::Temperature> mReader;
    ::dds::core::cond::StatusCondition mReaderStatusCondition;
};


/**
 * 舱外环境温度
 * topic: hvac_msgs::msg::TOPIC_ENV_OUT_TEMPERATURE
 * struct: std_msgs::msg::Temperature
 * prop: ENV_OUTSIDE_TEMPERATURE
 */
class DdsEnvOutTemperatureReceiver : public DdsReceiver {
public:
    DdsEnvOutTemperatureReceiver(const OnHalEvent&, VehiclePropValuePool*, const ::dds::domain::DomainParticipant&, AsyncWaitSet& asyncWaitSet);
    ~DdsEnvOutTemperatureReceiver();
    virtual void handle();
private:
    virtual VehiclePropValuePtr toVehicleProperty(const std_msgs::msg::Temperature& value);
private:
    ::dds::sub::DataReader<std_msgs::msg::Temperature> mReader;
    ::dds::core::cond::StatusCondition mReaderStatusCondition;
};

/**
 * 云端发过来的天气预报
 * topic: cloud_msgs::msg::TOPIC_WEATHER_FORCAST_TEMPERATURE
 * struct: cloud_msgs::msg::WeatherForcastTemperature
 * prop: WEATHER_FORCAST_TEMPERATURE
 */
class DdsWeatherForcastTemperatureReceiver : public DdsReceiver {
public:
    DdsWeatherForcastTemperatureReceiver(const OnHalEvent&, VehiclePropValuePool*, const ::dds::domain::DomainParticipant&, AsyncWaitSet& asyncWaitSet);
    ~DdsWeatherForcastTemperatureReceiver();
    virtual void handle();
private:
    virtual VehiclePropValuePtr toVehicleProperty(const cloud_msgs::msg::WeatherForcastTemperature& value);
private:
    ::dds::sub::DataReader<cloud_msgs::msg::WeatherForcastTemperature> mReader;
    ::dds::core::cond::StatusCondition mReaderStatusCondition;
};

/**
 * 座椅通风状态
 * topic: seat_msgs::msg::TOPIC_SEAT_VENTILATE
 * struct: seat_msgs::msg::Ventilate
 * prop: SEAT_VENTILATE
 */
class DdsSeatVentilateReceiver : public DdsReceiver {
public:
    DdsSeatVentilateReceiver(const OnHalEvent&, VehiclePropValuePool*, const ::dds::domain::DomainParticipant&, AsyncWaitSet& asyncWaitSet);
    ~DdsSeatVentilateReceiver();
    virtual void handle();
private:
    virtual VehiclePropValuePtr toVehicleProperty(const seat_msgs::msg::Ventilate& value);
private:
    ::dds::sub::DataReader<seat_msgs::msg::Ventilate> mReader;
    ::dds::core::cond::StatusCondition mReaderStatusCondition;
};


/**
 * 调节座椅通风开关
 * topic: seat_msgs::srv::TOPIC_SET_SEAT_VENTILATE
 * struct: seat_msgs::msg::Ventilate
 * prop: SEAT_SET_VENTILATE
 */
class DdsSeatSetVentilateSender : public DdsSender {
public:
    DdsSeatSetVentilateSender(const ::dds::domain::DomainParticipant& participant);
    virtual void send(const VehiclePropValue& propValue);
private:
    ::dds::pub::DataWriter<seat_msgs::msg::Ventilate> mWriter;
};

/**
 * 座椅加热状态
 * topic: seat_msgs::msg::TOPIC_SEAT_HEAT
 * struct: seat_msgs::msg::Heat
 * prop: SEAT_HEAT
 */
class DdsSeatHeatReceiver : public DdsReceiver {
public:
    DdsSeatHeatReceiver(const OnHalEvent&, VehiclePropValuePool*, const ::dds::domain::DomainParticipant&, AsyncWaitSet& asyncWaitSet);
    ~DdsSeatHeatReceiver();
    virtual void handle();
private:
    virtual VehiclePropValuePtr toVehicleProperty(const seat_msgs::msg::Heat& value);
private:
    ::dds::sub::DataReader<seat_msgs::msg::Heat> mReader;
    ::dds::core::cond::StatusCondition mReaderStatusCondition;
};

/**
 * 调节座椅加热开关
 * topic: seat_msgs::srv::TOPIC_SET_SEAT_HEAT
 * struct: seat_msgs::msg::Heat
 * prop: SEAT_SET_HEAT
 */
class DdsSeatSetHeatSender : public DdsSender {
public:
    DdsSeatSetHeatSender(const ::dds::domain::DomainParticipant& participant);
    virtual void send(const VehiclePropValue& propValue);
private:
    ::dds::pub::DataWriter<seat_msgs::msg::Heat> mWriter;
};

/**
 * 方向盘加热状态
 * topic: hvac_msgs::msg::TOPIC_STEERING_WHEEL_HEAT
 * struct: hvac_msgs::msg::SteeringWheelHeat
 * prop: HVAC_STEERING_WHEEL_HEAT
 */
class DdsSteeringWheelHeatReceiver : public DdsReceiver {
public:
    DdsSteeringWheelHeatReceiver(const OnHalEvent&, VehiclePropValuePool*, const ::dds::domain::DomainParticipant&, AsyncWaitSet& asyncWaitSet);
    ~DdsSteeringWheelHeatReceiver();
    virtual void handle();
private:
    virtual VehiclePropValuePtr toVehicleProperty(const hvac_msgs::msg::SteeringWheelHeat& value);
private:
    ::dds::sub::DataReader<hvac_msgs::msg::SteeringWheelHeat> mReader;
    ::dds::core::cond::StatusCondition mReaderStatusCondition;
};

/**
 * 调节方向盘加热开关
 * topic: hvac_msgs::srv::TOPIC_SET_STEERING_WHEEL_HEAT
 * struct: hvac_msgs::msg::SteeringWheelHeat
 * prop: HVAC_SET_STEERING_WHEEL_HEAT
 */
class DdsSetSteeringWheelHeatSender : public DdsSender {
public:
    DdsSetSteeringWheelHeatSender(const ::dds::domain::DomainParticipant& participant);
    virtual void send(const VehiclePropValue& propValue);
private:
    ::dds::pub::DataWriter<hvac_msgs::msg::SteeringWheelHeat> mWriter;
};

/**
 * 空调当前温度
 * topic: hvac_msgs::msg::TOPIC_HVAC_CURRENT_TEMPERATURE
 * struct: std_msgs::msg::Temperature
 * prop: HVAC_TEMPERATURE_CURRENT
 */
class DdsHvacCurrentTemperatureReceiver : public DdsReceiver {
public:
    DdsHvacCurrentTemperatureReceiver(const OnHalEvent&, VehiclePropValuePool*, const ::dds::domain::DomainParticipant&, AsyncWaitSet& asyncWaitSet);
    ~DdsHvacCurrentTemperatureReceiver();
    virtual void handle();
private:
    virtual VehiclePropValuePtr toVehicleProperty(const std_msgs::msg::Temperature& value);
private:
    ::dds::sub::DataReader<std_msgs::msg::Temperature> mReader;
    ::dds::core::cond::StatusCondition mReaderStatusCondition;
};

/**
 * 设置空调温度
 * topic: hvac_msgs::srv::TOPIC_HVAC_SET_TEMPERATURE
 * struct: std_msgs::msg::Temperature
 * prop: HVAC_TEMPERATURE_SET
 */
class DdsHvacSetCurrentTemperatureSender : public DdsSender {
public:
    DdsHvacSetCurrentTemperatureSender(const ::dds::domain::DomainParticipant& participant);
    virtual void send(const VehiclePropValue& propValue);
private:
    ::dds::pub::DataWriter<std_msgs::msg::Temperature> mWriter;
};








/**
 * topic: habit_msgs::srv::TOPIC_APPOINTMENT_DRIVE
 * prop: APPOINTMENT_DRIVE
 */
class DdsAppointmentDriveReceiver : public DdsReceiver {
public:
    DdsAppointmentDriveReceiver(const OnHalEvent&, VehiclePropValuePool*, const ::dds::domain::DomainParticipant&, AsyncWaitSet& asyncWaitSet);
    ~DdsAppointmentDriveReceiver();
    virtual void handle();
private:
    virtual VehiclePropValuePtr toVehicleProperty(const habit_msgs::srv::AppointmentDriveRequest& );
private:
    ::dds::sub::DataReader<habit_msgs::srv::AppointmentDriveRequest> mReader;
    ::dds::core::cond::StatusCondition mReaderStatusCondition;
};

} // namespace dds
} // namespace headunit
 
#endif //__DdsTranseiver_hpp__
