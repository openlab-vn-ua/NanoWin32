/* Unit tests inspired by Safe C library (https://sourceforge.net/projects/safeclib/) */

#include "NWUnitTest.h"

#ifdef __linux
 #include "errno.h"
 #include "NanoWinMsSafeString.h"
#else
 #include <windows.h>
#endif

#if !defined(EOK)
 #define EOK (0)
#endif

#define NW_CHECK_RC_OK(rc)        NW_CHECK_EQUAL_INTS(EOK,rc)
#define NW_CHECK_RC_ERR(rc)       NW_CHECK_FALSE(rc == EOK)
#define NW_CHECK_STR_EMPTY(s)     NW_CHECK_TRUE((s)[0] == '\0')
#define NW_CHECK_STR_NOT_EMPTY(s) NW_CHECK_TRUE((s)[0] != '\0')

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMsSafeStringTestGroup)

NW_TEST(NanoWinMsSafeStringTestGroup, StrCpySTest)
{
 #define MAX   ( 128 )
 #define LEN   ( 128 )

 char   str1[LEN];
 char   str2[LEN];
 char   dest[LEN];

 errno_t rc;
 rsize_t nlen;
 int32_t ind;

 nlen = 5;
 rc = strncpy_s(NULL, LEN, str2, nlen);

 NW_CHECK_RC_ERR(rc);

 /*--------------------------------------------------*/

 strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

 nlen = 5;
 rc = strncpy_s(str1, 5, NULL, nlen);

 NW_CHECK_RC_ERR(rc);
 NW_CHECK_STR_EMPTY(str1);

 /*--------------------------------------------------*/

 nlen = 5;
 rc = strncpy_s(str1, 0, str2, nlen);

 NW_CHECK_RC_ERR(rc);

 /*--------------------------------------------------*/

 rc = strncpy_s(str1, (RSIZE_MAX_STR + 1), str2, nlen);

 NW_CHECK_RC_ERR(rc);

 /*--------------------------------------------------*/
 /*--------------------------------------------------*/

 /*TODO: check correct behavior
 strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
 str2[0] = '\0';

 rc = strncpy_s(str1, 5, str2, 0);

 NW_CHECK_RC_ERR(rc);
 NW_CHECK_STR_EMPTY(str1);
 */

 /*--------------------------------------------------*/

 strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
 strcpy(str2, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

 rc = strncpy_s(str1, 5, str2, (RSIZE_MAX_STR + 1));

 NW_CHECK_RC_ERR(rc);

 /*--------------------------------------------------*/

 strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
 str2[0] = '\0';
 nlen = 5;

 rc = strncpy_s(&str1[0], LEN / 2, &str2[0], nlen);

 NW_CHECK_RC_OK(rc);
 NW_CHECK_STR_EMPTY(str1);

 /*--------------------------------------------------*/

 strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
 nlen = 5;

 /* test overlap */
 rc = strncpy_s(str1, LEN, str1, nlen);

 NW_CHECK_RC_ERR(rc);
 NW_CHECK_STR_EMPTY(str1);

 /*--------------------------------------------------*/

 strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
 nlen = 18;

 rc = strncpy_s(&str1[0], LEN, &str1[5], nlen);

 NW_CHECK_RC_ERR(rc);
 NW_CHECK_STR_EMPTY(str1);

 /*--------------------------------------------------*/

 strcpy(str1, "keep it simple");
 str2[0] = '\0';

 nlen = 10;
 rc = strncpy_s(str1, LEN, str2, nlen);

 NW_CHECK_RC_OK(rc);
 NW_CHECK_STR_EMPTY(str1);

 /*--------------------------------------------------*/

 str1[0] = '\0';
 strcpy(str2, "keep it simple");

 nlen = 20;
 rc = strncpy_s(str1, LEN, str2, nlen);

 NW_CHECK_RC_OK(rc);

 /* be sure the results are the same as strcmp */
 ind = strcmp(str1, str2);
 NW_CHECK_EQUAL(0, ind);

 /*--------------------------------------------------*/

 strcpy(str1, "qqweqeqeqeq");
 strcpy(str2, "keep it simple");

 nlen = 32;
 rc = strncpy_s(str1, LEN, str2, nlen);

 NW_CHECK_RC_OK(rc);

 /* be sure the results are the same as strcmp */
 ind = strcmp(str1, str2);

 NW_CHECK_EQUAL(0, ind);

 /*--------------------------------------------------*/
 /*--------------------------------------------------*/

 strcpy(str1, "qqweqeqeqeq");
 strcpy(str2, "keep it simple");

 rc = strncpy_s(str1, 1, str2, nlen);

 NW_CHECK_RC_ERR(rc);
 NW_CHECK_STR_EMPTY(str1);

 /*--------------------------------------------------*/

 strcpy(str1, "qqweqeqeqeq");
 strcpy(str2, "keep it simple");

 rc = strncpy_s(str1, 2, str2, nlen);

 NW_CHECK_RC_ERR(rc);
 NW_CHECK_STR_EMPTY(str1);

 /*--------------------------------------------------*/
 /* TR example */

 strcpy(dest, "                            ");
 strcpy(str1, "hello");

 rc = strncpy_s(dest, 6, str1, 100);

 NW_CHECK_RC_OK(rc);

 /* be sure the results are the same as strcmp */
 ind = strcmp(dest, str1);

 NW_CHECK_EQUAL(0, ind);

 /*--------------------------------------------------*/
 /* TR example */

 strcpy(dest, "                            ");
 strcpy(str2, "goodbye");

 rc = strncpy_s(dest, 5, str2, 7);

 NW_CHECK_RC_ERR(rc);

 /*--------------------------------------------------*/
 /* TR example */

 /*TODO: check correct behavior
 strcpy(dest, "                            ");
 strcpy(str2, "goodbye");

 rc = strncpy_s(dest, 5, str2, 4);

 NW_CHECK_RC_OK(rc);
*/
 /* be sure the results are the same as strcmp */
 /*
 ind = strcmp(dest, "good");

 NW_CHECK_EQUAL(0, ind);
 */

 /*--------------------------------------------------*/

 strcpy(dest, "                            ");
 strcpy(str2, "good");

 /*   strnlen("good") < 5   */
 rc = strncpy_s(dest, 5, str2, 8);

 NW_CHECK_RC_OK(rc);

 /* be sure the results are the same as strcmp */
 ind = strcmp(dest, "good");

 NW_CHECK_EQUAL(0, ind);

 /*--------------------------------------------------*/

 strcpy(str1, "qq12345weqeqeqeq");
 strcpy(str2, "it");

 nlen = 10;
 rc = strncpy_s(str1, 10, str2, nlen);

 NW_CHECK_RC_OK(rc);

 /* be sure the results are the same as strcmp */
 ind = strcmp(str1, str2);

 NW_CHECK_EQUAL(0, ind);
 
 #undef MAX
 #undef LEN
}

NW_END_TEST_GROUP()
