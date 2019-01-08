//
// Created by 李寅斌 on 2019/1/7.
//

#include <gtest/gtest.h>
#include <kob/ksl/type_traits.h>

TEST(type_traits, is_const)
{
    EXPECT_EQ(ksl::is_const<int>::value, false);
    EXPECT_EQ(ksl::is_const<const int>::value, true);
    EXPECT_EQ(ksl::is_const<const int*>::value, false);
    EXPECT_EQ(ksl::is_const<int * const>::value, true);
}

TEST(type_traits, is_volatile)
{
    EXPECT_EQ(ksl::is_volatile<int>::value, false);
    EXPECT_EQ(ksl::is_volatile<volatile int>::value, true);
    EXPECT_EQ(ksl::is_volatile<volatile int*>::value, false);
    EXPECT_EQ(ksl::is_volatile<int* volatile>::value, true);
}

TEST(type_traits, remove_const)
{
    bool v = ksl::is_same<ksl::remove_const<int>::type,int>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_const<const int>::type,int>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_const<const int*>::type,int*>::value;
    EXPECT_EQ(v, false);
    v = ksl::is_same<ksl::remove_const<int* const>::type,int*>::value;
    EXPECT_EQ(v, true);
}

TEST(type_traits, remove_volatile)
{
    bool v = ksl::is_same<ksl::remove_volatile<int>::type,int>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_volatile<volatile int>::type,int>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_volatile<volatile int*>::type,int*>::value;
    EXPECT_EQ(v, false);
    v = ksl::is_same<ksl::remove_volatile<int* volatile>::type,int*>::value;
    EXPECT_EQ(v, true);
}

TEST(type_traits, remove_cv)
{
    bool v = ksl::is_same<ksl::remove_cv<int>::type,int>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_cv<volatile int>::type,int>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_cv<volatile int*>::type,int*>::value;
    EXPECT_EQ(v, false);
    v = ksl::is_same<ksl::remove_cv<int* volatile>::type,int*>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_cv<int* volatile>::type,int*>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_cv<int>::type,int>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_cv<const int>::type,int>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_cv<const int*>::type,int*>::value;
    EXPECT_EQ(v, false);
    v = ksl::is_same<ksl::remove_cv<int* const>::type,int*>::value;
    EXPECT_EQ(v, true);

    v = ksl::is_same<ksl::remove_cv<const volatile int>::type,int>::value;
    EXPECT_EQ(v, true);
    v = ksl::is_same<ksl::remove_cv<const volatile int*>::type,int*>::value;
    EXPECT_EQ(v, false);
    v = ksl::is_same<ksl::remove_cv<int* volatile const>::type,int*>::value;
    EXPECT_EQ(v, true);

    v = ksl::is_same<ksl::remove_cv<int* const volatile >::type,int*>::value;
    EXPECT_EQ(v, true);
}

TEST(type_traits, is_reference)
{
    EXPECT_EQ(ksl::is_reference<int>::value, false);
    EXPECT_EQ(ksl::is_reference<int&>::value, true);
    EXPECT_EQ(ksl::is_reference<int&&>::value, true);
    class Dumy{};
    EXPECT_EQ(ksl::is_reference<Dumy>::value, false);
    EXPECT_EQ(ksl::is_reference<Dumy&>::value, true);
    EXPECT_EQ(ksl::is_reference<Dumy&&>::value, true);

}

namespace is_empty_c{
    class A {};
    class B{
    public:
        int m;
    };
    class C{ static int m; };
    class D{ virtual ~D();};
}
TEST(type_traits, is_empty)
{
    EXPECT_EQ(ksl::is_empty<is_empty_c::A>::value, true);
    EXPECT_EQ(ksl::is_empty<is_empty_c::B>::value, false);
    EXPECT_EQ(ksl::is_empty<is_empty_c::C>::value, true);
    EXPECT_EQ(ksl::is_empty<is_empty_c::D>::value, false);//not a class
}

TEST(type_traits, is_void)
{
    EXPECT_EQ(ksl::is_void<int>::value, false);
    EXPECT_EQ(ksl::is_void<void>::value, true);
    EXPECT_EQ(ksl::is_void<void*>::value, false);
}

TEST(type_traits, is_object)
{
    EXPECT_EQ(ksl::is_object<int>(), true);
    EXPECT_EQ(ksl::is_object<int[]>(), true);
    EXPECT_EQ(ksl::is_object<int&>(), false);
    EXPECT_EQ(ksl::is_object<int*>(), true);
}

TEST(type_traits, is_null_pointer)
{
    EXPECT_EQ(ksl::is_null_pointer<decltype(nullptr)>::value, true);
    EXPECT_EQ(ksl::is_null_pointer<int*>::value, false);
}

TEST(type_traits, is_pointer)
{
    EXPECT_EQ(ksl::is_pointer<decltype(nullptr)>::value, false);
    EXPECT_EQ(ksl::is_pointer<int*>::value, true);
}

TEST(type_traits, is_scalar)
{
   class A{};
   EXPECT_EQ(ksl::is_scalar<int>(), true);
   EXPECT_EQ(ksl::is_scalar<const int>(), true);
   EXPECT_EQ(ksl::is_scalar<int*>(), true);
   EXPECT_EQ(ksl::is_scalar<int* const>(), true);
   EXPECT_EQ(ksl::is_scalar<int* volatile const>(), true);
   EXPECT_EQ(ksl::is_scalar<A>(), false);
}

TEST(type_traits, is_array)
{
    class A{};
    EXPECT_EQ(ksl::is_array<A>(), false);
    EXPECT_EQ(ksl::is_array<A[]>(), true);
    EXPECT_EQ(ksl::is_array<A[1]>(), true);
    EXPECT_EQ(ksl::is_array<int>(), false);
    EXPECT_EQ(ksl::is_array<int[]>(), true);
    EXPECT_EQ(ksl::is_array<int[1]>(), true);
}