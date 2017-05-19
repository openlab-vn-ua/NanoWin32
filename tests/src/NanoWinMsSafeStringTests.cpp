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

#define NW_CHECK_EQUAL_TYPES(a,b,t)    NW_CHECK_EQUAL((t)a,(t)b)
#define NW_CHECK_EQUAL_BYTES(a,b)      NW_CHECK_EQUAL_TYPES(a,b,uint8_t)

#define NW_CHECK_RC_OK(rc)        NW_CHECK_EQUAL_INTS(EOK,rc)
#define NW_CHECK_RC_ERR(rc)       NW_CHECK_FALSE(rc == EOK)
#define NW_CHECK_STR_EMPTY(s)     NW_CHECK_TRUE((s)[0] == '\0')
#define NW_CHECK_STR_NOT_EMPTY(s) NW_CHECK_TRUE((s)[0] != '\0')

NW_BEGIN_TEST_GROUP_SIMPLE(NanoWinMsSafeStringTestGroup)

NW_TEST(NanoWinMsSafeStringTestGroup, StrNCpySTest)
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

 #if 0 // TODO: check correct behavior
 strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
 str2[0] = '\0';

 rc = strncpy_s(str1, 5, str2, 0);

 NW_CHECK_RC_ERR(rc);
 NW_CHECK_STR_EMPTY(str1);
 #endif

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

 #if 0 // TODO: check correct behavior
 strcpy(dest, "                            ");
 strcpy(str2, "goodbye");

 rc = strncpy_s(dest, 5, str2, 4);

 NW_CHECK_RC_OK(rc);

 /* be sure the results are the same as strcmp */

 ind = strcmp(dest, "good");

 NW_CHECK_EQUAL(0, ind);
 #endif

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

NW_TEST(NanoWinMsSafeStringTestGroup, MemCpySTest)
{
#define LEN   ( 1024 )

	uint8_t  mem1[LEN + 2];
	uint8_t  mem2[LEN + 2];


	errno_t rc;
	uint32_t i;
	rsize_t len;

	/*--------------------------------------------------*/

	rc = memcpy_s(NULL, LEN, mem2, LEN);
	NW_CHECK_RC_ERR(rc);


	/*--------------------------------------------------*/

	rc = memcpy_s(mem1, 0, mem2, LEN);
	NW_CHECK_RC_ERR(rc);


	/*--------------------------------------------------*/

	rc = memcpy_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN);
	NW_CHECK_RC_ERR(rc);


	/*--------------------------------------------------*/

	rc = memcpy_s(mem1, LEN, NULL, LEN);
	NW_CHECK_RC_ERR(rc);

	/*--------------------------------------------------*/

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//        it is not clear why original test expects error

	rc = memcpy_s(mem1, 10, mem2, 0);
	NW_CHECK_RC_ERR(rc);
	#endif

	/*--------------------------------------------------*/

	rc = memcpy_s(mem1, LEN, mem2, RSIZE_MAX_MEM + 1);
	NW_CHECK_RC_ERR(rc);

	/*--------------------------------------------------*/

	for (i = 0; i < LEN + 1; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memcpy_s(mem1, len, mem2, len);
	
	NW_CHECK_RC_OK(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(mem1[i], mem2[i]);
	}

	NW_CHECK_EQUAL_BYTES(33, mem1[i]);

	/*--------------------------------------------------*/

	for (i = 0; i < LEN + 1; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memcpy_s(mem1, len, mem2, (len + 1));
	
	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	NW_CHECK_EQUAL_BYTES(33, mem1[i]);

	/*--------------------------------------------------*/

	for (i = 0; i < LEN + 2; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN / 2;
	rc = memcpy_s(mem1, len, mem2, LEN);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	NW_CHECK_EQUAL_BYTES(33, mem1[len]);

	/*--------------------------------------------------*/
	
	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//        it is not clear why original test expects error

	for (i = 0; i < LEN + 2; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memcpy_s(mem1, len, mem2, 0);
	
	NW_CHECK_RC_ERR(rc);

	for (i=0; i<len; i++) 
	{
	    NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memcpy_s(mem1, len, mem2, RSIZE_MAX_MEM + 1);
	
	NW_CHECK_RC_ERR(rc);
	
	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	
	NW_CHECK_FALSE(mem1[len] == 0);
	
	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 55; }

	/* same ptr - no move */
	rc = memcpy_s(mem1, LEN, mem1, LEN);
	NW_CHECK_RC_OK(rc);

	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 55; }
	for (i = 0; i < LEN; i++) { mem2[i] = 65; }

	/* overlap */
	len = 100;
	rc = memcpy_s(&mem1[0], len, &mem1[10], len);

	NW_CHECK_RC_ERR(rc);
	NW_CHECK_FALSE(mem1[len] == 0);
	
	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 55; }
	for (i = 0; i < LEN; i++) { mem2[i] = 65; }

	/* overlap */
	len = 100;
	rc = memcpy_s(&mem1[10], len, &mem1[0], len);

	NW_CHECK_RC_ERR(rc);

	for (i = 10; i < len + 10; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 35; }
	for (i = 0; i < LEN; i++) { mem2[i] = 55; }

	len = 5;
	rc = memcpy_s(mem1, len, mem2, len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(55, mem1[i]);
	}

	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 35; }
	for (i = 0; i < LEN; i++) { mem2[i] = 55; }

	rc = memcpy_s(mem1, LEN, mem2, LEN / 2);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i<LEN / 2; i++) 
	{
		NW_CHECK_EQUAL_BYTES(55, mem1[i]);
	}

	/*--------------------------------------------------*/


#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, MemMoveSTest)
{
#define LEN   ( 1024 )

	uint8_t  mem1[LEN];
	uint8_t  mem2[LEN];


	errno_t rc;
	uint32_t i;
	uint32_t len;

	/*--------------------------------------------------*/

	rc = memmove_s(NULL, LEN, mem2, LEN);
	NW_CHECK_RC_ERR(rc);


	/*--------------------------------------------------*/

	rc = memmove_s(mem1, 0, mem2, LEN);
	NW_CHECK_RC_ERR(rc);             


			/*--------------------------------------------------*/

	rc = memmove_s(mem1, RSIZE_MAX_MEM + 1, mem2, LEN);
	NW_CHECK_RC_ERR(rc);


	/*--------------------------------------------------*/

	rc = memmove_s(mem1, LEN, NULL, LEN);
	NW_CHECK_RC_ERR(rc);

	/*--------------------------------------------------*/
	
	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//  it is not clear why original test expects error

	rc = memmove_s(mem1, 10, mem2, 0);
	NW_CHECK_RC_ERR(rc);           
	#endif

	/*--------------------------------------------------*/

	rc = memmove_s(mem1, LEN, mem2, RSIZE_MAX_MEM + 1);
	NW_CHECK_RC_ERR(rc);


	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	/* a valid move */
	len = LEN;
	rc = memmove_s(mem1, len, mem2, LEN);

	NW_CHECK_RC_OK(rc);
	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(mem2[i], mem1[i]);
	}

	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	/* length error */
	len = LEN / 2;
	rc = memmove_s(mem1, len, mem2, LEN);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	/*--------------------------------------------------*/

	#if 0 // TODO: check correct behavior [verify TEST expectation]
	//  NOTE: documentation says that if count == 0, result must be EOK
	//        it is not clear why original test expects error

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	len = LEN;
	rc = memmove_s(mem1, len, mem2, 0);

	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}
	#endif

	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	/* invalid length - zero dest */
	len = LEN;
	rc = memmove_s(mem1, len, mem2, RSIZE_MAX_MEM + 1);


	NW_CHECK_RC_ERR(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(0, mem1[i]);
	}

	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 33; }
	for (i = 0; i < LEN; i++) { mem2[i] = 44; }

	/* same ptr - no move */
	rc = memmove_s(mem1, LEN, mem1, LEN);

	NW_CHECK_RC_OK(rc);


	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 25; }
	for (i = 10; i < LEN - 10; i++) { mem1[i] = 35; }

	/* overlap move */
	len = 20;
	rc = memmove_s(&mem1[0], len, &mem1[10], len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(35, mem1[i]);
	}

	/*--------------------------------------------------*/

	for (i = 0; i < LEN; i++) { mem1[i] = 25; }
	for (i = 10; i < LEN - 10; i++) { mem1[i] = 35; }

	/* overlap move */
	len = 20;
	rc = memmove_s(&mem1[10], len, &mem1[0], len);

	NW_CHECK_RC_OK(rc);

	for (i = 0; i < LEN; i++) { mem2[i] = 25; }
	for (i = 10; i < LEN - 10; i++) { mem2[i] = 35; }


	for (i = 0; i < len; i++)
	{
		NW_CHECK_EQUAL_BYTES(25, mem1[i]);
	}

	/*--------------------------------------------------*/
	/*--------------------------------------------------*/


#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, StrCatSTest)
{
#define LEN   ( 128 )

	char   str1[LEN];
	char   str2[LEN];

	errno_t rc;
	int32_t  ind;
	int32_t  len1;
	int32_t  len2;
	int32_t  len3;

	/*--------------------------------------------------*/

	rc = strcat_s(NULL, LEN, str2);
	
	NW_CHECK_RC_ERR(rc);
	
	/*--------------------------------------------------*/

	rc = strcat_s(str1, LEN, NULL);
	
	NW_CHECK_RC_ERR(rc);
	
	/*--------------------------------------------------*/

	rc = strcat_s(str1, 0, str2);
	
	NW_CHECK_RC_ERR(rc);

	/*--------------------------------------------------*/

	rc = strcat_s(str1, (RSIZE_MAX_STR + 1), str2);
	
	NW_CHECK_RC_ERR(rc);

	/*--------------------------------------------------*/
	/*--------------------------------------------------*/

	strcpy(str1, "aaaaaaaaaa");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 1, str2);
	
	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');
	
	/*--------------------------------------------------*/

	strcpy(str1, "aaaaaaaaaa");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 2, str2);
	
	NW_CHECK_RC_ERR(rc);
	
	NW_CHECK(str1[0] == '\0');

	/*--------------------------------------------------*/

	strcpy(&str1[0], "aaaaaaaaaa");
	strcpy(&str2[0], "keep it simple");

	len1 = strlen(str1);
	len2 = strlen(str2);

	rc = strcat_s(str1, 50, str2);
	
	NW_CHECK_RC_OK(rc);
	
	len3 = strlen(str1);
	
	NW_CHECK_EQUAL(len1 + len2, len3);
	
	/*--------------------------------------------------*/
	/*--------------------------------------------------*/

	str1[0] = '\0';
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 1, str2);
	
	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	/*--------------------------------------------------*/

	str1[0] = '\0';
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 2, str2);
	
	NW_CHECK_RC_ERR(rc);
	
	NW_CHECK(str1[0] == '\0');
	
	/*--------------------------------------------------*/

	str1[0] = '\0';
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 20, str2);
	
	NW_CHECK_RC_OK(rc);

	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);

	/*--------------------------------------------------*/
	/*--------------------------------------------------*/

	str1[0] = '\0';
	str2[0] = '\0';

	rc = strcat_s(str1, LEN, str2);
	
	NW_CHECK_RC_OK(rc);

	NW_CHECK(str1[0] == '\0');

	/*--------------------------------------------------*/

	str1[0] = '\0';
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, LEN, str2);
	
	NW_CHECK_RC_OK(rc);

	ind = strcmp(str1, str2);
	
	NW_CHECK_EQUAL(0, ind);

	/*--------------------------------------------------*/

	strcpy(str1, "qqweqq");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, LEN, str2);
	
	NW_CHECK_RC_OK(rc);

	ind = strcmp(str1, "qqweqqkeep it simple");
	
	NW_CHECK_EQUAL(0, ind);

	/*--------------------------------------------------*/

	strcpy(str1, "1234");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 12, str2);
	
	NW_CHECK_RC_ERR(rc);

	/*--------------------------------------------------*/

	strcpy(str1, "1234");
	strcpy(str2, "keep it simple");

	rc = strcat_s(str1, 52, str2);
	
	NW_CHECK_RC_OK(rc);

	ind = strcmp(str1, "1234keep it simple");
	
	NW_CHECK_EQUAL(0, ind);

	/*--------------------------------------------------*/

	strcpy(str1, "12345678901234567890");

	rc = strcat_s(str1, 8, &str1[7]);
	
	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	/*--------------------------------------------------*/

	strcpy(str1, "123456789");

	rc = strcat_s(str1, 9, &str1[8]);
	
	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');
	
	/*--------------------------------------------------*/

	strcpy(str2, "123");
	strcpy(str1, "keep it simple");

	rc = strcat_s(str2, 31, &str1[0]);
	
	NW_CHECK_RC_OK(rc);

	ind = strcmp(str2, "123keep it simple");
	
	NW_CHECK_EQUAL(0, ind);

	/*--------------------------------------------------*/

	strcpy(str2, "1234");
	strcpy(str1, "56789");

	rc = strcat_s(str2, 10, str1);
	
	NW_CHECK_RC_OK(rc);

	ind = strcmp(str2, "123456789");
	
	NW_CHECK_EQUAL(0, ind);


#undef LEN
}

NW_TEST(NanoWinMsSafeStringTestGroup, StrCpySTest)
{
#define MAX   ( 128 )
#define LEN   ( 128 )

	char   str1[LEN];
	char   str2[LEN];

	errno_t rc;
	uint32_t i;
	int32_t  ind;

	/*--------------------------------------------------*/

	rc = strcpy_s(NULL, LEN, str2);
	
	NW_CHECK_RC_ERR(rc);

	/*--------------------------------------------------*/

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	rc = strcpy_s(str1, 5, NULL);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');
	
	/*--------------------------------------------------*/

	rc = strcpy_s(str1, 0, str2);

	NW_CHECK_RC_ERR(rc);

	/*--------------------------------------------------*/

	rc = strcpy_s(str1, (RSIZE_MAX_STR + 1), str2);

	NW_CHECK_RC_ERR(rc);

	/*--------------------------------------------------*/
	/*--------------------------------------------------*/

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	str2[0] = '\0';

	rc = strcpy_s(str1, LEN / 2, str2);
	
	NW_CHECK_RC_OK(rc);

	NW_CHECK(str1[0] == '\0');

	/*--------------------------------------------------*/
	
	#if 0  // TODO: check correct behavior [verify TEST expectation]
	//  test is uncorrect, standard says that result of this operation will be error
	//  to check if src==dest should this be considered as "overlap" ?
	//  memcpy does not assume that (src==dest is ok for memecpy)
	//  should we just check that length assumption is match and go on?

	strcpy(str1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	rc = strcpy_s(str1, LEN, str1);
	
	NW_CHECK_RC_OK(rc);
	#endif

	/*--------------------------------------------------*/

	strcpy(&str1[0], "keep it simple");

	rc = strcpy_s(&str1[0], LEN, &str1[5]);

	NW_CHECK_RC_ERR(rc);

	NW_CHECK(str1[0] == '\0');

	/*--------------------------------------------------*/

	strcpy(&str1[0], "keep it simple");
	str2[0] = '\0';

	rc = strcpy_s(str1, LEN, str2);
	
	NW_CHECK_RC_OK(rc);

	NW_CHECK(*str1 == '\0');
	
	/*--------------------------------------------------*/

	str1[0] = '\0';
	strcpy(&str2[0], "keep it simple");

	rc = strcpy_s(str1, LEN, str2);
	
	NW_CHECK_RC_OK(rc);

	/* be sure the results are the same as strcmp */
	ind = strcmp(str1, str2);

	NW_CHECK_EQUAL(0, ind);
	
	/*--------------------------------------------------*/

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "keep it simple");

	rc = strcpy_s(str1, LEN, str2);
	
	NW_CHECK_RC_OK(rc);

	/* be sure the results are the same as strcmp */
	ind = strcmp(str1, str2);
	
	NW_CHECK_EQUAL(0, ind);

	/*--------------------------------------------------*/

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "keep it simple");

	rc = strcpy_s(str1, 1, str2);
	
	NW_CHECK_RC_ERR(rc);

	NW_CHECK(*str1 == '\0');

	/*--------------------------------------------------*/

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "keep it simple");

	rc = strcpy_s(str1, 2, str2);
	
	NW_CHECK_RC_ERR(rc);

	NW_CHECK(*str1 == '\0');

	/*--------------------------------------------------*/

	strcpy(str1, "qqweqeqeqeq");
	strcpy(str2, "it");

	rc = strcpy_s(str1, 3, str2);
	
	NW_CHECK_RC_OK(rc);

	/* be sure the results are the same as strcmp */
	ind = strcmp(str1, str2);
	
	NW_CHECK_EQUAL(0, ind);

	/*--------------------------------------------------*/

	strcpy(str1, "qq12345weqeqeqeq");
	strcpy(str2, "it");

	rc = strcpy_s(str1, 10, str2);
	
	NW_CHECK_RC_OK(rc);

	/* be sure the results are the same as strcpy */
	ind = strcmp(str1, str2);
	
	NW_CHECK_EQUAL(0, ind);

#undef MAX
#undef LEN
}


NW_END_TEST_GROUP()
