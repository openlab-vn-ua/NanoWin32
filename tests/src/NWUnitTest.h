#ifndef __NWUNITTEST_H__
#define __NWUNITTEST_H__

#ifdef __linux
 #define NW_UNIT_TEST_USE_CPPUTEST (1)

 #include <CppUTest/TestHarness.h>
#else
 #define NW_UNIT_TEST_USE_VSTEST (1)

 #include "CppUnitTest.h"

 #include <string.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

#ifdef NW_UNIT_TEST_USE_VSTEST
#define NW_BEGIN_TEST_GROUP_SIMPLE(GroupName) \
 TEST_CLASS(GroupName) \
  { \
   public :

#define NW_BEGIN_TEST_GROUP(GroupName) \
 TEST_CLASS(GroupName) \
  { \
   public :

#define NW_BEGIN_SETUP_TEARDOWN
#define NW_END_SETUP_TEARDOWN

#define NW_SETUP_METHOD() \
 TEST_METHOD_INITIALIZE(setup)

#define NW_TEARDOWN_METHOD() \
 TEST_METHOD_CLEANUP(teardown)

#define NW_END_TEST_GROUP() \
  };

#define NW_TEST(GroupName,TestName) \
 TEST_METHOD(TestName)

#define NW_SKIP_IF(condition) \
 if (condition) return

#define NW_CHECK(condition) \
 Assert::IsTrue(condition, L"" "(" #condition ")" " // expected:true // ", LINE_INFO())

#define NW_CHECK_TRUE(condition) \
 Assert::IsTrue(condition, L"" "(" #condition ")" " // expected:true // ", LINE_INFO())

#define NW_CHECK_FALSE(condition) \
 Assert::IsFalse(condition, L"" "(" #condition ")" " // expected:false // ", LINE_INFO())

#define NW_CHECK_EQUAL(expected,actual) \
 Assert::AreEqual(expected,actual, L"" "(" #expected "==" #actual")" " // expected:== // ", LINE_INFO());

#define NW_CHECK_EQUAL_STRCMP(expected,actual) \
 Assert::AreEqual(expected,actual, L"" "(" #expected "==" #actual")" " // expected:==[str] // ", LINE_INFO());

#define NW_CHECK_EQUAL_MEMCMP(expected,actual,size) \
 Assert::AreEqual(0,memcmp(expected,actual,size), L"" "(" #expected "==" #actual")" " // expected:==[mem] // ", LINE_INFO());

#define NW_CHECK_EQUAL_TYPES(expected,actual,type)    NW_CHECK_EQUAL((type)expected,(type)actual)

#define NW_CHECK_EQUAL_INTS(expected,actual)          NW_CHECK_EQUAL_TYPES(expected,actual,int)
#define NW_CHECK_EQUAL_UINTS(expected,actual)         NW_CHECK_EQUAL_TYPES(expected,actual,unsigned int)
#define NW_CHECK_EQUAL_LONGS(expected,actual)         NW_CHECK_EQUAL_TYPES(expected,actual,long)
#define NW_CHECK_EQUAL_ULONGS(expected,actual)        NW_CHECK_EQUAL_TYPES(expected,actual,unsigned long)
#define NW_CHECK_EQUAL_BYTES(expected,actual)         NW_CHECK_EQUAL_TYPES(expected,actual,uint8_t)

#else

#define NW_BEGIN_TEST_GROUP_SIMPLE(GroupName) \
 TEST_GROUP(GroupName) \
  { \
  };

#define NW_END_TEST_GROUP()

#define NW_BEGIN_TEST_GROUP(GroupName) \
 TEST_GROUP(GroupName) \
  {

#define NW_BEGIN_SETUP_TEARDOWN
#define NW_END_SETUP_TEARDOWN };

#define NW_SETUP_METHOD() \
 void setup()

#define NW_TEARDOWN_METHOD() \
 void teardown()

#define NW_TEST(GroupName,TestName) \
 TEST(GroupName,TestName)

#define NW_SKIP_IF(condition) \
 if (condition) return

#define NW_CHECK(condition) \
 CHECK(condition)

#define NW_CHECK_TRUE(condition) \
 CHECK(condition)

#define NW_CHECK_FALSE(condition) \
 CHECK_FALSE(condition)

#define NW_CHECK_EQUAL(expected,actual) \
 CHECK_EQUAL(expected,actual)

#define NW_CHECK_EQUAL_STRCMP(expected,actual) \
 STRCMP_EQUAL(expected,actual)

#define NW_CHECK_EQUAL_MEMCMP(expected,actual,size) \
 MEMCMP_EQUAL(expected,actual,size)

#define NW_CHECK_EQUAL_TYPES(expected,actual,type)    NW_CHECK_EQUAL((type)expected,(type)actual)

#define NW_CHECK_EQUAL_INTS(expected,actual)          NW_CHECK_EQUAL_TYPES(expected,actual,int)
#define NW_CHECK_EQUAL_UINTS(expected,actual)         NW_CHECK_EQUAL_TYPES(expected,actual,unsigned int)
#define NW_CHECK_EQUAL_LONGS(expected,actual)         NW_CHECK_EQUAL_TYPES(expected,actual,long)
#define NW_CHECK_EQUAL_ULONGS(expected,actual)        NW_CHECK_EQUAL_TYPES(expected,actual,unsigned long)
#define NW_CHECK_EQUAL_BYTES(expected,actual)         NW_CHECK_EQUAL_TYPES(expected,actual,uint8_t)

#endif

#endif
