
#include "S32K148.h" /* include peripheral declarations S32K148 */


int main()
{
	for(;;){

	}

	/* to avoid the warning message for GHS: statement is unreachable*/
#if defined (__ghs__)
#pragma ghs nowarning 111
#endif
#if defined (__ICCARM__)
#pragma diag_suppress=Pe111
#endif        
	return 0;
}
