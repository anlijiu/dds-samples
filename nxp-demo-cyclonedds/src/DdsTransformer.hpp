/**
 * 将 VehiclePropertyValue 转为 DDS message 
 * 将 DDS message 转为 VehiclePropertyValue 
 *
 * 对于CAN总线来说 , 可以用更 generic 的方式
 * 但是dds 就只能用更具体的方式, 这里采用模板
 * 并且对dds中的每一个message 都全特化来实现
 * 转换的目的。
 */
#ifndef __DdsTransformer_hpp__
#define __DdsTransformer_hpp__

namespace headunit {
namespace dds {

template <typename T>
class DdsTransformer {
public:
    VehiclePropValuePtr propertyFromDds(::dds::sub::LoanedSamples<T> val);
    ::dds::sub::LoanedSamples<T> propertyToDds(VehiclePropValuePtr val);
private:
    ::dds::sub::DataReader<T> reader;
    ::dds::pub::DataWriter<T> writer;
}

} // namespace dds
} // namespace headunit


#endif //__DdsTransformer_hpp__
