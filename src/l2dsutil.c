/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsutil.c,v 1.16 2013/02/09 08:21:05 siva Exp $       */
/*****************************************************************************/
/*    FILE  NAME            : l2dsutil.c                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snooping                                */
/*    MODULE NAME           : L2DHCP Snooping Util Module                    */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains the utility funtions        */
/*                            for L2DHCP Snooping module                     */
/*---------------------------------------------------------------------------*/

#include "l2dsinc.h"
#include "l2dsextn.h"

/*****************************************************************************/
/* Function Name      : L2dsUtilGetOption                                    */
/*                                                                           */
/* Description        : This routine is to get the specified option from     */
/*                      the DHCP Packet                                      */
/*                                                                           */
/* Input(s)           : pL2DS- Incoming DHCP packet               */
/*                      u1OptCode - Option to be obtained                 */
/*                                                                           */
/* Output(s)          : pL2DSOpt - Option                            */
/*                                                                           */
/* Return Value(s)    : L2DS_FOUND/L2DS_NOT_FOUND                            */
/*****************************************************************************/
INT4
L2dsUtilGetOption (tL2DhcpSnpPacket * pL2DS, tL2DhcpSnpOption * pL2DSOpt, UINT1 u1OptCode)
{
    UINT1              *pu1Options = NULL;
    UINT2               u2OptLen = L2DS_ZERO;
    UINT2               u2Offset = L2DS_ZERO;
    UINT1               u1Type = L2DS_ZERO;
    UINT1               u1Len = L2DS_ZERO;
    UINT1               u1OptOverLoad = L2DS_ZERO;

    L2DS_TRC(L2DS_FN_ENTRY, "[L2dsUtilGetOption:Begin]\r\n");
    L2DS_TRC_ARG1(L2DS_FN_ARGS, "[L2dsUtilGetOption]"
		"Arguments: option code=%d\r\n", (INT4)u1OptCode);

    MEMSET (pL2DSOpt, L2DS_ZERO, sizeof (tL2DhcpSnpOption));
	MEMSET (gau1OptionVal, L2DS_ZERO, L2DS_OPTION_LEN);
	
    u2OptLen = pL2DS->u2OptLen;
    pu1Options = pL2DS->au1Options;

    /* Loop through the option */
    while (u2OptLen > u2Offset)
    {
        u1Type = pu1Options[u2Offset];
        u1Len = pu1Options[u2Offset + L2DS_ONE];

        if (u1Type == u1OptCode)
        {
            /* If the particular option is found, return the value */
            pL2DSOpt->u1Type = u1Type;
            pL2DSOpt->u1Len = u1Len;

            pL2DSOpt->pu1Val = (UINT1 *) &gau1OptionVal[L2DS_ZERO];

            if (pL2DSOpt->pu1Val != NULL)
            {
                MEMSET (pL2DSOpt->pu1Val, L2DS_ZERO, u1Len);

                MEMCPY (pL2DSOpt->pu1Val,
                        (pu1Options + u2Offset + L2DS_OPTION_VAL_OFFSET),
                        u1Len);

                L2DS_TRC_ARG1(L2DS_FN_EXIT, "[L2dsUtilGetOption]"
					"Exit after finding the option %d \r\n", (INT4)u1OptCode);
                return L2DS_FOUND;
            }
            else
            {
                break;
            }
        }
        else if (u1Type == L2DS_OPT_PAD)
        {
            /* If the option is PAD option(It does not have type and value), 
             * continue */
            u2Offset++;
            continue;
        }
        else if (u1Type == L2DS_OPT_END)
        {
            /* If the option is END option, break the loop */
            break;
        }
        else if (u1Type == L2DS_OPT_OVERLOAD)
        {
            /* If option overloading is there, SNAME and FILE fields will be 
             * used for carrying the options */
            u1OptOverLoad = pu1Options[u2Offset + L2DS_OPTION_VAL_OFFSET];
        }

        u2Offset = (UINT2) (u2Offset + u1Len + L2DS_OPTION_VAL_OFFSET);
    }

    /* If Option overload value is 1, FILE field is used for carrying options
     * If Option overload value is 2, SNAME field is used for carrying options
     * If Option overload value is 3, both FILE and SNAME fields are used for 
     * carrying options */
    if ((u1OptOverLoad == L2DS_ZERO) || (u1OptOverLoad > L2DS_THREE))
    {
        L2DS_TRC_ARG1 (L2DS_FN_EXIT, "[L2dsUtilGetOption]"
			"Exit after failing to find the option %d\r\n", (INT4)u1OptCode);
        return L2DS_NOT_FOUND;
    }

    if (u1OptOverLoad == L2DS_ONE)
    {
        u2OptLen = L2DS_FILE_LEN;
        pu1Options = pL2DS->u1File;
    }
    else if (u1OptOverLoad == L2DS_TWO)
    {
        u2OptLen = L2DS_SNAME_LEN;
        pu1Options = pL2DS->u1Sname;
    }
    else
    {
        u2OptLen = L2DS_FILE_LEN + L2DS_SNAME_LEN;
        pu1Options = pL2DS->u1Sname;
    }

    u2Offset = L2DS_ZERO;

    while (u2OptLen > u2Offset)
    {
        u1Type = pu1Options[u2Offset];
        u1Len = pu1Options[u2Offset + L2DS_ONE];

        if (u1Type == u1OptCode)
        {
            pL2DSOpt->u1Type = u1OptCode;
            pL2DSOpt->u1Len = u1Len;

            pL2DSOpt->pu1Val = (UINT1 *) &gau1OptionVal[L2DS_ZERO];

            if (pL2DSOpt->pu1Val != NULL)
            {
                MEMSET (pL2DSOpt->pu1Val, L2DS_ZERO, u1Len);

                MEMCPY (pL2DSOpt->pu1Val,
                        (pu1Options + u2Offset + L2DS_OPTION_VAL_OFFSET),
                        u1Len);

                L2DS_TRC_ARG1 (L2DS_FN_EXIT, "[L2dsUtilGetOption]"
					"Exit after finding the option %d\r\n", (INT4)u1OptCode);

                return L2DS_FOUND;
            }
            else
            {
                break;
            }
        }
        else if (u1Type == L2DS_OPT_PAD)
        {
            u2Offset++;
            continue;
        }
        else if (u1Type == L2DS_OPT_END)
        {
            /* If option overload value is 3, and the options in SNAME field 
             * have come to an end, scan through the FILE field */
            if (u1OptOverLoad == L2DS_THREE)
            {
                u2Offset = L2DS_ZERO;
                if (u2OptLen == L2DS_FILE_LEN)
                {
                    break;
                }
                u2OptLen = L2DS_FILE_LEN;
                pu1Options = pL2DS->u1File;
                continue;
            }
            else
            {
                break;
            }
        }
        u2Offset = (UINT2) (u2Offset + u1Len + L2DS_OPTION_VAL_OFFSET);
    }

    L2DS_TRC(L2DS_FN_EXIT, "[L2dsUtilGetOption:End]\r\n");
    return L2DS_NOT_FOUND;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilGetDslOptions                                */
/*                                                                           */
/* Description        : This routine is to get the DSL options for the       */
/*                      DHCP Packet                                          */
/*                                                                           */
/* Input(s)           : u2Port - Incoming Port                               */
/*                                                                           */
/* Output(s)          : pu1VndrOption - DSL Options                          */
/*                      pu1VndrLen - Length of DSL Option                    */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsUtilGetDslOptions (UINT1 *pu1VndrOption, UINT1 *pu1VndrLen, UINT4 u4Port,
                       tVlanTag VlanTag)
{
    UINT1               au1DslOptions[L2DS_DSL_OPTION_LEN] = { L2DS_ZERO };
    UINT1               u1DslLen = L2DS_ZERO;
    /* Get the DSL Sub-Options to be added */
    if (L2dsPortGetDslOptions (au1DslOptions, &u1DslLen, u4Port,
                               VlanTag) != L2DS_SUCCESS)
    {
        return L2DS_FAILURE;
    }

    /* Copy the Type and length of vendor specific option */
    pu1VndrOption[L2DS_ZERO] = L2DS_OPT_VENDOR_SPEC;
    pu1VndrOption[L2DS_ONE] = u1DslLen;

    /* Copy DSL Sub-option to vendor specific option */
    MEMCPY (pu1VndrOption + L2DS_TWO, au1DslOptions, u1DslLen);

    *pu1VndrLen = (UINT1) (u1DslLen + L2DS_TWO);
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilAddOptions                                   */
/*                                                                           */
/* Description        : This routine is to add option-82 and DSL options     */
/*                      to DHCP Packet                                       */
/*                                                                           */
/* Input(s)           : pL2DS - DHCP packet                      */
/*                                                                           */
/* Output(s)          : pu2AddLen - No of additional bytes              */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsUtilAddOptions (tL2DhcpSnpPktInfo * pL2DS, UINT2 *pu2AddLen)
{
    UINT1              *pu1Options = NULL;
    UINT1               au1CircuitId[DCS_CIRCUIT_ID_LEN] = { L2DS_ZERO };
    UINT1               au1RemoteId[DCS_REMOTE_ID_LEN + L2DS_ONE] = { L2DS_ZERO };
    UINT1               au1VndrOptions[L2DS_VENDOR_OPTION_LEN] = { L2DS_ZERO };
    UINT2               u2TempOffset = L2DS_ZERO;
    UINT1               u1CircuitIDLen = L2DS_ZERO;
    UINT1               u1RemoteIDLen = L2DS_ZERO;
    UINT1               u1VndrLen = L2DS_ZERO;
    UINT1               u1IfType = L2DS_ZERO;
    tVlanId             VlanId = L2DS_ZERO;
    UINT1               u1Status = L2DS_ZERO;
    UINT2               u2OptLen = L2DS_ZERO;
    UINT2               u2Offset = L2DS_ZERO;
    UINT1               u1Type = L2DS_ZERO;
    UINT1               u1Len = L2DS_ZERO;

    u2OptLen = pL2DS->L2DSPkt.u2OptLen;

    pu1Options = pL2DS->L2DSPkt.au1Options;
    VlanId = pL2DS->VlanTag.InnerVlanTag.u2VlanId;

    if (VlanId == L2DS_ZERO)
    {
		/* Failed to get the C-VLAN ID from the packet. 
		* So get the default PVID configured for the port 
		* TBD: This function is to be added 
		*/
        if(L2DS_FAILURE == L2dsPortGetDefaultCTag(
			pL2DS->u2InPort, &VlanId))
        {
            return L2DS_FAILURE;
        }
    }

    /* Parse till the end of options */
    while (u2OptLen > u2Offset)
    {
        u1Type = pu1Options[u2Offset];

        if (u1Type == L2DS_OPT_PAD)
        {
            u2Offset++;
            continue;
        }
        else if (u1Type == L2DS_OPT_END)
        {
            break;
        }
        u1Len = pu1Options[u2Offset + L2DS_ONE];
        u2Offset = (UINT2) (u2Offset + u1Len + L2DS_OPTION_VAL_OFFSET);
    }
    /* Now the u2Offset will point to the type of end option.
     * The end option will be overwritten, and will again be 
     * added at the end */

    pu1Options[u2Offset] = L2DS_OPT_RAI;
    u2Offset++;

    /* Fill the length of the option later,after getting 
     * circuitID and RemoteID */
    u2TempOffset = u2Offset;

    /*Moving the offset to point to circuit id */
    u2Offset++;

    /* calling a function to get circuit id string */
    if(DCS_FAILURE == DcsUtilGetFreeStringACI(pL2DS->u2InPort, 
		au1CircuitId, &u1CircuitIDLen))
    {
        L2DS_TRC(L2DS_FAIL_TRC, "[L2dsUtilAddOptions]"
			"Get Free String ACI failed.\r\n");

        return L2DS_FAILURE;
    }

    /* If length is equal to zero then we are going to add the automatic
     * generated string to the Agent circuit Identifier string as 
     * circuit id (sub-option 1(0x01))in incoming DHCP packets.*/
    if (u1CircuitIDLen == L2DS_ZERO)
    {
    	/*CAMEOTAG: Added by zhaopan on 2015/11/26. Default Circuit ID getting about option82 information*/
#ifdef CAMEO_DSP_DEFAULT_OPTION82_FORMAT_WANTED
		if (DCS_FAILURE == CmDSPOption82DefCircuitIdGet(pL2DS->u2InPort, VlanId, 
														au1CircuitId, &u1CircuitIDLen))
#else
        if (DCS_FAILURE == DcsUtilGetCircuitIdString(pL2DS->u2InPort, VlanId, 
													 au1CircuitId, &u1CircuitIDLen))
#endif
        {
            L2DS_TRC(L2DS_FAIL_TRC, "[L2dsUtilAddOptions]"
				"Get circuit id failed.\r\n");
            return L2DS_FAILURE;
        }
    }

    /* Calling a function to get Remote Agent ID Status. */
    if(DCS_SUCCESS == DcsGetRemoteAgentIdStatus(
		pL2DS->u2InPort, &u1Status))
    {
        /* If the status is enabled then we are going to add configured Remote"
           Agent Identifier srting as a remote id (sub-option 2 (0x02) in "
           "incoming DHCP packets. */
        if (u1Status == DCS_ENABLE)
        {
            /* calling a function to get Remote id String */
			/*CAMEOTAG: Added by zhaopan on 2015/11/26. Default Remote ID getting about option82 information*/
#ifdef CAMEO_DSP_DEFAULT_OPTION82_FORMAT_WANTED
			if (DCS_FAILURE == CmDSPOption82DefRemoteIdGet(au1RemoteId, &u1RemoteIDLen))
#else
            if (DCS_FAILURE == DcsUtilGetRemoteIdString(au1RemoteId, &u1RemoteIDLen))
#endif
            {
                 L2DS_TRC(L2DS_FAIL_TRC, "[L2dsUtilAddOptions]"
					"Get Remote ID failed!\r\n");
                return L2DS_FAILURE;
            }
        }
    }

    /* Addition of options should not make the packet size greater than MTU */
    if (u2OptLen + u1CircuitIDLen + u1RemoteIDLen + L2DS_OPTION_VAL_OFFSET >
        L2DS_OPTION_LEN)
    {
        return L2DS_SUCCESS;
    }

    /* Appending the circuit Id before L2DS_OPT_PAD */
    /* u2Offset is pointing after option type and option length */
    /* Circuit Id consists of suboption, total circuitId length and
     * access node string */

    MEMCPY ((pu1Options + u2Offset), au1CircuitId, (u1CircuitIDLen));

    /* Moving the offset to point to end of circuit id */
    u2Offset = (UINT2) (u2Offset + u1CircuitIDLen);

    /* Appending the remote Id after circuit Id and before L2DS_OPT_PAD */
    MEMCPY ((pu1Options + u2Offset), au1RemoteId, u1RemoteIDLen);
    u2Offset = (UINT2) (u2Offset + u1RemoteIDLen);

    /*Appending the length of option-82, that is (circuit id + remoteid) */
    pu1Options[u2TempOffset] = (UINT1) (u1RemoteIDLen + u1CircuitIDLen);
    pu1Options[u2Offset] = L2DS_OPT_END;

    /* total length of option-82 (Type + Length + CircuitId + Remote Id) */
    *pu2AddLen =(UINT2)(u1CircuitIDLen + u1RemoteIDLen + L2DS_OPTION_VAL_OFFSET);

    /* update the Length in DHCP Packet */
    pL2DS->L2DSPkt.u2OptLen += u1CircuitIDLen + 
    	u1RemoteIDLen + L2DS_OPTION_VAL_OFFSET;

    /* Assumption: Option 43 with vendor enterprise number as 
     * DSL forum enterprise number will not be generated by any interface 
     * for which IP address needs to be allocated from DHCP server. 
     *
     * So there is no need to check whether option 43 is present 
     * there already in the packet from host. 
     *
     * Note: There is a possibility of having many option 43 in the
     * same DHCP discover/request packet from host with different
     * vendor specific number. 
     * */

    /* Get the type of the incoming port.If the port-type is DSL, we will add 
     * option-82 which contains agent circuit identifier and agent remote 
     * identifier and DSL line characteristics. */

    /* If the port-type is Ethernet, we will add only option-82 which contains 
     * agent circuit identifier and agent remote identifier */
    CfaGetIfaceType(pL2DS->u2InPort, &u1IfType);
    if((u1IfType == CFA_XDSL2) || (u1IfType == CFA_PVC))
    {
        /* Calling a function to get Access Loop Characteristics Status. */
        if(DCS_SUCCESS == DcsGetAccessLoopStatus (
			pL2DS->u2InPort, &u1Status))
        {
            /* If the status is enabled then we are going to add configured 
             * Access Loop Characteristics in incoming DHCP packets. */
            if (u1Status == DCS_ENABLE)
            {
                /* Get the DSL Options */
                if(L2DS_SUCCESS == L2dsUtilGetDslOptions(au1VndrOptions, 
					&u1VndrLen, pL2DS->u2InPort,pL2DS->VlanTag))
                {
                    if ((u2OptLen + u1CircuitIDLen + u1RemoteIDLen +
                         L2DS_OPTION_VAL_OFFSET + u1VndrLen) < L2DS_OPTION_LEN)
                    {
                        /* If the packetsize does not exceed maximum MTU, copy 
                         * the DSL Options to the existing options */
                        MEMCPY((pu1Options + u2Offset), au1VndrOptions, u1VndrLen);
                        u2Offset = (UINT2)(u2Offset + u1VndrLen);
                        pu1Options[u2Offset] = L2DS_OPT_END;
                        *pu2AddLen += u1VndrLen;
                        pL2DS->L2DSPkt.u2OptLen += u1VndrLen;
                        return L2DS_SUCCESS;
                    }
                    else
                    {
						 L2DS_TRC(L2DS_FAIL_TRC, "[L2dsUtilAddOptions]"
							"Packet size exceeds the maximum MTU!\r\n");
                    }
                }
            }
            else
            {
            	L2DS_TRC(L2DS_FAIL_TRC, "[L2dsUtilAddOptions]"
					"Access Loop Characteristics status is not enabled !\r\n");
            }
        }
        else
        {
        	L2DS_TRC(L2DS_FAIL_TRC, "[L2dsUtilAddOptions]"
				"Unable to get the Access Loop Characteristics status.!\r\n");
        }
    }
    else
    {
        pu1Options[u2Offset] = L2DS_OPT_END;
    }

    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilRemoveOptions                                */
/*                                                                           */
/* Description        : This routine is to remove the option-82 and DSL      */
/*                      options from DHCP Packet                             */
/*                                                                           */
/* Input(s)           : pL2DhcpSnpPktInfo - DHCP Packet                      */
/*                                                                           */
/* Output(s)          : pu1RemovedLen - No. of byte removed                  */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsUtilRemoveOptions (tL2DhcpSnpPktInfo * pL2DS,
                       UINT1 *pu1RemovedLen)
{
    UINT1              *pu1Options = NULL;
    UINT2               u2TempOffset = L2DS_ZERO;
    UINT2               u2OptLen = L2DS_ZERO;
    UINT2               u2Offset = L2DS_ZERO;
    UINT1               u1Type = L2DS_ZERO;
    UINT1               u1Len = L2DS_ZERO;
    UINT1               u1VndrLen = L2DS_ZERO;

    L2DS_TRC(L2DS_FN_ENTRY, "[L2dsUtilRemoveOptions:Begin]\r\n");

    u2OptLen = (pL2DS->L2DSPkt).u2OptLen;
    pu1Options = (pL2DS->L2DSPkt).au1Options;

    *pu1RemovedLen = L2DS_ZERO;

    while (u2OptLen > u2Offset)
    {
        u1Type = pu1Options[u2Offset];

        if (u1Type == L2DS_OPT_PAD)
        {
            u2Offset++;
            continue;
        }
        else if (u1Type == L2DS_OPT_RAI)
        {
            /* u2TempOffset is having the length of circuit id + remote id */
            u2TempOffset = pu1Options[u2Offset + L2DS_ONE];

            /* Remove the Relay Agent information option */
            MEMCPY ((pu1Options + u2Offset),
                    (pu1Options + u2Offset + u2TempOffset +
                     L2DS_OPTION_VAL_OFFSET),
                    (u2OptLen - (u2Offset + u2TempOffset +
                                 L2DS_OPTION_VAL_OFFSET)));

            MEMSET ((pu1Options + u2OptLen - (u2TempOffset +
                                              L2DS_OPTION_VAL_OFFSET)),
                    L2DS_ZERO, u2TempOffset + L2DS_OPTION_VAL_OFFSET);

            /* updating the removed length in the packet */
            (pL2DS->L2DSPkt).u2OptLen -= u2TempOffset + L2DS_OPTION_VAL_OFFSET;

            /* pu1RemovedLen is total length of option-82 */
            *pu1RemovedLen = (UINT1) (*pu1RemovedLen + u2TempOffset +
                                      L2DS_OPTION_VAL_OFFSET);

            /* there is no change in the value of u2Offset; but the u2OptLen 
             * will be decreased by L2DS_OPTION_RAI_LEN */
            u2OptLen = (UINT2)
                (u2OptLen - u2TempOffset + L2DS_OPTION_VAL_OFFSET);
            continue;
        }
        else if (u1Type == L2DS_OPT_VENDOR_SPEC)
        {
            /* TBD: Now we are removing the Vendor specific options 
             * blindily. Actually we should remove only DSL sub-option */
            if (pu1Options[u2Offset + L2DS_TWO] == L2DS_SUBOPT_DSL)
            {
                /* The vendor specific option contains DSL Suboption 
                 * So we can safely assume that this option was added by us */

                /* Get DSL SUB-Option length */
                u1Len = pu1Options[u2Offset + L2DS_ONE];

                /* Total  Vendor specific options length */
                u1VndrLen = (UINT1) (u1Len + L2DS_TWO);

                /* Remove the Vendor specific options */
                MEMCPY ((pu1Options + u2Offset),
                        (pu1Options + u2Offset + u1VndrLen),
                        (u2OptLen - (u2Offset + u1VndrLen)));

                MEMSET ((pu1Options + u2OptLen - u1VndrLen), L2DS_ZERO,
                        u1VndrLen);

                (pL2DS->L2DSPkt).u2OptLen -= u1VndrLen;

                *pu1RemovedLen = (UINT1) (*pu1RemovedLen + u1VndrLen);

                /* there is no change in the value of u2Offset; but the u2OptLen
                 * will be decreased by u1VndrLen */
                u2OptLen = (UINT2) (u2OptLen - u1VndrLen);
                continue;
            }
        }
        else if (u1Type == L2DS_OPT_END)
        {
            return L2DS_SUCCESS;
        }

        u1Len = pu1Options[u2Offset + L2DS_ONE];
        u2Offset = (UINT2) (u2Offset + u1Len + L2DS_OPTION_VAL_OFFSET);
    }

    L2DS_TRC(L2DS_FN_EXIT, "[L2dsUtilRemoveOptions:End]\r\n");
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilGetPortAndVlanFromOption                     */
/*                                                                           */
/* Description        : This is to get the out-port and C-Vlan from the      */
/*                      CIRCUIT-ID sub-option                                */
/*                                                                           */
/* Input(s)           : pL2DSOpt - Option-82                         */
/*                                                                           */
/* Output(s)          : pu4OutPort - Out port                                */
/*                    : pVlanId    - Vlan Identifier                         */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsUtilGetPortAndVlanFromOption (tL2DhcpSnpOption * pL2DhcpSnpOption,
                                  UINT4 *pu4OutPort, tVlanId * pVlanId)
{
    UINT1               au1CircuitID[DCS_CIRCUIT_ID_LEN + L2DS_ONE]
        = { L2DS_ZERO };
    UINT1               au1Name[L2DS_CIRCUIT_MAX_LEN] = { L2DS_ZERO };
    UINT1              *pu1SecondTmpStr = NULL;
    UINT1              *pu1Options = NULL;
    UINT1              *pu1IfType = NULL;
    UINT1              *pu1Port = NULL;
    UINT1              *pu1Slot = NULL;
    UINT1              *pu1Vpi = NULL;
    UINT1              *pu1Vci = NULL;
    UINT1              *pu1Name = NULL;
    UINT1              *pi1VlanId = NULL;
    UINT4               u4DslIfIndex = L2DS_ZERO;
    UINT4               u4IfIndex = L2DS_ZERO;
    UINT4               u4OutPort = L2DS_ZERO;
    UINT1               u1OptLen = L2DS_ZERO;
    UINT1               u1Offset = L2DS_ZERO;
    UINT1               u1Type = L2DS_ZERO;
    UINT1               u1SubType = L2DS_ZERO;
    UINT1               u1SubTypeLen = L2DS_ZERO;
    INT4                i4Vci = L2DS_ZERO;
    INT4                i4Vpi = L2DS_ZERO;
    INT4                i4Slot = L2DS_ZERO;
    INT4                i4Port = L2DS_ZERO;
    tVlanId             VlanId = L2DS_ZERO;

    L2DS_TRC(L2DS_FN_ENTRY, "[L2dsUtilGetPortAndVlanFromOption:Begin]\r\n");

    pu1Name = au1Name;

    pu1Options = pL2DhcpSnpOption->pu1Val;

    u1Type = pu1Options[L2DS_ZERO];

    if (pL2DhcpSnpOption->u1Type != L2DS_OPT_RAI)
    {
        /* The passed option is not option-82 */
        return L2DS_FAILURE;
    }

    /* Get the option length */
    u1OptLen = pL2DhcpSnpOption->u1Len;

    while (u1OptLen > u1Offset)
    {
        u1SubType = pu1Options[u1Offset++];
        u1SubTypeLen = pu1Options[u1Offset++];

        if (u1SubType != L2DS_CIRCUIT_SUBOPT)
        {
            /* if the sub-option is not Circuit-ID, get the next sub-option */
            u1Offset = (UINT1) (u1Offset + u1SubTypeLen);
            continue;
        }
        else
        {
            break;
        }
    }

    /* Agent circuit id inserted by Access node must not 
       exceed 63 characters  */
    if (u1SubTypeLen > DCS_CIRCUIT_ID_LEN)
    {
        return L2DS_FAILURE;
    }

    /* offset is pointing to circuit id 
     * string(access-node-id atm slot/port:vpi.vci). 
     * au1CircuitID is having this string.*/
    MEMCPY (au1CircuitID, (pu1Options + u1Offset), u1SubTypeLen);

    au1CircuitID[u1SubTypeLen] = '\0';

    /* pu1SecondTmpStr is having access-node-id */
    pu1SecondTmpStr = (UINT1 *) STRTOK (au1CircuitID, " ");

    /* pu1IfType is having atm/eth */
    pu1IfType = (UINT1 *) STRTOK (NULL, " ");
    if (pu1IfType == L2DS_ZERO)
    {
        return L2DS_FAILURE;
    }

    if (STRCMP (pu1IfType, "atm") == L2DS_ZERO)
    {
        /* pu1Slot is having slot value */
        pu1Slot = (UINT1 *) STRTOK (NULL, "/");
            /* Converting the string into integer */
            i4Slot = (INT4) ATOI (pu1Slot);

        /* pu1Port is having Port value */
        pu1Port = (UINT1 *) STRTOK (NULL, ":");
            /* Converting the string into integer */
            i4Port = (INT4) ATOI (pu1Port);

        /* pu1Vpi is having VPI value */
        pu1Vpi = (UINT1 *) STRTOK (NULL, ".");
            /* Converting the string into integer */
            i4Vpi = (INT4) ATOI (pu1Vpi);

        /* pu1Vci is having VCI value */
        pu1Vci = (UINT1 *) STRTOK (NULL, " ");
            /* Converting the string into integer */
            i4Vci = (INT4) ATOI (pu1Vci);

        SPRINTF ((CHR1 *) pu1Name, "%s%d/%d", ISS_ALIAS_PREFIX, i4Slot, i4Port);

        if (CfaGetInterfaceIndexFromName (pu1Name, &u4DslIfIndex) !=
            OSIX_FAILURE)
        {
            /* calling a function to get PVC index from vpi,vci and DSL
               index */
            if (CfaGetPvcIfIndexFromDslIndexVpiVci (u4DslIfIndex,
                                                    i4Vpi, i4Vci,
                                                    &u4IfIndex) != CFA_FAILURE)
            {
                *pu4OutPort = u4IfIndex;
                *pVlanId = VlanId;

                L2DS_TRC(L2DS_FN_EXIT, "[L2dsUtilGetPortAndVlanFromOption]"
					"Exit after getting outport from PVC Index,vpi and vci \r\n");

                return L2DS_SUCCESS;
            }
        }
    }

    if (STRCMP (pu1IfType, "eth") == L2DS_ZERO)
    {
        /* pu1Slot is having slot value */
        pu1Slot = (UINT1 *) STRTOK (NULL, "/");
            /* Converting the string into integer */
            i4Slot = (INT4) ATOI (pu1Slot);

        /* pi4Port is having Port value */
        pu1Port = (UINT1 *) STRTOK (NULL, ":");
            /* Converting the string into integer */
            i4Port = (INT4) ATOI (pu1Port);

        /* pi1VlanId is having VlanId */
        pi1VlanId = (UINT1 *) STRTOK (NULL, "");
            /* Converting the string into integer */
            VlanId = (UINT2) ATOI (pi1VlanId);

        SPRINTF ((CHR1 *) pu1Name, "%s%d/%d", ISS_ALIAS_PREFIX, i4Slot, i4Port);

        /* calling a function to get outport from the ifname */
        if (CfaGetInterfaceIndexFromName (pu1Name, &u4OutPort) != OSIX_FAILURE)
        {
            *pu4OutPort = u4OutPort;
            *pVlanId = VlanId;

            L2DS_TRC(L2DS_FN_EXIT, "[L2dsUtilGetPortAndVlanFromOption]"
				"Exit after getting outport and VID from DSL Index\r\n");
            return L2DS_SUCCESS;
        }
    }

    L2DS_TRC(L2DS_FN_EXIT, "[L2dsUtilGetPortAndVlanFromOption:End]\r\n");
    return L2DS_FAILURE;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilGetRemoteIdMac                               */
/*                                                                           */
/* Description        : This is to get the MAC in the REMOTE-ID sub-option   */
/*                                                                           */
/* Input(s)           : pL2DhcpSnpOption - Option-82                         */
/*                                                                           */
/* Output(s)          : RemoteMac - Mac address                              */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsUtilGetRemoteIdMac (tL2DhcpSnpOption * pL2DhcpSnpOption, tMacAddr RemoteMac)
{
    UINT1              *pu1Options = NULL;
    UINT1               u1OptLen = L2DS_ZERO;
    UINT1               u1Offset = L2DS_ZERO;
    UINT1               u1Type = L2DS_ZERO;
    UINT1               u1SubType = L2DS_ZERO;
    UINT1               u1SubTypeLen = L2DS_ZERO;

    pu1Options = pL2DhcpSnpOption->pu1Val;

    u1Type = pu1Options[L2DS_ZERO];

    if (pL2DhcpSnpOption->u1Type != L2DS_OPT_RAI)
    {
        /* The passed option is not option-82 */
        return L2DS_FAILURE;
    }

    u1OptLen = pL2DhcpSnpOption->u1Len;

    while (u1OptLen > u1Offset)
    {
        u1SubType = pu1Options[u1Offset++];
        u1SubTypeLen = pu1Options[u1Offset++];

        if (u1SubType != L2DS_REMOTE_SUBOPT)
        {
            /* if the sub-option is not Remote-ID, get the next sub-option */
            u1Offset = (UINT1) (u1Offset + u1SubTypeLen);
            continue;
        }
        else
        {
            if (u1SubTypeLen != L2DS_REMOTE_SUBOPT_LEN)
            {
                return L2DS_FAILURE;
            }
            if (pu1Options[u1Offset++] != L2DS_REMOTE_ID_TYPE)
            {
                return L2DS_FAILURE;
            }
            if (pu1Options[u1Offset++] != L2DS_REMOTE_LEN)
            {
                return L2DS_FAILURE;
            }

            MEMCPY (RemoteMac, (pu1Options + u1Offset), sizeof (tMacAddr));

            return L2DS_SUCCESS;
        }
    }
    return L2DS_FAILURE;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilGetAccessNodeId                              */
/*                                                                           */
/* Description        : This is to get the AccessNodeID in the CIRCUIT-ID    */
/*                      sub-option                                           */
/*                                                                           */
/* Input(s)           : pL2DhcpSnpOption - Option-82                         */
/*                      u1AccessIdLen    - Length of AccessNodeID            */
/*                                                                           */
/* Output(s)          : pu1AccessId - access node id                         */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsUtilGetAccessNodeId (tL2DhcpSnpOption * pL2DhcpSnpOption,
                         UINT1 u1AccessIdLen, UINT1 *pu1AccessId)
{
    UINT1              *pu1Options = NULL;
    UINT1               u1OptLen = L2DS_ZERO;
    UINT1               u1Offset = L2DS_ZERO;
    UINT1               u1Type = L2DS_ZERO;
    UINT1               u1SubType = L2DS_ZERO;
    UINT1               u1SubTypeLen = L2DS_ZERO;

    pu1Options = pL2DhcpSnpOption->pu1Val;

    u1Type = pu1Options[L2DS_ZERO];

    if (pL2DhcpSnpOption->u1Type != L2DS_OPT_RAI)
    {
        /* The passed option is not option-82 */
        return L2DS_FAILURE;
    }

    u1OptLen = pL2DhcpSnpOption->u1Len;

    while (u1OptLen > u1Offset)
    {
        u1SubType = pu1Options[u1Offset++];
        u1SubTypeLen = pu1Options[u1Offset++];

        if (u1SubType != L2DS_CIRCUIT_SUBOPT)
        {
            /* if the sub-option is not Circuit-ID, get the next sub-option */
            u1Offset = (UINT1) (u1Offset + u1SubTypeLen);
            continue;
        }
        else
        {
            MEMCPY (pu1AccessId, (pu1Options + u1Offset), u1AccessIdLen);

            return L2DS_SUCCESS;
        }
    }
    return L2DS_FAILURE;
}

/*****************************************************************************/
/* Function Name      : L2dsUtileGetFreeString                               */
/*                                                                           */
/* Description        : This is to get the FreeString in the CIRCUIT-ID      */
/*                      sub-option                                           */
/*                                                                           */
/* Input(s)           : pL2DhcpSnpOption - Option-82                         */
/*                      u1ACILen   - Length of FreeString                    */
/*                                                                           */
/* Output(s)          : pu1ACI     -  free string identifier                 */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsUtilGetFreeString (tL2DhcpSnpOption * pL2DhcpSnpOption,
                       UINT1 u1ACILen, UINT1 *pu1ACI)
{
    UINT1              *pu1Options = NULL;
    UINT1               u1OptLen = L2DS_ZERO;
    UINT1               u1Offset = L2DS_ZERO;
    UINT1               u1Type = L2DS_ZERO;
    UINT1               u1SubType = L2DS_ZERO;
    UINT1               u1SubTypeLen = L2DS_ZERO;

    pu1Options = pL2DhcpSnpOption->pu1Val;

    u1Type = pu1Options[L2DS_ZERO];

    if (pL2DhcpSnpOption->u1Type != L2DS_OPT_RAI)
    {
        /* The passed option is not option-82 */
        return L2DS_FAILURE;
    }

    u1OptLen = pL2DhcpSnpOption->u1Len;

    while (u1OptLen > u1Offset)
    {
        u1SubType = pu1Options[u1Offset++];
        u1SubTypeLen = pu1Options[u1Offset++];

        if (u1SubType != L2DS_CIRCUIT_SUBOPT)
        {
            /* if the sub-option is not Circuit-ID, get the next sub-option */
            u1Offset = (UINT1) (u1Offset + u1SubTypeLen);
            continue;
        }
        else
        {
            MEMCPY (pu1ACI, (pu1Options + u1Offset), u1ACILen);

            return L2DS_SUCCESS;
        }
    }
    return L2DS_FAILURE;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilVerifyCookie                                 */
/*                                                                           */
/* Description        : This routine verifies the Magic cookie in the DHCP   */
/*                      packet                                               */
/*                                                                           */
/* Input(s)           : pu1LinearBuf - Linear buffer                         */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsUtilVerifyCookie (UINT1 *pu1LinearBuf)
{
    UINT1               au1MagicCookie[L2DS_MAGIC_COOKIE_LEN] =
        { 0x63, 0x82, 0x53, 0x63 };

    if (MEMCMP (pu1LinearBuf, au1MagicCookie, L2DS_MAGIC_COOKIE_LEN) !=
        L2DS_ZERO)
    {
        return L2DS_FAILURE;
    }
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilCalcUdpCkSum                                 */
/*                                                                           */
/* Description        : This routine calculates the UDP Checksum of the DHCP */
/*                      packet                                               */
/*                                                                           */
/* Input(s)           : u2UdpLen - Pkt length includes UDP header and data   */
/*                      u4DestIp - destination IP                            */
/*                      u4SrcIp  - source IP                                 */
/*                      pu1LinearBuf - Linear buffer                         */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : Calculated UDP Checksum                              */
/*****************************************************************************/
INT2
L2dsUtilCalcUdpCkSum (UINT2 u2UdpLen, UINT4 u4DestIp, UINT4 u4SrcIp,
                      UINT1 *pu1LinearBuf)
{
    UINT4               u4Sum = L2DS_ZERO;
    UINT2               u2Tmp = L2DS_ZERO;
    UINT2               u2Size = L2DS_ZERO;

    /* Adding the Pseudo Header */
    u4Sum += (u4SrcIp >> L2DS_SIXTEEN);
    u4Sum += (u4SrcIp & 0xffff);
    u4Sum += (u4DestIp >> L2DS_SIXTEEN);
    u4Sum += (u4DestIp & 0xffff);
    u4Sum += (UINT4) L2DS_UDP_PROTO_ID;    /* UDP protocol = 17. */
    u4Sum += (UINT4) u2UdpLen;

    u2Size = u2UdpLen;

    while (u2Size > L2DS_ONE)
    {
        /* Make a UINT2 word of adjacent 2 bytes */
        u2Tmp = (UINT2) (((pu1LinearBuf[L2DS_ZERO] << L2DS_EIGHT) &
                          (UINT2) 0xFF00) +
                         (pu1LinearBuf[L2DS_ONE] & (UINT1) 0xFF));
        u4Sum += (UINT4) u2Tmp;
        pu1LinearBuf += (UINT1) sizeof (UINT2);
        u2Size -= sizeof (UINT2);
    }

    if (u2Size == L2DS_ONE)
    {
        u2Tmp = (UINT2) (((pu1LinearBuf[L2DS_ZERO] << L2DS_EIGHT) &
                          (UINT2) 0xFF00));
        u4Sum += (UINT4) u2Tmp;
    }

    /* keep only the last 16 bits of the 32 bit calculated
     * sum and add the carries */
    while (u4Sum >> L2DS_SIXTEEN)
    {
        u4Sum = (u4Sum & 0xFFFF) + (u4Sum >> L2DS_SIXTEEN);
    }
    /* Take the one's complement of sum */
    u2Tmp = (UINT2) ~u4Sum;

    u2Tmp = OSIX_HTONS (u2Tmp);

    return (u2Tmp);
}

/*****************************************************************************/
/* Function Name      : L2dsUtilLinearToCRU                                  */
/*                                                                           */
/* Description        : This routine converts the given linear buffer        */
/*                      to CRU Buffer                                        */
/*                                                                           */
/* Input(s)           : pu1LinearBuf - Linear buffer                         */
/*                      u4PktLength  - Packet length                         */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : Pointer to CRU Buffer/NULL                           */
/*****************************************************************************/
tCRU_BUF_CHAIN_HEADER *
L2dsUtilLinearToCRU (UINT1 *pu1LinearBuf, UINT4 u4PktLength)
{
    tCRU_BUF_CHAIN_HEADER *pBuf = NULL;

    pBuf = CRU_BUF_Allocate_MsgBufChain (u4PktLength, L2DS_ZERO);

    if (pBuf == NULL)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "[L2dsUtilLinearToCRU]"
			"Allocate CRU buffer failed.\r\n");
        return NULL;
    }
    CRU_BUF_Copy_OverBufChain (pBuf, pu1LinearBuf, L2DS_ZERO, u4PktLength);

    return pBuf;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilRBTreeIntfEntryCmp                           */
/*                                                                           */
/* Description        : This function is used for comparing two entries of   */
/*                      RBTree for interface entries                         */
/*                                                                           */
/* Input(s)           : pL2DhcpSnpIfaceEntryOne -First Entry                 */
/*                      pL2DhcpSnpIfaceEntryTwo -second Entry                */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : -1/1 -> when entry 1 is Less/Greater than entry 2    */
/*                      0    -> when entries elements are Equal              */
/*****************************************************************************/
INT4
L2dsUtilRBTreeIntfEntryCmp (tRBElem * pL2DhcpSnpIfaceEntryOne,
                            tRBElem * pL2DhcpSnpIfaceEntryTwo)
{
    if (((tL2DhcpSnpIfaceEntry *) pL2DhcpSnpIfaceEntryOne)->u4L2dsCxtId <
        ((tL2DhcpSnpIfaceEntry *) pL2DhcpSnpIfaceEntryTwo)->u4L2dsCxtId)
    {
        return L2DS_MINUS_ONE;
    }
    if (((tL2DhcpSnpIfaceEntry *) pL2DhcpSnpIfaceEntryOne)->u4L2dsCxtId >
        ((tL2DhcpSnpIfaceEntry *) pL2DhcpSnpIfaceEntryTwo)->u4L2dsCxtId)
    {
        return L2DS_ONE;
    }

    if (((tL2DhcpSnpIfaceEntry *) pL2DhcpSnpIfaceEntryOne)->VlanId <
        ((tL2DhcpSnpIfaceEntry *) pL2DhcpSnpIfaceEntryTwo)->VlanId)
    {
        return L2DS_MINUS_ONE;
    }
    else if (((tL2DhcpSnpIfaceEntry *) pL2DhcpSnpIfaceEntryOne)->VlanId >
             ((tL2DhcpSnpIfaceEntry *) pL2DhcpSnpIfaceEntryTwo)->VlanId)
    {
        return L2DS_ONE;
    }
    else
    {
        return L2DS_ZERO;
    }
}

/*****************************************************************************/
/* Function Name      : L2dsUtilRBTreeEntryFree                              */
/*                                                                           */
/* Description        : This routine is used for releasing the Memory        */
/*                      allocated for RbTree Node for Interface structure.   */
/*                                                                           */
/* Input(s)           : pL2DhcpSnpIfaceEntry - Pointer to Interface entry    */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None                                                 */
/*****************************************************************************/
VOID
L2dsUtilRBTreeEntryFree (tRBElem * pL2DhcpSnpIfaceEntry)
{
    if (pL2DhcpSnpIfaceEntry != NULL)
    {
        L2DS_RELEASE_MEM_BLOCK (L2DS_INTF_POOL_ID, pL2DhcpSnpIfaceEntry);
    }
    return;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilTakeLock                                     */
/*                                                                           */
/* Description        : This function is to take the protocol lock for the   */
/*                      module                                               */
/*                                                                           */
/* Input(s)           : None                                                 */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : SNMP_SUCCESS or SNMP_FAILURE                         */
/*****************************************************************************/
INT4
L2dsUtilTakeLock (VOID)
{
    if (L2DS_TAKE_SEM (L2DS_ZERO, L2DS_SEM_NAME, L2DS_ZERO, L2DS_ZERO)
        != OSIX_SUCCESS)
    {
        return SNMP_FAILURE;
    }
    return SNMP_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilReleaseLock                                  */
/*                                                                           */
/* Description        : This function is to release the protocol lock taken  */
/*                                                                           */
/* Input(s)           : None                                                 */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : SNMP_SUCCESS or SNMP_FAILURE                         */
/*****************************************************************************/
INT4
L2dsUtilReleaseLock (VOID)
{
    L2DS_GIVE_SEM (L2DS_ZERO, L2DS_SEM_NAME);
    return SNMP_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilRegisterFsDhcSnpMib                          */
/*                                                                           */
/* Description        : This function is to register the protocol MIB        */
/*                                                                           */
/* Input(s)           : None                                                 */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None                                                 */
/*****************************************************************************/
VOID
L2dsUtilRegisterFsDhcSnpMib (VOID)
{
    RegisterFSMIDH ();
    /* Registering SI mibs in case of SI alone. */
    if (L2dsGetVcmSystemModeExt (L2DS_PROTOCOL_ID) == VCM_SI_MODE)
    {
        RegisterFSDHCS ();
    }
    return;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilValidateFreeString                           */
/*                                                                           */
/* Description        : This is to validate to which port free string is     */
/*                      configured.                                          */
/*                                                                           */
/* Input(s)           : pL2DhcpSnpOption - Option-82                         */
/*                                                                           */
/* Output(s)          : pu4OutPort     - The port to which the free-string   */
/*                                       is configured                       */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsUtilValidateFreeString (tL2DhcpSnpOption * pL2DhcpSnpOption,
                            UINT4 *pu4OutPort)
{
    UINT1               au1FreeString[DCS_CIRCUIT_ID_LEN] = { L2DS_ZERO };
    UINT1               au1FreeStringACI[DCS_CIRCUIT_ID_LEN] = { L2DS_ZERO };
    UINT4               u4IfIndex = L2DS_ZERO;
    INT4                i4FreeStringLen = L2DS_ZERO;
    INT4                i4RetStatus = L2DS_ZERO;

    for (u4IfIndex = L2DS_ONE; u4IfIndex <= ((UINT4)SYS_DEF_MAX_PHYSICAL_INTERFACES +
                                             SYS_DEF_MAX_PVCS +
                                             LA_MAX_PORTS_PER_AGG); u4IfIndex++)
    {
        if (DcsGetFreeStringACI
            ((INT4) u4IfIndex, au1FreeString, &i4FreeStringLen) != DCS_FAILURE)
        {
            if (i4FreeStringLen != L2DS_ZERO)
            {
                /* Get the Free String configured in the ACI */
                i4RetStatus =
                    L2dsUtilGetFreeString (pL2DhcpSnpOption,
                                           (UINT1) i4FreeStringLen,
                                           au1FreeStringACI);

                if (i4RetStatus == L2DS_SUCCESS)
                {
                    if (MEMCMP
                        (au1FreeStringACI, au1FreeString,
                         i4FreeStringLen) == L2DS_ZERO)
                    {
                        *pu4OutPort = u4IfIndex;
                        return L2DS_SUCCESS;
                    }
                }
            }
        }
    }
    return L2DS_FAILURE;
}

/*****************************************************************************
 *
 *    Function Name        : DcsGetAccessNodeIdentifier
 *
 *    Description          : This function is used to get configured Access 
 *                           Node Identifier.
 *
 *    Input(s)             : pu1AccessNodeName -Pointer to the AccessNodeID
 *
 *    Output(s)            : NONE
 *    
 *    Return               : pu1AccessNodeName - Name will be return.
 *   
 *****************************************************************************/
UINT1              *
DcsGetAccessNodeIdentifier (UINT1 *pu1AccessNodeName)
{
    pu1AccessNodeName = IssSysGetSwitchName ();
    return pu1AccessNodeName;
}

/*****************************************************************************/
/* Function Name      : DcsUtilGetCircuitIdString                            */
/*                                                                           */
/* Description        : This routine is to add Agent Circuit ID in the       */
/*                      incoming PPPoE Discovery Stage Packet                */
/*                                                                           */
/* Input(s)           : u4PortNumber - Incoming Port                         */
/*                      VlanId       - Vlan Identifier                       */
/*                                                                           */
/* Output(s)          : pu1CircuitId - Pointer to Agent Circuit ID           */
/*                      pu1CircuitIDLen -Total circuit id Length             */
/*                                                                           */
/* Return Value(s)    : DCS_SUCCESS/DCS_FAILURE                              */
/*****************************************************************************/
INT4
DcsUtilGetCircuitIdString (UINT4 u4PortNumber, tVlanId VlanId,
                           UINT1 *pu1CircuitId, UINT1 *pu1CircuitIDLen)
{
    UINT1               au1Temp[DCS_CIRCUIT_ID_LEN] = { L2DS_ZERO };
    UINT1               au1AccessNode[DCS_ACCESS_NODE_LEN + L2DS_ONE]
        = { L2DS_ZERO };
    UINT1              *pu1AccessNodeName = NULL;
    INT1               *pi1CircuitIdString = NULL;
    UINT1               u1AccessNodeLength = L2DS_ZERO;
    INT4                i4SlotPortNum = L2DS_ZERO;
    UINT4               u4DslIfIndex = L2DS_ZERO;
    UINT1               u1IfType = L2DS_ZERO;
    INT4                i4SlotNum = L2DS_ZERO;
    INT4                i4Vpi = L2DS_ZERO;
    INT4                i4Vci = L2DS_ZERO;

    /* Maximum Length that can be filled in the Circuit-Id Fields */
    /* AccessNodeName = 24 (DCS_ACCESS_NODE_LEN)
     * SlotNumber     = 4 Bytes
     * PortNumber     = 4 Bytes
     * VPI            = 4 Bytes
     * VCI            = 4 Bytes
     * VlanId(1-4094) = 2 Bytes.*/

    /* If the incoming interface is DSL Interface then  
     * circuit-Id value will be as follows :               */
    /*******************************************************/
    /* AccessNodeName "atm" SlotNum / PortNumber : VPI.VCI */
    /*******************************************************/
    /*  Example : ISS atm 0/25:8.35                        */
    /*******************************************************/

    /* If the incoming interface is ethernet then  
     * circuit-Id packet format will be as follows :       */
    /*******************************************************/
    /* AccessNodeName "eth" SlotNum / PortNumber : VlanId  */
    /*******************************************************/
    /*  Example : ISS eth 0/1:2                            */
    /*******************************************************/

    pi1CircuitIdString = (INT1 *) au1Temp;

    if (CfaGetIfaceType (u4PortNumber, &u1IfType) == CFA_FAILURE)
    {
        L2DS_TRC_ARG1(L2DS_FAIL_TRC, "[DcsUtilGetCircuitIdString]"
			"Get interface%d type failed.\r\n", u4PortNumber);
        return DCS_FAILURE;
    }

    /*calling a function to get Access Node Identifier */
    pu1AccessNodeName = DcsGetAccessNodeIdentifier (au1AccessNode);

    if (u1IfType == CFA_PVC)
    {
        /*calling a function to get DSL Interface Index, vpi and vci
         * from PVC Interface Index */
        if (CfaGetVciVpiDslIfIndex (u4PortNumber,
                                    &u4DslIfIndex, &i4Vpi, &i4Vci) ==
            CFA_FAILURE)
        {
            L2DS_TRC (L2DS_FAIL_TRC, "[DcsUtilGetCircuitIdString]"
				"Get VPI,VCI and DslIndex for the PVC Interface Index failed\r\n");
            return DCS_FAILURE;
        }

        u4PortNumber = u4DslIfIndex;
    }

    /*calling a function to get slot and port from Interface index */
    if (CfaGetSlotAndPortFromIfIndex (u4PortNumber,
                                      &i4SlotNum,
                                      &i4SlotPortNum) == CFA_FAILURE)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "[DcsUtilGetCircuitIdString]"
			"Get Slot and Port for the Interface Index failed\r\n");

        return DCS_FAILURE;
    }

    if (u1IfType == CFA_PVC)
    {
        SPRINTF ((CHR1 *) pi1CircuitIdString, "%s %s %d/%d:%d.%d",
                 pu1AccessNodeName, "atm", i4SlotNum, i4SlotPortNum, i4Vpi,
                 i4Vci);
    }
    else
    {
        SPRINTF ((CHR1 *) pi1CircuitIdString, "%s %s %d/%d:%d",
                 pu1AccessNodeName, "eth", i4SlotNum, i4SlotPortNum, VlanId);
    }

    u1AccessNodeLength = (UINT1) STRLEN (pi1CircuitIdString);

    pu1CircuitId[L2DS_ZERO] = DCS_CIRCUIT_SUBOPT;    /* Sub-tag type */
    pu1CircuitId[L2DS_ONE] = u1AccessNodeLength;    /* Length */

    MEMCPY (&(pu1CircuitId[L2DS_TWO]), pi1CircuitIdString, u1AccessNodeLength);

    /* Circuit ID is Type-Length-Value format.To get Total Circuit ID Length
     * Adding Type and Length Bytes with the Circuit ID string length*/
    *pu1CircuitIDLen = (UINT1) (u1AccessNodeLength + DCS_CIRCUIT_ID_HEADER_LEN);

    return DCS_SUCCESS;

}

/*****************************************************************************/
/* Function Name      : DcsUtilGetRemoteIdString                             */
/*                                                                           */
/* Description        : This routine is to add Agent Remote ID in the        */
/*                      incoming PPPoE Discovery Stage Packet                */
/*                                                                           */
/* Output(s)          : au1RemoteId - Remote ID                              */
/*                      pu1RemoteIdLength -Total Remote ID Length            */
/*                                                                           */
/* Return Value(s)    : DCS_SUCCESS/DCS_FAILURE                              */
/*****************************************************************************/
INT4
DcsUtilGetRemoteIdString (UINT1 *pu1RemoteId, UINT1 *pu1RemoteIdLength)
{
    /* Remote ID is Type-Length-Value format.To get Total Remote ID Length
     *      * Adding Type and Length Bytes with the Remote ID length*/

    pu1RemoteId[L2DS_ZERO] = DCS_REMOTE_SUBOPT;
    pu1RemoteId[L2DS_ONE] = L2DS_ZERO;

    *pu1RemoteIdLength = DCS_REMOTE_ID_HEADER_LEN;

    return DCS_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/*    Function Name       : L2dsSelectContext                                */
/*                                                                           */
/*    Description         : This function switches to given context          */
/*                                                                           */
/*    Input(s)            : u4ContextId - Context Identifier                 */
/*                                                                           */
/*    Output(s)           : None.                                            */
/*                                                                           */
/*    Returns             : L2DS_SUCCESS / L2DS_FAILURE.                     */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

PUBLIC INT4
L2dsSelectContext (UINT4 u4ContextId)
{
    if (L2dsGetVcmSystemMode (L2DS_PROTOCOL_ID) == VCM_MI_MODE)
    {
        if (L2dsVcmIsVcExist (u4ContextId) == L2DS_FALSE)
        {
            return L2DS_FAILURE;
        }
        L2DS_CURR_CXT_ID = u4ContextId;
    }
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/*     FUNCTION NAME    : L2dsVcmIsSwitchExist                               */
/*                                                                           */
/*     DESCRIPTION      : This function will return whether the given        */
/*                        context exist or not.                              */
/*                                                                           */
/*     INPUT            : u4ContextId - Context Identifier                   */
/*                                                                           */
/*     OUTPUT           : None                                               */
/*                                                                           */
/*     RETURNS          : L2DS_TRUE / L2DS_FALSE                             */
/*                                                                           */
/*****************************************************************************/
PUBLIC INT4
L2dsVcmIsSwitchExist (UINT4 u4ContextId)
{
    if (L2dsGetVcmSystemMode (L2DS_PROTOCOL_ID) == VCM_MI_MODE)
    {
        if (L2dsVcmIsVcExist (u4ContextId) == VCM_FALSE)
        {
            return L2DS_FAILURE;
        }
    }
    else
    {
        if (u4ContextId != L2DS_DEFAULT_CXT_ID)
        {
            return L2DS_FAILURE;
        }
    }
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/*     FUNCTION NAME    : L2dsVcmIsVcExist                                   */
/*                                                                           */
/*     DESCRIPTION      : This function will return whether the given        */
/*                        context exist or not.                              */
/*                                                                           */
/*     INPUT            : u4ContextId - Context Identifier                   */
/*                                                                           */
/*     OUTPUT           : None                                               */
/*                                                                           */
/*     RETURNS          : L2DS_TRUE / L2DS_FALSE                             */
/*                                                                           */
/*****************************************************************************/

PUBLIC INT4
L2dsVcmIsVcExist (UINT4 u4ContextId)
{
    INT4                i4RetVal = VCM_FALSE;

    i4RetVal = VcmIsL2VcExist (u4ContextId);

    return ((i4RetVal == VCM_FALSE) ? L2DS_FALSE : L2DS_TRUE);
}

/*****************************************************************************/
/*                                                                           */
/*     FUNCTION NAME    : L2dsGetVcmSystemMode                               */
/*                                                                           */
/*     DESCRIPTION      : This function calls the VCM Module to get the      */
/*                        mode of the system (SI / MI).                      */
/*                                                                           */
/*     INPUT            : u2ProtocolId - Protocol Identifier                 */
/*                                                                           */
/*     OUTPUT           : None.                                              */
/*                                                                           */
/*     RETURNS          : VCM_MI_MODE / VCM_SI_MODE                          */
/*                                                                           */
/*****************************************************************************/
PUBLIC INT4
L2dsGetVcmSystemMode (UINT2 u2ProtocolId)
{
    return (VcmGetSystemMode (u2ProtocolId));
}

/*****************************************************************************/
/*                                                                           */
/*     FUNCTION NAME    : L2dsGetVcmSystemModeExt                            */
/*                                                                           */
/*     DESCRIPTION      : This function calls the VCM Module to get the      */
/*                        mode of the system (SI / MI).                      */
/*                                                                           */
/*     INPUT            : u2ProtocolId - Protocol Identifier                 */
/*                                                                           */
/*     OUTPUT           : None.                                              */
/*                                                                           */
/*     RETURNS          : VCM_MI_MODE / VCM_SI_MODE                          */
/*                                                                           */
/*****************************************************************************/
PUBLIC INT4
L2dsGetVcmSystemModeExt (UINT2 u2ProtocolId)
{
    return (VcmGetSystemModeExt (u2ProtocolId));
}

/*****************************************************************************/
/*                                                                           */
/* Function     : L2dsReleaseContext                                         */
/*                                                                           */
/* Description  : This function makes the switch context to default Context  */
/*                                                                           */
/* Input        : None                                                       */
/*                                                                           */
/* Output       : None                                                       */
/*                                                                           */
/* Returns      : None                                                       */
/*                                                                           */
/*****************************************************************************/

PUBLIC VOID
L2dsReleaseContext ()
{
    if (L2dsGetVcmSystemMode (L2DS_PROTOCOL_ID) == VCM_MI_MODE)
    {
        L2DS_CURR_CXT_ID = L2DS_DEFAULT_CXT_ID;
    }
    return;
}

/*****************************************************************************
 * FUNCTION NAME    : L2dsVcmIsSwitchNameExist                                   
 *
 * DESCRIPTION      : Routine used to get the context Id for the Alias Name
 *
 * INPUT            : pu1Alias - Context Name
 *                   
 * OUTPUT           : pu4VcNum - Context Identifier
 *
 * RETURNS          : L2DS_TRUE/L2DS_FALSE
 *  
 * ***************************************************************************/
PUBLIC INT4
L2dsVcmIsSwitchNameExist (UINT1 *pu1Alias, UINT4 *pu4VcNum)
{
    INT4                i4RetVal = VCM_FALSE;

    i4RetVal = VcmIsSwitchExist (pu1Alias, pu4VcNum);

    return ((i4RetVal == VCM_FALSE) ? L2DS_FALSE : L2DS_TRUE);
}

/*****************************************************************************/
/* Function Name      : L2dsVcmGetContextInfoFromIfIndex                     */
/*                                                                           */
/* Description        : This function calls the VCM Module to get the        */
/*                      Context-Id and the Localport number.                 */
/*                                                                           */
/* Input(s)           : u4IfIndex      - Interface Identifier.               */
/*                                                                           */
/* Output(s)          : pu4ContextId   - Context Identifier.                 */
/*                      pu2LocalPortId - Local port number.                  */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*                                                                           */
/*****************************************************************************/
INT4
L2dsVcmGetContextInfoFromIfIndex (UINT4 u4IfIndex, UINT4 *pu4ContextId,
                                  UINT2 *pu2LocalPortId)
{
    INT4	i4RetVal = VCM_FAILURE;

    i4RetVal = VcmGetContextInfoFromIfIndex (u4IfIndex, pu4ContextId,
                                             pu2LocalPortId);

    return ((i4RetVal == VCM_FAILURE) ? L2DS_FAILURE : L2DS_SUCCESS);
}

/*****************************************************************************/
/*                                                                           */
/*    Function Name       : L2dsGetNextActiveContext                         */
/*                                                                           */
/*    Description         : This function is used to get the next Active     */
/*                          context present in the system.                   */
/*                                                                           */
/*    Input(s)            : u4CurrContextId - Current Context Id.            */
/*                                                                           */
/*    Output(s)           : pu4NextContextId - Next Context Id.              */
/*                                                                           */
/*    Returns            : L2DS_SUCCESS/L2DS_FAILURE                         */
/*****************************************************************************/

PUBLIC INT4
L2dsGetNextActiveContext (UINT4 u4CurrContextId, UINT4 *pu4NextContextId)
{
    if (VcmGetNextActiveL2Context (u4CurrContextId, pu4NextContextId)
        == VCM_FAILURE)
    {
        return L2DS_FAILURE;
    }
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsUtilIsDhcpSnoopingEnabled                        */
/*                                                                           */
/* Description        : This function checks if Dhcp snooping feature is     */
/*                      enabled in the system or not.                        */
/*                                                                           */
/* Input(s)           : None                                                 */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : SNOOP_SUCCESS / SNOOP_FAILURE                        */
/*****************************************************************************/
INT4
L2dsUtilIsDhcpSnoopingEnabled (UINT4 u4ContextId)
{
    if (L2DS_ADMIN_STATUS (u4ContextId) != L2DS_ENABLED)
    {
        return L2DS_DISABLED;
    }
    return L2DS_ENABLED;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsReadDB
 *
 *    Description        :  This function is used to read binding database from flash
 *
 *    Input(s)		   : None.
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4 CmL2dsReadDB(VOID)
{
	INT1		i1Char;
	INT1		i1Buf[DCS_BACKUP_DB_RWBUF_LEN] = {0};
	INT2		i2ReadLen = 0;
	INT4		i4Fd;
	INT4 		i4Flag;
	INT4		i4LineNum = 0;

	/* open file using read only mode */
	i4Flag = OSIX_FILE_RO;
	i4Fd = FileOpen((CONST UINT1 *)DCS_BACKUP_DB_FILE, i4Flag);
	if(i4Fd < 0)
		return L2DS_FAILURE;

	/* Read a line from DB file */
	i2ReadLen = 0;
	i4LineNum = 0;
	while(FileRead(i4Fd, (CHR1 *)&i1Char, 1) == 1)
    {
		if(i1Char == '\n')
		{
			CmL2dsReadEntry(i1Buf);
			i2ReadLen = 0;
			MEMSET(i1Buf, 0, DCS_BACKUP_DB_RWBUF_LEN);
		}
		else
		{
			i1Buf[i2ReadLen++] = i1Char;
		}
    }

    FileClose(i4Fd);
	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsReadEntry
 *
 *    Description        :  This function is used to read one entry from assigned line.
 *						And update the entry into RAM binding database.
 *
 *    Input(s)		   : pi1Str	- original line read from flash.
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4 CmL2dsReadEntry(INT1 *pi1OneLine)
{
	INT1		*pi1Tok			= NULL;
	INT1		au1Temp[128]	= {0};
	UINT1		u1Num			= 0;
	UINT2		u2InstPort		= 0;
	
	tIpDbEntry          IpDbEntry;
    tIpDbGateway        IpDbGateway;

	L2DS_TRC(L2DS_FN_ENTRY, "[CmL2dsReadEntry:Begin]\r\n");
	
	MEMSET(&IpDbEntry, 0, sizeof(tIpDbEntry));
	MEMSET(&IpDbGateway, 0, sizeof(tIpDbGateway));

	pi1Tok = (INT1*)STRTOK(pi1OneLine, ",");
	while(pi1Tok != NULL)
	{
		u1Num++;
		
		/* vlanid mac binding-type ip-type ip in-port release-time */
		if(u1Num == 1)
		{
			/* vlan id */
			IpDbEntry.VlanId = (tVlanId)ATOL(pi1Tok);
			L2DS_TRC_ARG1(L2DS_DBG_TRC, "VlanId = %d\r\n", IpDbEntry.VlanId);
		}
		else if(u1Num == 2)
		{
			/* mac */
			STRCPY(au1Temp, pi1Tok);
			ClomHexStr2Bitmap((CHR1 *)au1Temp, 64, IpDbEntry.HostMac,
				sizeof(tMacAddr), ':');

			L2DS_TRC_ARG1(L2DS_DBG_TRC, "Mac = %s\r\n", pi1Tok);
		}
		else if(u1Num == 3)
		{
			/* binding-type */
			IpDbEntry.u1BindingType = (UINT1)ATOL(pi1Tok);
			
			L2DS_TRC_ARG1(L2DS_DBG_TRC, "BindingType = %d\r\n", 
				(INT4)IpDbEntry.u1BindingType);
		}
		else if(u1Num == 4)
		{
			/* ip-type */
			IpDbEntry.u1IpType = (UINT1)ATOL(pi1Tok);
			
			L2DS_TRC_ARG1(L2DS_DBG_TRC, "IpType = %d\r\n", 
				(INT4)IpDbEntry.u1IpType);
		}
		else if(u1Num == 5)
		{
			/* ip */
			if(IpDbEntry.u1IpType == IPVX_ADDR_FMLY_IPV4)
			{
				STRCPY(au1Temp, pi1Tok);
				UtlInetAton((const CHR1 *)au1Temp, 
					(tUtlInAddr *)&IpDbEntry.u4HostIp);

				L2DS_TRC_ARG1(L2DS_DBG_TRC, "HostIp = %d\r\n", 
					IpDbEntry.u4HostIp);
			}
			else
			{
				STRCPY(au1Temp, pi1Tok);
				UtlInetAton6((const CHR1 *)au1Temp, 
					(tUtlIn6Addr *)&IpDbEntry.HostIpv6);

				L2DS_TRC_ARG1(L2DS_DBG_TRC, "HostIp = %s\r\n", au1Temp);
			}
		}
		else if(u1Num == 6)
		{
			/* in-port */
			IpDbEntry.u4InIfIndex = (UINT4)ATOL(pi1Tok);

			L2DS_TRC_ARG1(L2DS_DBG_TRC, "InPort = %d\r\n", 
				IpDbEntry.u4InIfIndex);
		}
		else if(u1Num == 7)
		{
			/* lease-time */
			IpDbEntry.u4LeaseTime = (UINT4)ATOL(pi1Tok);

			L2DS_TRC_ARG1(L2DS_DBG_TRC, "LeaseTime = %d\r\n", 
				IpDbEntry.u4LeaseTime);
		}

		pi1Tok = (INT1 *)STRTOK(NULL, ",");
	}

	if(L2dsVcmGetContextInfoFromIfIndex(IpDbEntry.u4InIfIndex, 
		&IpDbEntry.u4ContextId, &u2InstPort) != L2DS_SUCCESS)
    {
		L2DS_TRC(L2DS_FAIL_TRC, "[CmL2dsReadEntry]Get context id fail!\r\n");
        return L2DS_FAILURE;
    }

	if(IpdbApiUpdateBindingEntry(&IpDbEntry, L2DS_ONE, &IpDbGateway, 
		IPDB_CREATE_ENTRY) != IPDB_SUCCESS)
	{
		L2DS_TRC(L2DS_FAIL_TRC, "[CmL2dsReadEntry]"
			"Create an entry in dhcp snp db failed\r\n");
		return L2DS_FAILURE;
	}

	return L2DS_SUCCESS;
}

/*****************************************************************************
*	  Function Name 	   : CmAPIL2dsRecvEvt
 *	  Description		   : send event to l2ds module
 *	  INPUT			   : u4Evt
 *      Output                  : None
 *	  Returns			   : ISS_SUCCESS/ISS_FAILURE
*****************************************************************************/
INT4 
CmAPIL2dsRecvEvt(UINT4 u4Evt)
{
    if(L2DS_SEND_EVENT(L2DS_ZERO, L2DS_TASK_NAME, u4Evt)
		!= OSIX_SUCCESS)
    {
        return ISS_FAILURE;
    }
	else
	{
		return ISS_SUCCESS;
	}
}

/*****************************************************************************
*	  Function Name 	   : CmL2dsFdbAdd
 *	  Description		   : Add an entry into DS fdb
 *	  INPUT			   : Vid		- vlan id
 *						Mac		- mac address
 *						u4Port	- port id
 *      Output                  : None
 *	  Returns			   : L2DS_SUCCESS/L2DS_FAILURE
*****************************************************************************/
INT4 
CmL2dsFdbAdd(tVlanId Vid, tMacAddr Mac, UINT4 u4Port)
{
	INT4			i4EntryIdx;
	UINT4			u4PortIdx;
	
	L2DS_TRC_ARG1(L2DS_TRC_FDB, 
		"[CmL2dsAddFdb]Current fdb entries is %d\r\n",
		gL2DSFdbGlob.i4UsedNum);

	/*CAMEOTAG: Added by zhaopan on 2013/3/28. for untrust port,
	    dsp need maintain one fdb table which including untust port, vid and 
	    client mac. 
	    when handling dhcp server pkts from trust port, will extract untrust port
	    connected with dhcp client from dsp fdb table in order to bind learnt 
	    entry with the untrust port*/
	i4EntryIdx = CmL2dsFdbFind(Vid, Mac, &u4PortIdx);
	if((u4PortIdx != 0) && 
	   (u4PortIdx == u4Port))
	{
		/*Already exists*/
		L2DS_TRC_ARG1(L2DS_TRC_FDB, 
			"Entry Already exists, index = %d\r\n", i4EntryIdx);
		return L2DS_SUCCESS;
	}
	
	if(i4EntryIdx == L2DS_INVALID_FDB_IDX)
	{
		/*There is not a empty entry existed*/
		L2DS_TRC_ARG2(L2DS_TRC_ERR, 
			"Max entries %d, current entries %d, add failed!\r\n",
			L2DS_MAX_FDB_ENTRY, gL2DSFdbGlob.i4UsedNum);
		return L2DS_FAILURE;
	}

	gL2DSFdbGlob.L2DSFdb[i4EntryIdx].Vid = Vid;
	gL2DSFdbGlob.L2DSFdb[i4EntryIdx].u4Port = u4Port;
	MEMCPY(gL2DSFdbGlob.L2DSFdb[i4EntryIdx].Mac, Mac, sizeof(tMacAddr));
    if(TmrStart(L2DS_TMR_LIST, &gL2DSFdbGlob.L2DSFdb[i4EntryIdx].TmrBlk,
		L2DS_TMR_FDB, L2DS_FDB_EXPIRE_TIME, 0) != TMR_SUCCESS)
    {
    	L2DS_TRC(L2DS_TRC_ERR, "TmrStart FAILED!\r\n");
    	MEMSET(&gL2DSFdbGlob.L2DSFdb[i4EntryIdx], 0, sizeof(tL2DhcpSnpFdb));
        return IPDB_FAILURE;
    }
	gL2DSFdbGlob.i4UsedNum++;
	
	return L2DS_SUCCESS;
}

/*****************************************************************************
*	  Function Name 	   : CmL2dsFdbAdd
 *	  Description		   : Delete an entry from DS fdb
 *	  INPUT			   : Vid		- vlan id
 *						Mac		- mac address
 *						u4Port	- port id
 *      Output                  : None
 *	  Returns			   : L2DS_SUCCESS/L2DS_FAILURE
*****************************************************************************/
INT4 
CmL2dsFdbDel(tVlanId Vid, tMacAddr Mac, UINT4 u4Port)
{
	INT4			i4EntryIdx;
	UINT4			u4PortIdx;

	UNUSED_PARAM(u4Port);
	L2DS_TRC_ARG1(L2DS_TRC_FDB, 
		"[CmL2dsFdbDel]Current fdb entries is %d\r\n",
		gL2DSFdbGlob.i4UsedNum);

	i4EntryIdx = CmL2dsFdbFind(Vid, Mac, &u4PortIdx);
	if(u4PortIdx == 0)
	{
		/*Not find the entry*/
		L2DS_TRC(L2DS_TRC_ERR, "Not find the entry, delte failed\r\n");
		return L2DS_FAILURE;
	}

	/*Vid and Mac are right, get related port id and return success*/
	TmrStop(L2DS_TMR_LIST, &gL2DSFdbGlob.L2DSFdb[i4EntryIdx].TmrBlk);
	MEMSET(&gL2DSFdbGlob.L2DSFdb[i4EntryIdx], 0, sizeof(tL2DhcpSnpFdb));
	gL2DSFdbGlob.i4UsedNum--;
	L2DS_TRC(L2DS_TRC_FDB, "Delete successfully!\r\n");
	return L2DS_SUCCESS;
}

/*****************************************************************************
*	  Function Name 	   : CmL2dsFdbFind
 *	  Description		   : Find an entry in DS fdb
 *	  INPUT			   : Vid			- vlan id
 *						Mac			- mac address
 *      Output                  : pu4Port	- port id(LA port id if having LA groups)
 *	  Returns			   : entry index
*****************************************************************************/
INT4 
CmL2dsFdbFind(tVlanId Vid, tMacAddr Mac, UINT4 *pu4Port)
{
	INT4	i;
	INT4	i4EmptyIdx = L2DS_INVALID_FDB_IDX;

	L2DS_TRC(L2DS_TRC_FDB, "[CmL2dsFdbFind]");
	L2DS_TRC_ARG1(L2DS_TRC_FDB, "Vid = %d,", Vid);
	L2DS_TRC_ARG6(L2DS_TRC_FDB, "Mac = %02X:%02X:%02X:%02X:%02X:%02X\r\n",
		(INT4)Mac[0], (INT4)Mac[1], (INT4)Mac[2],
		(INT4)Mac[3], (INT4)Mac[4], (INT4)Mac[5]);
	
	*pu4Port = 0;

	/*Check whether fdb is empty*/
	if(gL2DSFdbGlob.i4UsedNum == 0)
	{
		L2DS_TRC(L2DS_TRC_FDB, "Fdb is empty!\r\n");
		i4EmptyIdx = 0;
		return i4EmptyIdx;
	}
	
	for(i=0; i<L2DS_MAX_FDB_ENTRY; i++)
	{
		if(gL2DSFdbGlob.L2DSFdb[i].Vid == 0)
		{
			/*this is a null entry, record it and go on checking next entry*/
			i4EmptyIdx = i;
			continue;
		}

		if(gL2DSFdbGlob.L2DSFdb[i].Vid != Vid)
		{
			/*Vid is not right, go on checking next entry*/
			continue;
		}
		else if(MEMCMP(gL2DSFdbGlob.L2DSFdb[i].Mac, Mac, 
			sizeof(tMacAddr)) != 0)
		{
			/*Mac is not right, go on checking next entry*/
			continue;
		}
		else
		{
			/*Vid and Mac are right, get related port id and return success*/
			L2DS_TRC(L2DS_TRC_FDB, "Find successfully!\r\n");
			*pu4Port = 	gL2DSFdbGlob.L2DSFdb[i].u4Port;
			return i;
		}
	}

	/*There is no such fdb entry, return fail*/
	L2DS_TRC(L2DS_TRC_FDB, "Find failed!\r\n");
	return i4EmptyIdx;
}

/*****************************************************************************
*	  Function Name 	   : CmL2dsFdbPrint
 *	  Description		   : Print DS fdb every 5 minutes
 *	  INPUT			   : None
 *      Output                  : None
 *	  Returns			   : L2DS_SUCCESS/L2DS_FAILURE
*****************************************************************************/
INT4 
CmL2dsFdbPrint()
{
#ifdef L2DS_DBG_FDB
	INT4			i;
	tL2DhcpSnpFdb	L2DSFdbNullEntry;
	
	PRINTF("==========[CmL2dsFdbPrint]Begin=========\r\n");

	if(gL2DSFdbGlob.i4UsedNum == 0)
	{
		PRINTF("DS FDB is empty!\r\n");
		return L2DS_SUCCESS;
	}
	else
	{
		PRINTF("Vid\tMac\t\t\t\t\t\tPort\r\n");
	}
	
	for(i=0; i<L2DS_MAX_FDB_ENTRY; i++)
	{
		if(MEMCMP(&L2DSFdbNullEntry, &gL2DSFdbGlob.L2DSFdb[i],
			sizeof(tL2DhcpSnpFdb)) == 0)
		{
			continue;
		}

		PRINTF("%d\t", gL2DSFdbGlob.L2DSFdb[i].Vid);
		PRINTF("%02X:%02X:%02X:%02X:%02X:%02X\t\t\t\t", 
			gL2DSFdbGlob.L2DSFdb[i].Mac[0], 
			gL2DSFdbGlob.L2DSFdb[i].Mac[1],
			gL2DSFdbGlob.L2DSFdb[i].Mac[2], 
			gL2DSFdbGlob.L2DSFdb[i].Mac[3],
			gL2DSFdbGlob.L2DSFdb[i].Mac[4], 
			gL2DSFdbGlob.L2DSFdb[i].Mac[5]);
		PRINTF("%d\r\n", gL2DSFdbGlob.L2DSFdb[i].u4Port);
	}

	PRINTF("==========[CmL2dsFdbPrint]End===========\r\n\r\n\r\n");
#endif
	return L2DS_SUCCESS;
}


/*****************************************************************************
* Function Name      : CmDSPOption82DefCircuitIdGet
*
* Description           : This routine to get default circuit ID information
*
* Input(s)               :  u4PortNumber----port number
*                    	    VlanId----vlan id
*
* Output(s)             : pu1CircuitId----circuit ID information
*				   pu1CircuitIDLen----circuit ID length
*
* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE
*****************************************************************************/
INT4 CmDSPOption82DefCircuitIdGet (UINT4 u4PortNumber, tVlanId VlanId,
                           				  UINT1 *pu1CircuitId, UINT1 *pu1CircuitIDLen)
{
	UINT2	u2Vid = OSIX_HTONS(VlanId);
	
    /*CAMEOTAG: Added by zhaopan on 2015/11/26. option82 circuit id definition
	1. Byte 1    : Sub-option type = 1
	2. Byte 2    : Length          = 6
	3. Byte 3    : Circuit ID type = 0/1(default/userdefined)
	4. Byte 4    : Length          = 4
	5. Byte 5-6 : VLAN, the incoming VLAN ID of DHCP client packet
	6. Byte 7    : Module, For a standalone switch, the Module is always 0; For a stackable switch,
	                  the Module is the Unit ID.
	7. Byte 8   :  Port, The incoming port number of DHCP client packet, port number starts from 1*/

	/*circuit ID contents*/
    pu1CircuitId[L2DS_ZERO] = DCS_CIRCUIT_SUBOPT;    /* Sub-option type */
    pu1CircuitId[L2DS_ONE] = 6;    					 /* Length */
	pu1CircuitId[L2DS_TWO] = 0;    					 /* Default */
	pu1CircuitId[L2DS_THREE] = 4;    				 /* Length */
	MEMCPY(&(pu1CircuitId[L2DS_FOUR]), &u2Vid, sizeof(tVlanId));	/*Vlan*/
	pu1CircuitId[L2DS_SIX] = 0;							/* Module */
	pu1CircuitId[L2DS_SEVEN] = (UINT1)u4PortNumber;		/* Port */
	
	/*Circuit ID length*/
    *pu1CircuitIDLen = L2DS_EIGHT;

    return DCS_SUCCESS;

}


/*****************************************************************************
* Function Name      : CmDSPOption82DefRemoteIdGet
*
* Description           : This routine to get default remote ID information
*
* Input(s)               :  NONE
*
* Output(s)             : pu1RemoteId----remote ID information
*				   pu1RemoteIdLength----remote ID length
*
* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE
*****************************************************************************/
INT4 CmDSPOption82DefRemoteIdGet (UINT1 *pu1RemoteId, UINT1 *pu1RemoteIdLength)
{
	tMacAddr	BaseMac;

    MEMSET (BaseMac, 0, sizeof (tMacAddr));
		
    /*CAMEOTAG: Added by zhaopan on 2015/11/26. option82 remote id definition*/
	/*Remote ID's Format:
	   1. Byte1: Sub-option type = 02(DHCP_SUBOPTION_REMOTEID)
	   2. Byte2: Length = StringLength +2 
	   3. Byte3: Remote-id type = 0/1(default/userdefined)
	   4. Byte4: Length = StringLength
	   5. RemoteID = remote id String */

	/*remote id contents*/
    pu1RemoteId[L2DS_ZERO] = DCS_REMOTE_SUBOPT;			/*Sub-option type*/
    pu1RemoteId[L2DS_ONE] = MAC_ADDR_LEN + L2DS_TWO;	/*Length*/
	pu1RemoteId[L2DS_TWO] = 0;							/*Default*/
	pu1RemoteId[L2DS_THREE] = MAC_ADDR_LEN;				/*String Length*/

	CmAPIGetIssSwitchBaseMacAddr(&BaseMac);
	MEMCPY (&(pu1RemoteId[L2DS_FOUR]), &BaseMac, MAC_ADDR_LEN);

	/*remote id length*/
    *pu1RemoteIdLength = MAC_ADDR_LEN + L2DS_FOUR;

    return DCS_SUCCESS;
}


/*                                                                           */
/***************************** END OF FILE ***********************************/
/*                                                                           */
