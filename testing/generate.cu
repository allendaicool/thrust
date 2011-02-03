#include <unittest/unittest.h>
#include <thrust/generate.h>
#include <thrust/iterator/discard_iterator.h>

__THRUST_DISABLE_MSVC_POSSIBLE_LOSS_OF_DATA_WARNING_BEGIN

template<typename T>
struct return_value
{
    T val;

    return_value(void){}
    return_value(T v):val(v){}

    __host__ __device__
    T operator()(void){ return val; }
};

template<class Vector>
void TestGenerateSimple(void)
{
    typedef typename Vector::value_type T;

    Vector result(5);

    T value = 13;

    return_value<T> f(value);

    thrust::generate(result.begin(), result.end(), f);

    ASSERT_EQUAL(result[0], value);
    ASSERT_EQUAL(result[1], value);
    ASSERT_EQUAL(result[2], value);
    ASSERT_EQUAL(result[3], value);
    ASSERT_EQUAL(result[4], value);
}
DECLARE_VECTOR_UNITTEST(TestGenerateSimple);

template <typename T>
void TestGenerate(const size_t n)
{
    thrust::host_vector<T> h_result(n);
    thrust::device_vector<T> d_result(n);

    T value = 13;
    return_value<T> f(value);

    thrust::generate(h_result.begin(), h_result.end(), f);
    thrust::generate(d_result.begin(), d_result.end(), f);

    ASSERT_EQUAL(h_result, d_result);
}
DECLARE_VARIABLE_UNITTEST(TestGenerate);

template <typename T>
void TestGenerateToDiscardIterator(const size_t n)
{
    T value = 13;
    return_value<T> f(value);

    thrust::discard_iterator<thrust::host_space_tag> h_first;
    thrust::generate(h_first, h_first + 10, f);

    thrust::discard_iterator<thrust::device_space_tag> d_first;
    thrust::generate(d_first, d_first + 10, f);

    // there's nothing to actually check except that it compiles
}
DECLARE_VARIABLE_UNITTEST(TestGenerateToDiscardIterator);

template<class Vector>
void TestGenerateNSimple(void)
{
    typedef typename Vector::value_type T;

    Vector result(5);

    T value = 13;

    return_value<T> f(value);

    thrust::generate_n(result.begin(), result.size(), f);

    ASSERT_EQUAL(result[0], value);
    ASSERT_EQUAL(result[1], value);
    ASSERT_EQUAL(result[2], value);
    ASSERT_EQUAL(result[3], value);
    ASSERT_EQUAL(result[4], value);
}
DECLARE_VECTOR_UNITTEST(TestGenerateNSimple);

template <typename T>
void TestGenerateNToDiscardIterator(const size_t n)
{
    T value = 13;
    return_value<T> f(value);

    thrust::discard_iterator<thrust::host_space_tag> h_result = 
      thrust::generate_n(thrust::discard_iterator<thrust::host_space_tag>(), n, f);

    thrust::discard_iterator<thrust::device_space_tag> d_result = 
      thrust::generate_n(thrust::discard_iterator<thrust::device_space_tag>(), n, f);

    thrust::discard_iterator<> reference(n);

    ASSERT_EQUAL_QUIET(reference, h_result);
    ASSERT_EQUAL_QUIET(reference, d_result);
}
DECLARE_VARIABLE_UNITTEST(TestGenerateNToDiscardIterator);


template <typename Vector>
void TestGenerateZipIterator(void)
{
#if 1
    KNOWN_FAILURE;
#else
    typedef typename Vector::value_type T;

    Vector v1(3,T(0));
    Vector v2(3,T(0));

    thrust::generate(thrust::make_zip_iterator(thrust::make_tuple(v1.begin(),v2.begin())),
                     thrust::make_zip_iterator(thrust::make_tuple(v1.end(),v2.end())),
                     return_value< thrust::tuple<T,T> > (thrust::tuple<T,T>(4,7)));

    ASSERT_EQUAL(v1[0], 4);
    ASSERT_EQUAL(v1[1], 4);
    ASSERT_EQUAL(v1[2], 4);
    ASSERT_EQUAL(v2[0], 7);
    ASSERT_EQUAL(v2[1], 7);
    ASSERT_EQUAL(v2[2], 7);
#endif
};
DECLARE_VECTOR_UNITTEST(TestGenerateZipIterator);


void TestGenerateTuple(void)
{
    typedef int T;
    typedef thrust::tuple<T,T> Tuple;

    thrust::host_vector<Tuple>   h(3, Tuple(0,0));
    thrust::device_vector<Tuple> d(3, Tuple(0,0));

    thrust::generate(h.begin(), h.end(), return_value<Tuple>(Tuple(4,7)));
    thrust::generate(d.begin(), d.end(), return_value<Tuple>(Tuple(4,7)));

    ASSERT_EQUAL_QUIET(h, d);
};
DECLARE_UNITTEST(TestGenerateTuple);

__THRUST_DISABLE_MSVC_POSSIBLE_LOSS_OF_DATA_WARNING_END
