#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinBmp.h"
#else
 #include <windows.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinBMPTestGroup)

NW_TEST(NanoWinBMPTestGroup, GetRValueTest)
{
	NW_CHECK_EQUAL((unsigned char)0x00, GetRValue(0xFFFFFF00));
	NW_CHECK_EQUAL((unsigned char)0xFF, GetRValue(0x000000FF));
	NW_CHECK_EQUAL((unsigned char)0x11, GetRValue(0x44332211));
	NW_CHECK_EQUAL((unsigned char)0xCC, GetRValue(0xFFEEDDCC));
}

NW_TEST(NanoWinBMPTestGroup, GetGValueTest)
{
	NW_CHECK_EQUAL((unsigned char)0x00, GetGValue(0xFFFF00FF));
	NW_CHECK_EQUAL((unsigned char)0xFF, GetGValue(0x0000FFFF));
	NW_CHECK_EQUAL((unsigned char)0x22, GetGValue(0x44332211));
	NW_CHECK_EQUAL((unsigned char)0xDD, GetGValue(0xFFEEDDCC));
}

NW_TEST(NanoWinBMPTestGroup, GetBValueTest)
{
	NW_CHECK_EQUAL((unsigned char)0x00, GetBValue(0xFF00FFFF));
	NW_CHECK_EQUAL((unsigned char)0xFF, GetBValue(0x00FF00FF));
	NW_CHECK_EQUAL((unsigned char)0x33, GetBValue(0x44332211));
	NW_CHECK_EQUAL((unsigned char)0xEE, GetBValue(0xFFEEDDCC));
}

NW_TEST(NanoWinBMPTestGroup, RGBTest)
{
	NW_CHECK_EQUAL(0LU,RGB(0xFF, 0xFF, 0xFF) >> 24);

	NW_CHECK_EQUAL(0x00112233LU, RGB(0x33, 0x22, 0x11));
	NW_CHECK_EQUAL(0x00FFFFFFLU, RGB(0xFF, 0xFF, 0xFF));
	NW_CHECK_EQUAL(0x00000000LU, RGB(0x00, 0x00, 0x00));
}

NW_END_TEST_GROUP()

