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

inline bool nw_memisset(const void *memory, unsigned char val, size_t size)
{
	// return true if memeory block is filled by val
	// inspired by cool solution by mihaif at:
	// https://stackoverflow.com/questions/6938219/how-to-check-whether-all-bytes-in-a-memory-block-are-zero 
	if (size <= 0) { return(false); }
	unsigned const char *mm = (unsigned const char*)memory;
	return (*mm == val) && memcmp(mm, mm + 1, size - 1) == 0;
}

#define NW_RESET_VAR_FILL_5A(arr) memset(arr,0x5A,sizeof(arr)) // fill with 'Z'
#define NW_CHECK_VAR_FILL_5A(arr) NW_CHECK_TRUE(nw_memisset(arr,0x5A,sizeof(arr))) // still filled by with 'Z'

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

// TODO: Some static objects in .h file, Move me to .cpp file later [not critical, since this module included in tests only]

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

class NW_ERR_HANDLER_SET_FAIL_FOR_TEST_AUTO_CLASS
{
	public:
	NW_ERR_HANDLER_SET_FAIL_FOR_TEST_AUTO_CLASS()  { NW_ERR_HANDLER_SET_FAIL_AS_REPORT_AND_CONTINUE(); }
	~NW_ERR_HANDLER_SET_FAIL_FOR_TEST_AUTO_CLASS() { NW_ERR_HANDLER_SET_FAIL_AS_DEFAULT(); }
};

#define NW_SETUP_ERR_HANDLER_FOR_TEST() NW_ERR_HANDLER_SET_FAIL_FOR_TEST_AUTO_CLASS __NW__set_fail_auto_init_done_object; NW_RESET_ERR_HANDLER_COUNT();

#elif 1 // NanoWin error handler

namespace NanoWin {

// TODO: Some static objects in .h file, Move me to .cpp file later [not critical, since this module included in tests only]

static struct
{
	unsigned int call_count;
	unsigned int call_count_last_seen;
} invalid_parameter_handler_stat = { 0, 0 };

static void invalid_parameter_handler_as_report_and_continue (const char *msg, void *ptr, errno_t error)
{
	invalid_parameter_handler_stat.call_count++;
	#if 1
	// //printf("Safety handler invoked with (msg=%s, ptr=%p, error=%d) [%p,total %d]\n", msg, ptr, (int)error, &invalid_parameter_handler_stat, (int)invalid_parameter_handler_stat.call_count);
	// printf("Safety handler invoked with (msg=%s, ptr=%p, error=%d)\n", msg, ptr, (int)error);
	//abort();
	#endif
	return;
}

static void invalid_parameter_handler_set_fail_as_report_and_continue(void)
{
	set_constraint_handler_s(NanoWin::invalid_parameter_handler_as_report_and_continue);
}

static void invalid_parameter_handler_set_fail_by_restore_default(void)
{
	set_constraint_handler_s(NULL);
}

static void invalid_parameter_handler_reset_counter(void)
{
	invalid_parameter_handler_stat.call_count_last_seen = invalid_parameter_handler_stat.call_count;
}

static bool invalid_parameter_handler_was_fired(void)
{
	//printf("ASK invalid_parameter_handler_was_fired [%p,last:%d,total:%d]", &invalid_parameter_handler_stat, invalid_parameter_handler_stat.call_count_last_seen, invalid_parameter_handler_stat.call_count);
	return(invalid_parameter_handler_stat.call_count_last_seen != invalid_parameter_handler_stat.call_count);
}

class InitDoneCaller
{
	public:
	typedef void (*func_t)(void);

	protected:

	func_t done;

	public:
	InitDoneCaller(func_t initFunc, func_t doneFunc) { initFunc(); this->done = doneFunc; }
	~InitDoneCaller() { (this->done)(); }
};

} // namespace NanoWin

#define NW_ERR_HANDLER_SET_FAIL_AS_REPORT_AND_CONTINUE()   invalid_parameter_handler_set_fail_as_report_and_continue()
#define NW_ERR_HANDLER_SET_FAIL_AS_DEFAULT()		       invalid_parameter_handler_set_fail_by_restore_default()

#define NW_ERR_HANDLER_IS_FIRED()        NanoWin::invalid_parameter_handler_was_fired()
#define NW_RESET_ERR_HANDLER_COUNT()     { NanoWin::invalid_parameter_handler_reset_counter(); } // { printf("%s:%d:NW_RESET_ERR_HANDLER_COUNT()\n", __FILE__, __LINE__); }
#define NW_CHECK_ERR_HANDLER_FIRED()     NW_CHECK(NW_ERR_HANDLER_IS_FIRED());  NW_RESET_ERR_HANDLER_COUNT();
#define NW_CHECK_ERR_HANDLER_NOT_FIRED() NW_CHECK(!NW_ERR_HANDLER_IS_FIRED()); NW_RESET_ERR_HANDLER_COUNT();

#define NW_SETUP_ERR_HANDLER_FOR_TEST()  NanoWin::InitDoneCaller __NW__set_fail_auto_init_done_object(NanoWin::invalid_parameter_handler_set_fail_as_report_and_continue, NanoWin::invalid_parameter_handler_set_fail_by_restore_default); NW_RESET_ERR_HANDLER_COUNT();

#else // Stub just in case error handler is not tested

#define NW_ERR_HANDLER_SET_FAIL_AS_REPORT_AND_CONTINUE() // nothing yet
#define NW_ERR_HANDLER_SET_FAIL_AS_DEFAULT() // nothing yet

#define NW_ERR_HANDLER_IS_FIRED()        (false)
#define NW_RESET_ERR_HANDLER_COUNT()     // Nothing (yet)
#define NW_CHECK_ERR_HANDLER_FIRED()     // Nothing (yet)
#define NW_CHECK_ERR_HANDLER_NOT_FIRED() // Nothing (yet)

#define NW_SETUP_ERR_HANDLER_FOR_TEST()  // Nothing (yet)

#endif

#endif
