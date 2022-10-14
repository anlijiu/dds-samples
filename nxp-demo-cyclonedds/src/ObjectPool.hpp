#ifndef __ObjectPool_hpp__
#define __ObjectPool_hpp__

#include <atomic>
#include <map>
#include <functional>
#include <memory>
#include <mutex>
#include <deque>

#include "Can.hpp"
#include "vhal_compatible.hpp"

#define INC_METRIC_IF_DEBUG(val) PoolStats::instance()->val++;
struct PoolStats {
    std::atomic<uint32_t> Obtained {0};
    std::atomic<uint32_t> Created {0};
    std::atomic<uint32_t> Recycled {0};

    static PoolStats* instance() {
        static PoolStats inst;
        return &inst;
    }
};

template<typename T>
struct Deleter  {
    using OnDeleteFunc = std::function<void(T*)>;

    Deleter(const OnDeleteFunc& f) : mOnDelete(f) {};

    Deleter() = default;
    Deleter(const Deleter&) = default;

    void operator()(T* o) {
        mOnDelete(o);
    }
private:
    OnDeleteFunc mOnDelete;
};

/**
 * This is std::unique_ptr<> with custom delete operation that typically moves
 * the pointer it holds back to ObjectPool.
 */
template <typename T>
using recyclable_ptr = typename std::unique_ptr<T, Deleter<T>>;


template<typename T>
class ObjectPool {
public:
    ObjectPool() = default;
    virtual ~ObjectPool() = default;

    virtual recyclable_ptr<T> obtain() {
        std::lock_guard<std::mutex> g(mLock);
        INC_METRIC_IF_DEBUG(Obtained)
        if (mObjects.empty()) {
            INC_METRIC_IF_DEBUG(Created)
            return wrap(createObject());
        }

        auto o = wrap(mObjects.front().release());
        mObjects.pop_front();

        return o;
    }

    ObjectPool& operator =(const ObjectPool &) = delete;
    ObjectPool(const ObjectPool &) = delete;

protected:
    virtual T* createObject() = 0;

    virtual void recycle(T* o) {
        INC_METRIC_IF_DEBUG(Recycled)
        std::lock_guard<std::mutex> g(mLock);
        mObjects.push_back(std::unique_ptr<T> { o } );
    }

private:
    const Deleter<T>& getDeleter() {
        if (!mDeleter.get()) {
            Deleter<T> *d = new Deleter<T>(std::bind(
                &ObjectPool::recycle, this, std::placeholders::_1));
            mDeleter.reset(d);
        }
        return *mDeleter.get();
    }

    recyclable_ptr<T> wrap(T* raw) {
        return recyclable_ptr<T> { raw, getDeleter() };
    }

private:
    mutable std::mutex mLock;
    std::deque<std::unique_ptr<T>> mObjects;
    std::unique_ptr<Deleter<T>> mDeleter;
};

using CanFramePtr = recyclable_ptr<can_frame_t>;

class CanFramePool {
public:
    using RecyclableType = recyclable_ptr<can_frame_t>;

    CanFramePool() :
        mPool(std::make_unique<InternalPool>()) {}

    CanFramePool(CanFramePool& ) = delete;

    RecyclableType obtain();

    CanFramePool& operator=(CanFramePool&) = delete;
private:
    class InternalPool: public ObjectPool<can_frame_t> {
    public:
        InternalPool() {}

        RecyclableType obtain() {
            return ObjectPool<can_frame_t>::obtain();
        }
    protected:
        can_frame_t* createObject() override;
    private:
    };

private:
    const Deleter<can_frame_t> mDisposableDeleter {
        [] (can_frame_t* v) {
            delete v;
        }
    };

private:
    mutable std::mutex mLock;
    std::unique_ptr<InternalPool> mPool;
    // std::map<int32_t, std::unique_ptr<InternalPool>> mValueTypePools;
};

using VehiclePropValuePtr = recyclable_ptr<VehiclePropValue>;

class VehiclePropValuePool {
public:
    using RecyclableType = recyclable_ptr<VehiclePropValue>;

    /**
     * Creates VehiclePropValuePool
     *
     * @param maxRecyclableVectorSize - vector value types (e.g.
     * VehiclePropertyType::INT32_VEC) with size equal or less to this value
     * will be stored in the pool. If users tries to obtain value with vector
     * size greater than maxRecyclableVectorSize user will receive appropriate
     * object, but once it goes out of scope it will be deleted immediately, not
     * returning back to the object pool.
     * maxRecyclableVectorSize = 4-->11 (gxc)
     */
    VehiclePropValuePool(size_t maxRecyclableVectorSize = 11) :
        mMaxRecyclableVectorSize(maxRecyclableVectorSize) {};

    RecyclableType obtain(VehiclePropertyType type);

    RecyclableType obtain(VehiclePropertyType type, size_t vecSize);
    RecyclableType obtain(const VehiclePropValue& src);
    RecyclableType obtainBoolean(bool value);
    RecyclableType obtainInt32(int32_t value);
    RecyclableType obtainInt64(int64_t value);
    RecyclableType obtainFloat(float value);
    RecyclableType obtainString(const char* cstr);
    RecyclableType obtainComplex();

    VehiclePropValuePool(VehiclePropValuePool& ) = delete;
    VehiclePropValuePool& operator=(VehiclePropValuePool&) = delete;
private:
    bool isDisposable(VehiclePropertyType type, size_t vecSize) const {
        return vecSize > mMaxRecyclableVectorSize || VehiclePropertyType::STRING == type ||
               VehiclePropertyType::COMPLEX == type;
    }

    RecyclableType obtainDisposable(VehiclePropertyType valueType,
                                    size_t vectorSize) const;
    RecyclableType obtainRecylable(VehiclePropertyType type,
                                   size_t vecSize);

    class InternalPool: public ObjectPool<VehiclePropValue> {
    public:
        InternalPool(VehiclePropertyType type, size_t vectorSize)
            : mPropType(type), mVectorSize(vectorSize) {}

        RecyclableType obtain() {
            return ObjectPool<VehiclePropValue>::obtain();
        }
    protected:
        VehiclePropValue* createObject() override;
        void recycle(VehiclePropValue* o) override;
    private:
        bool check(VehiclePropValue::RawValue* v);

        template <typename VecType>
        bool check(std::vector<VecType>* vec, bool expected) {
            return vec->size() == (expected ? mVectorSize : 0);
        }
    private:
        VehiclePropertyType mPropType;
        size_t mVectorSize;
    };

private:
    const Deleter<VehiclePropValue> mDisposableDeleter {
        [] (VehiclePropValue* v) {
            delete v;
        }
    };

private:
    mutable std::mutex mLock;
    const size_t mMaxRecyclableVectorSize;
    std::map<int32_t, std::unique_ptr<InternalPool>> mValueTypePools;
};




#endif //__ObjectPool_hpp__
