#ifndef _CM_DHCPSNP_API_C_
#define _CM_DHCPSNP_API_C_

#include "l2dsinc.h"
#include "l2dsextn.h"
#include "cmdsapi.h"

/*****************************************************************************
* Function Name      : CmAPIDspStatusGet
*
* Description           : Get dhcp snooping global status
*
* Input(s)               : None
* 
* Output(s)             : pi4DspStatus----dhcp snooping status
*
* Return Value(s)     : ISS_SUCCESS or ISS_FAILURE
*****************************************************************************/
INT4 CmAPIDspStatusGet (INT4 *pi4DspStatus)
{
	*pi4DspStatus = (INT1) L2DS_ADMIN_STATUS (L2DS_CURR_CXT_ID);
    
    return ISS_SUCCESS;
}

/*****************************************************************************
* Function Name      : CmAPIDhcSnpPfRuleUpdateWithLock
* Description        : The routine is used for update dhcp snoop pf rules when attach new unit
*					   
* Input(s)           : none
* Output(s)          : none
* Return Value(s)    : ISS_SUCCESS or ISS_FAILURE
*****************************************************************************/
INT4 CmAPIDhcSnpPfRuleUpdateWithLock(VOID)
{
	L2DS_LOCK();

	if(L2DS_ADMIN_STATUS(L2DS_CURR_CXT_ID) != L2DS_ENABLED)
	{
		/*when dsp disable, modify ipv6 rule copy to cpu*/
		CmDspIpv6RuleActionModify(ISS_SWITCH_COPYTOCPU);
	}
	else
	{
		/* when dsp enable, modify ipv6 rule trap to cpu*/
		CmDspIpv6RuleActionModify(ISS_DROP_COPYTOCPU);
	}
    
	L2DS_UNLOCK();
	return ISS_SUCCESS;
}

#endif

