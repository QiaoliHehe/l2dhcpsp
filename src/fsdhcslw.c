/********************************************************************
* Copyright (C) 2006 Aricent Inc . All Rights Reserved
*
* $Id: fsdhcslw.c,v 1.3 2010/08/04 08:51:03 prabuc Exp $
*
* Description: Protocol Low Level Routines
*********************************************************************/
#include "fssnmp.h"
#include "l2dsinc.h"
#include "l2dsextn.h"

/* Low Level GET Routine for All Objects  */
/****************************************************************************
 Function    :  nmhGetFsDhcpSnpSnoopingAdminStatus 
 Input       :  The Indices

                The Object 
                retValFsDhcpSnpSnoopingAdminStatus
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpSnoopingAdminStatus (INT4 *pi4RetValFsDhcpSnpSnoopingAdminStatus)
{
    *pi4RetValFsDhcpSnpSnoopingAdminStatus =
        (INT1) L2DS_ADMIN_STATUS (L2DS_CURR_CXT_ID);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpMacVerifyStatus
 Input       :  The Indices

                The Object 
                retValFsDhcpSnpMacVerifyStatus
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpMacVerifyStatus (INT4 *pi4RetValFsDhcpSnpMacVerifyStatus)
{
    *pi4RetValFsDhcpSnpMacVerifyStatus =
        (INT1) L2DS_MAC_VERIFY_STATUS (L2DS_CURR_CXT_ID);

    return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhGetCmDhcpSnpInsertOption82Status
 Input       :  The Indices

                The Object 
                retValCmDhcpSnpInsertOption82Status
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhGetCmDhcpSnpInsertOption82Status(INT4 *pi4RetValCmDhcpSnpInsertOption82Status)
{
	*pi4RetValCmDhcpSnpInsertOption82Status =
        (INT4) L2DS_ADD_OP82_STATUS (L2DS_CURR_CXT_ID);

    return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhGetCmDhcpSnpFwdOption82Status
 Input       :  The Indices

                The Object 
                retValCmDhcpSnpFwdOption82Status
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhGetCmDhcpSnpFwdOption82Status(INT4 *pi4RetValCmDhcpSnpFwdOption82Status)
{
	*pi4RetValCmDhcpSnpFwdOption82Status = 
		(INT4)L2DS_FWD_OP82_STATUS(L2DS_CURR_CXT_ID);
	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhGetCmDhcpSnpBackupDBStatus
 Input       :  The Indices

                The Object 
                retValCmDhcpSnpBackupDBStatus
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhGetCmDhcpSnpBackupDBStatus(INT4 *pi4RetValCmDhcpSnpBackupDBStatus)
{
	*pi4RetValCmDhcpSnpBackupDBStatus = 
		(INT4)L2DS_BACKUP_DB_STATUS(L2DS_CURR_CXT_ID);
	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhGetCmDhcpSnpBackupDBInterval
 Input       :  The Indices

                The Object 
                retValCmDhcpSnpBackupDBInterval
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhGetCmDhcpSnpBackupDBInterval(INT4 *pi4RetValCmDhcpSnpBackupDBInterval)
{
	*pi4RetValCmDhcpSnpBackupDBInterval = 
		(INT4)L2DS_BACKUP_INTERVAL(L2DS_CURR_CXT_ID);
	return SNMP_SUCCESS;
}
/* Low Level SET Routine for All Objects  */
/****************************************************************************
 Function    :  nmhSetFsDhcpSnpSnoopingAdminStatus
 Input       :  The Indices

                The Object 
                setValFsDhcpSnpSnoopingAdminStatus
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhSetFsDhcpSnpSnoopingAdminStatus(INT4 i4SetValFsDhcpSnpSnoopingAdminStatus)
{
	UINT1	u1DhcpSnpState = L2DS_DISABLED;

	u1DhcpSnpState = (UINT1)i4SetValFsDhcpSnpSnoopingAdminStatus;
    if(L2DS_ADMIN_STATUS(L2DS_CURR_CXT_ID) == u1DhcpSnpState)
    {
        return SNMP_SUCCESS;
    }
	else
	{
		L2DS_ADMIN_STATUS(L2DS_CURR_CXT_ID) = u1DhcpSnpState;
	}

	/*Enabled change to Disabled, we need to do following changes*/
	if(L2DS_ADMIN_STATUS(L2DS_CURR_CXT_ID) != L2DS_ENABLED)
	{
		/*Set all ports to trusted ports*/
		CmAPICfaSetAllPortsState(CFA_PORT_STATE_TRUSTED);
		
		/*Delete all IPDB entries, include static and dynamic*/
		L2dsPortDeleteEntries(L2DS_CURR_CXT_ID, L2DS_ZERO);
		
		/*Delete all DHCP Snooping VLAN interfaces*/
		CmL2dsIntfDelAll();
		
		/*Disable DB backup*/
		nmhSetCmDhcpSnpBackupDBStatus(L2DS_DISABLED);

		/*when dsp disable, modify ipv6 rule copy to cpu*/
		CmDspIpv6RuleActionModify(ISS_SWITCH_COPYTOCPU);
	}
	else
	{
		/* when dsp enable, modify ipv6 rule trap to cpu*/
		CmDspIpv6RuleActionModify(ISS_DROP_COPYTOCPU);
	}

	return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhSetFsDhcpSnpMacVerifyStatus
 Input       :  The Indices

                The Object 
                setValFsDhcpSnpMacVerifyStatus
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhSetFsDhcpSnpMacVerifyStatus (INT4 i4SetValFsDhcpSnpMacVerifyStatus)
{
    /* Checking the Mac Verify Status with already exists Value   *
     * if it is same return Success                              */
    if (L2DS_MAC_VERIFY_STATUS (L2DS_CURR_CXT_ID) ==
        (UINT1) i4SetValFsDhcpSnpMacVerifyStatus)
    {
        return SNMP_SUCCESS;
    }

    L2DS_MAC_VERIFY_STATUS (L2DS_CURR_CXT_ID) =
        (UINT1) i4SetValFsDhcpSnpMacVerifyStatus;
    return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhSetCmDhcpSnpInsertOption82Status
 Input       :  The Indices

                The Object 
                setValCmDhcpSnpInsertOption82Status
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhSetCmDhcpSnpInsertOption82Status(INT4 i4SetValCmDhcpSnpInsertOption82Status)
{
    L2DS_ADD_OP82_STATUS(L2DS_CURR_CXT_ID) =
        (UINT1)i4SetValCmDhcpSnpInsertOption82Status;
	
    return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhSetCmDhcpSnpFwdOption82Status
 Input       :  The Indices

                The Object 
                setValCmDhcpSnpFwdOption82Status
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhSetCmDhcpSnpFwdOption82Status(INT4 i4SetValCmDhcpSnpFwdOption82Status)
{
    L2DS_FWD_OP82_STATUS(L2DS_CURR_CXT_ID) =
        (UINT1)i4SetValCmDhcpSnpFwdOption82Status;
	
	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhSetCmDhcpSnpBackupDBStatus
 Input       :  The Indices

                The Object 
                setValCmDhcpSnpBackupDBStatus
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhSetCmDhcpSnpBackupDBStatus(INT4 i4SetValCmDhcpSnpBackupDBStatus)
{
	L2DS_BACKUP_DB_STATUS(L2DS_CURR_CXT_ID) = (UINT1)i4SetValCmDhcpSnpBackupDBStatus;

	/*Enabled change to Disabled, we need to do following changes*/
	if (L2DS_BACKUP_DB_STATUS(L2DS_CURR_CXT_ID) != L2DS_ENABLED)
	{
		/*Stop backup timer*/
		TmrStop(L2DS_TMR_LIST, &L2DS_TMR_DB_BLK);
		
		/*Delete backup file*/
		CmAPIIpdbDelete((UINT1 *)DCS_BACKUP_DB_FILE);
	}
	else
	{
		/*when backup enable, write ipdb binding entry into flash immediately, not wait for 
		    backup interval aging out. but in mib restore process when sys starts, will not write
		    database into flash*/
		TmrStart(L2DS_TMR_LIST, &L2DS_TMR_DB_BLK, L2DS_TMR_DB, 
				L2DS_BACKUP_INTERVAL(L2DS_DEFAULT_CXT_ID), 0);

		if (CmGetSysInitCompleteFlag() == OSIX_TRUE)
		{
			CmAPIIpdbWrite((UINT1 *)DCS_BACKUP_DB_FILE);
		}
	}

	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhSetCmDhcpSnpBackupDBInterval
 Input       :  The Indices

                The Object 
                setValCmDhcpSnpBackupDBInterval
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhSetCmDhcpSnpBackupDBInterval(INT4 i4SetValCmDhcpSnpBackupDBInterval)
{
    L2DS_BACKUP_INTERVAL(L2DS_CURR_CXT_ID) =
        (UINT4)i4SetValCmDhcpSnpBackupDBInterval;

	if(L2DS_ADMIN_STATUS(L2DS_DEFAULT_CXT_ID) == L2DS_ENABLED &&
		L2DS_BACKUP_DB_STATUS(L2DS_DEFAULT_CXT_ID) == L2DS_ENABLED)
	{
		TmrStop(L2DS_TMR_LIST, &L2DS_TMR_DB_BLK);
		TmrStart(L2DS_TMR_LIST, &L2DS_TMR_DB_BLK, L2DS_TMR_DB, 
			L2DS_BACKUP_INTERVAL(L2DS_DEFAULT_CXT_ID), 0);
	}

	return SNMP_SUCCESS;
}
/* Low Level TEST Routines for All Objects  */
/****************************************************************************
 Function    :  nmhTestv2FsDhcpSnpSnoopingAdminStatus
 Input       :  The Indices

                The Object 
                testValFsDhcpSnpSnoopingAdminStatus
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhTestv2FsDhcpSnpSnoopingAdminStatus (UINT4 *pu4ErrorCode,
                                       INT4
                                       i4TestValFsDhcpSnpSnoopingAdminStatus)
{
    if ((i4TestValFsDhcpSnpSnoopingAdminStatus != L2DS_ENABLED) &&
        (i4TestValFsDhcpSnpSnoopingAdminStatus != L2DS_DISABLED))
    {
        *pu4ErrorCode = SNMP_ERR_WRONG_VALUE;
        return SNMP_FAILURE;
    }

    *pu4ErrorCode = SNMP_ERR_NO_ERROR;
    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhTestv2FsDhcpSnpMacVerifyStatus
 Input       :  The Indices

                The Object 
                testValFsDhcpSnpMacVerifyStatus
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhTestv2FsDhcpSnpMacVerifyStatus (UINT4 *pu4ErrorCode,
                                   INT4 i4TestValFsDhcpSnpMacVerifyStatus)
{
    if ((i4TestValFsDhcpSnpMacVerifyStatus != L2DS_ENABLED) &&
        (i4TestValFsDhcpSnpMacVerifyStatus != L2DS_DISABLED))
    {
        *pu4ErrorCode = SNMP_ERR_WRONG_VALUE;
        return SNMP_FAILURE;
    }

    *pu4ErrorCode = SNMP_ERR_NO_ERROR;
    return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhTestv2CmDhcpSnpInsertOption82Status
 Input       :  The Indices

                The Object 
                testValCmDhcpSnpInsertOption82Status
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhTestv2CmDhcpSnpInsertOption82Status(UINT4 *pu4ErrorCode , INT4 i4TestValCmDhcpSnpInsertOption82Status)
{
    if ((i4TestValCmDhcpSnpInsertOption82Status != L2DS_ENABLED) &&
        (i4TestValCmDhcpSnpInsertOption82Status != L2DS_DISABLED))
    {
        *pu4ErrorCode = DHCPSNP_OP82_ADD_ST_ERR;
        return SNMP_FAILURE;
    }

    return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhTestv2CmDhcpSnpFwdOption82Status
 Input       :  The Indices

                The Object 
                testValCmDhcpSnpFwdOption82Status
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhTestv2CmDhcpSnpFwdOption82Status(UINT4 *pu4ErrorCode , INT4 i4TestValCmDhcpSnpFwdOption82Status)
{
    if ((i4TestValCmDhcpSnpFwdOption82Status != L2DS_ENABLED) &&
        (i4TestValCmDhcpSnpFwdOption82Status != L2DS_DISABLED))
    {
        *pu4ErrorCode = DHCPSNP_OP82_FWD_ST_ERR;
        return SNMP_FAILURE;
    }

	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhTestv2CmDhcpSnpBackupDBStatus
 Input       :  The Indices

                The Object 
                testValCmDhcpSnpBackupDBStatus
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhTestv2CmDhcpSnpBackupDBStatus(UINT4 *pu4ErrorCode , INT4 i4TestValCmDhcpSnpBackupDBStatus)
{
    if ((i4TestValCmDhcpSnpBackupDBStatus != L2DS_ENABLED) &&
        (i4TestValCmDhcpSnpBackupDBStatus != L2DS_DISABLED))
    {
        *pu4ErrorCode = DHCPSNP_OP82_BACKUP_ST_ERR;
        return SNMP_FAILURE;
    }

	UNUSED_PARAM(pu4ErrorCode);
	UNUSED_PARAM(i4TestValCmDhcpSnpBackupDBStatus);
	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhTestv2CmDhcpSnpBackupDBInterval
 Input       :  The Indices

                The Object 
                testValCmDhcpSnpBackupDBInterval
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhTestv2CmDhcpSnpBackupDBInterval(UINT4 *pu4ErrorCode , INT4 i4TestValCmDhcpSnpBackupDBInterval)
{
    if ((i4TestValCmDhcpSnpBackupDBInterval < DCS_BACKUP_DB_ITVL_MIN) ||
        (i4TestValCmDhcpSnpBackupDBInterval > DCS_BACKUP_DB_ITVL_MAX))
    {
        *pu4ErrorCode = DHCPSNP_OP82_BACKUP_ITVL_ERR;
        return SNMP_FAILURE;
    }

	return SNMP_SUCCESS;
}
/* Low Level Dependency Routines for All Objects  */

/****************************************************************************
 Function    :  nmhDepv2FsDhcpSnpSnoopingAdminStatus
 Output      :  The Dependency Low Lev Routine Take the Indices &
                check whether dependency is met or not.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhDepv2FsDhcpSnpSnoopingAdminStatus(UINT4 *pu4ErrorCode, tSnmpIndexList *pSnmpIndexList, tSNMP_VAR_BIND *pSnmpVarBind)
{
	UNUSED_PARAM(pu4ErrorCode);
	UNUSED_PARAM(pSnmpIndexList);
	UNUSED_PARAM(pSnmpVarBind);
	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhDepv2FsDhcpSnpMacVerifyStatus
 Output      :  The Dependency Low Lev Routine Take the Indices &
                check whether dependency is met or not.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhDepv2FsDhcpSnpMacVerifyStatus (UINT4 *pu4ErrorCode,
                                  tSnmpIndexList * pSnmpIndexList,
                                  tSNMP_VAR_BIND * pSnmpVarBind)
{
    UNUSED_PARAM (pu4ErrorCode);
    UNUSED_PARAM (pSnmpIndexList);
    UNUSED_PARAM (pSnmpVarBind);
    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhDepv2CmDhcpSnpInsertOption82Status
 Output      :  The Dependency Low Lev Routine Take the Indices &
                check whether dependency is met or not.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhDepv2CmDhcpSnpInsertOption82Status(UINT4 *pu4ErrorCode, tSnmpIndexList *pSnmpIndexList, tSNMP_VAR_BIND *pSnmpVarBind)
{
	UNUSED_PARAM(pu4ErrorCode);
	UNUSED_PARAM(pSnmpIndexList);
	UNUSED_PARAM(pSnmpVarBind);
	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhDepv2CmDhcpSnpFwdOption82Status
 Output      :  The Dependency Low Lev Routine Take the Indices &
                check whether dependency is met or not.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhDepv2CmDhcpSnpFwdOption82Status(UINT4 *pu4ErrorCode, tSnmpIndexList *pSnmpIndexList, tSNMP_VAR_BIND *pSnmpVarBind)
{
	UNUSED_PARAM(pu4ErrorCode);
	UNUSED_PARAM(pSnmpIndexList);
	UNUSED_PARAM(pSnmpVarBind);
	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhDepv2CmDhcpSnpBackupDBStatus
 Output      :  The Dependency Low Lev Routine Take the Indices &
                check whether dependency is met or not.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhDepv2CmDhcpSnpBackupDBStatus(UINT4 *pu4ErrorCode, tSnmpIndexList *pSnmpIndexList, tSNMP_VAR_BIND *pSnmpVarBind)
{
	UNUSED_PARAM(pu4ErrorCode);
	UNUSED_PARAM(pSnmpIndexList);
	UNUSED_PARAM(pSnmpVarBind);
	return SNMP_SUCCESS;
}
/****************************************************************************
 Function    :  nmhDepv2CmDhcpSnpBackupDBInterval
 Output      :  The Dependency Low Lev Routine Take the Indices &
                check whether dependency is met or not.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1 nmhDepv2CmDhcpSnpBackupDBInterval(UINT4 *pu4ErrorCode, tSnmpIndexList *pSnmpIndexList, tSNMP_VAR_BIND *pSnmpVarBind)
{
	UNUSED_PARAM(pu4ErrorCode);
	UNUSED_PARAM(pSnmpIndexList);
	UNUSED_PARAM(pSnmpVarBind);
	return SNMP_SUCCESS;
}
/* LOW LEVEL Routines for Table : FsDhcpSnpInterfaceTable. */
/****************************************************************************
 Function    :  nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
 Input       :  The Indices
                FsDhcpSnpVlanId
 Output      :  The Routines Validates the Given Indices.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
/* GET_EXACT Validate Index Instance Routine. */

INT1
nmhValidateIndexInstanceFsDhcpSnpInterfaceTable (INT4 i4FsDhcpSnpVlanId)
{
    /* Checking the Vlan Id with Default Values */
    if ((i4FsDhcpSnpVlanId > L2DS_MAX_VLAN_ID) ||
        (i4FsDhcpSnpVlanId <= L2DS_MIN_VLAN_ID))
    {
        return SNMP_FAILURE;
    }

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFirstIndexFsDhcpSnpInterfaceTable
 Input       :  The Indices
                FsDhcpSnpVlanId
 Output      :  The Get First Routines gets the Lexicographicaly
                First Entry from the Table.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
/* GET_FIRST Routine. */
INT1
nmhGetFirstIndexFsDhcpSnpInterfaceTable (INT4 *pi4FsDhcpSnpVlanId)
{
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry 		*pL2DSVlanEntry = NULL;
    tL2DhcpSnpIfaceEntry 		L2DSVlanEntry;

    /* Getting the First Element from the Tree */
    if ((pRBElem = RBTreeGetFirst (L2DS_INTF_RBTREE)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DSVlanEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;

    while (pL2DSVlanEntry->u4L2dsCxtId != L2DS_CURR_CXT_ID)
    {
        if (pL2DSVlanEntry->u4L2dsCxtId > L2DS_CURR_CXT_ID)
        {
            return SNMP_FAILURE;
        }

        MEMSET (&L2DSVlanEntry, L2DS_ZERO, sizeof (tL2DhcpSnpIfaceEntry));
        L2DSVlanEntry.u4L2dsCxtId = pL2DSVlanEntry->u4L2dsCxtId;
        L2DSVlanEntry.VlanId = pL2DSVlanEntry->VlanId;

        if ((pL2DSVlanEntry = (tL2DhcpSnpIfaceEntry *)
             RBTreeGetNext (L2DS_INTF_RBTREE,
                            (tL2DhcpSnpIfaceEntry *) & L2DSVlanEntry,
                            NULL)) == NULL)
        {
            return SNMP_FAILURE;
        }
    }

    *pi4FsDhcpSnpVlanId = pL2DSVlanEntry->VlanId;

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetNextIndexFsDhcpSnpInterfaceTable
 Input       :  The Indices
                FsDhcpSnpVlanId
                nextFsDhcpSnpVlanId
 Output      :  The Get Next function gets the Next Index for
                the Index Value given in the Index Values. The
                Indices are stored in the next_varname variables.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
/* GET_NEXT Routine.  */
INT1
nmhGetNextIndexFsDhcpSnpInterfaceTable (INT4 i4FsDhcpSnpVlanId,
                                        INT4 *pi4NextFsDhcpSnpVlanId)
{
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DSVlanEntry = NULL;
    tL2DhcpSnpIfaceEntry L2DSVlanEntry;

    L2DSVlanEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DSVlanEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;
    /* Getting the Next Element from the Tree */
    if ((pRBElem = RBTreeGetNext (L2DS_INTF_RBTREE,
                                  (tRBElem *) & L2DSVlanEntry,
                                  NULL)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DSVlanEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    if (pL2DSVlanEntry->u4L2dsCxtId != L2DS_CURR_CXT_ID)
    {
        return SNMP_FAILURE;
    }
    *pi4NextFsDhcpSnpVlanId = pL2DSVlanEntry->VlanId;

    return SNMP_SUCCESS;
}

/* Low Level GET Routine for All Objects  */
/****************************************************************************
 Function    :  nmhGetFsDhcpSnpVlanId
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpVlanId
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpVlanId (INT4 i4FsDhcpSnpVlanId,
                              INT4 *pi4RetValFsDhcpSnpVlanId)
{
    tL2DhcpSnpIfaceEntry L2DSVlanEntry;
    tRBElem            *pRBElem = NULL;

    L2DSVlanEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DSVlanEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get Structure Pointer and   *
     * Checking whether that entry is Null or not          */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DSVlanEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    *pi4RetValFsDhcpSnpVlanId = i4FsDhcpSnpVlanId;

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpVlanSnpStatus
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpVlanSnpStatus
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpVlanSnpStatus (INT4 i4FsDhcpSnpVlanId,
                              INT4 *pi4RetValFsDhcpSnpVlanSnpStatus)
{
    tL2DhcpSnpIfaceEntry L2DSVlanEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DSVlanEntry = NULL;

    L2DSVlanEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DSVlanEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get Structure Pointer and   *
     * Checking whether that entry is Null or not          */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DSVlanEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DSVlanEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pi4RetValFsDhcpSnpVlanSnpStatus =
        L2DS_INTF_SNP_STATUS (pL2DSVlanEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpRxDiscovers
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpRxDiscovers
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpRxDiscovers (INT4 i4FsDhcpSnpVlanId,
                            UINT4 *pu4RetValFsDhcpSnpRxDiscovers)
{
    tL2DhcpSnpIfaceEntry L2DSVlanEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DSVlanEntry = NULL;

    L2DSVlanEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DSVlanEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DSVlanEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DSVlanEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpRxDiscovers = L2DS_STAT_RX_DISC (pL2DSVlanEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpRxRequests
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpRxRequests
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpRxRequests (INT4 i4FsDhcpSnpVlanId,
                           UINT4 *pu4RetValFsDhcpSnpRxRequests)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpRxRequests = L2DS_STAT_RX_REQS (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpRxReleases
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpRxReleases
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpRxReleases (INT4 i4FsDhcpSnpVlanId,
                           UINT4 *pu4RetValFsDhcpSnpRxReleases)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpRxReleases = L2DS_STAT_RX_RELS (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpRxDeclines
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpRxDeclines
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpRxDeclines (INT4 i4FsDhcpSnpVlanId,
                           UINT4 *pu4RetValFsDhcpSnpRxDeclines)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpRxDeclines = L2DS_STAT_RX_DECL (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpRxInforms
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpRxInforms
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpRxInforms (INT4 i4FsDhcpSnpVlanId,
                          UINT4 *pu4RetValFsDhcpSnpRxInforms)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpRxInforms = L2DS_STAT_RX_INF (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpTxOffers
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpTxOffers
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpTxOffers (INT4 i4FsDhcpSnpVlanId,
                         UINT4 *pu4RetValFsDhcpSnpTxOffers)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpTxOffers = L2DS_STAT_TX_OFFR (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpTxAcks
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpTxAcks
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpTxAcks (INT4 i4FsDhcpSnpVlanId, UINT4 *pu4RetValFsDhcpSnpTxAcks)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpTxAcks = L2DS_STAT_TX_ACKS (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpTxNaks
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpTxNaks
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpTxNaks (INT4 i4FsDhcpSnpVlanId, UINT4 *pu4RetValFsDhcpSnpTxNaks)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpTxNaks = L2DS_STAT_TX_NAKS (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpNoOfDiscards
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpNoOfDiscards
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpNoOfDiscards (INT4 i4FsDhcpSnpVlanId,
                             UINT4 *pu4RetValFsDhcpSnpNoOfDiscards)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpNoOfDiscards = L2DS_STAT_TTL_DISC (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpMacDiscards
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpMacDiscards
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpMacDiscards (INT4 i4FsDhcpSnpVlanId,
                            UINT4 *pu4RetValFsDhcpSnpMacDiscards)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;
    tRBElem            *pRBElem = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpMacDiscards = L2DS_STAT_MAC_DISC (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpServerDiscards
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpServerDiscards
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpServerDiscards (INT4 i4FsDhcpSnpVlanId,
                               UINT4 *pu4RetValFsDhcpSnpServerDiscards)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;
    tRBElem            *pRBElem = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpServerDiscards
        = L2DS_STAT_SRV_DISC (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpOptionDiscards
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpOptionDiscards
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpOptionDiscards (INT4 i4FsDhcpSnpVlanId,
                               UINT4 *pu4RetValFsDhcpSnpOptionDiscards)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;
    tRBElem            *pRBElem = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pu4RetValFsDhcpSnpOptionDiscards =
        L2DS_STAT_OPT_DISC (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhGetFsDhcpSnpInterfaceStatus
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                retValFsDhcpSnpInterfaceStatus
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsDhcpSnpInterfaceStatus (INT4 i4FsDhcpSnpVlanId,
                                INT4 *pi4RetValFsDhcpSnpInterfaceStatus)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;
    tRBElem            *pRBElem = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;
    *pi4RetValFsDhcpSnpInterfaceStatus =
        L2DS_INT_ROWSTATUS (pL2DhcpSnpIfaceEntry);

    return SNMP_SUCCESS;
}

/* Low Level SET Routine for All Objects  */
/****************************************************************************
 Function    :  nmhSetFsDhcpSnpVlanSnpStatus
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                setValFsDhcpSnpVlanSnpStatus
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhSetFsDhcpSnpVlanSnpStatus (INT4 i4FsDhcpSnpVlanId,
                              INT4 i4SetValFsDhcpSnpVlanSnpStatus)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Calling the Function to get the Structure Pointer and   *
     * Checking whether that entry is Null or not              */
    if ((pRBElem = RBTreeGet (L2DS_INTF_RBTREE,
                              (tRBElem *) & L2DhcpSnpIfaceEntry)) == NULL)
    {
        return SNMP_FAILURE;
    }

    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *) pRBElem;

    L2DS_INTF_SNP_STATUS (pL2DhcpSnpIfaceEntry) = (UINT1)
        i4SetValFsDhcpSnpVlanSnpStatus;

    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhSetFsDhcpSnpInterfaceStatus
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhSetFsDhcpSnpInterfaceStatus (INT4 i4FsDhcpSnpVlanId,
                                INT4 i4SetValFsDhcpSnpInterfaceStatus)
{
    tVlanId             VlanId;
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    VlanId = (tVlanId) i4FsDhcpSnpVlanId;
    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = VlanId;

    if ((i4SetValFsDhcpSnpInterfaceStatus == L2DS_CREATE_AND_GO) ||
        (i4SetValFsDhcpSnpInterfaceStatus == L2DS_CREATE_AND_WAIT))
    {
        /* If the rowstatus is create and wait or create and go, create the 
         * entry and set the rowstatus */
        pL2DhcpSnpIfaceEntry =
            L2dsIntfCreateIntfEntry (L2DS_CURR_CXT_ID, VlanId);

        if (pL2DhcpSnpIfaceEntry == NULL)
        {
            return SNMP_FAILURE;
        }

        if (i4SetValFsDhcpSnpInterfaceStatus == L2DS_CREATE_AND_GO)
        {
            L2DS_INT_ROWSTATUS (pL2DhcpSnpIfaceEntry) = L2DS_ACTIVE;
        }
        else if (i4SetValFsDhcpSnpInterfaceStatus == L2DS_CREATE_AND_WAIT)
        {
            L2DS_INT_ROWSTATUS (pL2DhcpSnpIfaceEntry) = L2DS_NOT_READY;
        }
        return SNMP_SUCCESS;
    }

    if ((i4SetValFsDhcpSnpInterfaceStatus == L2DS_NOT_IN_SERVICE) ||
        (i4SetValFsDhcpSnpInterfaceStatus == L2DS_ACTIVE))
    {
        /* If the rowstatus is not in service or active, set the value, after 
         * retrieving the entry */
        pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *)
            RBTreeGet (L2DS_INTF_RBTREE, &L2DhcpSnpIfaceEntry);

        if (pL2DhcpSnpIfaceEntry == NULL)
        {
            return SNMP_FAILURE;
        }

        if ((i4SetValFsDhcpSnpInterfaceStatus == L2DS_NOT_IN_SERVICE) &&
            (L2DS_INT_ROWSTATUS (pL2DhcpSnpIfaceEntry) == L2DS_ACTIVE))
        {
            /* If previous status was active, and if we are making it in to 
             * not in service, deleta all the DHCP Entries associated */
            L2dsPortDeleteEntries (L2DS_CURR_CXT_ID, VlanId);
        }

        L2DS_INT_ROWSTATUS (pL2DhcpSnpIfaceEntry) =
            (UINT1) i4SetValFsDhcpSnpInterfaceStatus;

        return SNMP_SUCCESS;
    }

    if (i4SetValFsDhcpSnpInterfaceStatus == L2DS_DESTROY)
    {
        /* If the rowstatus is destroy, delete the interface entry */
        if (L2dsIntfDeleteIntfEntry (L2DS_CURR_CXT_ID, VlanId) != L2DS_SUCCESS)
        {
            return SNMP_FAILURE;
        }
        return SNMP_SUCCESS;
    }
    return SNMP_FAILURE;
}

/* Low Level TEST Routines for All Objects  */
/****************************************************************************
 Function    :  nmhTestv2FsDhcpSnpVlanSnpStatus
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                testValFsDhcpSnpVlanSnpStatus
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhTestv2FsDhcpSnpVlanSnpStatus (UINT4 *pu4ErrorCode,
                                 INT4 i4FsDhcpSnpVlanId,
                                 INT4 i4TestValFsDhcpSnpVlanSnpStatus)
{
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry L2DSVlanEntry;

    L2DSVlanEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DSVlanEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    if ((i4FsDhcpSnpVlanId > L2DS_MAX_VLAN_ID) ||
        (i4FsDhcpSnpVlanId <= L2DS_MIN_VLAN_ID))
    {
        CmSetParamN("L2DS_MAX_VLAN_ID", L2DS_MAX_VLAN_ID);
        *pu4ErrorCode = DHCPSNP_INTF_ENTRY_VID_ERR;
        return SNMP_FAILURE;
    }

    /* Calling the Function to get the Structure Pointer 
       *  and checking whether that entry is Null or not .
       */
	pRBElem = RBTreeGet(L2DS_INTF_RBTREE, (tRBElem *)&L2DSVlanEntry);
    if (pRBElem == NULL)
    {
        *pu4ErrorCode = DHCPSNP_INTF_ENTRY_EXIST_ERR;
        return SNMP_FAILURE;
    }

    if ((i4TestValFsDhcpSnpVlanSnpStatus != L2DS_ENABLED) &&
        (i4TestValFsDhcpSnpVlanSnpStatus != L2DS_DISABLED))
    {
        *pu4ErrorCode = SNMP_ERR_WRONG_VALUE;
        return SNMP_FAILURE;
    }

    *pu4ErrorCode = SNMP_ERR_NO_ERROR;
    return SNMP_SUCCESS;
}

/****************************************************************************
 Function    :  nmhTestv2FsDhcpSnpInterfaceStatus
 Input       :  The Indices
                FsDhcpSnpVlanId

                The Object 
                testValFsDhcpSnpInterfaceStatus
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhTestv2FsDhcpSnpInterfaceStatus (UINT4 *pu4ErrorCode,
                                   INT4 i4FsDhcpSnpVlanId,
                                   INT4 i4TestValFsDhcpSnpInterfaceStatus)
{
    tRBElem            *pRBElem = NULL;
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = L2DS_CURR_CXT_ID;
    L2DhcpSnpIfaceEntry.VlanId = (tVlanId) i4FsDhcpSnpVlanId;

    /* Checking the Vlan Id with Default Values */
    if ((i4FsDhcpSnpVlanId > L2DS_MAX_VLAN_ID) ||
        (i4FsDhcpSnpVlanId <= L2DS_MIN_VLAN_ID))
    {
        CmSetParamN("L2DS_MAX_VLAN_ID", L2DS_MAX_VLAN_ID);
        *pu4ErrorCode = DHCPSNP_INTF_ENTRY_VID_ERR;
        return SNMP_FAILURE;
    }

    pRBElem = RBTreeGet (L2DS_INTF_RBTREE, (tRBElem *) & L2DhcpSnpIfaceEntry);

    if ((i4TestValFsDhcpSnpInterfaceStatus == L2DS_CREATE_AND_WAIT) ||
        (i4TestValFsDhcpSnpInterfaceStatus == L2DS_CREATE_AND_GO))
    {
        if (pRBElem != NULL)
        {
            *pu4ErrorCode = DHCPSNP_INTF_ENTRY_EXIST_ERR;
            return SNMP_FAILURE;
        }
        *pu4ErrorCode = SNMP_ERR_NO_ERROR;
        return SNMP_SUCCESS;
    }

    if ((i4TestValFsDhcpSnpInterfaceStatus == L2DS_NOT_IN_SERVICE) ||
        (i4TestValFsDhcpSnpInterfaceStatus == L2DS_ACTIVE) ||
        (i4TestValFsDhcpSnpInterfaceStatus == L2DS_DESTROY))
    {
        if (pRBElem == NULL)
        {
            *pu4ErrorCode = DHCPSNP_INTF_ENTRY_NOT_EXIST_ERR;
            return SNMP_FAILURE;
        }

        *pu4ErrorCode = SNMP_ERR_NO_ERROR;
        return SNMP_SUCCESS;
    }

    *pu4ErrorCode = SNMP_ERR_WRONG_VALUE;
    return SNMP_FAILURE;
}

/* Low Level Dependency Routines for All Objects  */

/****************************************************************************
 Function    :  nmhDepv2FsDhcpSnpInterfaceTable
 Input       :  The Indices
                FsDhcpSnpVlanId
 Output      :  The Dependency Low Lev Routine Take the Indices &
                check whether dependency is met or not.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhDepv2FsDhcpSnpInterfaceTable (UINT4 *pu4ErrorCode,
                                 tSnmpIndexList * pSnmpIndexList,
                                 tSNMP_VAR_BIND * pSnmpVarBind)
{
    UNUSED_PARAM (pu4ErrorCode);
    UNUSED_PARAM (pSnmpIndexList);
    UNUSED_PARAM (pSnmpVarBind);
    return SNMP_SUCCESS;
}

/*                                                                           */
/***************************** END OF FILE ***********************************/
/*                                                                           */
