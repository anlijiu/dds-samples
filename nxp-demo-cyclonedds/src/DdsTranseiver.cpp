#include "DdsTranseiver.hpp" 
#include "DdsUtils.hpp"

namespace headunit {
namespace dds {

static const char *simple_config_uri =
    "file:///etc/cyclonedds/config.xml";

using namespace hvac_msgs::srv;

DdsTranseiver::DdsTranseiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, AsyncWaitSet& asyncWaitSet, int domainId)
    : mPropValuePool(pool)
    , mOnHalEvent(onHalEvent)
    , mParticipant(::dds::domain::DomainParticipant(
            domainId,
            ::dds::domain::DomainParticipant::default_participant_qos(),
            0,
            ::dds::core::status::StatusMask::none(),
            simple_config_uri))
    , mAsyncWaitSet(asyncWaitSet)
{
    std::cout<< "DdsTranseiver constructor " << std::endl;
    init();
}

void DdsTranseiver::init() {
    std::cout<< "DdsTranseiver::init  mParticipant domain id:" <<  mParticipant.domain_id() << std::endl;;
    senderMap[toInt(VehicleProperty::HVAC_SET_AC_ON)] = std::make_unique<DdsHvacSetAcSender>(mParticipant);
    senderMap[toInt(VehicleProperty::SEAT_SET_VENTILATE)] = std::make_unique<DdsSeatSetVentilateSender>(mParticipant);
    senderMap[toInt(VehicleProperty::SEAT_SET_HEAT)] = std::make_unique<DdsSeatSetHeatSender>(mParticipant);
    senderMap[toInt(VehicleProperty::HVAC_SET_STEERING_WHEEL_HEAT)] = std::make_unique<DdsSetSteeringWheelHeatSender>(mParticipant);
    senderMap[toInt(VehicleProperty::HVAC_TEMPERATURE_SET)] = std::make_unique<DdsHvacSetCurrentTemperatureSender>(mParticipant);

    receiverMap[toInt(VehicleProperty::HVAC_AC_ON)] = std::make_unique<DdsHvacAcReceiver>(mOnHalEvent, mPropValuePool, mParticipant, mAsyncWaitSet);
    receiverMap[toInt(VehicleProperty::APPOINTMENT_DRIVE)] = std::make_unique<DdsAppointmentDriveReceiver>(mOnHalEvent, mPropValuePool, mParticipant, mAsyncWaitSet);

    receiverMap[toInt(VehicleProperty::CABIN_TEMPERATURE)] = std::make_unique<DdsCabinTemperatureReceiver>(mOnHalEvent, mPropValuePool, mParticipant, mAsyncWaitSet);
    receiverMap[toInt(VehicleProperty::ENV_OUTSIDE_TEMPERATURE)] = std::make_unique<DdsEnvOutTemperatureReceiver>(mOnHalEvent, mPropValuePool, mParticipant, mAsyncWaitSet);
    receiverMap[toInt(VehicleProperty::WEATHER_FORCAST_TEMPERATURE)] = std::make_unique<DdsWeatherForcastTemperatureReceiver>(mOnHalEvent, mPropValuePool, mParticipant, mAsyncWaitSet);
    receiverMap[toInt(VehicleProperty::SEAT_VENTILATE)] = std::make_unique<DdsSeatVentilateReceiver>(mOnHalEvent, mPropValuePool, mParticipant, mAsyncWaitSet);
    receiverMap[toInt(VehicleProperty::SEAT_HEAT)] = std::make_unique<DdsSeatHeatReceiver>(mOnHalEvent, mPropValuePool, mParticipant, mAsyncWaitSet);
    receiverMap[toInt(VehicleProperty::HVAC_STEERING_WHEEL_HEAT)] = std::make_unique<DdsSteeringWheelHeatReceiver>(mOnHalEvent, mPropValuePool, mParticipant, mAsyncWaitSet);
    receiverMap[toInt(VehicleProperty::HVAC_TEMPERATURE_CURRENT)] = std::make_unique<DdsHvacCurrentTemperatureReceiver>(mOnHalEvent, mPropValuePool, mParticipant, mAsyncWaitSet);
}

void DdsTranseiver::send(const VehiclePropValue& propValue) {
    std::cout<< "DdsTranseiver::send in " << std::endl;
    auto iter = senderMap.find(propValue.prop);
    if(iter != senderMap.end()) {
        iter->second->send(propValue);
    } else {
        std::cout<< "DdsTranseiver::send error , no sender for prop:" << propValue.prop << std::endl;
    }

}

DdsReceiver::DdsReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, AsyncWaitSet & asyncWaitSet)
    : mOnHalEvent(onHalEvent)
    , mVehiclePropValuePool(pool)
    , mAsyncWaitSet(asyncWaitSet)
{
}

DdsHvacAcReceiver::DdsHvacAcReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, const ::dds::domain::DomainParticipant& participant, AsyncWaitSet & asyncWaitSet)
    : DdsReceiver(onHalEvent, pool, asyncWaitSet)
    , mReaderStatusCondition(::dds::core::null)
    , mReader(::dds::core::null)
{
 
    std::cout<< "DdsHvacAcReceiver constructor " << std::endl;
    ::dds::topic::Topic<hvac_msgs::msg::Ac> topicAc = ::dds::core::null;
    utils::find_or_create_topic<hvac_msgs::msg::Ac>(
            participant, topicAc, hvac_msgs::msg::TOPIC_AC);

    ::dds::sub::qos::SubscriberQos acSubscriberQos = participant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos acDataReaderQos(topicAc.qos());

    utils::create_datareader<hvac_msgs::msg::Ac>(
            mReader,
            participant,
            topicAc,
            acDataReaderQos,
            acSubscriberQos);

    mReaderStatusCondition = ::dds::core::cond::StatusCondition(mReader);

    mReaderStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    
    mReaderStatusCondition.handler(std::bind(&DdsHvacAcReceiver::handle, this));
    DdsReceiver::attach_condition(mReaderStatusCondition);
    std::cout<< "DdsHvacAcReceiver constructor end" << std::endl;
}

DdsHvacAcReceiver::~DdsHvacAcReceiver() {
    DdsReceiver::detach_condition(mReaderStatusCondition);
}

void DdsHvacAcReceiver::handle() {
    ::dds::sub::LoanedSamples<hvac_msgs::msg::Ac> samples = mReader.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    for (::dds::sub::LoanedSamples<hvac_msgs::msg::Ac>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            onNewProperty(std::move(toVehicleProperty(sample_it->data())));
        }
    }
}

VehiclePropValuePtr DdsHvacAcReceiver::toVehicleProperty(const hvac_msgs::msg::Ac& ac) {
    auto v = getValuePool()->obtainBoolean(ac.on());
    v->prop = static_cast<int32_t>(VehicleProperty::HVAC_AC_ON);
    v->areaId = ac.zone();
    v->timestamp = elapsedRealtimeNano();
    return v;
}


/**
 * 舱内温度
 * topic: cabin_msgs::msg::TOPIC_CABIN_TEMPERATURE
 * struct: std_msgs::msg::Temperature
 * prop: CABIN_TEMPERATURE
 */
DdsCabinTemperatureReceiver::DdsCabinTemperatureReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, const ::dds::domain::DomainParticipant& participant, AsyncWaitSet & asyncWaitSet)
    : DdsReceiver(onHalEvent, pool, asyncWaitSet)
    , mReaderStatusCondition(::dds::core::null)
    , mReader(::dds::core::null)
{
 
    std::cout<< "DdsCabinTemperatureReceiver constructor " << std::endl;
    ::dds::topic::Topic<std_msgs::msg::Temperature> topic = ::dds::core::null;
    utils::find_or_create_topic<std_msgs::msg::Temperature>(
            participant, topic, cabin_msgs::msg::TOPIC_CABIN_TEMPERATURE);

    ::dds::sub::qos::SubscriberQos subscriberQos = participant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos dataReaderQos(topic.qos());

    utils::create_datareader<std_msgs::msg::Temperature>(
            mReader,
            participant,
            topic,
            dataReaderQos,
            subscriberQos);

    mReaderStatusCondition = ::dds::core::cond::StatusCondition(mReader);

    mReaderStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    
    mReaderStatusCondition.handler(std::bind(&DdsCabinTemperatureReceiver::handle, this));
    DdsReceiver::attach_condition(mReaderStatusCondition);
    std::cout<< "DdsCabinTemperatureReceiver constructor end" << std::endl;
}

DdsCabinTemperatureReceiver::~DdsCabinTemperatureReceiver() {
    DdsReceiver::detach_condition(mReaderStatusCondition);
}

void DdsCabinTemperatureReceiver::handle() {
    ::dds::sub::LoanedSamples<std_msgs::msg::Temperature> samples = mReader.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    for (::dds::sub::LoanedSamples<std_msgs::msg::Temperature>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            onNewProperty(std::move(toVehicleProperty(sample_it->data())));
        }
    }
}

VehiclePropValuePtr DdsCabinTemperatureReceiver::toVehicleProperty(const std_msgs::msg::Temperature& value) {
    auto v = getValuePool()->obtainFloat(value.value());
    v->prop = static_cast<int32_t>(VehicleProperty::CABIN_TEMPERATURE);
    v->areaId = value.zone();
    v->timestamp = elapsedRealtimeNano();
    return v;
}

/**
 * 舱外环境温度
 * topic: hvac_msgs::msg::TOPIC_ENV_OUT_TEMPERATURE
 * struct: std_msgs::msg::Temperature
 * prop: ENV_OUTSIDE_TEMPERATURE
 */
DdsEnvOutTemperatureReceiver::DdsEnvOutTemperatureReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, const ::dds::domain::DomainParticipant& participant, AsyncWaitSet & asyncWaitSet)
    : DdsReceiver(onHalEvent, pool, asyncWaitSet)
    , mReaderStatusCondition(::dds::core::null)
    , mReader(::dds::core::null)
{
 
    std::cout<< "DdsEnvOutTemperatureReceiver constructor " << std::endl;
    ::dds::topic::Topic<std_msgs::msg::Temperature> topic = ::dds::core::null;
    utils::find_or_create_topic<std_msgs::msg::Temperature>(
            participant, topic, hvac_msgs::msg::TOPIC_ENV_OUT_TEMPERATURE);

    ::dds::sub::qos::SubscriberQos subscriberQos = participant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos dataReaderQos(topic.qos());

    utils::create_datareader<std_msgs::msg::Temperature>(
            mReader,
            participant,
            topic,
            dataReaderQos,
            subscriberQos);

    mReaderStatusCondition = ::dds::core::cond::StatusCondition(mReader);

    mReaderStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    
    mReaderStatusCondition.handler(std::bind(&DdsEnvOutTemperatureReceiver::handle, this));
    DdsReceiver::attach_condition(mReaderStatusCondition);
    std::cout<< "DdsEnvOutTemperatureReceiver constructor end" << std::endl;
}

DdsEnvOutTemperatureReceiver::~DdsEnvOutTemperatureReceiver() {
    DdsReceiver::detach_condition(mReaderStatusCondition);
}

void DdsEnvOutTemperatureReceiver::handle() {
    ::dds::sub::LoanedSamples<std_msgs::msg::Temperature> samples = mReader.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    for (::dds::sub::LoanedSamples<std_msgs::msg::Temperature>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            onNewProperty(std::move(toVehicleProperty(sample_it->data())));
        }
    }
}

VehiclePropValuePtr DdsEnvOutTemperatureReceiver::toVehicleProperty(const std_msgs::msg::Temperature& value) {
    auto v = getValuePool()->obtainFloat(value.value());
    v->prop = static_cast<int32_t>(VehicleProperty::ENV_OUTSIDE_TEMPERATURE);
    v->areaId = value.zone();
    v->timestamp = elapsedRealtimeNano();
    return v;
}

/**
 * 云端发过来的天气预报
 * topic: cloud_msgs::msg::TOPIC_WEATHER_FORCAST_TEMPERATURE
 * struct: cloud_msgs::msg::WeatherForcastTemperature
 * prop: WEATHER_FORCAST_TEMPERATURE
 */
DdsWeatherForcastTemperatureReceiver::DdsWeatherForcastTemperatureReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, const ::dds::domain::DomainParticipant& participant, AsyncWaitSet & asyncWaitSet)
    : DdsReceiver(onHalEvent, pool, asyncWaitSet)
    , mReaderStatusCondition(::dds::core::null)
    , mReader(::dds::core::null)
{
 
    std::cout<< "DdsWeatherForcastTemperatureReceiver constructor " << std::endl;
    ::dds::topic::Topic<cloud_msgs::msg::WeatherForcastTemperature> topic = ::dds::core::null;
    utils::find_or_create_topic<cloud_msgs::msg::WeatherForcastTemperature>(
            participant, topic, cloud_msgs::msg::TOPIC_WEATHER_FORCAST_TEMPERATURE);

    ::dds::sub::qos::SubscriberQos subscriberQos = participant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos dataReaderQos(topic.qos());

    utils::create_datareader<cloud_msgs::msg::WeatherForcastTemperature>(
            mReader,
            participant,
            topic,
            dataReaderQos,
            subscriberQos);

    mReaderStatusCondition = ::dds::core::cond::StatusCondition(mReader);

    mReaderStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    
    mReaderStatusCondition.handler(std::bind(&DdsWeatherForcastTemperatureReceiver::handle, this));
    DdsReceiver::attach_condition(mReaderStatusCondition);
    std::cout<< "DdsWeatherForcastTemperatureReceiver constructor end" << std::endl;
}

DdsWeatherForcastTemperatureReceiver::~DdsWeatherForcastTemperatureReceiver() {
    DdsReceiver::detach_condition(mReaderStatusCondition);
}

void DdsWeatherForcastTemperatureReceiver::handle() {
    ::dds::sub::LoanedSamples<cloud_msgs::msg::WeatherForcastTemperature> samples = mReader.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    for (::dds::sub::LoanedSamples<cloud_msgs::msg::WeatherForcastTemperature>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            onNewProperty(std::move(toVehicleProperty(sample_it->data())));
        }
    }
}

VehiclePropValuePtr DdsWeatherForcastTemperatureReceiver::toVehicleProperty(const cloud_msgs::msg::WeatherForcastTemperature& value) {
    auto v = getValuePool()->obtain(VehiclePropertyType::FLOAT_VEC, 10);
    v->prop = static_cast<int32_t>(VehicleProperty::WEATHER_FORCAST_TEMPERATURE);
    for(int i = 0; i < 5; ++i) {
        v->value.floatValues[i+i] = static_cast<float>(value.min()[i]);
        v->value.floatValues[i+i+1] = static_cast<float>(value.max()[i]);
    }
    v->areaId = 0;
    v->timestamp = elapsedRealtimeNano();
    return v;
}


/**
 * 座椅通风状态
 * topic: seat_msgs::msg::TOPIC_SEAT_VENTILATE
 * struct: seat_msgs::msg::Ventilate
 * prop: SEAT_VENTILATE
 */
DdsSeatVentilateReceiver::DdsSeatVentilateReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, const ::dds::domain::DomainParticipant& participant, AsyncWaitSet & asyncWaitSet)
    : DdsReceiver(onHalEvent, pool, asyncWaitSet)
    , mReaderStatusCondition(::dds::core::null)
    , mReader(::dds::core::null)
{
 
    std::cout<< "DdsSeatVentilateReceiver constructor " << std::endl;
    ::dds::topic::Topic<seat_msgs::msg::Ventilate> topic = ::dds::core::null;
    utils::find_or_create_topic<seat_msgs::msg::Ventilate>(
            participant, topic, seat_msgs::msg::TOPIC_SEAT_VENTILATE);

    ::dds::sub::qos::SubscriberQos subscriberQos = participant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos dataReaderQos(topic.qos());

    utils::create_datareader<seat_msgs::msg::Ventilate>(
            mReader,
            participant,
            topic,
            dataReaderQos,
            subscriberQos);

    mReaderStatusCondition = ::dds::core::cond::StatusCondition(mReader);

    mReaderStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    
    mReaderStatusCondition.handler(std::bind(&DdsSeatVentilateReceiver::handle, this));
    DdsReceiver::attach_condition(mReaderStatusCondition);
    std::cout<< "DdsSeatVentilateReceiver constructor end" << std::endl;
}

DdsSeatVentilateReceiver::~DdsSeatVentilateReceiver() {
    DdsReceiver::detach_condition(mReaderStatusCondition);
}

void DdsSeatVentilateReceiver::handle() {
    ::dds::sub::LoanedSamples<seat_msgs::msg::Ventilate> samples = mReader.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    for (::dds::sub::LoanedSamples<seat_msgs::msg::Ventilate>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            onNewProperty(std::move(toVehicleProperty(sample_it->data())));
        }
    }
}

VehiclePropValuePtr DdsSeatVentilateReceiver::toVehicleProperty(const seat_msgs::msg::Ventilate& value) {
    auto v = getValuePool()->obtainInt32(value.level());
    v->prop = static_cast<int32_t>(VehicleProperty::SEAT_VENTILATE);
    v->areaId = value.zone();
    v->timestamp = elapsedRealtimeNano();
    return v;
}

/**
 * 调节座椅通风开关
 * topic: seat_msgs::srv::TOPIC_SET_SEAT_VENTILATE
 * struct: seat_msgs::msg::Ventilate
 * prop: SEAT_SET_VENTILATE
 */
DdsSeatSetVentilateSender::DdsSeatSetVentilateSender(const ::dds::domain::DomainParticipant& participant) 
    : DdsSender()
    , mWriter(::dds::core::null)
{

    std::cout<< "DdsSeatSetVentilateSender constructor " << std::endl;
    ::dds::topic::Topic<seat_msgs::msg::Ventilate> topic = ::dds::core::null;
    utils::find_or_create_topic<seat_msgs::msg::Ventilate>(
            participant, topic, seat_msgs::srv::TOPIC_SET_SEAT_VENTILATE);

    ::dds::pub::qos::PublisherQos publisherQos = participant.default_publisher_qos();
    ::dds::pub::qos::DataWriterQos dataWriterQos(topic.qos());

    utils::create_datawriter<seat_msgs::msg::Ventilate>(
            mWriter,
            participant,
            topic,
            dataWriterQos,
            publisherQos);
} 

void DdsSeatSetVentilateSender::send(const VehiclePropValue& propValue) {
    seat_msgs::msg::Ventilate request(propValue.value.int32Values[0], propValue.areaId);
    mWriter->write(request);
};

/**
 * 座椅加热状态
 * topic: seat_msgs::msg::TOPIC_SEAT_HEAT
 * struct: seat_msgs::msg::Heat
 * prop: SEAT_HEAT
 */
DdsSeatHeatReceiver::DdsSeatHeatReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, const ::dds::domain::DomainParticipant& participant, AsyncWaitSet & asyncWaitSet)
    : DdsReceiver(onHalEvent, pool, asyncWaitSet)
    , mReaderStatusCondition(::dds::core::null)
    , mReader(::dds::core::null)
{
 
    std::cout<< "DdsSeatHeatReceiver constructor " << std::endl;
    ::dds::topic::Topic<seat_msgs::msg::Heat> topic = ::dds::core::null;
    utils::find_or_create_topic<seat_msgs::msg::Heat>(
            participant, topic, seat_msgs::msg::TOPIC_SEAT_HEAT);

    ::dds::sub::qos::SubscriberQos subscriberQos = participant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos dataReaderQos(topic.qos());

    utils::create_datareader<seat_msgs::msg::Heat>(
            mReader,
            participant,
            topic,
            dataReaderQos,
            subscriberQos);

    mReaderStatusCondition = ::dds::core::cond::StatusCondition(mReader);

    mReaderStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    
    mReaderStatusCondition.handler(std::bind(&DdsSeatHeatReceiver::handle, this));
    DdsReceiver::attach_condition(mReaderStatusCondition);
    std::cout<< "DdsSeatHeatReceiver constructor end" << std::endl;
}

DdsSeatHeatReceiver::~DdsSeatHeatReceiver() {
    DdsReceiver::detach_condition(mReaderStatusCondition);
}

void DdsSeatHeatReceiver::handle() {
    ::dds::sub::LoanedSamples<seat_msgs::msg::Heat> samples = mReader.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    for (::dds::sub::LoanedSamples<seat_msgs::msg::Heat>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            onNewProperty(std::move(toVehicleProperty(sample_it->data())));
        }
    }
}

VehiclePropValuePtr DdsSeatHeatReceiver::toVehicleProperty(const seat_msgs::msg::Heat& value) {
    auto v = getValuePool()->obtainInt32(value.level());
    v->prop = static_cast<int32_t>(VehicleProperty::SEAT_HEAT);
    v->areaId = value.zone();
    v->timestamp = elapsedRealtimeNano();
    return v;
}

/**
 * 调节座椅加热开关
 * topic: seat_msgs::srv::TOPIC_SET_SEAT_HEAT
 * struct: seat_msgs::msg::Heat
 * prop: SEAT_SET_HEAT
 */
DdsSeatSetHeatSender::DdsSeatSetHeatSender(const ::dds::domain::DomainParticipant& participant) 
    : DdsSender()
    , mWriter(::dds::core::null)
{

    std::cout<< "DdsSeatSetHeatSender constructor " << std::endl;
    ::dds::topic::Topic<seat_msgs::msg::Heat> topic = ::dds::core::null;
    utils::find_or_create_topic<seat_msgs::msg::Heat>(
            participant, topic, seat_msgs::srv::TOPIC_SET_SEAT_HEAT);

    ::dds::pub::qos::PublisherQos publisherQos = participant.default_publisher_qos();
    ::dds::pub::qos::DataWriterQos dataWriterQos(topic.qos());

    utils::create_datawriter<seat_msgs::msg::Heat>(
            mWriter,
            participant,
            topic,
            dataWriterQos,
            publisherQos);
} 

void DdsSeatSetHeatSender::send(const VehiclePropValue& propValue) {
    std::cout<< "DdsSeatSetHeatSender send value: " << propValue.value.int32Values[0]
        << ", zone: " << propValue.areaId << std::endl;
    seat_msgs::msg::Heat request(propValue.value.int32Values[0], propValue.areaId);
    mWriter->write(request);
};

/**
 * 方向盘加热状态
 * topic: hvac_msgs::msg::TOPIC_STEERING_WHEEL_HEAT
 * struct: hvac_msgs::msg::SteeringWheelHeat
 * prop: HVAC_STEERING_WHEEL_HEAT
 */
DdsSteeringWheelHeatReceiver::DdsSteeringWheelHeatReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, const ::dds::domain::DomainParticipant& participant, AsyncWaitSet & asyncWaitSet)
    : DdsReceiver(onHalEvent, pool, asyncWaitSet)
    , mReaderStatusCondition(::dds::core::null)
    , mReader(::dds::core::null)
{
 
    std::cout<< "DdsSteeringWheelHeatReceiver constructor " << std::endl;
    ::dds::topic::Topic<hvac_msgs::msg::SteeringWheelHeat> topic = ::dds::core::null;
    utils::find_or_create_topic<hvac_msgs::msg::SteeringWheelHeat>(
            participant, topic, hvac_msgs::msg::TOPIC_STEERING_WHEEL_HEAT);

    ::dds::sub::qos::SubscriberQos subscriberQos = participant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos dataReaderQos(topic.qos());

    utils::create_datareader<hvac_msgs::msg::SteeringWheelHeat>(
            mReader,
            participant,
            topic,
            dataReaderQos,
            subscriberQos);

    mReaderStatusCondition = ::dds::core::cond::StatusCondition(mReader);

    mReaderStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    
    mReaderStatusCondition.handler(std::bind(&DdsSteeringWheelHeatReceiver::handle, this));
    DdsReceiver::attach_condition(mReaderStatusCondition);
    std::cout<< "DdsSteeringWheelHeatReceiver constructor end" << std::endl;
}

DdsSteeringWheelHeatReceiver::~DdsSteeringWheelHeatReceiver() {
    DdsReceiver::detach_condition(mReaderStatusCondition);
}

void DdsSteeringWheelHeatReceiver::handle() {
    ::dds::sub::LoanedSamples<hvac_msgs::msg::SteeringWheelHeat> samples = mReader.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    for (::dds::sub::LoanedSamples<hvac_msgs::msg::SteeringWheelHeat>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            onNewProperty(std::move(toVehicleProperty(sample_it->data())));
        }
    }
}

VehiclePropValuePtr DdsSteeringWheelHeatReceiver::toVehicleProperty(const hvac_msgs::msg::SteeringWheelHeat& value) {
    auto v = getValuePool()->obtainInt32(value.level());
    v->prop = static_cast<int32_t>(VehicleProperty::HVAC_STEERING_WHEEL_HEAT);
    v->areaId = 0;
    v->timestamp = elapsedRealtimeNano();
    return v;
}

/**
 * 调节方向盘加热开关
 * topic: hvac_msgs::srv::TOPIC_SET_STEERING_WHEEL_HEAT
 * struct: hvac_msgs::msg::SteeringWheelHeat
 * prop: HVAC_SET_STEERING_WHEEL_HEAT
 */
DdsSetSteeringWheelHeatSender::DdsSetSteeringWheelHeatSender(const ::dds::domain::DomainParticipant& participant) 
    : DdsSender()
    , mWriter(::dds::core::null)
{

    std::cout<< "DdsSetSteeringWheelHeatSender constructor " << std::endl;
    ::dds::topic::Topic<hvac_msgs::msg::SteeringWheelHeat> topic = ::dds::core::null;
    utils::find_or_create_topic<hvac_msgs::msg::SteeringWheelHeat>(
            participant, topic, hvac_msgs::srv::TOPIC_SET_STEERING_WHEEL_HEAT);

    ::dds::pub::qos::PublisherQos publisherQos = participant.default_publisher_qos();
    ::dds::pub::qos::DataWriterQos dataWriterQos(topic.qos());

    utils::create_datawriter<hvac_msgs::msg::SteeringWheelHeat>(
            mWriter,
            participant,
            topic,
            dataWriterQos,
            publisherQos);
} 

void DdsSetSteeringWheelHeatSender::send(const VehiclePropValue& propValue) {
    hvac_msgs::msg::SteeringWheelHeat request(propValue.value.int32Values[0]);
    mWriter->write(request);
};

/**
 * 空调当前温度
 * topic: hvac_msgs::msg::TOPIC_HVAC_CURRENT_TEMPERATURE
 * struct: std_msgs::msg::Temperature
 * prop: HVAC_TEMPERATURE_CURRENT
 */
DdsHvacCurrentTemperatureReceiver::DdsHvacCurrentTemperatureReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, const ::dds::domain::DomainParticipant& participant, AsyncWaitSet & asyncWaitSet)
    : DdsReceiver(onHalEvent, pool, asyncWaitSet)
    , mReaderStatusCondition(::dds::core::null)
    , mReader(::dds::core::null)
{
 
    std::cout<< "DdsHvacCurrentTemperatureReceiver constructor " << std::endl;
    ::dds::topic::Topic<std_msgs::msg::Temperature> topic = ::dds::core::null;
    utils::find_or_create_topic<std_msgs::msg::Temperature>(
            participant, topic, hvac_msgs::msg::TOPIC_HVAC_CURRENT_TEMPERATURE);

    ::dds::sub::qos::SubscriberQos subscriberQos = participant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos dataReaderQos(topic.qos());

    utils::create_datareader<std_msgs::msg::Temperature>(
            mReader,
            participant,
            topic,
            dataReaderQos,
            subscriberQos);

    mReaderStatusCondition = ::dds::core::cond::StatusCondition(mReader);

    mReaderStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    
    mReaderStatusCondition.handler(std::bind(&DdsHvacCurrentTemperatureReceiver::handle, this));
    DdsReceiver::attach_condition(mReaderStatusCondition);
    std::cout<< "DdsHvacCurrentTemperatureReceiver constructor end" << std::endl;
}

DdsHvacCurrentTemperatureReceiver::~DdsHvacCurrentTemperatureReceiver() {
    DdsReceiver::detach_condition(mReaderStatusCondition);
}

void DdsHvacCurrentTemperatureReceiver::handle() {
    ::dds::sub::LoanedSamples<std_msgs::msg::Temperature> samples = mReader.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    for (::dds::sub::LoanedSamples<std_msgs::msg::Temperature>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            // std::cout << "Received sample:\n\t" << sample_it->data()
            //           << std::endl;
            onNewProperty(std::move(toVehicleProperty(sample_it->data())));
        }
    }
}

VehiclePropValuePtr DdsHvacCurrentTemperatureReceiver::toVehicleProperty(const std_msgs::msg::Temperature& value) {
    auto v = getValuePool()->obtainFloat(value.value());
    v->prop = static_cast<int32_t>(VehicleProperty::HVAC_TEMPERATURE_CURRENT);
    v->areaId = value.zone();
    v->timestamp = elapsedRealtimeNano();
    return v;
}

/**
 * 设置空调温度
 * topic: hvac_msgs::srv::TOPIC_HVAC_SET_TEMPERATURE
 * struct: std_msgs::msg::Temperature
 * prop: HVAC_TEMPERATURE_SET
 */
DdsHvacSetCurrentTemperatureSender::DdsHvacSetCurrentTemperatureSender(const ::dds::domain::DomainParticipant& participant) 
    : DdsSender()
    , mWriter(::dds::core::null)
{

    std::cout<< "DdsHvacSetCurrentTemperatureSender constructor " << std::endl;
    ::dds::topic::Topic<std_msgs::msg::Temperature> topic = ::dds::core::null;
    utils::find_or_create_topic<std_msgs::msg::Temperature>(
            participant, topic, hvac_msgs::srv::TOPIC_HVAC_SET_TEMPERATURE);

    ::dds::pub::qos::PublisherQos publisherQos = participant.default_publisher_qos();
    ::dds::pub::qos::DataWriterQos dataWriterQos(topic.qos());

    utils::create_datawriter<std_msgs::msg::Temperature>(
            mWriter,
            participant,
            topic,
            dataWriterQos,
            publisherQos);
} 

void DdsHvacSetCurrentTemperatureSender::send(const VehiclePropValue& propValue) {
    std_msgs::msg::Temperature request(propValue.value.floatValues[0], propValue.areaId);
    mWriter->write(request);
};











DdsHvacSetAcSender::DdsHvacSetAcSender(const ::dds::domain::DomainParticipant& participant) 
    : DdsSender()
    , mWriter(::dds::core::null)
{

    std::cout<< "DdsHvacSetAcSender constructor " << std::endl;
    ::dds::topic::Topic<hvac_msgs::srv::SetAcRequest> topic = ::dds::core::null;
    utils::find_or_create_topic<hvac_msgs::srv::SetAcRequest>(
            participant, topic, hvac_msgs::srv::TOPIC_SET_AC);

    ::dds::pub::qos::PublisherQos publisherQos = participant.default_publisher_qos();
    ::dds::pub::qos::DataWriterQos dataWriterQos(topic.qos());

    utils::create_datawriter<hvac_msgs::srv::SetAcRequest>(
            mWriter,
            participant,
            topic,
            dataWriterQos,
            publisherQos);
} 


void DdsHvacSetAcSender::send(const VehiclePropValue& propValue) {
    SetAcRequest request(propValue.value.int32Values[0], propValue.areaId);
    mWriter->write(request);
};

DdsAppointmentDriveReceiver::DdsAppointmentDriveReceiver(const OnHalEvent& onHalEvent, VehiclePropValuePool* pool, const ::dds::domain::DomainParticipant& participant, AsyncWaitSet & asyncWaitSet)
    : DdsReceiver(onHalEvent, pool, asyncWaitSet)
    , mReaderStatusCondition(::dds::core::null)
    , mReader(::dds::core::null)
{
 
    std::cout<< "DdsAppointmentDriveReceiver constructor " << std::endl;
    ::dds::topic::Topic<habit_msgs::srv::AppointmentDriveRequest> topic = ::dds::core::null;
    utils::find_or_create_topic<habit_msgs::srv::AppointmentDriveRequest>(
            participant, topic, habit_msgs::srv::TOPIC_APPOINTMENT_DRIVE);

    ::dds::sub::qos::SubscriberQos subscriberQos = participant.default_subscriber_qos();
    ::dds::sub::qos::DataReaderQos dataReaderQos(topic.qos());

    utils::create_datareader<habit_msgs::srv::AppointmentDriveRequest>(
            mReader,
            participant,
            topic,
            dataReaderQos,
            subscriberQos);

    mReaderStatusCondition = ::dds::core::cond::StatusCondition(mReader);

    mReaderStatusCondition.enabled_statuses(
            ::dds::core::status::StatusMask::data_available());
    
    mReaderStatusCondition.handler(std::bind(&DdsAppointmentDriveReceiver::handle, this));
    DdsReceiver::attach_condition(mReaderStatusCondition);
    std::cout<< "DdsAppointmentDriveReceiver constructor end" << std::endl;
}

DdsAppointmentDriveReceiver::~DdsAppointmentDriveReceiver() {
    DdsReceiver::detach_condition(mReaderStatusCondition);
}

void DdsAppointmentDriveReceiver::handle() {
    
    std::cout<< "DdsAppointmentDriveReceiver handle start" << std::endl;
    ::dds::sub::LoanedSamples<habit_msgs::srv::AppointmentDriveRequest> samples = mReader.take();
    std::vector<VehiclePropValuePtr> samples_vec;

    for (::dds::sub::LoanedSamples<habit_msgs::srv::AppointmentDriveRequest>::const_iterator sample_it =
                 samples.begin();
         sample_it != samples.end();
         sample_it++) {
        if (sample_it->info().valid()) {
            onNewProperty(std::move(toVehicleProperty(sample_it->data())));
        }
    }
}

VehiclePropValuePtr DdsAppointmentDriveReceiver::toVehicleProperty(const habit_msgs::srv::AppointmentDriveRequest& value) {
    auto v = getValuePool()->obtainInt64(value.timestamp());
    v->prop = static_cast<int32_t>(VehicleProperty::APPOINTMENT_DRIVE);
    v->areaId = toInt(VehicleArea::GLOBAL);
    v->timestamp = elapsedRealtimeNano();
    return v;
}

} // namespace dds
} // namespace headunit
