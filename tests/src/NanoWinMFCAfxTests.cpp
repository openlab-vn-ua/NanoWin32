#include "NWUnitTest.h"

#ifdef __linux
 #include "NanoWinMFCAfxFile.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <unistd.h>
#else
 #include <afx.h>
#endif

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMFCAfxCRectTestGroup)

NW_TEST(NanoWinMFCAfxCRectTestGroup, SetRectTest)
{
	CRect rect;

	rect.SetRect(1, 2, 3, 4);

	NW_CHECK_EQUAL_INTS(1, rect.left);
	NW_CHECK_EQUAL_INTS(2, rect.top);
	NW_CHECK_EQUAL_INTS(3, rect.right);
	NW_CHECK_EQUAL_INTS(4, rect.bottom);
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, IsRectNullTest)
{
	CRect rect;

	NW_CHECK_TRUE(rect.IsRectNull());

	rect.SetRect(1, 2, 3, 4);

	NW_CHECK_FALSE(rect.IsRectNull());
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, IsRectEmptyTest)
{
	CRect rect;

	NW_CHECK_TRUE(rect.IsRectEmpty());

	rect.SetRect(1, 2, 3, 4);
	NW_CHECK_FALSE(rect.IsRectEmpty());

	rect.SetRect(1, 2, 1, 4);
	NW_CHECK_TRUE(rect.IsRectEmpty());
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, SetRectEmptyTest)
{
	CRect rect(1, 2, 3, 4);

	NW_CHECK_FALSE(rect.IsRectEmpty());

	rect.SetRectEmpty();
	NW_CHECK_TRUE(rect.IsRectEmpty());
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, WidthTest)
{
	CRect rect(1, 2, 3, 4);
	
	int width = rect.Width();
	
	NW_CHECK_EQUAL(2, width);

	rect.SetRect(1, 2, 0, 4);
	width = rect.Width();

	NW_CHECK_EQUAL(-1, width);
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, HeightTest)
{
	CRect rect(1, 2, 3, 4);

	int height = rect.Height();

	NW_CHECK_EQUAL(2, height);

	rect.SetRect(1, 2, 3, 0);
	height = rect.Height();

	NW_CHECK_EQUAL(-2, height);
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, SizeTest)
{
	CRect rect(1, 2, 3, 5);

	CSize size = rect.Size();

	NW_CHECK_EQUAL_INTS(2, size.cx);
	NW_CHECK_EQUAL_INTS(3, size.cy);
}
NW_TEST(NanoWinMFCAfxCRectTestGroup, PtInRectTest)
{
	CRect  rect(1, 2, 3, 5);

	CPoint point1(2, 3);
	NW_CHECK_TRUE(rect.PtInRect(point1));

	CPoint point2(2, 2);
	NW_CHECK_TRUE(rect.PtInRect(point2));

	CPoint point3(1, 2);
	NW_CHECK_TRUE(rect.PtInRect(point3));

	CPoint point4(3, 3);
	NW_CHECK_FALSE(rect.PtInRect(point4));

	CPoint point5(2, 5);
	NW_CHECK_FALSE(rect.PtInRect(point5));

	CPoint point6(3, 2);
	NW_CHECK_FALSE(rect.PtInRect(point6));

	CPoint point7(1, 5);
	NW_CHECK_FALSE(rect.PtInRect(point7));

	CPoint point8(10, 10);
	NW_CHECK_FALSE(rect.PtInRect(point8));
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, DeflateRectTest)
{
	CRect rect(1, 2, 6, 5);

	rect.DeflateRect(1, 1, -2, 3);

	NW_CHECK_EQUAL_INTS(2, rect.left);
	NW_CHECK_EQUAL_INTS(3, rect.top);
	NW_CHECK_EQUAL_INTS(8, rect.right);
	NW_CHECK_EQUAL_INTS(2, rect.bottom);

	rect.DeflateRect(2, -2);

	NW_CHECK_EQUAL_INTS(4, rect.left);
	NW_CHECK_EQUAL_INTS(1, rect.top);
	NW_CHECK_EQUAL_INTS(6, rect.right);
	NW_CHECK_EQUAL_INTS(4, rect.bottom);

	CSize size(2, 3);

	rect.DeflateRect(size);

	NW_CHECK_EQUAL_INTS(6, rect.left);
	NW_CHECK_EQUAL_INTS(4, rect.top);
	NW_CHECK_EQUAL_INTS(4, rect.right);
	NW_CHECK_EQUAL_INTS(1, rect.bottom);
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, InflateRectTest)
{
	CRect rect(1, 2, 6, 5);

	rect.InflateRect(-1, -1, 2, -3);

	NW_CHECK_EQUAL_INTS(2, rect.left);
	NW_CHECK_EQUAL_INTS(3, rect.top);
	NW_CHECK_EQUAL_INTS(8, rect.right);
	NW_CHECK_EQUAL_INTS(2, rect.bottom);

	rect.InflateRect(-2, 2);

	NW_CHECK_EQUAL_INTS(4, rect.left);
	NW_CHECK_EQUAL_INTS(1, rect.top);
	NW_CHECK_EQUAL_INTS(6, rect.right);
	NW_CHECK_EQUAL_INTS(4, rect.bottom);

	CSize size(2, 1);

	rect.InflateRect(size);

	NW_CHECK_EQUAL_INTS(2, rect.left);
	NW_CHECK_EQUAL_INTS(0, rect.top);
	NW_CHECK_EQUAL_INTS(8, rect.right);
	NW_CHECK_EQUAL_INTS(5, rect.bottom);
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, IntersectRectTest)
{
  CRect rect1(1, 2, 3, 4);
  CRect rect2(5, 6, 7, 8);

  bool res1 = rect1.IntersectRect(&rect1, &rect2);

  NW_CHECK_FALSE(res1);

  CRect rect3(1, 2, 3, 4);
  CRect rect4(2, 3, 7, 8);
  
  bool res2 = rect3.IntersectRect(&rect3, &rect4);

  NW_CHECK_TRUE(res2);

  CRect rect5(1, 2, 5, 6);
  CRect rect6(2, 3, 4, 5);

  bool res3 = rect5.IntersectRect(&rect5, &rect6);

  NW_CHECK_TRUE(res3);

  CRect rect7(1, 2, 3, 4);
  CRect rect8(1, 2, 3, 4);

  bool res4 = rect7.IntersectRect(&rect7, &rect8);

  NW_CHECK_TRUE(res4);
}

NW_TEST(NanoWinMFCAfxCRectTestGroup, OperatorAreEqualRectTest)
{
  CRect rect1(1, 2, 3, 4);
  CRect rect2(5, 6, 7, 8);

  NW_CHECK_FALSE(rect1 == rect2);

  CRect rect3(1, 2, 3, 4);
  CRect rect4(1, 2, 3, 4);

  NW_CHECK_TRUE(rect3 == rect4);

  CRect rect5(1, 2, 3, 4);
  CRect rect6(3, 4, 1, 2);

  NW_CHECK_TRUE(rect3 == rect4);
}

NW_END_TEST_GROUP()