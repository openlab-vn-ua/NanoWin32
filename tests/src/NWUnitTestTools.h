#ifndef __NWUNITTESTTOOLS_H__
#define __NWUNITTESTTOOLS_H__

// NanoWin32 Unit tests
// -----------------------------------------------------------------------
// Test tools

// Some usefull defines
// ---------------------------------------------

#if !defined(EOK)
 #define EOK (0)
#endif

#if !defined(RSIZE_MAX_STR)
#define RSIZE_MAX_STR RSIZE_MAX
#endif

#if !defined(RSIZE_MAX_MEM)
#define RSIZE_MAX_MEM RSIZE_MAX
#endif

// Extra test funcs
// ---------------------------------------------

#define NW_FILL_VAR_WITH_5A(arr)  memset(arr,0x5A,sizeof(arr)) // fill with 'Z'

// Extra test checks
// ---------------------------------------------

#define NW_CHECK_RC_OK(rc)        NW_CHECK_EQUAL_INTS(EOK,rc)
#define NW_CHECK_RC_ERR(rc)       NW_CHECK_TRUE(rc != EOK)
#define NW_CHECK_STR_EMPTY(s)     NW_CHECK_TRUE((s)[0] == '\0')
#define NW_CHECK_STR_NOT_EMPTY(s) NW_CHECK_TRUE((s)[0] != '\0')

#define NW_RESET_ERRNO()          { errno = 0; }
#define NW_CHECK_ERRNO_OK()       NW_CHECK_RC_OK(errno);  NW_RESET_ERRNO();
#define NW_CHECK_ERRNO_ERR()      NW_CHECK_RC_ERR(errno); NW_RESET_ERRNO();

// Error handler setup
// ---------------------------------------------

#if defined(_MSC_VER)

namespace NanoWin {

static unsigned int invalid_parameter_handler_call_count = 0;
static void invalid_parameter_handler_as_report_and_continue
			(
				const wchar_t * expression,  
				const wchar_t * function,   
				const wchar_t * file,   
				unsigned int line,  
				uintptr_t pReserved  
			)
{
	invalid_parameter_handler_call_count++;
	#if 0
	wprintf(L"Invalid parameter detected in function %s."
			L" File: %s Line: %d\n", function, file, line);
	wprintf(L"Expression: %s\n", expression);
	//abort();
	#endif
	return;
}

class  invalid_parameter_handler_manager_class
{
	protected:
	bool                       old_app_param_defined;
    _invalid_parameter_handler old_app_invalid_parameter_handler;
	int                        old_app_crtdbg_report_mode;

	public:
	invalid_parameter_handler_manager_class()
	{
		old_app_param_defined = false;
	}

	void set_fail_as_report_and_continue()
	{
		if (!old_app_param_defined)
		{
			// First call - save app-default handler
			old_app_crtdbg_report_mode = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_REPORT_MODE);
			old_app_invalid_parameter_handler = _set_invalid_parameter_handler(invalid_parameter_handler_as_report_and_continue);
		}
		else
		{
			// App handler already saved, just update our handler
			_set_invalid_parameter_handler(invalid_parameter_handler_as_report_and_continue);
		}

		_CrtSetReportMode(_CRT_ASSERT, 0);
		old_app_param_defined = true;
	}

	void set_fail_by_restore_default()
	{
	  if (old_app_param_defined)
	  {
		// app-wide old info available, restore it
		_set_invalid_parameter_handler(old_app_invalid_parameter_handler);
		_CrtSetReportMode(_CRT_ASSERT, old_app_crtdbg_report_mode);
	  }
	}

};

static invalid_parameter_handler_manager_class   invalid_parameter_handler_manager;
static unsigned int invalid_parameter_handler_call_count_last_seen = 0;

} // namespace NanoWin

#define NW_ERR_HANDLER_SET_FAIL_AS_REPORT_AND_CONTINUE()   NanoWin::invalid_parameter_handler_manager.set_fail_as_report_and_continue()
#define NW_ERR_HANDLER_SET_FAIL_AS_DEFAULT()		       NanoWin::invalid_parameter_handler_manager.set_fail_by_restore_default()

#define NW_ERR_HANDLER_IS_FIRED()        (NanoWin::invalid_parameter_handler_call_count_last_seen != NanoWin::invalid_parameter_handler_call_count)
#define NW_RESET_ERR_HANDLER_COUNT()     { NanoWin::invalid_parameter_handler_call_count_last_seen = NanoWin::invalid_parameter_handler_call_count; }
#define NW_CHECK_ERR_HANDLER_FIRED()     NW_CHECK(NW_ERR_HANDLER_IS_FIRED());  NW_RESET_ERR_HANDLER_COUNT();
#define NW_CHECK_ERR_HANDLER_NOT_FIRED() NW_CHECK(!NW_ERR_HANDLER_IS_FIRED()); NW_RESET_ERR_HANDLER_COUNT();

#else

#define NW_ERR_HANDLER_SET_FAIL_AS_REPORT_AND_CONTINUE() // nothing yet
#define NW_ERR_HANDLER_SET_FAIL_AS_DEFAULT() // nothing yet

#define NW_ERR_HANDLER_IS_FIRED()        (false)
#define NW_RESET_ERR_HANDLER_COUNT()     // Nothing (yet)
#define NW_CHECK_ERR_HANDLER_FIRED()     // Nothing (yet)
#define NW_CHECK_ERR_HANDLER_NOT_FIRED() // Nothing (yet)

#endif

class NW_ERR_HANDLER_SET_FAIL_FOR_TEST_AUTO_CLASS
{
	public:
	NW_ERR_HANDLER_SET_FAIL_FOR_TEST_AUTO_CLASS()  { NW_ERR_HANDLER_SET_FAIL_AS_REPORT_AND_CONTINUE(); }
	~NW_ERR_HANDLER_SET_FAIL_FOR_TEST_AUTO_CLASS() { NW_ERR_HANDLER_SET_FAIL_AS_DEFAULT(); }
};

#define NW_SETUP_ERR_HANDLER_FOR_TEST() NW_ERR_HANDLER_SET_FAIL_FOR_TEST_AUTO_CLASS __NW__set_fail_auto_init_done_object; NW_RESET_ERR_HANDLER_COUNT();

#endif
