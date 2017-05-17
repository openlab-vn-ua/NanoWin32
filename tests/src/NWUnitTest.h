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
 Assert::IsTrue(condition)

#define NW_CHECK_TRUE(condition) \
 Assert::IsTrue(condition)

#define NW_CHECK_FALSE(condition) \
 Assert::IsFalse(condition)

#define NW_CHECK_EQUAL(expected,actual) \
 Assert::AreEqual(expected,actual);

#define NW_CHECK_EQUAL_STRCMP(expected,actual) \
 Assert::AreEqual(expected,actual);

#define NW_CHECK_EQUAL_MEMCMP(expected,actual,size) \
 Assert::AreEqual(0,memcmp(expected,actual,size));

#define NW_CHECK_EQUAL_INTS(expected,actual) \
 Assert::AreEqual((int)(expected),(int)actual);

#define NW_CHECK_EQUAL_UINTS(expected,actual) \
 Assert::AreEqual((unsigned int)(expected),(unsigned int)actual);

#define NW_CHECK_EQUAL_LONGS(expected,actual) \
 Assert::AreEqual((long)(expected),(long)actual);

#define NW_CHECK_EQUAL_ULONGS(expected,actual) \
 Assert::AreEqual((unsigned long)(expected),(unsigned long)actual);

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

#define NW_CHECK_EQUAL_INTS(expected,actual) \
 LONGS_EQUAL((int)expected,(int)actual)

#define NW_CHECK_EQUAL_UINTS(expected,actual) \
 UNSIGNED_LONGS_EQUAL((unsigned int)expected,(unsigned int)actual)

#define NW_CHECK_EQUAL_LONGS(expected,actual) \
 LONGS_EQUAL(expected,actual)

#define NW_CHECK_EQUAL_ULONGS(expected,actual) \
 UNSIGNED_LONGS_EQUAL(expected,actual)

#endif

#endif
