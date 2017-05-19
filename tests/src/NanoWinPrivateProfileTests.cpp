#include <string.h>
#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinPrivateProfile.h"
#else
 #include <windows.h>
#endif

#include "NanoWinPrivateProfileTestTools.h"

NW_BEGIN_TEST_GROUP(GetPrivateProfileIntATestGroup)
NW_BEGIN_SETUP_TEARDOWN
NW_TEARDOWN_METHOD()
 {
   TestINIFileRemove();
 }
NW_END_SETUP_TEARDOWN

NW_TEST(GetPrivateProfileIntATestGroup,TestExistingValueReadSimple)
 {
  bool fileCreated = TestINIFileCreateBinary(
  "[Generic]\n"
  "key1=42\n"
  "key2=33\n");

  NW_CHECK(fileCreated);

  UINT value = GetPrivateProfileIntA
               ("Generic", "key1", 0,
                TestINIFileName);

  NW_CHECK_EQUAL_UINTS(42, value);
 }

NW_TEST(GetPrivateProfileIntATestGroup,TestKeyNotExists)
 {
  bool fileCreated = TestINIFileCreateBinary(
  "[Generic]\n"
  "key1=42\n"
  "key2=33\n");

  NW_CHECK(fileCreated);

  UINT value = GetPrivateProfileIntA
               ("Generic", "key3", 0,
                TestINIFileName);

  NW_CHECK_EQUAL_UINTS(0, value);
 }

NW_TEST(GetPrivateProfileIntATestGroup,TestCharValueReadSimple)
 {
  bool fileCreated = TestINIFileCreateBinary(
  "[Generic]\n"
  "key1=bbb\n"
  "key2=33\n");

  NW_CHECK(fileCreated);

  UINT value = GetPrivateProfileIntA
               ("Generic", "key1", 0,
                TestINIFileName);

  NW_CHECK_EQUAL_UINTS(0, value);
 }

NW_END_TEST_GROUP()

NW_BEGIN_TEST_GROUP(GetPrivateProfileSectionATestGroup)
NW_BEGIN_SETUP_TEARDOWN
NW_TEARDOWN_METHOD()
 {
   TestINIFileRemove();
 }
NW_END_SETUP_TEARDOWN

NW_TEST(GetPrivateProfileSectionATestGroup, TestExistingSectionReadSimple)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic]\n"
		"key1=42\n"
		"key2=33\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileSectionA
	("Generic", value, sizeof(value),
		TestINIFileName);

	NW_CHECK_EQUAL_ULONGS(1, valueLen);
	NW_CHECK_EQUAL_STRCMP(" ", value);
}

NW_TEST(GetPrivateProfileSectionATestGroup, TestExistingSectionReadSimple2)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic1]\n"
		"key1=42\n"
		"key2=33\n"
		"[Generic2]\n"
		"key1=1\n"
		"key2=2\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileSectionA
	("Generic1", value, sizeof(value),
		TestINIFileName);

	NW_CHECK_EQUAL_ULONGS(1, valueLen);
	NW_CHECK_EQUAL_STRCMP(" ", value);
}

NW_TEST(GetPrivateProfileSectionATestGroup, TestExistingSectionReadSimple3)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic1]\n"
		"key1=42\n"
		"key2=33\n"
		"[Generic2]\n"
		"key1=1\n"
		"key2=2\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileSectionA
	("Generic2", value, sizeof(value),
		TestINIFileName);

	NW_CHECK_EQUAL_ULONGS(1, valueLen);
	NW_CHECK_EQUAL_STRCMP(" ", value);
}

NW_TEST(GetPrivateProfileSectionATestGroup, TestSectionNotExists)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic1]\n"
		"key1=42\n"
		"key2=33\n"
		"[Generic2]\n"
		"key1=1\n"
		"key2=2\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileSectionA
	("Generic3", value, sizeof(value),
		TestINIFileName);

	NW_CHECK_EQUAL_ULONGS(0, valueLen);
	NW_CHECK_EQUAL('\0', value[0]);
}

NW_TEST(GetPrivateProfileSectionATestGroup, TestFileNotExists)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic1]\n"
		"key1=42\n"
		"key2=33\n"
		"[Generic2]\n"
		"key1=1\n"
		"key2=2\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileSectionA
	("Generic1", value, sizeof(value),
		"/home/asti/Work/UnexistFile.ini");

	NW_CHECK_EQUAL_ULONGS(0, valueLen);
	NW_CHECK_EQUAL('\0', value[0]);
}

NW_END_TEST_GROUP()

NW_BEGIN_TEST_GROUP(GetPrivateProfileStringATestGroup)
NW_BEGIN_SETUP_TEARDOWN
NW_TEARDOWN_METHOD()
 {
   TestINIFileRemove();
 }
NW_END_SETUP_TEARDOWN

NW_TEST(GetPrivateProfileStringATestGroup, TestExistingKeyReadSimple)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic]\n"
		"key1=42\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileStringA
	("Generic", "key1",
		"undefined", value, sizeof(value),
		TestINIFileName);


	NW_CHECK_EQUAL_ULONGS(2, valueLen);
	NW_CHECK_EQUAL_STRCMP("42", value);
}

NW_TEST(GetPrivateProfileStringATestGroup, TestExistingKeyReadSimple2)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic]\n"
		"key2=33\n"
		"key1=42\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileStringA
	("Generic", "key1",
		"undefined", value, sizeof(value),
		TestINIFileName);


	NW_CHECK_EQUAL_ULONGS(2, valueLen);
	NW_CHECK_EQUAL_STRCMP("42", value);
}

NW_TEST(GetPrivateProfileStringATestGroup, TestExistingKeyReadSimple3)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic]\r\n"
		"key2=33\r\n"
		"key1=42\r\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileStringA
	("Generic", "key1",
		"undefined", value, sizeof(value),
		TestINIFileName);


	NW_CHECK_EQUAL_ULONGS(2, valueLen);
	NW_CHECK_EQUAL_STRCMP("42", value);
}

NW_TEST(GetPrivateProfileStringATestGroup, TestFileNotExist)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic]\n"
		"key1=42\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileStringA
	("Generic", "key1",
		"undefined", value, sizeof(value),
		"/home/asti/work/unexistFile.ini");

	NW_CHECK_EQUAL(9, valueLen);
	NW_CHECK_EQUAL_STRCMP("undefined", value);
}

NW_TEST(GetPrivateProfileStringATestGroup, TestSectionNotExist)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic]\n"
		"key1=42\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileStringA
	("Generic1", "key1",
		"undefined", value, sizeof(value),
		TestINIFileName);

	NW_CHECK_EQUAL(9, valueLen);
	NW_CHECK_EQUAL_STRCMP("undefined", value);
}

NW_TEST(GetPrivateProfileStringATestGroup, TestKeyNotExist)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic]\n"
		"key1=42\n");

	NW_CHECK(fileCreated);

	TestINIFileValueString value;

	DWORD valueLen = GetPrivateProfileStringA
	("Generic", "key4",
		"undefined", value, sizeof(value),
		TestINIFileName);

	NW_CHECK_EQUAL(9, valueLen);
	NW_CHECK_EQUAL_STRCMP("undefined", value);
}

NW_TEST(GetPrivateProfileStringATestGroup, TestShortInputBufferSupport)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic]\n"
		"key1=42424242\n");

	NW_CHECK(fileCreated);

	char value[5];

	DWORD valueLen = GetPrivateProfileStringA
	("Generic", "key1",
		"undefined", value, sizeof(value),
		TestINIFileName);

	NW_CHECK_EQUAL_ULONGS(4, valueLen);
	NW_CHECK_EQUAL_STRCMP("4242", value);
}

NW_END_TEST_GROUP()


NW_BEGIN_TEST_GROUP(WritePrivateProfileStringATestGroup)
NW_BEGIN_SETUP_TEARDOWN
NW_TEARDOWN_METHOD()
 {
   TestINIFileRemove();
 }
NW_END_SETUP_TEARDOWN

NW_TEST(WritePrivateProfileStringATestGroup, TestExistingKeyWriteSimple)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic]\n"
		"key1=3\n");

	NW_CHECK(fileCreated);

	BOOL fileWritten = WritePrivateProfileStringA
	("Generic", "key1",
		"55",
		TestINIFileName);

	NW_CHECK(fileWritten);

	std::string content = TestINIFileReadBinary();

	NW_CHECK_EQUAL_STRCMP("[Generic]\n"
		"key1=55\n",
		content.c_str());
}

NW_TEST(WritePrivateProfileStringATestGroup, TestExistingKeyWriteSimple2)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic1]\n"
		"key1=3\n"
		"[Generic2]\n"
		"key2=11\n");

	NW_CHECK(fileCreated);

	BOOL fileWritten = WritePrivateProfileStringA
	("Generic2", "key2",
		"55",
		TestINIFileName);

	NW_CHECK(fileWritten);

	std::string content = TestINIFileReadBinary();

	NW_CHECK_EQUAL_STRCMP("[Generic1]\n"
		"key1=3\n"
		"[Generic2]\n"
		"key2=55\n",
		content.c_str());
}

NW_TEST(WritePrivateProfileStringATestGroup, TestExistingKeyWriteSimple3)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic1]\n"
		"key1=3\n"
		"[Generic2]\n"
		"key2=11\n");

	NW_CHECK(fileCreated);

	BOOL fileWritten = WritePrivateProfileStringA
	("Generic1", "key1",
		"55",
		TestINIFileName);

	NW_CHECK(fileWritten);

	std::string content = TestINIFileReadBinary();

	NW_CHECK_EQUAL_STRCMP("[Generic1]\n"
		"key1=55\n"
		"[Generic2]\n"
		"key2=11\n",
		content.c_str());
}

NW_TEST(WritePrivateProfileStringATestGroup, TestExistingKeyWriteSimple4)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic1]\r\n"
		"key1=3\r\n"
		"[Generic2]\r\n"
		"key2=11\r\n");

	NW_CHECK(fileCreated);

	BOOL fileWritten = WritePrivateProfileStringA
	("Generic1", "key1",
		"55",
		TestINIFileName);

	NW_CHECK(fileWritten);

	std::string content = TestINIFileReadBinary();

	NW_CHECK_EQUAL_STRCMP("[Generic1]\r\n"
		"key1=55\r\n"
		"[Generic2]\r\n"
		"key2=11\r\n",
		content.c_str());
}

NW_TEST(WritePrivateProfileStringATestGroup, TestFileNotExists)
{
	BOOL fileWritten = WritePrivateProfileStringA
	("Generic1", "key1",
		"55",
		TestINIFileName);

	NW_CHECK(fileWritten);

	std::string content = TestINIFileReadBinary();

	NW_CHECK_EQUAL_STRCMP("[Generic1]\n"
		"key1=55\n",
		content.c_str());
}

NW_TEST(WritePrivateProfileStringATestGroup, TestSectionNotExists)
{
	bool fileCreated = TestINIFileCreateBinary(
		"[Generic1]\n"
		"key2=11\n");

	NW_CHECK(fileCreated);

	BOOL fileWritten = WritePrivateProfileStringA
	("Generic3", "key1",
		"55",
		TestINIFileName);

	NW_CHECK(fileWritten);

	std::string content = TestINIFileReadBinary();

	NW_CHECK_EQUAL_STRCMP("[Generic1]\n"
		"key2=11\n"
		"[Generic3]\n"
		"key1=55\n",
		content.c_str());
}

NW_END_TEST_GROUP()
