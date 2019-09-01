/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dscli.c,v 1.9 2013/01/10 12:29:05 siva Exp $ */
/*****************************************************************************/
/*    FILE  NAME            : l2dscli.c                                      */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snooping                                */
/*    MODULE NAME           : L2DHCP Snooping Cli Module                     */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains CLI SET/GET/TEST and        */
/*                            GETNEXT routines for the MIB objects           */
/*                            specified in fsdhcsnp.mib funtions             */
/*                            for L2DHCP Snooping Cli module                 */
/*---------------------------------------------------------------------------*/

#include "l2dsinc.h"
#include "l2dsextn.h"

/*****************************************************************************/
/* Function Name      : cli_process_l2ds_cmd                                 */
/*                                                                           */
/* Description        : This function takes in variable no. of arguments     */
/*                      and process the commands for DHCP Snooping Cli module*/
/*                      as defined in l2dscmd.def                            */
/*                                                                           */
/* Input(s)           : CliHandle -  CLIHandler                              */
/*                      u4Command -  Command Identifier                      */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None                                                 */
/*****************************************************************************/
INT4
cli_process_l2ds_cmd (tCliHandle CliHandle, UINT4 u4Command, ...)
{
    va_list             ap;
    UINT4              *apu4Args[CLI_MAX_ARGS] = { L2DS_ZERO };
    UINT4               u4IfIndex = L2DS_ZERO;
    INT1                i1argno = L2DS_ZERO;
    tVlanId             VlanId = L2DS_ZERO;
    INT4                i4Inst = L2DS_ZERO;
    INT4                i4Trace = L2DS_ZERO;
    UINT4               u4ContextId = L2DS_ZERO;
    INT4                i4RetStatus = CLI_SUCCESS;

    CliRegisterLock (CliHandle, L2DS_LOCK, L2DS_UNLOCK);
    L2DS_LOCK ();

    if (L2DSCliSelectContextOnMode
        (CliHandle, u4Command, &u4ContextId) == L2DS_FAILURE)
    {
        L2DS_UNLOCK ();
        CliUnRegisterLock (CliHandle);
        return CLI_FAILURE;
    }

    va_start (ap, u4Command);
    /* third argument is always interface name/index */
    i4Inst = CLI_PTR_TO_I4 (va_arg (ap, UINT1 *));

    if (i4Inst != L2DS_ZERO)
    {
        u4IfIndex = (UINT4) i4Inst;
    }
    else
    {
        u4IfIndex = CLI_GET_IFINDEX ();
    }
    /* Walk through the rest of the arguments and store in args array,
     * till the number of arguments reaches CLI_MAX_ARGS */
    while (L2DS_ONE)
    {
        apu4Args[i1argno++] = va_arg (ap, UINT4 *);

        if (i1argno == CLI_MAX_ARGS)
        {
            break;
        }
    }

    va_end (ap);
    VlanId = (tVlanId) CLI_GET_VLANID ();

    CLI_SET_ERR (L2DS_ZERO);
    switch (u4Command)
    {
        case L2DS_STATUS_ENABLE:
            i4RetStatus = L2dsCliSetSnoopingStatus (CliHandle, L2DS_ENABLED);
            break;

        case L2DS_STATUS_DISABLE:
            i4RetStatus = L2dsCliSetSnoopingStatus (CliHandle, L2DS_DISABLED);
            break;

        case L2DS_MAC_STATUS_ENABLE:
            i4RetStatus = L2dsCliSetMacVerifyStatus (CliHandle, L2DS_ENABLED);
            break;

        case L2DS_MAC_STATUS_DISABLE:
            i4RetStatus = L2dsCliSetMacVerifyStatus (CliHandle, L2DS_DISABLED);
            break;

        case L2DS_IFACE_STATUS_ENABLE:
            i4RetStatus = L2dsCliSetSnpIfaceStatus (CliHandle, L2DS_ENABLED,
                                                    VlanId);
            break;

        case L2DS_IFACE_STATUS_DISABLE:
            i4RetStatus = L2dsCliSetSnpIfaceStatus (CliHandle, L2DS_DISABLED,
                                                    VlanId);
            break;

        case L2DS_GLOBAL_SHOW:
            i4RetStatus = L2dsCliShowSnpGlobConf (CliHandle, (UINT1 *)
                                                  apu4Args[L2DS_ZERO]);
            break;

        case L2DS_VLAN_SHOW:

            if ((apu4Args[L2DS_ONE]) != NULL)
            {
                VlanId = (tVlanId) * (apu4Args[L2DS_ONE]);
            }
            else
            {
                VlanId = L2DS_ZERO;
            }
            i4RetStatus = L2dsCliShowSnpVlanConf (CliHandle, (UINT1 *)
                                                  apu4Args[L2DS_ZERO], VlanId);
            break;

        case L2DS_TRACE_ENABLE:
            i4Trace = CLI_PTR_TO_I4 (apu4Args[L2DS_ZERO]);
            i4RetStatus = L2dsCliSetTrace (i4Trace);
            break;

        case L2DS_PORT_STATE_TRUSTED:
            i4RetStatus = L2dsCliSetPortState (CliHandle, u4IfIndex,
                                               L2DS_TRUSTED_PORT);
            break;

        case L2DS_PORT_STATE_UNTRUSTED:
            i4RetStatus = L2dsCliSetPortState (CliHandle, u4IfIndex,
                                               L2DS_UNTRUSTED_PORT);
            break;

        case L2DS_TRACE_DISABLE:
            i4Trace = L2DS_ZERO;
            i4RetStatus = L2dsCliSetTrace (i4Trace);
            break;

        default:
            CliPrintf (CliHandle, "\r%% Unknown command \r\n");
            L2DS_UNLOCK ();
            CliUnRegisterLock (CliHandle);
            return CLI_FAILURE;
    }

    CLI_SET_CMD_STATUS (i4RetStatus);
    L2DS_UNLOCK ();
    CliUnRegisterLock (CliHandle);

    return i4RetStatus;
}

/*****************************************************************************/
/* Function Name      : L2dsCliSetSnoopingStatus                             */
/*                                                                           */
/* Description        : This function Enables/Disables Layer 2               */
/*                      DHCP Snooping                                        */
/*                                                                           */
/* Input(s)           : CliHandle            -  CLIHandler                   */
/*                      i4L2dsSnoopingStatus -  Snooping Status              */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : CLI_SUCCESS/CLI_FAILURE                              */
/*****************************************************************************/
INT4
L2dsCliSetSnoopingStatus (tCliHandle CliHandle, INT4 i4L2dsSnoopingStatus)
{
    UINT4               u4ErrCode = L2DS_ZERO;

    if (nmhTestv2FsDhcpSnpSnoopingAdminStatus (&u4ErrCode,
                                               i4L2dsSnoopingStatus) ==
        SNMP_FAILURE)
    {
        CliPrintf (CliHandle, "\r%% Enabling/Disabling of "
                   " DHCP Snooping failed\r\n");
        return CLI_FAILURE;
    }

    if (nmhSetFsDhcpSnpSnoopingAdminStatus (i4L2dsSnoopingStatus) ==
        SNMP_FAILURE)
    {
        CLI_FATAL_ERROR (CliHandle);
        return CLI_FAILURE;
    }

    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      :  L2dsCliSetMacVerifyStatus                           */
/*                                                                           */
/* Description        :  This function Enables/Disables Layer 2              */
/*                       DHCP Snooping Mac Verification                      */
/*                                                                           */
/* Input(s)           :  CliHandle             -  CLIHandler                 */
/*                       i4L2dsMacVerifyStatus -  Mac Verify Status          */
/*                                                                           */
/* Output(s)          :  None                                                */
/*                                                                           */
/* Return Value(s)    :  CLI_SUCCESS/CLI_FAILURE                             */
/*****************************************************************************/
INT4
L2dsCliSetMacVerifyStatus (tCliHandle CliHandle, INT4 i4L2dsMacVerifyStatus)
{
    UINT4               u4ErrCode = L2DS_ZERO;

    if (nmhTestv2FsDhcpSnpMacVerifyStatus (&u4ErrCode,
                                           i4L2dsMacVerifyStatus) ==
        SNMP_FAILURE)
    {
        CliPrintf (CliHandle, "\r%% Enabling/Disabling of DHCP Snooping"
                   " Mac Verification failed\r\n");
        return CLI_FAILURE;
    }

    if (nmhSetFsDhcpSnpMacVerifyStatus (i4L2dsMacVerifyStatus) == SNMP_FAILURE)
    {
        CLI_FATAL_ERROR (CliHandle);
        return CLI_FAILURE;
    }

    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      :  L2dsCliSetPortState                                 */
/*                                                                           */
/* Description        :  This function is to set the DHCP Snooping           */
/*                       Trusted/Untrusted.                                  */
/*                                                                           */
/* Input(s)           :  CliHandle             -  CLIHandler                 */
/*                       u4IfIndex             -  Interface Index            */
/*                       i4PortSecState        -  PortSecurityStatus         */
/*                                                                           */
/* Output(s)          :  None                                                */
/*                                                                           */
/* Return Value(s)    :  CLI_SUCCESS/CLI_FAILURE                             */
/*****************************************************************************/

INT4
L2dsCliSetPortState (tCliHandle CliHandle, UINT4 u4IfIndex, INT4 i4PortSecState)
{
    if ((CfaApiSetIfPortSecState (u4IfIndex, (UINT1) i4PortSecState))
        == CFA_FAILURE)
    {
        CliPrintf (CliHandle, "\r%%Port security state configuration on "
                   "port %d failed \r\n", u4IfIndex);
        return CLI_FAILURE;
    }

    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      :  L2dsCliSetSnpIfaceStatus                            */
/*                                                                           */
/* Description        :  This function Enables/Disables Layer 2              */
/*                       DHCP Snooping Interface Status                      */
/*                                                                           */
/* Input(s)           :  CliHandle -  CLIHandler                             */
/*                       i4L2dsSnpIfaceStatus -  Interface Status            */
/*                       i4VlanId - VLAN Identifier                          */
/*                                                                           */
/* Output(s)          :  None                                                */
/*                                                                           */
/* Return Value(s)    :  CLI_SUCCESS/CLI_FAILURE                             */
/*****************************************************************************/
INT4
L2dsCliSetSnpIfaceStatus (tCliHandle CliHandle, INT4 i4L2dsSnpIfaceStatus,
                          INT4 i4VlanId)
{
    UINT4               u4ErrCode = L2DS_ZERO;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    /* Create the interface entry, if it is not there */
    if (i4L2dsSnpIfaceStatus == L2DS_ENABLED)
    {
        if ((pL2DhcpSnpIfaceEntry = L2dsIntfCreateIntfEntry
             (L2DS_CURR_CXT_ID, (UINT2) i4VlanId)) == NULL)
        {
            CliPrintf (CliHandle, "\r%% Enabling of "
                       " DHCP Snooping on Interface failed\r\n");
            return CLI_FAILURE;
        }
        pL2DhcpSnpIfaceEntry->u1RowStatus = L2DS_ACTIVE;
    }

    if (i4L2dsSnpIfaceStatus == L2DS_DISABLED)
    {
        if (L2dsIntfDeleteIntfEntry (L2DS_CURR_CXT_ID,
                                     (UINT2) i4VlanId) == L2DS_FAILURE)
        {
            CliPrintf (CliHandle, "\r%% Disabling of "
                       " DHCP Snooping on Interface failed\r\n");
            return CLI_FAILURE;
        }
        else
        {
            return CLI_SUCCESS;
        }
    }

    if (nmhTestv2FsDhcpSnpVlanSnpStatus (&u4ErrCode, i4VlanId,
                                         i4L2dsSnpIfaceStatus) == SNMP_FAILURE)
    {
        CliPrintf (CliHandle, "\r%% Enabling/Disabling of DHCP Snooping"
                   "Interface Status Failed\r\n");
        return CLI_FAILURE;
    }

    if (nmhSetFsDhcpSnpVlanSnpStatus (i4VlanId, i4L2dsSnpIfaceStatus) ==
        SNMP_FAILURE)
    {
        CLI_FATAL_ERROR (CliHandle);
        return CLI_FAILURE;
    }

    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      :  L2dsCliShowSnpGlobConf                              */
/*                                                                           */
/* Description        :  This function shows                                 */
/*                       DHCP Snooping Global Configuration                  */
/*                                                                           */
/* Input(s)           :  CliHandle  -  CLIHandler                            */
/*                       pu1CxtName   -  Context Name                        */
/*                                                                           */
/* Output(s)          :  None                                                */
/*                                                                           */
/* Return Value(s)    :  CLI_SUCCESS                                         */
/*****************************************************************************/
INT4
L2dsCliShowSnpGlobConf (tCliHandle CliHandle, UINT1 *pu1CxtName)
{
    UINT4               u4ContextId = L2DS_ZERO;
    INT4                i4ContextId = L2DS_ZERO;
    UINT1               u1CxtFlag = L2DS_FALSE;
    INT4                i4CurrCxtId = L2DS_ZERO;
    INT4                i4Status = L2DS_ZERO;
    INT4                i4CliContextId = L2DS_ZERO;

    /* Displays Global Information */
    CliPrintf (CliHandle, "\r\nDHCP Snooping Global information\r\n");
    CliPrintf (CliHandle, "--------------------------------\r\n\n");

    if (pu1CxtName != NULL)
    {
        /* If Switch-name is given then get the Context-Id from it */
        if (L2dsVcmIsSwitchNameExist (pu1CxtName, &u4ContextId) != L2DS_TRUE)
        {
            CliPrintf (CliHandle, "\r%% Switch %s Does not exist.\r\n",
                       pu1CxtName);
            return CLI_FAILURE;
        }
        i4CliContextId = (INT4) u4ContextId;
        u1CxtFlag = L2DS_TRUE;
    }

    if (nmhGetFirstIndexFsMIDhcpSnpGlobalConfigTable
        (&i4ContextId) != SNMP_SUCCESS)
    {
        return CLI_SUCCESS;
    }
    do
    {
        /*Selecting the Context */
        L2dsSelectContext (i4ContextId);
        if (u1CxtFlag == L2DS_TRUE)
        {
            /*Get the Vlan Information if Only Context-Id is Given Through CLI */
            if (i4CliContextId == i4ContextId)
            {
                L2dsPrintContextInfo (CliHandle, i4ContextId);
                if (nmhGetFsMIDhcpSnpSnoopingAdminStatus
                    (i4ContextId, &i4Status) == SNMP_SUCCESS)
                {
                    L2dsPrintGlobalSnoopingStatus (CliHandle, i4Status);
                }
                if (nmhGetFsMIDhcpSnpMacVerifyStatus
                    (i4ContextId, &i4Status) == SNMP_SUCCESS)
                {
                    L2dsPrintMacVerifyStatus (CliHandle, i4Status);
                }
                L2dsReleaseContext ();
                break;

            }
        }
        else
        {
            L2dsPrintContextInfo (CliHandle, i4ContextId);
            if (nmhGetFsMIDhcpSnpSnoopingAdminStatus
                (i4ContextId, &i4Status) == SNMP_SUCCESS)
            {
                L2dsPrintGlobalSnoopingStatus (CliHandle, i4Status);
            }
            if (nmhGetFsMIDhcpSnpMacVerifyStatus
                (i4ContextId, &i4Status) == SNMP_SUCCESS)
            {
                L2dsPrintMacVerifyStatus (CliHandle, i4Status);
            }
        }
        /* Releasing the Context */
        L2dsReleaseContext ();
        i4CurrCxtId = i4ContextId;

    }
    /* Getting the Next Entry From the Table */
    while (nmhGetNextIndexFsMIDhcpSnpGlobalConfigTable
           (i4CurrCxtId, &i4ContextId) == SNMP_SUCCESS);

    CliPrintf (CliHandle, "\r\n");

    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsPrintGlobalSnoopingStatus                        */
/*                                                                           */
/* Description        : This function displays the GlobalSnooping Status     */
/*                                                                           */
/* Input(s)           : CliHandle  -  CLIHandler                             */
/*                      i4Status   -  Snooping Status                        */
/*                                                                           */
/* Output(s)          : NONE                                                 */
/*                                                                           */
/* Return Value(s)    : NONE                                                 */
/*****************************************************************************/

VOID
L2dsPrintGlobalSnoopingStatus (tCliHandle CliHandle, INT4 i4Status)
{
    if (i4Status == L2DS_ENABLED)
    {
        CliPrintf (CliHandle, "Layer 2 DHCP Snooping is "
                   "globally enabled \r\n");
    }
    else
    {
        CliPrintf (CliHandle,
                   "Layer 2 DHCP Snooping is " "globally disabled \r\n");
    }

}

/*****************************************************************************/
/* Function Name      : L2dsPrintMacVerifyStatus                             */
/*                                                                           */
/* Description        : This function displays Mac Verify Status             */
/*                                                                           */
/* Input(s)           : CliHandle  -  CLIHandler                             */
/*                      i4Status   -  Verify Status                          */
/*                                                                           */
/* Output(s)          : NONE                                                 */
/*                                                                           */
/* Return Value(s)    : NONE                                                 */
/*****************************************************************************/

VOID
L2dsPrintMacVerifyStatus (tCliHandle CliHandle, INT4 i4Status)
{
    if (i4Status == L2DS_ENABLED)
    {
        CliPrintf (CliHandle, "MAC Address verification is " "enabled \r\n\n");
    }
    else
    {
        CliPrintf (CliHandle, "MAC Address verification is " "disabled \r\n\n");
    }

}

/*****************************************************************************/
/* Function Name      :  L2dsCliShowSnpVlanConf                              */
/*                                                                           */
/* Description        :  This function shows                                 */
/*                       DHCP Snooping Vlan Configuration                    */
/*                                                                           */
/* Input(s)           :  CliHandle   -  CLIHandler                           */
/*                       pu1CxtName  -  Context Name                        */
/*                       i4VlanId    -  VlanId                               */
/*                                                                           */
/* Output(s)          :  None                                                */
/*                                                                           */
/* Return Value(s)    :  CLI_SUCCESS/CLI_FAILURE                             */
/*****************************************************************************/
INT4
L2dsCliShowSnpVlanConf (tCliHandle CliHandle, UINT1 *pu1CxtName, INT4 i4VlanId)
{
    UINT4               u4PagingStatus = CLI_SUCCESS;
    INT4                i4CurrVlanId = L2DS_ZERO;
    INT4                i4L2dsVlanId = L2DS_ZERO;
    INT4                i4ContextId = L2DS_ZERO;
    UINT4               u4ContextId = L2DS_ZERO;
    UINT1               u1CxtFlag = L2DS_FALSE;
    UINT1               u1VlanFlag = L2DS_FALSE;
    INT4                i4CliContextId = L2DS_ZERO;
    INT4                i4CurrCxtId = L2DS_ZERO;

    CliPrintf (CliHandle, "\r\nDHCP Snooping Vlan information\r\n");
    CliPrintf (CliHandle, "------------------------------\r\n");

    if (pu1CxtName != NULL)
    {
        /* If Switch-name is given then get the Context-Id from it */
        if (L2dsVcmIsSwitchNameExist (pu1CxtName, &u4ContextId) != L2DS_TRUE)
        {
            CliPrintf (CliHandle, "\r%% Switch %s Does not exist.\r\n",
                       pu1CxtName);
            return CLI_FAILURE;
        }
        i4CliContextId = (INT4) u4ContextId;
        u1CxtFlag = L2DS_TRUE;
    }
    if (i4VlanId != L2DS_ZERO)
    {
        u1VlanFlag = L2DS_TRUE;
    }

    /* Getting the First Entry From the Table */
    if (nmhGetFirstIndexFsMIDhcpSnpInterfaceTable
        (&i4ContextId, &i4L2dsVlanId) != SNMP_SUCCESS)
    {
        return CLI_SUCCESS;
    }

    do
    {
        /*Selecting the Context */
        L2dsSelectContext (i4ContextId);

        /*Check if both the Vlan and Context Flag's are set */
        if ((u1CxtFlag == L2DS_TRUE) && (u1VlanFlag == L2DS_TRUE))
        {
            /*Get the Vlan Information if Vlan and Context is Given
             *Through CLI.
             *1)Check for the Context-Id present in the Vlan Interface table
             *and the Context-Id which is give through CLI are Same.
             *2)Check for the Vlan-Id present in the Vlan Interface table and
             *the Vlan-Id which is give through CLI are Same
             *If both are same then print the Vlan Information*/

            if ((i4CliContextId == i4ContextId) && (i4VlanId == i4L2dsVlanId))
            {
                L2dsPrintContextInfo (CliHandle, i4ContextId);
                /* Calling the function to get the Structure information   *
                 * about the particular Vlan with Corresponding Context-Id */
                L2dsCliPrintVlanInformation (CliHandle, i4L2dsVlanId,
                                             &u4PagingStatus);
            }
        }
        else if (u1CxtFlag == L2DS_TRUE)
        {
            /*Get the Vlan Information if Only Context-Id is Given Through CLI */
            if (i4CliContextId == i4ContextId)
            {
                if (i4ContextId != i4CurrCxtId)
                {
                    L2dsPrintContextInfo (CliHandle, i4ContextId);
                }
                /* Calling the function to get the Structure information   *
                 * about the particular Vlan for the given Context-Id      */
                L2dsCliPrintVlanInformation (CliHandle, i4L2dsVlanId,
                                             &u4PagingStatus);
            }
        }
        else if (u1VlanFlag == L2DS_TRUE)
        {
            /*Get the Vlan Information if Only Vlan-Id is Given Through CLI */
            if (i4VlanId == i4L2dsVlanId)
            {
                if (i4ContextId != i4CurrCxtId)
                {
                    L2dsPrintContextInfo (CliHandle, i4ContextId);

                }
                /* Calling the function to get the Structure information   *
                 * of the given Vlan through CLI.                          */
                L2dsCliPrintVlanInformation (CliHandle, i4L2dsVlanId,
                                             &u4PagingStatus);
            }
        }
        else
        {
            if (i4ContextId != i4CurrCxtId)
            {
                L2dsPrintContextInfo (CliHandle, i4ContextId);
            }
            /* Calling the function to get the Structure information   *
             * about the particular Vlan                               */
            L2dsCliPrintVlanInformation (CliHandle, i4L2dsVlanId,
                                         &u4PagingStatus);
        }

        if ((u4PagingStatus == CLI_FAILURE))
        {
            /* User pressed 'q' at more prompt, *
             * no more print required, exit *
             */
            break;
        }
        /* Releasing the Context */
        L2dsReleaseContext ();
        i4CurrCxtId = i4ContextId;
        i4CurrVlanId = i4L2dsVlanId;
    }
    /* Getting the Next Entry From the Table */
    while (nmhGetNextIndexFsMIDhcpSnpInterfaceTable
           (i4CurrCxtId, &i4ContextId, i4CurrVlanId, &i4L2dsVlanId)
           == SNMP_SUCCESS);

    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsPrintContextInfo                                 */
/*                                                                           */
/* Description        : This function displays Cotext  Information           */
/*                                                                           */
/* Input(s)           : CliHandle     -  CLIHandler                          */
/*                      i4ContextId   -  Identifier                          */
/*                                                                           */
/* Output(s)          : NONE                                                 */
/*                                                                           */
/* Return Value(s)    : NONE                                                 */
/*****************************************************************************/
VOID
L2dsPrintContextInfo (tCliHandle CliHandle, INT4 i4ContextId)
{
    UINT1               au1ContextName[VCM_ALIAS_MAX_LEN] = { L2DS_ZERO };
    /*To get the Context-Name with the Context-Id */
    VcmGetAliasName (i4ContextId, au1ContextName);
    CliPrintf (CliHandle, "Switch : %s \r\n", au1ContextName);
    CliPrintf (CliHandle, "-------  -------\r\n");

}

/*****************************************************************************/
/* Function Name      :  L2dsCliPrintVlanInformation                         */
/*                                                                           */
/* Description        :  This function shows                                 */
/*                       DHCP Snooping Vlan Information                      */
/*                                                                           */
/* Input(s)           :  CliHandle   -  CLIHandler                           */
/*                       i4VlanId    -  VlanId                               */
/*                                                                           */
/* Output(s)          :  pu4PagingStatus - Paging status                     */
/*                                                                           */
/* Return Value(s)    :  CLI_FAILURE/CLI SUCCESS                             */
/*****************************************************************************/
INT4
L2dsCliPrintVlanInformation (tCliHandle CliHandle, INT4 i4VlanId,
                             UINT4 *pu4PagingStatus)
{
    UINT4               u4L2dsSnpRxDiscovers = L2DS_ZERO;
    UINT4               u4L2dsSnpRxRequests = L2DS_ZERO;
    UINT4               u4L2dsSnpRxReleases = L2DS_ZERO;
    UINT4               u4L2dsSnpRxDeclines = L2DS_ZERO;
    UINT4               u4L2dsSnpRxInforms = L2DS_ZERO;
    UINT4               u4L2dsSnpTxOffers = L2DS_ZERO;
    UINT4               u4L2dsSnpTxAcks = L2DS_ZERO;
    UINT4               u4L2dsSnpTxNaks = L2DS_ZERO;
    UINT4               u4L2dsSnpNoOfDiscards = L2DS_ZERO;
    UINT4               u4L2dsSnpMacDiscards = L2DS_ZERO;
    UINT4               u4L2dsSnpServerDiscards = L2DS_ZERO;
    UINT4               u4L2dsSnpOptionDiscards = L2DS_ZERO;
    INT4                i4L2dsSnpVlanSnpStatus = L2DS_ZERO;

    if (nmhGetFsDhcpSnpVlanSnpStatus (i4VlanId, &i4L2dsSnpVlanSnpStatus) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (i4L2dsSnpVlanSnpStatus != L2DS_ENABLED)
    {
        return CLI_SUCCESS;
    }

    if (nmhGetFsDhcpSnpRxDiscovers (i4VlanId, &u4L2dsSnpRxDiscovers) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpRxRequests (i4VlanId, &u4L2dsSnpRxRequests) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpRxReleases (i4VlanId, &u4L2dsSnpRxReleases) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpRxDeclines (i4VlanId, &u4L2dsSnpRxDeclines) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpRxInforms (i4VlanId, &u4L2dsSnpRxInforms) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpTxOffers (i4VlanId, &u4L2dsSnpTxOffers) == SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpTxAcks (i4VlanId, &u4L2dsSnpTxAcks) == SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpTxNaks (i4VlanId, &u4L2dsSnpTxNaks) == SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpNoOfDiscards (i4VlanId, &u4L2dsSnpNoOfDiscards) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpMacDiscards (i4VlanId, &u4L2dsSnpMacDiscards) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpServerDiscards (i4VlanId, &u4L2dsSnpServerDiscards) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    if (nmhGetFsDhcpSnpOptionDiscards (i4VlanId, &u4L2dsSnpOptionDiscards) ==
        SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }
    CliPrintf (CliHandle, "VLAN                            : %ld\r\n",
               i4VlanId);

    CliPrintf (CliHandle, "Snooping status                 : Enabled\r\n");

    CliPrintf (CliHandle, "Number of Incoming Discovers    : %ld\r\n",
               u4L2dsSnpRxDiscovers);

    CliPrintf (CliHandle, "Number of Incoming Requests     : %ld\r\n",
               u4L2dsSnpRxRequests);

    CliPrintf (CliHandle, "Number of Incoming Releases     : %ld\r\n",
               u4L2dsSnpRxReleases);

    CliPrintf (CliHandle, "Number of Incoming Declines     : %ld\r\n",
               u4L2dsSnpRxDeclines);

    CliPrintf (CliHandle, "Number of Incoming Informs      : %ld\r\n",
               u4L2dsSnpRxInforms);

    CliPrintf (CliHandle, "Number of Transmitted Offers    : %ld\r\n",
               u4L2dsSnpTxOffers);

    CliPrintf (CliHandle, "Number of Transmitted Acks      : %ld\r\n",
               u4L2dsSnpTxAcks);

    CliPrintf (CliHandle, "Number of Transmitted Naks      : %ld\r\n",
               u4L2dsSnpTxNaks);

    CliPrintf (CliHandle, "Total Number Of Discards        : %ld\r\n",
               u4L2dsSnpNoOfDiscards);

    CliPrintf (CliHandle, "Number of MAC Discards          : %ld\n",
               u4L2dsSnpMacDiscards);

    CliPrintf (CliHandle, "Number of Server Discards       : %ld\n",
               u4L2dsSnpServerDiscards);

    CliPrintf (CliHandle, "Number of Option Discards       : %ld\n",
               u4L2dsSnpOptionDiscards);

    *pu4PagingStatus = CliPrintf (CliHandle, "\r\n");

    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      :  L2dsShowRunningConfig                               */
/*                                                                           */
/* Description        :  This function displays the                          */
/*                       DHCP Snooping Configurations                        */
/*                                                                           */
/* Input(s)           :  CliHandle - Handle to the CLI Context               */
/*                                                                           */
/* Output(s)          :  None                                                */
/*                                                                           */
/* Return Value(s)    :  CLI_FAILURE/CLI SUCCESS                             */
/*****************************************************************************/
INT4
L2dsShowRunningConfig (tCliHandle CliHandle)
{
    INT4                i4Status = L2DS_ZERO;

    CliRegisterLock (CliHandle, L2DS_LOCK, L2DS_UNLOCK);
    L2DS_LOCK ();

    nmhGetFsDhcpSnpSnoopingAdminStatus (&i4Status);

    if (i4Status == L2DS_ENABLED)
    {
        CliPrintf (CliHandle, "ip dhcp snooping\r\n");
    }
    CliPrintf (CliHandle, "!\r\n");

    nmhGetFsDhcpSnpMacVerifyStatus (&i4Status);

    if (i4Status == L2DS_DISABLED)
    {
        CliPrintf (CliHandle, "no ip dhcp snooping verify mac-address\r\n");
    }
    CliPrintf (CliHandle, "!\r\n");

    L2dsShowRunningConfigGetId (CliHandle);

    L2DS_UNLOCK ();
    CliUnRegisterLock (CliHandle);

    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      :  L2dsShowRunningConfigGetId                          */
/*                                                                           */
/* Description        :  This function displays current configuration        */
/*                                                                           */
/* Input(s)           :  CliHandle  - Handle to the  CLI context             */
/*                                                                           */
/* Output(s)          :  None                                                */
/*                                                                           */
/* Return Value(s)    :  CLI_FAILURE/CLI SUCCESS                             */
/*****************************************************************************/
INT4
L2dsShowRunningConfigGetId (tCliHandle CliHandle)
{
    INT4                i4VlanId = L2DS_ZERO;
    INT4                i4CurrVlanId = L2DS_ZERO;
    UINT4               u4PagingStatus = CLI_SUCCESS;

    if (nmhGetFirstIndexFsDhcpSnpInterfaceTable (&i4VlanId) == SNMP_FAILURE)
    {
        return CLI_FAILURE;
    }

    do
    {
        if (L2dsShowRunningConfigDetails (CliHandle, i4VlanId) == CLI_SUCCESS)
        {
            u4PagingStatus = CliPrintf (CliHandle, "!\r\n");

            if ((u4PagingStatus == CLI_FAILURE))
            {
                /* User pressed 'q' at more prompt, *
                 * no more print required, exit *
                 */
                break;
            }
        }

        i4CurrVlanId = i4VlanId;
        /* Getting the Next Entry From the Table */
    }
    while (nmhGetNextIndexFsDhcpSnpInterfaceTable (i4CurrVlanId,
                                                   &i4VlanId) == SNMP_SUCCESS);

    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      :  L2dsShowRunningConfigDetails                        */
/*                                                                           */
/* Description        :  This function displays current Vlan Specific        */
/*                       Configurations in DHCP Snooping                     */
/*                                                                           */
/* Input(s)           :  CliHandle   -  CLIHandler                           */
/*                       i4VlanId    -  VlanId                               */
/*                                                                           */
/* Output(s)          :  None                                                */
/*                                                                           */
/* Return Value(s)    :  CLI_SUCCESS/CLI_FAILURE                             */
/*****************************************************************************/
INT4
L2dsShowRunningConfigDetails (tCliHandle CliHandle, INT4 i4VlanId)
{
    INT4                i4Status = L2DS_ZERO;

    if (nmhGetFsDhcpSnpVlanSnpStatus (i4VlanId, &i4Status) != SNMP_SUCCESS)
    {
        return CLI_FAILURE;
    }

    if (i4Status == L2DS_ENABLED)
    {

        CliPrintf (CliHandle, "vlan %d \r\n", i4VlanId);
        CliPrintf (CliHandle, "ip dhcp snooping \r\n");
    }
    return CLI_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsCliSetTrace                                      */
/*                                                                           */
/* Description        : This function This function will enable/disable trace*/
/*                                                                           */
/* Input(s)           : i4TraceValue- Trace Flag                             */
/*                                                                           */
/* Output(s)          : Update the Global Structure                          */
/*                                                                           */
/* Return Value(s)    : CLI_FAILURE/CLI_SUCCESS                              */
/*****************************************************************************/
INT4
L2dsCliSetTrace (INT4 i4TraceValue)
{
    L2DS_TRC_FLAG = (UINT4) i4TraceValue;
    return CLI_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/*    Function Name       : L2DSCliSelectContextOnMode                       */
/*                                                                           */
/*    Description         : This function is used to check the Mode of       */
/*                          the command and also if it a Config Mode         */
/*                          command it will do SelectContext for the         */
/*                          Context.                                         */
/*                                                                           */
/*    Input(s)            : u4Cmd - CLI Command.                             */
/*                                                                           */
/*    Output(s)           : CliHandle - Contains error messages.             */
/*                          pu4ContextId - Context Id.                       */
/*                                                                           */
/*    Returns            : L2DS_SUCCESS/L2DS_FAILURE                         */
/*                                                                           */
/*****************************************************************************/
PUBLIC INT4
L2DSCliSelectContextOnMode (tCliHandle CliHandle, UINT4 u4Cmd,
                            UINT4 *pu4Context)
{
    UINT4               u4ContextId = L2DS_ZERO;

    /* For debug commands the context-name will be not be applicable as
     * they are global configuration and per context based configuration */
    if ((u4Cmd == L2DS_GLOBAL_SHOW) || (u4Cmd == L2DS_VLAN_SHOW) ||
        (u4Cmd == L2DS_TRACE_ENABLE) || (u4Cmd == L2DS_TRACE_DISABLE))
    {
        return L2DS_SUCCESS;
    }

    *pu4Context = L2DS_DEFAULT_CXT_ID;

    /* Get the Context-Id from the pCliContext structure */
    u4ContextId = CLI_GET_CXT_ID ();

    /* Check if the command is a switch mode command */
    if (u4ContextId != L2DS_INVALID_CXT_ID)
    {
        /* Switch-mode Command */
        *pu4Context = u4ContextId;
    }

    if (L2dsSelectContext (u4ContextId) == L2DS_FAILURE)
    {
        CliPrintf (CliHandle, "\r%% Invalid Virtual Switch.\r\n ");
        return L2DS_FAILURE;
    }

    return L2DS_SUCCESS;
}

/*                                                                           */
/***************************** END OF FILE ***********************************/
/*                                                                           */
