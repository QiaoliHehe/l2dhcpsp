#ifndef _CM_DHCPSNP_RULE_C_
#define _CM_DHCPSNP_RULE_C_

#include "l2dsinc.h"
#include "l2dsextn.h"

/*****************************************************************************
* Function Name      : CmDspIpv6RuleActionModify
*
* Description           : The routine to be used for ipv6 rule action modify
*
* Input(s)               : i4Action
* 
* Output(s)             : None
*
* Return Value(s)     : L2DS_SUCCESS/L2DS_FAILURE
*****************************************************************************/
INT4 CmDspIpv6RuleActionModify(INT4 i4Action)
{
	tCtrlPktFilterData	CtrlPktFilterInfo;
		
	MEMSET(&CtrlPktFilterInfo, 0, sizeof(tCtrlPktFilterData));

	CmPfApiCtrlPktRuleDataInit(&CtrlPktFilterInfo);
	CtrlPktFilterInfo.u1RuleType = CMPF_IPV6_RULE_TYPE;
	CtrlPktFilterInfo.u2RuleIndex = CMPF_IPV6_TRAP_RULE_IDX;
	CtrlPktFilterInfo.Action.IngressForwardAction = i4Action;

	/*update ipv6 rule action*/
	if (CmPfApiCtrlPktRuleActionModify(&CtrlPktFilterInfo) == CMPF2_FAILURE)
	{
		return L2DS_FAILURE;
	}
	
	return L2DS_SUCCESS;
}


#endif

