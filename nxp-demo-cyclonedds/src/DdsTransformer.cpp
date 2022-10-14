


template <>
VehiclePropValuePtr DdsTransformer::propertyFromDds(::dds::sub::LoanedSamples<T> val);

template <>
::dds::sub::LoanedSamples<T> DdsTransformer::propertyToDds(VehiclePropValuePtr val);

template<typename D>
class DdsMessageMeta {
    ::dds::sub::DataReader<D> reader;
    ::dds::pub::DataWriter<D> writer;
}

std::map<uint64_t, std::shared_ptr<DdsMessageMeta>>
