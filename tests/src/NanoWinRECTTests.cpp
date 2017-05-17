#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinRECT.h"
#else
 #include <windows.h>
#endif

#define NW_TEST_MAKE_RECT(rect,x0,y0,x1,y1) \
  	(rect).left   = (x0);                   \
    (rect).right  = (x1);                   \
    (rect).top    = (y0);                   \
    (rect).bottom = (y1);

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinRECTTestGroup)

NW_TEST(NanoWinRECTTestGroup, IsRectEmpty_BothAxesEqualTest)
{
	RECT rect;

	NW_TEST_MAKE_RECT(rect, 0, 0, 0, 0);

	NW_CHECK(IsRectEmpty(&rect));

	NW_TEST_MAKE_RECT(rect, 1, 5, 1, 5);

	NW_CHECK(IsRectEmpty(&rect));
}

NW_TEST(NanoWinRECTTestGroup, IsRectEmpty_YAxeEqualsTest)
{
	RECT rect;

	NW_TEST_MAKE_RECT(rect, 0, 1, 5, 1);

	NW_CHECK(IsRectEmpty(&rect));
}

NW_TEST(NanoWinRECTTestGroup, IsRectEmpty_XAxeEqualTest)
{
	RECT rect;

	NW_TEST_MAKE_RECT(rect, 1, 0, 1, 5);
	
	NW_CHECK(IsRectEmpty(&rect));
}

NW_TEST(NanoWinRECTTestGroup, IsRectEmpty_RectNotEmptyTest)
{
	RECT rect;

	NW_TEST_MAKE_RECT(rect, 0, 0, 5, 5);

	NW_CHECK_FALSE(IsRectEmpty(&rect));
}

NW_TEST(NanoWinRECTTestGroup, IsRectEmpty_SizeNegativeTest)
{
	RECT rect;

	NW_TEST_MAKE_RECT(rect, 5, 5, 0, 0);

	NW_CHECK(IsRectEmpty(&rect));
}

////

NW_TEST(NanoWinRECTTestGroup, EqualRect_EmptyTest)
{
	RECT rect1;
	RECT rect2;

	NW_TEST_MAKE_RECT(rect1, 0, 0, 0, 0);
	NW_TEST_MAKE_RECT(rect2, 0, 0, 0, 0);

	NW_CHECK(EqualRect(&rect1, &rect2));
}

NW_TEST(NanoWinRECTTestGroup, EqualRect_NotEmptyTest)
{
	RECT rect1;
	RECT rect2;

	NW_TEST_MAKE_RECT(rect1, 1, 3, 2, 4);
	NW_TEST_MAKE_RECT(rect2, 1, 3, 2, 4);

	NW_CHECK(EqualRect(&rect1, &rect2));
	NW_CHECK(EqualRect(&rect2, &rect1));
}

NW_TEST(NanoWinRECTTestGroup, EqualRect_SizeNegativeTest)
{
	RECT rect1;
	RECT rect2;

	NW_TEST_MAKE_RECT(rect1, 1, 3, 2, 4);
	NW_TEST_MAKE_RECT(rect2, 2, 4, 1, 3);

	NW_CHECK_FALSE(EqualRect(&rect1, &rect2));
}

//////

NW_TEST(NanoWinRECTTestGroup, CopyRectTest)
{
	RECT rectDst;
	RECT rectSrc;

	NW_TEST_MAKE_RECT(rectSrc, 1, 3, 2, 4);

	NW_CHECK(CopyRect(&rectDst, &rectSrc));

	NW_CHECK_EQUAL(rectSrc.left, rectDst.left);
	NW_CHECK_EQUAL(rectSrc.right, rectDst.right);
	NW_CHECK_EQUAL(rectSrc.top, rectDst.top);
	NW_CHECK_EQUAL(rectSrc.bottom, rectDst.bottom);
}

/////

NW_TEST(NanoWinRECTTestGroup, SetRectEmptyTest)
{
	RECT rectDst;

	NW_TEST_MAKE_RECT(rectDst, 1, 3, 2, 4);

	NW_CHECK(SetRectEmpty(&rectDst));

	NW_CHECK_EQUAL_LONGS(0, rectDst.left);
	NW_CHECK_EQUAL_LONGS(0, rectDst.right);
	NW_CHECK_EQUAL_LONGS(0, rectDst.top);
	NW_CHECK_EQUAL_LONGS(0, rectDst.bottom);
}

///////

NW_TEST(NanoWinRECTTestGroup, SetRectTest)
{
	RECT rect;

	NW_TEST_MAKE_RECT(rect, 1, 3, 2, 4);

	NW_CHECK(SetRect(&rect, 1, 3, 2, 4));

	NW_CHECK_EQUAL_LONGS(1, rect.left);
	NW_CHECK_EQUAL_LONGS(3, rect.top);
	NW_CHECK_EQUAL_LONGS(2, rect.right);
	NW_CHECK_EQUAL_LONGS(4, rect.bottom);
}

/////

NW_TEST(NanoWinRECTTestGroup, IntersectRect_NoIntersectTest)
{
	RECT rect1;
	RECT rect2;
	RECT rectDst;

	NW_TEST_MAKE_RECT(rect1, 1, 3, 2, 4);
	NW_TEST_MAKE_RECT(rect2, 5, 7, 6, 8);

	NW_CHECK_FALSE(IntersectRect(&rectDst, &rect1, &rect2));

	NW_CHECK_EQUAL_LONGS(0, rectDst.left);
	NW_CHECK_EQUAL_LONGS(0, rectDst.right);
	NW_CHECK_EQUAL_LONGS(0, rectDst.top);
	NW_CHECK_EQUAL_LONGS(0, rectDst.bottom);
}

NW_TEST(NanoWinRECTTestGroup, IntersectRect_CommonPointTest)
{
	RECT rect1;
	RECT rect2;
	RECT rectDst;

	NW_TEST_MAKE_RECT(rect1, 1, 3, 2, 4);
	NW_TEST_MAKE_RECT(rect2, 2, 4, 6, 8);

	NW_CHECK_FALSE(IntersectRect(&rectDst, &rect1, &rect2));

	NW_CHECK_EQUAL_LONGS(0, rectDst.left);
	NW_CHECK_EQUAL_LONGS(0, rectDst.right);
	NW_CHECK_EQUAL_LONGS(0, rectDst.top);
	NW_CHECK_EQUAL_LONGS(0, rectDst.bottom);
}

NW_TEST(NanoWinRECTTestGroup, IntersectRect_CommonLineTest)
{
	RECT rect1;
	RECT rect2;
	RECT rectDst;

	NW_TEST_MAKE_RECT(rect1, 1, 3, 2, 5);
	NW_TEST_MAKE_RECT(rect2, 2, 4, 6, 8);

	NW_CHECK_FALSE(IntersectRect(&rectDst, &rect1, &rect2));

	NW_CHECK_EQUAL_LONGS(0, rectDst.left);
	NW_CHECK_EQUAL_LONGS(0, rectDst.right);
	NW_CHECK_EQUAL_LONGS(0, rectDst.top);
	NW_CHECK_EQUAL_LONGS(0, rectDst.bottom);
}

NW_TEST(NanoWinRECTTestGroup, IntersectRect_SomeIntersectTest)
{
	RECT rect1;
	RECT rect2;
	RECT rectDst;

	NW_TEST_MAKE_RECT(rect1, 1, 3, 3, 5);
	NW_TEST_MAKE_RECT(rect2, 2, 2, 6, 8);

	NW_CHECK(IntersectRect(&rectDst, &rect1, &rect2));

	NW_CHECK_EQUAL_LONGS(2, rectDst.left);
	NW_CHECK_EQUAL_LONGS(3, rectDst.right);
	NW_CHECK_EQUAL_LONGS(3, rectDst.top);
	NW_CHECK_EQUAL_LONGS(5, rectDst.bottom);
}

NW_TEST(NanoWinRECTTestGroup, IntersectRect_SomeIntersectTest2)
{
	RECT rect1;
	RECT rect2;
	RECT rectDst;

	NW_TEST_MAKE_RECT(rect1, 1, 3, 3, 5);
	NW_TEST_MAKE_RECT(rect2, 2, 2, 6, 8);

	NW_CHECK(IntersectRect(&rectDst, &rect1, &rect2));

	NW_CHECK_EQUAL_LONGS(2, rectDst.left);
	NW_CHECK_EQUAL_LONGS(3, rectDst.top);
	NW_CHECK_EQUAL_LONGS(3, rectDst.right);
	NW_CHECK_EQUAL_LONGS(5, rectDst.bottom);
}

NW_TEST(NanoWinRECTTestGroup, IntersectRect_RectInRectTest)
{
	RECT rect1;
	RECT rect2;
	RECT rectDst;

	NW_TEST_MAKE_RECT(rect1, 1, 1, 5, 5);
	NW_TEST_MAKE_RECT(rect2, 2, 3, 3, 4);

	NW_CHECK(IntersectRect(&rectDst, &rect1, &rect2));

	NW_CHECK_EQUAL_LONGS(2, rectDst.left);
	NW_CHECK_EQUAL_LONGS(3, rectDst.right);
	NW_CHECK_EQUAL_LONGS(3, rectDst.top);
	NW_CHECK_EQUAL_LONGS(4, rectDst.bottom);

	NW_CHECK(IntersectRect(&rectDst, &rect2, &rect1));

	NW_CHECK_EQUAL_LONGS(2, rectDst.left);
	NW_CHECK_EQUAL_LONGS(3, rectDst.right);
	NW_CHECK_EQUAL_LONGS(3, rectDst.top);
	NW_CHECK_EQUAL_LONGS(4, rectDst.bottom);
}

NW_TEST(NanoWinRECTTestGroup, IntersectRect_EmptyRectInRectTest)
{
	RECT rect1;
	RECT rect2;
	RECT rectDst;

	NW_TEST_MAKE_RECT(rect1, 1, 1, 5, 5);
	NW_TEST_MAKE_RECT(rect2, 2, 2, 2, 2);

	NW_CHECK_FALSE(IntersectRect(&rectDst, &rect1, &rect2));

	NW_CHECK_EQUAL_LONGS(0, rectDst.left);
	NW_CHECK_EQUAL_LONGS(0, rectDst.right);
	NW_CHECK_EQUAL_LONGS(0, rectDst.top);
	NW_CHECK_EQUAL_LONGS(0, rectDst.bottom);
}

NW_END_TEST_GROUP()
