/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsport.c,v 1.9 2012/02/01 12:42:57 siva Exp $       */
/*****************************************************************************/
/*    FILE  NAME            : l2dsport.c                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snooping                                */
/*    MODULE NAME           : L2DHCP Snooping Porting Module                 */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains porting functions           */
/*                            for L2DHCP Snooping module                     */
/*---------------------------------------------------------------------------*/

#include "l2dsinc.h"
#include "l2dsextn.h"

/*****************************************************************************/
/* Function Name      : L2dsPortEnquesDhcpPkts                               */
/*                                                                           */
/* Description        : This function is to enque DHCP packets for the       */
/*                      L2DHCP Snooping task                                 */
/*                                                                           */
/* Input(s)           : pBuf       - DHCP Packet                             */
/*                      u4IfIndex  - Incoming port index                     */
/*                      VlanTag    - Incoming VLAN tags                      */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None                                                 */
/*****************************************************************************/
VOID
L2dsPortEnquesDhcpPkts (tCRU_BUF_CHAIN_HEADER * pBuf,
                        UINT4 u4IfIndex, tVlanTag VlanTag)
{
    tL2DhcpSnpQMsg     *pL2DhcpSnpQMsg = NULL;
    UINT4               u4ContextId = L2DS_ZERO;
    UINT2               u2InstPort = L2DS_ZERO;

    L2DS_TRC(L2DS_FN_ENTRY, "[L2dsPortEnquesDhcpPkts:Begin]\r\n");
    L2DS_TRC_ARG2(L2DS_FN_ARGS, "[L2dsPortEnquesDhcpPkts]"
		"Arguments: VID=%d,Port=%d\r\n", 
                   VlanTag.OuterVlanTag.u2VlanId, u4IfIndex);

    if ((pL2DhcpSnpQMsg = (tL2DhcpSnpQMsg *)
         (L2DS_ALLOC_MEM_BLOCK (L2DS_Q_POOL_ID))) == NULL)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "[L2dsPortEnquesDhcpPkts]"
			"Memory Allocation failed.\r\n");
		
        CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
        return;
    }

    MEMSET (pL2DhcpSnpQMsg, L2DS_ZERO, sizeof (pL2DhcpSnpQMsg));

    if (L2dsVcmGetContextInfoFromIfIndex (u4IfIndex, &u4ContextId,
                                          &u2InstPort) != L2DS_SUCCESS)
    {
        L2DS_RELEASE_MEM_BLOCK (L2DS_Q_POOL_ID, pL2DhcpSnpQMsg);
        CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
        return;
    }

    pL2DhcpSnpQMsg->u4EventType = L2DS_PKT_RECEIVE_EVENT;
    pL2DhcpSnpQMsg->pInQMsg = pBuf;
    pL2DhcpSnpQMsg->u4InPort = u4IfIndex;
    pL2DhcpSnpQMsg->u4L2dsCxtId = u4ContextId;
    MEMCPY (&(pL2DhcpSnpQMsg->VlanTag), &VlanTag, sizeof (tVlanTag));

    if (L2DS_SEND_TO_QUEUE (L2DS_QUEUE_ID, (UINT1 *) &pL2DhcpSnpQMsg,
                            OSIX_DEF_MSG_LEN) != OSIX_SUCCESS)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "[L2dsPortEnquesDhcpPkts]"
			"Send the packet to queue failed \r\n");

        L2DS_RELEASE_MEM_BLOCK (L2DS_Q_POOL_ID, pL2DhcpSnpQMsg);
        CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
        return;
    }

    if (L2DS_SEND_EVENT (L2DS_ZERO, L2DS_TASK_NAME, L2DS_EVENT_ARRIVED)
        != OSIX_SUCCESS)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "[L2dsPortEnquesDhcpPkts]"
			"Send the event failed\r\n");
        return;
    }
    L2DS_TRC(L2DS_FN_EXIT, "[L2dsPortEnquesDhcpPkts:End]\r\n");
    return;
}

/*****************************************************************************/
/* Function Name      : L2dsPortVlanDelete                                   */
/*                                                                           */
/* Description        : This function is to indicate the L2DHCP Snooping     */
/*                      task about VLAN Deletion                             */
/*                                                                           */
/* Input(s)           : u4ContextId - Context Identifier                     */
/*                      VlanId     - Deleted VLAN Id                         */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None                                                 */
/*****************************************************************************/
VOID
L2dsPortVlanDelete (UINT4 u4ContextId, tVlanId VlanId)
{
    tL2DhcpSnpQMsg     *pL2DhcpSnpQMsg = NULL;

    L2DS_TRC(L2DS_FN_ENTRY, "[L2dsPortVlanDelete:Begin]\r\n");
    L2DS_TRC_ARG1(L2DS_FN_ARGS, "[L2dsPortVlanDelete]"
		"Arguments: VlanId=%d\r\n", VlanId);
	
    if ((pL2DhcpSnpQMsg = (tL2DhcpSnpQMsg *)
         (L2DS_ALLOC_MEM_BLOCK (L2DS_Q_POOL_ID))) == NULL)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "[L2dsPortVlanDelete]"
			"Memory Allocation failed\r\n");
        return;
    }

    pL2DhcpSnpQMsg->u4EventType = L2DS_VLAN_INTERFACE_EVENT;
    pL2DhcpSnpQMsg->u4L2dsCxtId = u4ContextId;
    pL2DhcpSnpQMsg->VlanId = VlanId;

    if (L2DS_SEND_TO_QUEUE (L2DS_QUEUE_ID, (UINT1 *) &pL2DhcpSnpQMsg,
                            OSIX_DEF_MSG_LEN) != OSIX_SUCCESS)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "[L2dsPortVlanDelete]"
			"Send info to queue failed\r\n");

        L2DS_RELEASE_MEM_BLOCK (L2DS_Q_POOL_ID, pL2DhcpSnpQMsg);
        return;
    }

    if (L2DS_SEND_EVENT (L2DS_ZERO, L2DS_TASK_NAME, L2DS_EVENT_ARRIVED)
        != OSIX_SUCCESS)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "[L2dsPortVlanDelete]"
			"Send the event failed\r\n");
        return;
    }

    L2DS_TRC(L2DS_FN_EXIT, "[L2dsPortVlanDelete:End]\r\n");
    return;
}

/*****************************************************************************/
/* Function Name      : L2dsPortGetPortType                                  */
/*                                                                           */
/* Description        : This function is to get the port type of given port  */
/*                                                                           */
/* Input(s)           : u2Port     -  Incoming port                          */
/*                                                                           */
/* Output(s)          : pu1PortType - Returns value (port type)              */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortGetPortType (UINT2 u2InPort, UINT1 *pu1PortType)
{
    UINT1               u1PortType = L2DS_ZERO;
    /* Get the upstream status of the Port */
    if (CfaGetIfPortType (u2InPort, &u1PortType) != CFA_SUCCESS)
    {
        L2DS_TRC_ARG1(L2DS_FAIL_TRC, "[L2dsPortGetPortType]"
			"Get the port%d type failed.\r\n", (INT4)u2InPort);
        return L2DS_FAILURE;
    }

    if (u1PortType == CFA_PORT_TYPE_DOWNLINK)
    {
        *pu1PortType = L2DS_DOWNSTREAM_PORT;
    }
    else if (u1PortType == CFA_PORT_TYPE_UPLINK)
    {
        *pu1PortType = L2DS_UPSTREAM_PORT;
    }
    else
    {
        return L2DS_FAILURE;
    }
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsPortGetPortState                                 */
/*                                                                           */
/* Description        : This function is to get the port state of given port */
/*                                                                           */
/* Input(s)           : u2Port     - Incoming port                           */
/*                                                                           */
/* Output(s)          : pu1PortState - Returns value (port state)            */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortGetPortState (UINT2 u2InPort, UINT1 *pu1PortState)
{
    INT4	i4PortState = L2DS_ZERO;
    /* Get the trusted status of the Port */
    if (CmAPICfaGetIfSecState (u2InPort, &i4PortState) != ISS_SUCCESS)
    {
        L2DS_TRC_ARG1 (L2DS_FAIL_TRC, "[L2dsPortGetPortState]"
			"Get the port%d type failed\r\n", u2InPort);
        return L2DS_FAILURE;
    }

    if (i4PortState == CFA_PORT_STATE_TRUSTED)
    {
        *pu1PortState = L2DS_TRUSTED_PORT;
    }
    else if (i4PortState == CFA_PORT_STATE_UNTRUSTED)
    {
        *pu1PortState = L2DS_UNTRUSTED_PORT;
    }
    else
    {
        return L2DS_FAILURE;
    }
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsPortGetTrustedList                               */
/*                                                                           */
/* Description        : This function is to get the trusted portlist for     */
/*                      the given VLAN.                                      */
/*                                                                           */
/* Input(s)           : u4ContextId - Context Identifier                     */
/*                      VlanId      - VLAN Identifier                        */
/*                                                                           */
/* Output(s)          : PortList - list of upstream ports                    */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortGetTrustedList (UINT4 u4ContextId, tVlanId VlanId, tPortList PortList)
{
    MEMSET (PortList, L2DS_ZERO, sizeof (tPortList));

    /* Get the upstream status of the Port */
    if (CfaGetTrustedPortList (u4ContextId, (UINT2) VlanId, PortList)
        == CFA_SUCCESS)
    {
        return L2DS_SUCCESS;
    }
    else
    {
        L2DS_TRC_ARG1 (L2DS_FAIL_TRC, "[L2dsPortGetTrustedList]"
			"Get the trusted portlist failed for VLAN %d\r\n", VlanId);
        return L2DS_FAILURE;
    }
}

/*****************************************************************************/
/* Function Name      : L2dsPortGetUpStreamList                              */
/*                                                                           */
/* Description        : This function is to get the upstream portlist for    */
/*                      the given VLAN.                                      */
/*                                                                           */
/* Input(s)           : u4ContextId - Context Identifier                     */
/*                      VlanId      - VLAN Identifier                        */
/*                                                                           */
/* Output(s)          : PortList - list of upstream ports                    */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortGetUpStreamList (UINT4 u4ContextId, tVlanId VlanId, tPortList PortList)
{

    MEMSET (PortList, L2DS_ZERO, sizeof (tPortList));

    /* Get the upstream status of the Port */
    if (CfaGetUpLinkPortList (u4ContextId, VlanId, PortList) == CFA_SUCCESS)
    {
        return L2DS_SUCCESS;
    }
    else
    {
        L2DS_TRC_ARG1 (L2DS_FAIL_TRC, "[L2dsPortGetUpStreamList]"
			"Get the upstream portlist failed for VLAN %d\r\n", VlanId);
        return L2DS_FAILURE;
    }
}

/*****************************************************************************/
/* Function Name      : L2dsPortCalcIpCkSum                                  */
/*                                                                           */
/* Description        : This routine calculates the IP header checksum       */
/*                                                                           */
/* Input(s)           : pu1IpHdr - IP Header                                 */
/*                      u4Size - Ip Header Length                            */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : Calculated IP Checksum                               */
/*****************************************************************************/
INT2
L2dsPortCalcIpCkSum (UINT1 *pu1IpHdr, UINT4 u4Size)
{
    return (UtlIpCSumLinBuf ((INT1 *) pu1IpHdr, u4Size));
}

/*****************************************************************************/
/* Function Name      : L2dsPortGetEtherHdrLen                               */
/*                                                                           */
/* Description        : This routine calculates the Ethernet header length   */
/*                                                                           */
/* Input(s)           : pBuf - Pointer to the packet                         */
/*                      u4InPort - Incoming port                             */
/*                                                                           */
/* Output(s)          : pu2EthetOffset - Length of ethernet header           */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortGetEtherHdrLen (tCRU_BUF_CHAIN_HEADER * pBuf, UINT4 u4InPort,
                        UINT2 *pu2EthetOffset)
{
    UINT4               u4Offset = L2DS_ZERO;

    if (VlanGetTagLenInFrame (pBuf, (UINT2) u4InPort,
                              &u4Offset) != VLAN_SUCCESS)
    {
        return L2DS_FAILURE;
    }

    *pu2EthetOffset = (UINT2) (u4Offset + L2DS_TWO);
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsPortGetDefaultCTag                               */
/*                                                                           */
/* Description        : This routine gets the default C-Vlan Tag configured  */
/*                      for the given port                                   */
/*                                                                           */
/* Input(s)           : u4Port - The port for which VLAN ID is to be found   */
/*                                                                           */
/* Output(s)          : pVlanId - C-Vlan Id                                  */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortGetDefaultCTag (UINT4 u4Port, tVlanId * pVlanId)
{
    /* TBD: Check whether the function is correct */
    return (L2IwfGetVlanPortPvid ((UINT2) u4Port, pVlanId));
}

/*****************************************************************************/
/* Function Name      : L2dsPortForwardPkt                                   */
/*                                                                           */
/* Description        : Forwards the DHCP packet out                         */
/*                                                                           */
/* Input(s)           : pBuf       - The DHCP Packet                         */
/*                      u4ContextId - Context Identifier                     */
/*                      VlanTag    - VLAN tag informations of the DHCP Packet*/
/*                      u4InPort   - The port through which packet came in   */
/*                      DstMacAddr - Destination MAC address                 */
/*                      SrcMacAddr - Source MAC address                      */
/*                      PortList   - list of the outgoing port               */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortForwardPkt (tCRU_BUF_CHAIN_HEADER * pBuf, UINT4 u4ContextId,
                    tVlanTag VlanTag, UINT4 u4InPort, tMacAddr DstMacAddr,
                    tMacAddr SrcMacAddr, tPortList PortList)
{
    if (IpdbApiProcessPktWithPortList (pBuf, u4ContextId, VlanTag, u4InPort,
                                       DstMacAddr, SrcMacAddr, PortList)
        == IPDB_SUCCESS)
    {
        return L2DS_SUCCESS;
    }
    else
    {
        return L2DS_FAILURE;
    }
}

/*****************************************************************************/
/* Function Name      : L2dsPortUpdateEntry                                  */
/*                                                                           */
/* Description        : This function is to create or delete or update a     */
/*                      binding entry                                        */
/*                                                                           */
/* Input(s)           : u4ContextId - Context Identifier                     */
/*                      pL2DS - DHCP SNP packet info			*/
/*                      u1Action - DELETE/CREATE                             */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortUpdateEntry (UINT4 u4ContextId,
	tL2DhcpSnpPktInfo * pL2DS, UINT1 u1Action)
{
    tIpDbEntry          IpDbEntry;
    tIpDbGateway        IpDbGateway;
	
    MEMSET(&IpDbEntry, L2DS_ZERO, sizeof (tIpDbEntry));
	IpDbEntry.u4InIfIndex = pL2DS->u2OutPort;
    IpDbEntry.u4ContextId = u4ContextId;
	
	if(pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV4)
	{
		IpDbEntry.u1IpType = IPVX_ADDR_FMLY_IPV4;

		/*CAMEOTAG: Added by zhaopan on 2013/4/16. 
		    ipdb binding entry regarding vid/mac as index or
		    regarding vid/mac/ip as index. In order to support
		    above two requirement, should care ip learnt/release
		    two conditions*/
		if (u1Action == L2DS_IPDB_CREATE)
		{
			IpDbEntry.u4HostIp = OSIX_NTOHL(pL2DS->L2DSPkt.u4Yiaddr);
		}
		else
		{
			/*if ipdb binding entry regarding vid/mac/ip as index, pL2DS->L2DSPkt.u4Yiaddr
			   as 0 when client ip release, if so, can't find ipdb binding entry to delete,
			   so shoule use pL2DS->L2DSPkt.u4Ciaddr*/
			IpDbEntry.u4HostIp = OSIX_NTOHL(pL2DS->L2DSPkt.u4Ciaddr);
		}
		
		IpDbEntry.u4BindingId = pL2DS->L2DSPkt.u4Xid;
        IpDbEntry.u4LeaseTime 	= pL2DS->L2DSPkt.u4LeaseTime;
        IpDbGateway.u4GatewayIp	= pL2DS->L2DSPkt.u4DftGwIP;
		MEMCPY(IpDbEntry.HostMac, pL2DS->L2DSPkt.Chaddr, sizeof (tMacAddr));
	}
	else if(pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV6)
	{
		IpDbEntry.u1IpType = IPVX_ADDR_FMLY_IPV6;
		MEMCPY(IpDbEntry.HostIpv6.u1_addr, 
			   pL2DS->L2DS6Pkt.HostIPv6.u1_addr,
			   sizeof(tIp6Addr));
		IpDbEntry.u4BindingId = pL2DS->L2DS6Pkt.u4TransID;
		IpDbEntry.u4LeaseTime = pL2DS->L2DS6Pkt.u4LeaseTime;
		MEMCPY(IpDbEntry.HostMac, pL2DS->L2DS6Pkt.Chaddr, sizeof (tMacAddr));
	}

    IpDbEntry.VlanId = pL2DS->VlanTag.OuterVlanTag.u2VlanId;
    IpDbEntry.u1BindingType = IPDB_DHCP_BINDING;

	MEMSET(&IpDbGateway, L2DS_ZERO, sizeof (tIpDbGateway));
    if(u1Action == L2DS_IPDB_CREATE)
    {
        if(IPDB_SUCCESS != IpdbApiUpdateBindingEntry(&IpDbEntry, 
			L2DS_ONE, &IpDbGateway, IPDB_CREATE_ENTRY))
        {
			L2DS_TRC(L2DS_FAIL_TRC, "[L2dsPortUpdateEntry]"
				"Create an entry in dhcp snp db failed\r\n");
            return L2DS_FAILURE;
        }

        return L2DS_SUCCESS;
    }

    if (u1Action == L2DS_IPDB_DELETE)
    {
    	/*Delete this entry from DS fdb firstly*/
    	if (pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV4)
    	{
		    CmL2dsFdbDel(pL2DS->VlanTag.OuterVlanTag.u2VlanId, pL2DS->L2DSPkt.Chaddr, 
						 pL2DS->u2InPort);
    	}
		else if (pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV6)
		{
		    CmL2dsFdbDel(pL2DS->VlanTag.OuterVlanTag.u2VlanId, pL2DS->L2DS6Pkt.Chaddr, 
						 pL2DS->u2InPort);
		}
		
		/*Delete this entry from IPDB*/
        if(IPDB_SUCCESS != IpdbApiUpdateBindingEntry(&IpDbEntry, 
			L2DS_ZERO, NULL, IPDB_DELETE_ENTRY))
        {
        	L2DS_TRC(L2DS_FAIL_TRC, "[L2dsPortUpdateEntry]"
				"Delete an entry in dhcp snp db failed\r\n");
            return L2DS_FAILURE;
        }
        return L2DS_SUCCESS;
    }

    return L2DS_FAILURE;
}

/*****************************************************************************/
/* Function Name      : L2dsPortDeleteEntries                                */
/*                                                                           */
/* Description        : This function is to delete all the binding entries   */
/*                      associated with a VLAN                               */
/*                                                                           */
/* Input(s)           : u4ContextId - Context Identifier                    */
/*                      VlanId      - VLAN Identifier                        */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortDeleteEntries (UINT4 u4ContextId, tVlanId VlanId)
{
	IpdbApiDeleteBindingEntries(IPDB_STATIC_BINDING, 
		u4ContextId, VlanId);
	IpdbApiDeleteBindingEntries(IPDB_DHCP_BINDING, 
		u4ContextId, VlanId);
	IpdbApiDeleteBindingEntries(IPDB_DHCP_MANUAL_BINDING, 
		u4ContextId, VlanId);

    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsPortGetDslOptions                                */
/*                                                                           */
/* Description        : This function is get DSL Options from CFA Module     */
/*                                                                           */
/* Input(s)           : u4Port - Incoming port                               */
/*                                                                           */
/* Output(s)          : pu1DslOptions - DSL sub-options                     */
/*                      pu1DslLen - DSL sub-option length                    */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPortGetDslOptions (UINT1 *pu1DslOptions, UINT1 *pu1DslLen, UINT4 u4Port,
                       tVlanTag VlanTag)
{
    UINT1               u1Offset = L2DS_ZERO;
    UINT4               u4EntrpriseId = L2DS_ZERO;
    UINT1               u1IfType = L2DS_ZERO;
    UINT1               u1DsloptionLen = L2DS_ZERO;
    UINT1               au1DslOptions[DCS_DSL_OPTION_LEN + L2DS_ONE]
        = { L2DS_ZERO };

    /* Fill the sub-option type */
    pu1DslOptions[u1Offset++] = L2DS_SUBOPT_DSL;

    /* We will fill the length later */
    u1Offset++;

    u4EntrpriseId = L2DS_SUBOPT_ENTERPRISE_ID;
    u4EntrpriseId = OSIX_HTONL (u4EntrpriseId);

    MEMCPY ((pu1DslOptions + u1Offset), &u4EntrpriseId, L2DS_ENTERPRISE_ID_LEN);

    u1Offset = (UINT1) (u1Offset + L2DS_ENTERPRISE_ID_LEN);

    /* We will fill the length later */
    u1Offset++;

    CfaGetIfaceType (u4Port, &u1IfType);

    if (DcsUtilGetDslLineCharacteristics
        (u4Port, au1DslOptions, &u1DsloptionLen, VlanTag) == DCS_FAILURE)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "[L2dsPortGetDslOptions]"
			"get the DSL line Characteristics failed.\r\n");

        return L2DS_FAILURE;
    }

    *pu1DslLen = (UINT1) (u1Offset + u1DsloptionLen);
    /* Fill the DSL sub-option length field */
    pu1DslOptions[L2DS_ONE] = (UINT1) (u1DsloptionLen +
                                       L2DS_ENTERPRISE_ID_LEN + L2DS_ONE);

    pu1DslOptions[L2DS_SUB_OPTION_DATA_LEN] = u1DsloptionLen;
    MEMCPY ((pu1DslOptions + u1Offset), au1DslOptions, DCS_DSL_OPTION_LEN);

    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsApiCreateContext                                 */
/*                                                                           */
/* Description        : This function is invoked by l2iwf Module to Create   */
/*                      Context                                              */
/*                                                                           */
/* Input(s)           : Context Identifier                                   */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None                                                 */
/*****************************************************************************/

PUBLIC VOID
L2dsApiCreateContext (UINT4 u4ContextId)
{
    /* Set the Global status and MacVerification status */
    L2DS_ADMIN_STATUS (u4ContextId) = L2DS_DISABLED;
    L2DS_MAC_VERIFY_STATUS (u4ContextId) = L2DS_ENABLED;
    L2DS_IS_CONTEXT_EXIST (u4ContextId) = L2DS_TRUE;
    return;
}

/*****************************************************************************/
/* Function Name      : L2dsApiDeleteContext                                 */
/*                                                                           */
/* Description        : This function is invoked by l2iwf Module to Delete   */
/*                      Context                                              */
/*                                                                           */
/* Input(s)           : u4ContextId - Context Identifier                     */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None                                                 */
/*****************************************************************************/

PUBLIC VOID
L2dsApiDeleteContext (UINT4 u4ContextId)
{
    /* Set the Global status and MacVerification status */
    L2DS_ADMIN_STATUS (u4ContextId) = L2DS_DISABLED;
    L2DS_MAC_VERIFY_STATUS (u4ContextId) = L2DS_ENABLED;
    L2DS_IS_CONTEXT_EXIST (u4ContextId) = L2DS_FALSE;
    return;
}

/*                                                                           */
/***************************** END OF FILE ***********************************/
/*                                                                           */
