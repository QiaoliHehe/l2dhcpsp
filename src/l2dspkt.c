/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc. All Rights Reserved                       */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dspkt.c,v 1.9 2010/09/25 10:54:53 prabuc Exp $     */
/*****************************************************************************/
/*    FILE  NAME            : l2dspkt.c                                      */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snooping                                */
/*    MODULE NAME           : L2DHCP Snooping Packet handling Module         */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains the packet handling         */
/*                            functions for L2DHCP Snooping module           */
/*---------------------------------------------------------------------------*/

#include "l2dsinc.h"
#include "l2dsextn.h"
#include "fsvlan.h"
#include "ip6util.h"

/*****************************************************************************
 *
 *    Function Name        : L2dsPktHdl
 *
 *    Description        : This function handles the incoming DHCP Packet.
 *
 *    Input(s)		   : pBuf		- Incoming packet buffer
 * 						u4CxtId		- Context Identifier
 * 						u4InPort	- Incoming Port 
 * 						VlanTag		- VLAN tag informations of the packet 
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
L2dsPktHdl (tCRU_BUF_CHAIN_HEADER * pBuf, UINT4 u4CxtId, 
	UINT4 u4InPort, tVlanTag VlanTag)
{
	tL2DhcpSnpPktInfo		L2DS;

	tEnetV2Header			EthHdr;
    tL2DhcpSnpIfaceEntry 	L2DSVlanEntry;
    tL2DhcpSnpIfaceEntry 	*pL2DSVlanEntry = NULL;
	tVlanId					Vid = VlanTag.OuterVlanTag.u2VlanId;

	L2DS_TRC_ARG1(L2DS_TRC_FDB, 
		"[L2dsPktHdl]u4InPort = %d\r\n", u4InPort);

	/* Get dest mac and src mac address */
	MEMSET(&EthHdr, 0, sizeof(tEnetV2Header));
	L2DS_COPY_FROM_BUF(pBuf, EthHdr.au1DstAddr, 
		L2DS_ZERO, CFA_ENET_ADDR_LEN);
	L2DS_COPY_FROM_BUF(pBuf, EthHdr.au1SrcAddr, 
		CFA_ENET_ADDR_LEN, CFA_ENET_ADDR_LEN);

	/*begin extract pkts info processing*/
	MEMSET(gpau1L2dsDataBuffer, L2DS_ZERO, L2DS_MAX_MTU);
	CRU_BUF_Copy_FromBufChain (pBuf, (UINT1 *)gpau1L2dsDataBuffer, 
	L2DS_ZERO, CRU_BUF_Get_ChainValidByteCount(pBuf));
	MEMSET(&L2DS, L2DS_ZERO, sizeof (tL2DhcpSnpPktInfo));

	MEMCPY(&L2DS.EthHdr, &EthHdr, sizeof(tEnetV2Header));
	MEMCPY(&L2DS.VlanTag, &VlanTag, sizeof(tVlanTag));
	
	/*Convert this port to LA port if it's in LA groups */
	if(CmAPILaPortChannelForPortGetWithLock(u4InPort, 
		&L2DS.u2InPort) != ISS_SUCCESS)
	{
		L2DS.u2InPort = u4InPort;
	}

	/* Get the length of ethernet header (including VLAN tags) */
    if (L2dsPortGetEtherHdrLen(pBuf, u4InPort, &L2DS.u2EthHdrLen)
		!= L2DS_SUCCESS)
    {
        L2DS_TRC(L2DS_FAIL_TRC, 
			"[L2dsPktHdl]Get eth hdr length failed.\r\n");

        CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
        return L2DS_FAILURE;
    }
	
	if(L2dsPktGetPktInfo(gpau1L2dsDataBuffer, &L2DS) != L2DS_SUCCESS)
	{
		L2DS_TRC(L2DS_FAIL_TRC, 
			"[L2dsPktHdl]Extract DS packet failed.\r\n");

		CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
		return L2DS_FAILURE;
	}

    /* Get the type of the incoming port */
    if (L2dsPortGetPortType((UINT2)u4InPort, &L2DS.u1InPortType) 
		!= L2DS_SUCCESS)
    {
        L2DS_TRC_ARG1(L2DS_FAIL_TRC, "[L2dsPktHdl]"
			"Get port%d type  failed.\r\n", u4InPort);

        CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
        return L2DS_FAILURE;
    }

	/* Check the DHCP Snooping global status,
	* If status is disabled, broadcast the packet on the VLAN 
	*/
    if (L2DS_ADMIN_STATUS (u4CxtId) != L2DS_ENABLED)
    {
        L2DS_TRC(L2DS_FAIL_TRC, 
			"[L2dsPktHdl]Global DS status is disabled\r\n");
		
		/* CAMEOTAG: modify by linyu on 2013-03-20, fix packet duplicate when hw mirror packet  */
		/* packet released when  L2DS_SUCCESS, or not keep original process */
		if (CmL2dsPktReleaseForMirrored(pBuf, &L2DS) == L2DS_SUCCESS)
		{
			return L2DS_SUCCESS;
		}
        if (L2dsPortForwardPkt (pBuf, u4CxtId, VlanTag, u4InPort, 
			EthHdr.au1DstAddr, EthHdr.au1SrcAddr, NULL) != L2DS_SUCCESS)
        {
            CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
            return L2DS_FAILURE;
        }

        return L2DS_SUCCESS;
    }

	/* Get the corresponding VLAN interface entry info */
	MEMSET(&L2DSVlanEntry, L2DS_ZERO, sizeof (tL2DhcpSnpIfaceEntry));
    L2DSVlanEntry.u4L2dsCxtId = u4CxtId;
    L2DSVlanEntry.VlanId = Vid;
    pL2DSVlanEntry = (tL2DhcpSnpIfaceEntry *)RBTreeGet(
		L2DS_INTF_RBTREE, (tRBElem *) & L2DSVlanEntry);

    /* If there is no interface entry, broadcast the packet on the VLAN */
    if (pL2DSVlanEntry == NULL)
    {
        L2DS_TRC_ARG1(L2DS_FAIL_TRC, "[L2dsPktHdl]"
			"VLAN%d Interface entry does not exist.\r\n", Vid);
		/* packet released when  L2DS_SUCCESS, or not keep original process */
		if (CmL2dsPktReleaseForMirrored(pBuf, &L2DS) == L2DS_SUCCESS)
		{
			return L2DS_SUCCESS;
		}
        if (L2dsPortForwardPkt(pBuf, u4CxtId, VlanTag, u4InPort, 
			EthHdr.au1DstAddr, EthHdr.au1SrcAddr, NULL) != L2DS_SUCCESS)
        {
            CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
            return L2DS_FAILURE;
        }

        return L2DS_SUCCESS;
    }

	/* Check the rowstatus of the intercface entry
	* If status is not active, broadcast the packet on the VLAN 
	*/
    if (pL2DSVlanEntry->u1RowStatus != L2DS_ACTIVE)
    {
        L2DS_TRC_ARG1(L2DS_FAIL_TRC, "[L2dsPktHdl]"
			"VLAN%d Interface entry is not in active state.\r\n", Vid);
		/* packet released when  L2DS_SUCCESS, or not keep original process */
		if (CmL2dsPktReleaseForMirrored(pBuf, &L2DS) == L2DS_SUCCESS)
		{
			return L2DS_SUCCESS;
		}
        if (L2dsPortForwardPkt(pBuf, u4CxtId, VlanTag, u4InPort, 
			EthHdr.au1DstAddr, EthHdr.au1SrcAddr, NULL) != L2DS_SUCCESS)
        {
            CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
            return L2DS_FAILURE;
        }
        return L2DS_SUCCESS;
    }

    /* If L2 DHCP Snooping is not enabled on the VLAN, broadcast the packet on
     * the VLAN */
    if (pL2DSVlanEntry->u1VlanSnpStatus != L2DS_ENABLED)
    {
        L2DS_TRC_ARG1(L2DS_FAIL_TRC, 
			"[L2dsPktHdl] DS is not enabled on VLAN %d. \r\n", Vid);
		/* packet released when  L2DS_SUCCESS, or not keep original process */
		if (CmL2dsPktReleaseForMirrored(pBuf, &L2DS) == L2DS_SUCCESS)
		{
			return L2DS_SUCCESS;
		}
        if (L2dsPortForwardPkt(pBuf, u4CxtId, VlanTag, u4InPort, 
			EthHdr.au1DstAddr, EthHdr.au1SrcAddr, NULL) != L2DS_SUCCESS)
        {
            CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
            return L2DS_FAILURE;
        }

        return L2DS_SUCCESS;
    }

    CRU_BUF_Release_MsgBufChain (pBuf, FALSE);

	/* Get defalt forward portlist */
	if(CmL2dsGetBcPortlist(u4CxtId, gpau1L2dsDataBuffer, &L2DS)
		!= L2DS_SUCCESS)
	{
		L2DS_TRC(L2DS_FAIL_TRC, 
			"[L2dsPktHdl]Get trust port list failed.\r\n");
		return L2DS_FAILURE;
	}

	/* Handle packet */
    if(L2DS.u1InPortType == L2DS_DOWNSTREAM_PORT)
    {
		/* If the port is a downstream port, 
		* handle the packets as coming from a host 
		*/
		if(L2DS.u4IpType == IPVX_ADDR_FMLY_IPV4)/* IPV4 handler */
		{
			if(L2dsPktHdlV4FromDown(gpau1L2dsDataBuffer, 
				pL2DSVlanEntry, &L2DS) != L2DS_SUCCESS)
			{
				L2DS_TRC_ARG2(L2DS_FAIL_TRC, "[L2dsPktHdl]"
					"DHCPv4 failed, VLAN %d, Port %d. \r\n",
					Vid, u4InPort);
				return L2DS_FAILURE;
			}
		}
		else/* IPV6 handler */
		{
			if(L2dsPktHdlV6FromDown(gpau1L2dsDataBuffer, 
				pL2DSVlanEntry, &L2DS) != L2DS_SUCCESS)
			{
				L2DS_TRC_ARG2(L2DS_FAIL_TRC, "[L2dsPktHdl]"
					"DHCPv6 failed, VLAN %d, Port %d. \r\n",
					Vid, u4InPort);
				return L2DS_FAILURE;
			}
		}

    }
    else if(L2DS.u1InPortType == L2DS_UPSTREAM_PORT)
    {
		/* If the port is a upstream port, 
		* handle the packets as coming from the server
		*/
		if(L2DS.u4IpType == IPVX_ADDR_FMLY_IPV4)/* IPV4 handler */
		{
			if(L2dsPktHdlV4FromUp(gpau1L2dsDataBuffer, 
				pL2DSVlanEntry, &L2DS) != L2DS_SUCCESS)
	        {
				L2DS_TRC_ARG2(L2DS_FAIL_TRC, "[L2dsPktHdl]"
					"DHCPv4 failed, VLAN %d, Port %d. \r\n",
					Vid, u4InPort);

	            return L2DS_FAILURE;
	        }
		}
		else/* IPV6 handler */
		{
			if(L2dsPktHdlV6FromUp(gpau1L2dsDataBuffer, 
				pL2DSVlanEntry, &L2DS) != L2DS_SUCCESS)
	        {
				L2DS_TRC_ARG2(L2DS_FAIL_TRC, "[L2dsPktHdl]"
					"[%s:%d]DHCPv6 failed, VLAN %d, Port %d. \r\n",
					Vid, u4InPort);

	            return L2DS_FAILURE;
	        }
		}
    }

    return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : L2dsPktHdlV4FromDown
 *
 *    Description        :  This function handles the incoming DHCPv4 Packet, from 
 *						the hosts connected to downstream interface 
 *
 *    Input(s)		   : pu1RecvBuf           	- Incoming packet buffer
 * 					pL2DSVlanEntry 	- Interface entry
 * 					pL2DS			- DHCP Snp packet details info
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
L2dsPktHdlV4FromDown (UINT1 *pu1RecvBuf,
	tL2DhcpSnpIfaceEntry * pL2DSVlanEntry, 
	tL2DhcpSnpPktInfo * pL2DS)
{
	INT4				i4Ret;
	UINT4				u4CxtId;
	tL2DhcpSnpOption	L2DSOpt;

	u4CxtId = pL2DSVlanEntry->u4L2dsCxtId;
	
    /* If the incoming packet is not a packet which is supposed to come 
     * from a DHCP client, drop the packet 
     */
    switch(pL2DS->L2DSPkt.u1MsgType)
    {
    case L2DS_DHCP_DISCOVER:
    case L2DS_DHCP_DECLINE:
    case L2DS_DHCP_RELEASE:
    case L2DS_DHCP_INFORM:
    case L2DS_DHCP_REQUEST:
		CmL2dsFdbAdd(pL2DS->VlanTag.OuterVlanTag.u2VlanId,
			pL2DS->L2DSPkt.Chaddr, pL2DS->u2InPort);
        break;
		
    default:
        /* Other opcode packets not handle 
		* Update the statistics 
		*/
		/*CAMEOTAG: add by youchao 2013-1-18
		*add syslog for dhcpsnooping module 
		*/
		CMSYS_LOG_MSG(SYSLOG_WARN_LEVEL, gi4SysLogId,
		    SYSLOG_MSG_DHCPSNP_RECV_FROM_UNTRUST_PORT,
		    SYSLOG_KEY_PORT, &pL2DS->u2InPort);

		CmDhpSNPSnmpSendUntrustPortUtlTrap(pL2DS->u2InPort,
			(INT1 *)DHCP_SNP_UTIL_TRAPS_OID, (UINT1)DHCP_SNP_TRAPS_OID_LEN);

		L2DS_INCR_STAT_TOTAL_DISC(pL2DSVlanEntry);
		L2DS_INCR_STAT_SRV_DISC(pL2DSVlanEntry);
        return L2DS_FAILURE;
    }
	
    /* Update the VLAN statistics */
    L2dsPktStatUpd(pL2DS->L2DSPkt.u1MsgType, pL2DSVlanEntry);

	/* If the mac verify status is enabled, check the source MAC address 
	* against the client hardware address inside the DHCP Header 
	*/
	if(L2DS_MAC_VERIFY_STATUS(u4CxtId) == L2DS_ENABLED)
	{
		if(MEMCMP(pL2DS->EthHdr.au1SrcAddr,pL2DS->L2DSPkt.Chaddr, 
			sizeof(tMacAddr)) != L2DS_ZERO)
		{
			L2DS_TRC(L2DS_FAIL_TRC, "[L2dsPktHdlV4FromDown]MAC verify fail!\r\n");
			
			/* Update the statistics */
			L2DS_INCR_STAT_TOTAL_DISC(pL2DSVlanEntry);
			L2DS_INCR_STAT_MAC_DISC(pL2DSVlanEntry);
			return L2DS_FAILURE;
		}
	}

    /* Check whether the option 82 is already added or giaddr is set */
    if(pL2DS->L2DSPkt.u4Giaddr != L2DS_ZERO)
    {
        L2DS_TRC(L2DS_FAIL_TRC, "[L2dsPktHdlV4FromDown]"
			"Drop the pkt because Giaddr field is set!\r\n");

        /* Update the statistics */
        L2DS_INCR_STAT_TOTAL_DISC(pL2DSVlanEntry);
        L2DS_INCR_STAT_OPT_DISC(pL2DSVlanEntry);
        return L2DS_FAILURE;
    }

	MEMSET(&L2DSOpt, L2DS_ZERO, sizeof (tL2DhcpSnpOption));
    if(L2dsUtilGetOption (&(pL2DS->L2DSPkt),&L2DSOpt, L2DS_OPT_RAI)
		== L2DS_FOUND)
    {
		if(L2DS_FWD_OP82_STATUS(u4CxtId) != L2DS_ENABLED)
		{
			L2DS_TRC(L2DS_FAIL_TRC, "[L2dsPktHdlV4FromDown]"
				"Find Op82 in the packet!\r\n");

			/* Update the statistics */
			L2DS_INCR_STAT_TOTAL_DISC(pL2DSVlanEntry);
			L2DS_INCR_STAT_OPT_DISC(pL2DSVlanEntry);
			i4Ret = L2DS_FAILURE;
		}
		else
		{
			i4Ret = CmL2dsFwdPkt(u4CxtId, pu1RecvBuf, pL2DS);
		}
		
		return i4Ret;
    }

	/* Add option-82 and vendor specific options.
	* Even if the addition of options fail, 
	* we will send the packet out 
	*/
	if(L2DS_ADD_OP82_STATUS(u4CxtId) == L2DS_ENABLED)
	{
		CmL2dsPktMod(pu1RecvBuf, pL2DS, L2DS_OPTION_ADD);
	}

	if(CmL2dsUpdIPDB(u4CxtId, pL2DS, pu1RecvBuf) == L2DS_FAILURE)
	{
		return L2DS_FAILURE;
	}
	
	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : L2dsPktHdlV6FromDown
 *
 *    Description        :  This function handles the incoming DHCPv6 Packet, from 
 *						the hosts connected to downstream interface 
 *
 *    Input(s)		   : pu1RecvBuf           	- Incoming packet buffer
 * 					pL2DSVlanEntry 	- Interface entry
 * 					pL2DS			- DHCP Snp packet details info
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
L2dsPktHdlV6FromDown (UINT1 *pu1RecvBuf,
	tL2DhcpSnpIfaceEntry * pL2DSVlanEntry, 
	tL2DhcpSnpPktInfo * pL2DS)
{
	UINT4	u4CxtId = pL2DSVlanEntry->u4L2dsCxtId;

    switch(pL2DS->L2DS6Pkt.u1MsgType)
    {
    case L2DS_DHCP6_SOLICIT:
    case L2DS_DHCP6_REQUEST:
    case L2DS_DHCP6_CONFIRM:
    case L2DS_DHCP6_RENEW:
    case L2DS_DHCP6_REBIND:
	case L2DS_DHCP6_RELEASE:
	case L2DS_DHCP6_DECLINE:
	case L2DS_DHCP6_INFO_REQUEST:
	case L2DS_DHCP6_RELAY_FWD:
	case L2DS_DHCP6_RELAY_REPLY:
		CmL2dsFdbAdd(pL2DS->VlanTag.OuterVlanTag.u2VlanId,
			pL2DS->L2DS6Pkt.Chaddr, pL2DS->u2InPort);
        break;

    default:
		/*CAMEOTAG: add by youchao 2013-1-18 
		* add syslog for dhcpsnooping module 
		*/
        CMSYS_LOG_MSG(SYSLOG_WARN_LEVEL, gi4SysLogId,
            SYSLOG_MSG_DHCPSNP_RECV_FROM_UNTRUST_PORT,
            SYSLOG_KEY_PORT, &pL2DS->u2InPort);

		CmDhpSNPSnmpSendUntrustPortUtlTrap(pL2DS->u2InPort,
			(INT1 *)DHCP_SNP_UTIL_TRAPS_OID, (UINT1)DHCP_SNP_TRAPS_OID_LEN);
        return L2DS_FAILURE;
    }

    /* TODO: Update the statistics of DHCP6 packets */
    /* L2dsPktStatUpd(pL2DS->u1PktType, pL2DSVlanEntry); */

	/* If the mac verify status is enabled, check the source MAC address 
	* against the client hardware address inside the DHCP Header 
	*/
	if(L2DS_MAC_VERIFY_STATUS(u4CxtId) == L2DS_ENABLED)
	{
		if(L2DS_ZERO != MEMCMP(pL2DS->EthHdr.au1SrcAddr,
			pL2DS->L2DS6Pkt.Chaddr, sizeof(tMacAddr)))
		{
			L2DS_TRC(L2DS_FAIL_TRC, "[L2dsPktHdlV6FromDown]MAC verify fail!\r\n");
			
			/* TODO: Update the statistics of DHCP6 packets */
			return L2DS_FAILURE;
		}
	}

	/* Update IPDB */
    if (CmL2dsUpdIPDB(u4CxtId, pL2DS, pu1RecvBuf) == L2DS_FAILURE)
    {
    	return L2DS_FAILURE;
    }
	
	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : L2dsPktHdlV4FromUp
 *
 *    Description        :  This function handles the incoming DHCPv4 Packet, from 
 *						the hosts connected to upstream interface 
 *
 *    Input(s)		   : pu1RecvBuf           	- Incoming packet buffer
 * 					pL2DSVlanEntry 	- Interface entry
 * 					pL2DS			- DHCP Snp packet details info
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
L2dsPktHdlV4FromUp (UINT1 *pu1RecvBuf,
	tL2DhcpSnpIfaceEntry * pL2DSVlanEntry, 
	tL2DhcpSnpPktInfo * pL2DS)
{
	INT4		i4OP82Found	= L2DS_NOT_FOUND;
	UINT4		u4CxtId			= pL2DSVlanEntry->u4L2dsCxtId;
    tL2DhcpSnpOption    L2DSOpt;

    switch(pL2DS->L2DSPkt.u1MsgType)
    {
    case L2DS_DHCP_OFFER:
    case L2DS_DHCP_ACK: 
	case L2DS_DHCP_NACK:
		if(CmL2dsGetUcPortlist(u4CxtId, pu1RecvBuf, pL2DS) 
			!= L2DS_SUCCESS)
		{
			return L2DS_FAILURE;
		}
		break;

    default:
		/* Forward these packets */	
		return CmL2dsFwdPkt(u4CxtId, pu1RecvBuf, pL2DS);
    }

	/* Check whether have op82 in the packet */
	i4OP82Found = L2dsUtilGetOption (&pL2DS->L2DSPkt,
			&L2DSOpt, L2DS_OPT_RAI);

    /*CAMEOTAG:Modify by Lingling on 2015-7-9
         *Fix bug:When option 82 insert status is enabled, although the dhcpv4 client get IP from server, 
         *the IPV4 entry can not be added to dhcp white list; Here the check has no sense, so remove it. 
         */
#if 0
	/* Forward the packet when following conditions happen */
	if((L2DS_ADD_OP82_STATUS(u4CxtId) == L2DS_ENABLED 
		&& L2DS_NOT_FOUND == i4OP82Found)					||
		(L2DS_ADD_OP82_STATUS(u4CxtId) != L2DS_ENABLED
		&& L2DS_FOUND == i4OP82Found))
	{
		return CmL2dsFwdPkt(u4CxtId, pu1RecvBuf, pL2DS);
	}
#endif

	/* Update the VLAN statistics */
	L2dsPktStatUpd(pL2DS->L2DSPkt.u1MsgType, pL2DSVlanEntry);

	if(pL2DS->L2DSPkt.u1MsgType == L2DS_DHCP_ACK)
	{
		CmL2dsGetLTime(pL2DS);
		CmL2dsGetGw(pL2DS);
	}

    /* get the option 82 from the DHCP Packet */
    if(i4OP82Found == L2DS_FOUND && L2DS_ADD_OP82_STATUS(u4CxtId) == L2DS_ENABLED)
	{
		/* We will remove the options and sent the packet out . */
		CmL2dsPktMod(pu1RecvBuf, pL2DS, L2DS_OPTION_DEL);
	}

	if(CmL2dsUpdIPDB(u4CxtId, pL2DS, pu1RecvBuf) == L2DS_FAILURE)
	{
		return L2DS_FAILURE;
	}
	
	return L2DS_SUCCESS;
}
#if 0
/*****************************************************************************/
/* Function Name      : L2dsPktGetPktInfo                                    */
/*                                                                           */
/* Description        : This function do get the informations from the DHCP  */
/*                      packet                                               */
/*                                                                           */
/* Input(s)           : pu1RecvBuf            - Incoming packet buffer       */
/*                      u4InPort              - Incoming Port                */
/*                      VlanId                - Incoming VLAN ID             */
/*                      u2EtherOffset         - Ethernet Header length       */
/*                                                                           */
/* Output(s)          : pL2DhcpSnpPktInfo     - packet informations          */
/*                      pu4PktLength          - total packet length          */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPktGetPktInfo (UINT1 *pu1RecvBuf, tL2DhcpSnpPktInfo * pL2DhcpSnpPktInfo,
                   UINT4 u4InPort, tVlanId VlanId, UINT2 u2EtherOffset,
                   UINT4 *pu4PktLength)
{
    tL2DhcpSnpOption    L2DhcpSnpOption;
    UINT1              *pu1PktOffset = NULL;
    UINT2               u2Offset = L2DS_ZERO;
    UINT2               u2IpPktLen = L2DS_ZERO;
    UINT1               u1IpHdrLen = L2DS_ZERO;

    L2DS_TRC_ARG1 (L2DS_TRC_FLAG, L2DS_FN_ENTRY, L2DS_MODULE_NAME,
                   "L2dsPktGetPktInfo:Entering the function %s \r\n",
                   __FUNCTION__);

    MEMSET (&L2DhcpSnpOption, L2DS_ZERO, sizeof (tL2DhcpSnpOption));
    MEMSET (pL2DhcpSnpPktInfo, L2DS_ZERO, sizeof (tL2DhcpSnpPktInfo));

    pL2DhcpSnpPktInfo->u4PortNumber = u4InPort;
    pL2DhcpSnpPktInfo->VlanId = VlanId;
    pL2DhcpSnpPktInfo->u1EtherHdrLen = (UINT1) u2EtherOffset;

    /* Get the destination mac from the ethernet header */
    MEMCPY (pL2DhcpSnpPktInfo->DstMacAddress, pu1RecvBuf, sizeof (tMacAddr));

    /* Get the source mac from the ethernet header */
    u2Offset = L2DS_SRCMAC_OFFSET;
    pu1PktOffset = pu1RecvBuf + u2Offset;

    MEMCPY (pL2DhcpSnpPktInfo->SrcMacAddress, pu1PktOffset, sizeof (tMacAddr));

    /* Get the IP header length, which will be the last four bits of the first 
     * byte of the IP Header */
    u1IpHdrLen = *(pu1RecvBuf + u2EtherOffset);
    u1IpHdrLen = (UINT1) ((u1IpHdrLen & L2DS_IP_HDRLEN_BITMASK) *
                          L2DS_BYTE_IN_WORD);

    pL2DhcpSnpPktInfo->u1IpHdrLen = u1IpHdrLen;

    /* Get the total length of the Packet */
    u2Offset = (UINT2) L2DS_IP_LENGTH_OFFSET (pL2DhcpSnpPktInfo);
    pu1PktOffset = pu1RecvBuf + u2Offset;

    MEMCPY (&u2IpPktLen, pu1PktOffset, L2DS_IP_DGRAM_LEN);
    u2IpPktLen = OSIX_NTOHS (u2IpPktLen);

    *pu4PktLength = (UINT4) u2EtherOffset + (UINT4) u2IpPktLen;

    /* Get the Src IP address and Destination IP Address 
     * (This is for calculating checksum) */
    u2Offset = (UINT2) L2DS_IPSRC_ADDR_OFFSET (pL2DhcpSnpPktInfo);
    pu1PktOffset = pu1RecvBuf + u2Offset;

    MEMCPY ((UINT1 *) &(pL2DhcpSnpPktInfo->u4SrcAddr), pu1PktOffset,
            sizeof (UINT4));
    pL2DhcpSnpPktInfo->u4SrcAddr = OSIX_NTOHL (pL2DhcpSnpPktInfo->u4SrcAddr);

    u2Offset = (UINT2) L2DS_IPDST_ADDR_OFFSET (pL2DhcpSnpPktInfo);
    pu1PktOffset = pu1RecvBuf + u2Offset;

    MEMCPY ((UINT1 *) &(pL2DhcpSnpPktInfo->u4DstAddr), pu1PktOffset,
            sizeof (UINT4));
    pL2DhcpSnpPktInfo->u4DstAddr = OSIX_NTOHL (pL2DhcpSnpPktInfo->u4DstAddr);

    /* Verify Magic Cookie */
    u2Offset = (UINT2) L2DS_MAGIC_COOKIE_OFFSET (pL2DhcpSnpPktInfo);
    pu1PktOffset = pu1RecvBuf + u2Offset;

    if (L2dsUtilVerifyCookie (pu1PktOffset) != L2DS_SUCCESS)
    {
        L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                  "L2dsPktGetPktInfo:Verification of Magic Cookie failed.\r\n");

        return L2DS_FAILURE;
    }

    /* Copy the details in the DHCP Header */
    u2Offset = (UINT2) L2DS_DHCP_HDR_OFFSET (pL2DhcpSnpPktInfo);
    pu1PktOffset = pu1RecvBuf + u2Offset;

    L2dsPktCopyHeader (&(pL2DhcpSnpPktInfo->L2DhcpSnpPacket), pu1PktOffset,
                       (*pu4PktLength - u2Offset));

    if (L2dsUtilGetOption (&(pL2DhcpSnpPktInfo->L2DhcpSnpPacket),
                           &L2DhcpSnpOption, L2DS_OPT_MSG_TYPE) != L2DS_FOUND)
    {
        L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                  "L2dsPktGetPktInfo:Message type option not present."
                  "so return Failure.\r\n");
        return L2DS_FAILURE;
    }

    pL2DhcpSnpPktInfo->u1PktType = L2DhcpSnpOption.pu1Val[L2DS_ZERO];
    MEMSET (L2DhcpSnpOption.pu1Val, L2DS_ZERO, L2DS_OPTION_LEN);

    /* update the statistics */
    L2DS_TRC_ARG1 (L2DS_TRC_FLAG, L2DS_FN_EXIT, L2DS_MODULE_NAME,
                   "L2dsPktGetPktInfo:Exiting the function %s \r\n",
                   __FUNCTION__);
    return L2DS_SUCCESS;
}
#endif

/*****************************************************************************
 *
 *    Function Name        : L2dsPktHdlV6FromUp
 *
 *    Description        :  This function handles the incoming DHCPv6 Packet, from 
 *						the hosts connected to upstream interface 
 *
 *    Input(s)		   : pu1RecvBuf           	- Incoming packet buffer
 * 					pL2DSVlanEntry 	- Interface entry
 * 					pL2DS			- DHCP Snp packet details info
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
L2dsPktHdlV6FromUp (UINT1 *pu1RecvBuf,
	tL2DhcpSnpIfaceEntry * pL2DSVlanEntry, 
	tL2DhcpSnpPktInfo * pL2DS)
{
	UINT4	u4CxtId = pL2DSVlanEntry->u4L2dsCxtId;

    switch(pL2DS->L2DS6Pkt.u1MsgType)
    {
    case L2DS_DHCP6_ADVERTISE:
    case L2DS_DHCP6_REPLY:
    case L2DS_DHCP6_RECONFIG:
		if(CmL2dsGetUcPortlist(u4CxtId, pu1RecvBuf, pL2DS) 
			!= L2DS_SUCCESS)
		{
			return L2DS_FAILURE;
		}
		break;
		
    default:
		/* Forward these packets */
		return CmL2dsFwdPkt(u4CxtId, pu1RecvBuf, pL2DS);
    }

	if(CmL2dsUpdIPDB(u4CxtId, pL2DS, pu1RecvBuf) == L2DS_FAILURE)
	{
		return L2DS_FAILURE;
	}

    return L2DS_SUCCESS;
}
#if 0
/*****************************************************************************/
/* Function Name      : L2dsPktUpdateStat                                    */
/*                                                                           */
/* Description        : This function is to update VLAN statistics           */
/*                                                                           */
/* Input(s)           : u1PktType      - Incoming packet type                */
/*                      u4ContextId    - Context Identifier                  */
/*                      VlanId         - Incoming Vlan Id                    */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None                                                 */
/*****************************************************************************/
VOID
L2dsPktUpdateStat (UINT1 u1PktType, UINT4 u4ContextId, tVlanId VlanId)
{
    tL2DhcpSnpIfaceEntry L2DhcpSnpIfaceEntry;
    tL2DhcpSnpIfaceEntry *pL2DhcpSnpIfaceEntry = NULL;

    MEMSET (&L2DhcpSnpIfaceEntry, L2DS_ZERO, sizeof (tL2DhcpSnpIfaceEntry));

    L2DhcpSnpIfaceEntry.u4L2dsCxtId = u4ContextId;
    L2DhcpSnpIfaceEntry.VlanId = VlanId;

    /* Get the corresponding VLAN interface entry */
    pL2DhcpSnpIfaceEntry = (tL2DhcpSnpIfaceEntry *)
        RBTreeGet (L2DS_INTF_RBTREE, (tRBElem *) & L2DhcpSnpIfaceEntry);

    if (pL2DhcpSnpIfaceEntry != NULL)
    {
        switch (u1PktType)
        {
            case L2DS_DHCP_DISCOVER:
            {
                L2DS_INCR_STAT_RX_DISC (pL2DhcpSnpIfaceEntry);
                break;
            }
            case L2DS_DHCP_REQUEST:
            {
                L2DS_INCR_STAT_RX_REQS (pL2DhcpSnpIfaceEntry);
                break;
            }
            case L2DS_DHCP_RELEASE:
            {
                L2DS_INCR_STAT_RX_RELS (pL2DhcpSnpIfaceEntry);
                break;
            }
            case L2DS_DHCP_DECLINE:
            {
                L2DS_INCR_STAT_RX_DECL (pL2DhcpSnpIfaceEntry);
                break;
            }
            case L2DS_DHCP_INFORM:
            {
                L2DS_INCR_STAT_RX_INTF (pL2DhcpSnpIfaceEntry);
                break;
            }
            case L2DS_DHCP_OFFER:
            {
                L2DS_INCR_STAT_TX_OFFR (pL2DhcpSnpIfaceEntry);
                break;
            }
            case L2DS_DHCP_ACK:
            {
                L2DS_INCR_STAT_TX_ACKS (pL2DhcpSnpIfaceEntry);
                break;
            }
            case L2DS_DHCP_NACK:
            {
                L2DS_INCR_STAT_TX_NAKS (pL2DhcpSnpIfaceEntry);
                break;
            }
            default:
            {
                break;
            }
        }
        return;
    }
#endif

/*****************************************************************************
 *
 *    Function Name        : L2dsPktStatUpd
 *
 *    Description        :  This function is to update VLAN statistics according incoming
 *						DHCP packet type
 *
 *    Input(s)		   : u1PktType 			- packet type 
 * 					pL2DSVlanEntry	- Incoming Vlan interface entry
 *
 *    Output(s)            : None.
 *
 *    Returns            : None
 *
 *****************************************************************************/
VOID
L2dsPktStatUpd (UINT1 u1PktType, tL2DhcpSnpIfaceEntry * pL2DSVlanEntry)
{
    switch (u1PktType)
    {
        case L2DS_DHCP_DISCOVER:
        {
            L2DS_INCR_STAT_RX_DISC (pL2DSVlanEntry);
            break;
        }
        case L2DS_DHCP_REQUEST:
        {
            L2DS_INCR_STAT_RX_REQS (pL2DSVlanEntry);
            break;
        }
        case L2DS_DHCP_RELEASE:
        {
            L2DS_INCR_STAT_RX_RELS (pL2DSVlanEntry);
            break;
        }
        case L2DS_DHCP_DECLINE:
        {
            L2DS_INCR_STAT_RX_DECL (pL2DSVlanEntry);
            break;
        }
        case L2DS_DHCP_INFORM:
        {
            L2DS_INCR_STAT_RX_INTF (pL2DSVlanEntry);
            break;
        }
        case L2DS_DHCP_OFFER:
        {
            L2DS_INCR_STAT_TX_OFFR (pL2DSVlanEntry);
            break;
        }
        case L2DS_DHCP_ACK:
        {
            L2DS_INCR_STAT_TX_ACKS (pL2DSVlanEntry);
            break;
        }
        case L2DS_DHCP_NACK:
        {
            L2DS_INCR_STAT_TX_NAKS (pL2DSVlanEntry);
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}

/*****************************************************************************
 *
 *    Function Name        : L2dsPktGetPktInfo
 *
 *    Description        :  This function do get the informations from the DHCP packet
 *
 *    Input(s)		   : pu1RecvBuf 		- Incoming packet buffer
 * 						pL2DS              - DHCP packet information
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
L2dsPktGetPktInfo(UINT1 *pu1RecvBuf, tL2DhcpSnpPktInfo * pL2DS)
{
	INT4	i4Ret 			= L2DS_SUCCESS;
    UINT1	*pu1PktOffset 	= pu1RecvBuf;

	/* Get IP type:ipv4 or ipv6 */
	pu1PktOffset += pL2DS->u2EthHdrLen;

	if((*pu1PktOffset & L2DS_IP_IPTYPE_BITMASK) == 0x40)
	{
		pL2DS->u4IpType = IPVX_ADDR_FMLY_IPV4;
		
		i4Ret = CmL2dsGetIpv4Info(pu1PktOffset, pL2DS);
	}
	else if((*pu1PktOffset & L2DS_IP_IPTYPE_BITMASK) == 0x60)
	{
		pL2DS->u4IpType = IPVX_ADDR_FMLY_IPV6;

		i4Ret = CmL2dsGetIpv6Info(pu1PktOffset, pL2DS);
	}
	else
	{
		L2DS_TRC_ARG1(L2DS_FAIL_TRC, "[L2dsPktGetPktInfo]"
			"IP type %02x failed.\r\n", *pu1PktOffset);
		i4Ret = L2DS_FAILURE;
	}

    return i4Ret;
}
#if 0
/*****************************************************************************/
/* Function Name      : L2dsPktCopyHeader                                    */
/*                                                                           */
/* Description        : This function is to copy DHCP header informations    */
/*                                                                           */
/* Input(s)           : pu1DhcpBuf  - Incoming DHCP packet buffer            */
/*                      u4DhcpPktLength - DHCP packet length                 */
/*                                                                           */
/* Output(s)          : pL2DhcpSnpPacket - DHCP Header informations          */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPktCopyHeader (tL2DhcpSnpPacket * pL2DhcpSnpPacket, UINT1 *pu1DhcpBuf,
                   UINT4 u4DhcpPktLength)
{
    UINT2               u2Offset = L2DS_ZERO;

    /* Copy the OP code */
    pL2DhcpSnpPacket->u1Op = pu1DhcpBuf[u2Offset];

    pL2DhcpSnpPacket->u2OptLen = (UINT2) (u4DhcpPktLength -
                                          L2DS_HDR_LEN_WITH_COOKIE);

    /* Copy the transaction ID */
    MEMCPY (&(pL2DhcpSnpPacket->u4Xid), (pu1DhcpBuf + L2DS_XID_OFFSET),
            L2DS_XID_LEN);

    pL2DhcpSnpPacket->u4Xid = L2DS_NTOHL (pL2DhcpSnpPacket->u4Xid);

    /* Copy the client address */
    MEMCPY (&(pL2DhcpSnpPacket->u4Ciaddr), (pu1DhcpBuf + L2DS_CIADDR_OFFSET),
            L2DS_IP_ADDR_LEN);

    pL2DhcpSnpPacket->u4Ciaddr = L2DS_NTOHL (pL2DhcpSnpPacket->u4Ciaddr);

    /* copy the 'your' client address field */
    MEMCPY (&(pL2DhcpSnpPacket->u4Yiaddr), (pu1DhcpBuf + L2DS_YIADDR_OFFSET),
            L2DS_IP_ADDR_LEN);

    pL2DhcpSnpPacket->u4Yiaddr = L2DS_NTOHL (pL2DhcpSnpPacket->u4Yiaddr);

    /* copy the server address field */
    MEMCPY (&(pL2DhcpSnpPacket->u4Siaddr), (pu1DhcpBuf + L2DS_SIADDR_OFFSET),
            L2DS_IP_ADDR_LEN);

    pL2DhcpSnpPacket->u4Siaddr = L2DS_NTOHL (pL2DhcpSnpPacket->u4Siaddr);

    /* Copy the relay agent address field */
    MEMCPY (&(pL2DhcpSnpPacket->u4Giaddr), (pu1DhcpBuf + L2DS_GIADDR_OFFSET),
            L2DS_IP_ADDR_LEN);

    pL2DhcpSnpPacket->u4Giaddr = L2DS_NTOHL (pL2DhcpSnpPacket->u4Giaddr);

    /* Copy the client hardware address field */
    MEMCPY (&(pL2DhcpSnpPacket->Chaddr), (pu1DhcpBuf + L2DS_CHADDR_OFFSET),
            sizeof (tMacAddr));

    /* Copy the sname field */
    MEMCPY (&(pL2DhcpSnpPacket->u1Sname), (pu1DhcpBuf + L2DS_SNAME_OFFSET),
            L2DS_SNAME_LEN);

    /* copy the file field */
    MEMCPY (&(pL2DhcpSnpPacket->u1File), (pu1DhcpBuf + L2DS_FILE_OFFSET),
            L2DS_FILE_LEN);

    /* copy the options */
    MEMCPY (&(pL2DhcpSnpPacket->au1Options),
            (pu1DhcpBuf + L2DS_HDR_LEN_WITH_COOKIE),
            (u4DhcpPktLength - L2DS_HDR_LEN_WITH_COOKIE));

    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsPktProcessPktFromSrvr                            */
/*                                                                           */
/* Description        : This function handles the incoming DHCP Packet, from */
/*                      the servers connected to upstream interface          */
/*                                                                           */
/* Input(s)           : pu1RecvBuf   - Incoming packet buffer                */
/*                      u4ContextId  - Context Identifier                    */
/*                      u4InPort     - Incoming Port                         */
/*                      VlanTag      - VLAN tag informations of the packet   */
/*                      u2EtherOffset- Ethernet header length                */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPktProcessPktFromSrvr (UINT1 *pu1RecvBuf, UINT4 u4ContextId, UINT4 u4InPort,
                           tVlanTag VlanTag, UINT2 u2EtherOffset)
{

    tL2DhcpSnpPktInfo  *pL2DhcpSnpPktInfo = NULL;
    tL2DhcpSnpOption    L2DhcpSnpOption;
    UINT1               au1AccessNode[L2DS_ACCESS_NODE_ARRAY_LEN] =
        { L2DS_ZERO };
    UINT1               au1AccessNodeId[L2DS_MAX_OPT_LEN] = { L2DS_ZERO };
    tPortList          *pUpstreamPortList = NULL;
    tVlanId             CVlanId = L2DS_ZERO;
    tVlanId             SVlanId = L2DS_ZERO;
    UINT4               u4OutPort = L2DS_ZERO;
    UINT4               u4PktLength = L2DS_ZERO;
    UINT1               u1AccessNodeIdLen = L2DS_ZERO;
    UINT1              *pu1AccessNodeName = NULL;
    tCRU_BUF_CHAIN_HEADER *pBuf = NULL;

    if ((pL2DhcpSnpPktInfo = (tL2DhcpSnpPktInfo *)
         (L2DS_ALLOC_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID))) == NULL)
    {
        L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                  "L2dsPktProcessPktFromSrvr:Memory Allocation failed for Message"
                  "\r\n");
        return L2DS_FAILURE;
    }

    MEMSET (pL2DhcpSnpPktInfo, L2DS_ZERO, sizeof (tL2DhcpSnpPktInfo));
    MEMSET (&L2DhcpSnpOption, L2DS_ZERO, sizeof (tL2DhcpSnpOption));

    SVlanId = VlanTag.OuterVlanTag.u2VlanId;

    L2DS_TRC_ARG1 (L2DS_TRC_FLAG, L2DS_FN_ENTRY, L2DS_MODULE_NAME,
                   "L2dsPktProcessPktFromSrvr:Entering the function %s \r\n",
                   __FUNCTION__);

    L2DS_TRC_ARG4 (L2DS_TRC_FLAG, L2DS_FN_ARGS, L2DS_MODULE_NAME,
                   "L2dsPktProcessPktFromSrvr:The arguments to the function %s "
                   "are VlanId %d, InPort %d, Ethernet header length %d and "
                   "address of received buffer\r\n", __FUNCTION__, SVlanId,
                   u4InPort, u2EtherOffset);

    /* Extract the information from the packet */
    if (L2dsPktGetPktInfo (pu1RecvBuf, pL2DhcpSnpPktInfo, u4InPort, SVlanId,
                           u2EtherOffset, &u4PktLength) != L2DS_SUCCESS)
    {
        L2DS_TRC_ARG2 (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                       "L2dsPktProcessPktFromSrvr:Getting informations of the "
                       "incoming DHCP Packet on VLAN %d, Port %d failed.\r\n",
                       SVlanId, u4InPort);
        L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
        return L2DS_FAILURE;
    }

    if ((pL2DhcpSnpPktInfo->u1PktType == L2DS_DHCP_DISCOVER) ||
        (pL2DhcpSnpPktInfo->u1PktType == L2DS_DHCP_RELEASE) ||
        (pL2DhcpSnpPktInfo->u1PktType == L2DS_DHCP_INFORM))
    {
        /* If packet is any of the Client packets, forward it through
         * other upstream interfaces */

        /* Convert the packet to CRU-buffer */
        pBuf = L2dsUtilLinearToCRU (pu1RecvBuf, u4PktLength);
        if (pBuf == NULL)
        {
            L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                      "L2dsPktProcessPktFromSrvr:Unable to convert the linear "
                      "buffer to CRU-buffer\r\n");
            L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
            return L2DS_FAILURE;
        }

        if ((pUpstreamPortList = (tPortList *)
             (FsUtilAllocBitList (sizeof (tPortList)))) == NULL)
        {
            L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                      "L2dsPktProcessPktFromSrvr:Memory Allocation failed for Message"
                      "\r\n");
            L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
            /* Release the CRU Buffer */
            CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
            return L2DS_FAILURE;
        }
        MEMSET (pUpstreamPortList, L2DS_ZERO, sizeof (tPortList));

        /* For a broadcast packet, get the upstream list and sent the
         * packet out through all the upstream ports, exept through which 
         * the packet came. If it is unicast, sent the packet out through
         * the specified interface */

        if (L2dsPortGetUpStreamList (u4ContextId, SVlanId, *pUpstreamPortList)
            != L2DS_SUCCESS)
        {
            L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                      "L2dsPktProcessPktFromSrvr:Unable to get the upstream "
                      "portlist\r\n");

            /* Release the CRU Buffer */
            CRU_BUF_Release_MsgBufChain (pBuf, FALSE);

            FsUtilReleaseBitList ((UINT1 *) pUpstreamPortList);
            L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
            return L2DS_FAILURE;
        }

        /* exclude the incoming upstream port */
        OSIX_BITLIST_RESET_BIT (*pUpstreamPortList, u4InPort,
                                sizeof (tPortList));

        /* Send the packet out through the remaining upstream interface(s) */
        if (L2dsPortForwardPkt (pBuf, u4ContextId, VlanTag, u4InPort,
                                pL2DhcpSnpPktInfo->DstMacAddress,
                                pL2DhcpSnpPktInfo->SrcMacAddress,
                                *pUpstreamPortList) != L2DS_SUCCESS)
        {
            L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                      "L2dsPktProcessPktFromSrvr:Unable to send the packet "
                      "out through upstream interfaces.\r\n");

            /* Release the CRU Buffer */
            CRU_BUF_Release_MsgBufChain (pBuf, FALSE);

            FsUtilReleaseBitList ((UINT1 *) pUpstreamPortList);
            L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
            return L2DS_FAILURE;
        }
        L2DS_TRC_ARG1 (L2DS_TRC_FLAG, L2DS_FN_EXIT, L2DS_MODULE_NAME,
                       "L2dsPktProcessPktFromSrvr:Exiting the function %s \r\n",
                       __FUNCTION__);
        L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
        FsUtilReleaseBitList ((UINT1 *) pUpstreamPortList);
        return L2DS_SUCCESS;
    }

    /* get the option 82 from the DHCP Packet */
    if (L2dsUtilGetOption (&(pL2DhcpSnpPktInfo->L2DhcpSnpPacket),
                           &L2DhcpSnpOption, L2DS_OPT_RAI) == L2DS_FOUND)
    {
        if (L2dsUtilValidateFreeString (&L2DhcpSnpOption, &u4OutPort)
            != L2DS_SUCCESS)
        {
            /* TBD: Remote Agent ID validation */
            /* caliing a function to Get AccessNodeIdentifier */
            pu1AccessNodeName = DcsGetAccessNodeIdentifier (au1AccessNode);
            u1AccessNodeIdLen = (UINT1) STRLEN (pu1AccessNodeName);

            /* Get the Access node Identifier in the Circuit ID */
            if (L2dsUtilGetAccessNodeId (&L2DhcpSnpOption, u1AccessNodeIdLen,
                                         au1AccessNodeId) != L2DS_SUCCESS)
            {
                /* DHCP server does not understands option-82.That time it will
                 * not echo back the option-82 {circuit-id and Remote-id}.Server
                 * is not acquiring IP to HOST.
                 * So broadcast the packet on that VLAN */

                MEMSET (L2DhcpSnpOption.pu1Val, L2DS_ZERO, L2DS_OPTION_LEN);

                /* Convert the packet to CRU-buffer */
                pBuf = L2dsUtilLinearToCRU (pu1RecvBuf, u4PktLength);
                if (pBuf == NULL)
                {
                    L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                              "L2dsPktProcessPktFromSrvr:Unable to convert the"
                              "linear buffer to CRU-buffer\r\n");

                    L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID,
                                            pL2DhcpSnpPktInfo);
                    return L2DS_FAILURE;
                }

                if (L2dsPortForwardPkt (pBuf, u4ContextId, VlanTag, u4InPort,
                                        pL2DhcpSnpPktInfo->DstMacAddress,
                                        pL2DhcpSnpPktInfo->SrcMacAddress,
                                        NULL) != L2DS_SUCCESS)
                {
                    L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                              "L2dsPktProcessPktFromSrvr:Unable"
                              "to send the packet out\r\n");

                    /* Release the CRU Buffer */
                    CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
                    L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID,
                                            pL2DhcpSnpPktInfo);
                    return L2DS_FAILURE;
                }
                L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID,
                                        pL2DhcpSnpPktInfo);
                return L2DS_SUCCESS;
            }

            /* Check whether the remote Id and Access node id is added 
             * by us */
            if (MEMCMP (au1AccessNodeId, pu1AccessNodeName, u1AccessNodeIdLen)
                != L2DS_ZERO)
            {
                /* The options are not added by us */
                /* So forward the packet on all trusted port as well as on 
                 * remaining upstream ports */
                if (L2dsPktFwdOnTrusted
                    (pu1RecvBuf, u4ContextId, u4InPort, VlanTag,
                     pL2DhcpSnpPktInfo->DstMacAddress,
                     pL2DhcpSnpPktInfo->SrcMacAddress,
                     u4PktLength) != L2DS_SUCCESS)
                {
                    MEMSET (L2DhcpSnpOption.pu1Val, L2DS_ZERO, L2DS_OPTION_LEN);
                    L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC,
                              L2DS_MODULE_NAME, "L2dsPktProcessPktFromSrvr:"
                              "Unable to send the packet out through "
                              "upstream and trusted interfaces\r\n");
                    L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID,
                                            pL2DhcpSnpPktInfo);
                    return L2DS_FAILURE;
                }
                L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID,
                                        pL2DhcpSnpPktInfo);
                return L2DS_SUCCESS;
            }

            /* Options are added by us */
            /* Get the port, on which the packet is to be sent, from options */
            if (L2dsUtilGetPortAndVlanFromOption (&L2DhcpSnpOption, &u4OutPort,
                                                  &CVlanId) != L2DS_SUCCESS)
            {
                MEMSET (L2DhcpSnpOption.pu1Val, L2DS_ZERO, L2DS_OPTION_LEN);
                L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                          "L2dsPktProcessPktFromSrvr:Unable to get the outport "
                          "from the options\r\n");
                L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID,
                                        pL2DhcpSnpPktInfo);
                return L2DS_FAILURE;
            }
        }

        /* Remove the options and sent the packet out */
        if (L2dsPktHandlePktFromSrvr
            (pu1RecvBuf, u4ContextId, u4OutPort, VlanTag, CVlanId, u4PktLength,
             pL2DhcpSnpPktInfo) != L2DS_SUCCESS)
        {
            MEMSET (L2DhcpSnpOption.pu1Val, L2DS_ZERO, L2DS_OPTION_LEN);
            L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                      "L2dsPktProcessPktFromSrvr:Unable to remove options and "
                      "send packet out\r\n");
            L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
            return L2DS_FAILURE;
        }

        MEMSET (L2DhcpSnpOption.pu1Val, L2DS_ZERO, L2DS_OPTION_LEN);
    }

    else
    {
        /* There is no option 82 present in the packet. This means that the
         * DHCP server upstream does not support or understands option-82.
         * So broadcast the packet on that VLAN, or send the packet based on 
         * destination MAC address*/

        /* Convert the packet to CRU-buffer */
        pBuf = L2dsUtilLinearToCRU (pu1RecvBuf, u4PktLength);
        if (pBuf == NULL)
        {
            L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                      "L2dsPktProcessPktFromSrvr:Unable to convert the linear "
                      "buffer to CRU-buffer\r\n");
            L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
            return L2DS_FAILURE;
        }

        if (L2dsPortForwardPkt (pBuf, u4ContextId, VlanTag, u4InPort,
                                pL2DhcpSnpPktInfo->DstMacAddress,
                                pL2DhcpSnpPktInfo->SrcMacAddress,
                                NULL) != L2DS_SUCCESS)
        {
            L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                      "L2dsPktProcessPktFromSrvr:Unable to send the packet out"
                      "\r\n");
            L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
            /* Release the CRU Buffer */
            CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
            return L2DS_FAILURE;
        }
    }
    L2DS_TRC_ARG1 (L2DS_TRC_FLAG, L2DS_FN_EXIT, L2DS_MODULE_NAME,
                   "L2dsPktProcessPktFromSrvr:Exiting the function %s \r\n",
                   __FUNCTION__);
    L2DS_RELEASE_MEM_BLOCK (L2DS_PKT_INFO_POOL_ID, pL2DhcpSnpPktInfo);
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsPktHandlePktFromSrvr                             */
/*                                                                           */
/* Description        : This function handles the incoming DHCP Packet, from */
/*                      the servers connected to upstream interface          */
/*                                                                           */
/* Input(s)           : pu1RecvBuf         - Incoming packet buffer          */
/*                      u4ContextId        - Context Identifier              */
/*                      u4OutPort          - Outgoing Port                   */
/*                      VlanTag            - VLAN tag informations of the    */
/*                                           packet                          */
/*                      CVlanId            - Customer VLAN ID                */
/*                      u4PktLength        - total packet length             */
/*                      pL2DhcpSnpPktInfo  - Packet informations             */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPktHandlePktFromSrvr (UINT1 *pu1RecvBuf, UINT4 u4ContextId, UINT4 u4OutPort,
                          tVlanTag VlanTag, tVlanId CVlanId, UINT4 u4PktLength,
                          tL2DhcpSnpPktInfo * pL2DhcpSnpPktInfo)
{
    tPortList           OutPortList = { L2DS_ZERO };
    tL2DhcpSnpOption    L2DhcpSnpOption;
    tCRU_BUF_CHAIN_HEADER *pBuf = NULL;
    UINT1              *pu1Options = NULL;
    UINT1              *pu1PktOffset = NULL;
    tVlanId             VlanId = L2DS_ZERO;
    UINT4               u4DefaultGateway = L2DS_ZERO;
    UINT4               u4NewPktLength = L2DS_ZERO;
    UINT4               u4LeaseTime = L2DS_ZERO;
    UINT2               u2Offset = L2DS_ZERO;
    UINT2               u2NewOptLen = L2DS_ZERO;
    UINT2               u2PktChgLen = L2DS_ZERO;
    UINT2               u2CheckSum = L2DS_ZERO;
    UINT1               u1RemovedLength = L2DS_ZERO;

    /* As of now, we are not using the C-VLAN tag, 
     * that is contained in the Circuit-ID */

    L2DS_TRC_ARG1 (L2DS_TRC_FLAG, L2DS_FN_ENTRY, L2DS_MODULE_NAME,
                   "L2dsPktHandlePktFromSrvr:Entering the function %s \r\n",
                   __FUNCTION__);

    MEMSET (&L2DhcpSnpOption, L2DS_ZERO, sizeof (tL2DhcpSnpOption));

    VlanId = VlanTag.OuterVlanTag.u2VlanId;

    /* Update the VLAN statistics */
    L2dsPktUpdateStat (pL2DhcpSnpPktInfo->u1PktType, u4ContextId, VlanId);

    if (pL2DhcpSnpPktInfo->u1PktType == L2DS_DHCP_ACK)
    {
        /* The packet is an DHCP ACK Packet */
        /* Get the lease time */
        if (L2dsUtilGetOption (&(pL2DhcpSnpPktInfo->L2DhcpSnpPacket),
                               &L2DhcpSnpOption, L2DS_OPT_LEASE_TIME)
            != L2DS_FOUND)
        {
            L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                      "L2dsPktHandlePktFromSrvr:Unable to get the lease time "
                      "option\r\n");
        }
        else
        {
            MEMCPY (&u4LeaseTime, L2DhcpSnpOption.pu1Val, sizeof (UINT4));
            u4LeaseTime = OSIX_NTOHL (u4LeaseTime);
            pL2DhcpSnpPktInfo->u4LeaseDuration = u4LeaseTime;

            MEMSET (L2DhcpSnpOption.pu1Val, L2DS_ZERO, L2DS_OPTION_LEN);
        }

        /* get the default gateway IP address */
        if (L2dsUtilGetOption (&(pL2DhcpSnpPktInfo->L2DhcpSnpPacket),
                               &L2DhcpSnpOption, L2DS_OPT_ROUTER_OPTION)
            != L2DS_FOUND)
        {
            L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                      "L2dsPktHandlePktFromSrvr:Unable to get the default "
                      "gateway option\r\n");
        }
        else
        {
            MEMCPY (&u4DefaultGateway, L2DhcpSnpOption.pu1Val, sizeof (UINT4));
            u4DefaultGateway = OSIX_NTOHL (u4DefaultGateway);
            pL2DhcpSnpPktInfo->u4DefaultGwIP = u4DefaultGateway;

            MEMSET (L2DhcpSnpOption.pu1Val, L2DS_ZERO, L2DS_OPTION_LEN);
        }
    }

    /* Remove the options added by us */
    if (L2dsUtilRemoveOptions (pL2DhcpSnpPktInfo, &u1RemovedLength)
        != L2DS_SUCCESS)
    {
        L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                  "L2dsPktHandlePktFromSrvr:Unable to remove the options "
                  "added by us\r\n");
    }
    else
    {
        /* Copy the rest of the options back to the packet */
        u2Offset = (UINT2) L2DS_DHCP_OPT_OFFSET (pL2DhcpSnpPktInfo);
        pu1PktOffset = pu1RecvBuf + u2Offset;

        pu1Options = pL2DhcpSnpPktInfo->L2DhcpSnpPacket.au1Options;
        u2NewOptLen = pL2DhcpSnpPktInfo->L2DhcpSnpPacket.u2OptLen;

        MEMCPY (pu1PktOffset, pu1Options, u2NewOptLen);
        MEMSET ((pu1PktOffset + u2NewOptLen), L2DS_ZERO, u1RemovedLength);

        u4NewPktLength = u4PktLength - (UINT4) u1RemovedLength;

        /* Update the ip length field */
        u2Offset = (UINT2) L2DS_IP_LENGTH_OFFSET (pL2DhcpSnpPktInfo);
        pu1PktOffset = pu1RecvBuf + u2Offset;

        MEMCPY (&u2PktChgLen, pu1PktOffset, L2DS_IP_DGRAM_LEN);

        u2PktChgLen = OSIX_NTOHS (u2PktChgLen);
        u2PktChgLen -= u1RemovedLength;
        u2PktChgLen = OSIX_HTONS (u2PktChgLen);

        MEMCPY (pu1PktOffset, &u2PktChgLen, L2DS_IP_DGRAM_LEN);

        /* Calculate the new IP Header checksum */
        u2Offset = (UINT2) L2DS_IP_HDR_CKSUM_OFFSET (pL2DhcpSnpPktInfo);
        pu1PktOffset = pu1RecvBuf + u2Offset;

        MEMSET (pu1PktOffset, L2DS_ZERO, L2DS_IP_HDR_CKSUM_LEN);

        u2CheckSum = L2dsPortCalcIpCkSum
            ((pu1RecvBuf + pL2DhcpSnpPktInfo->u1EtherHdrLen),
             pL2DhcpSnpPktInfo->u1IpHdrLen);

        u2CheckSum = OSIX_HTONS (u2CheckSum);

        MEMCPY (pu1PktOffset, &u2CheckSum, L2DS_IP_HDR_CKSUM_LEN);

        /* Update the udp-header length field */
        u2Offset = (UINT2) L2DS_UDP_HDR_LEN_OFFSET (pL2DhcpSnpPktInfo);
        pu1PktOffset = pu1RecvBuf + u2Offset;

        MEMCPY (&u2PktChgLen, pu1PktOffset, L2DS_UDP_DGRAM_LEN);

        u2PktChgLen = OSIX_NTOHS (u2PktChgLen);
        u2PktChgLen -= u1RemovedLength;
        u2PktChgLen = OSIX_HTONS (u2PktChgLen);

        MEMCPY (pu1PktOffset, &u2PktChgLen, L2DS_UDP_DGRAM_LEN);

        /* Put the UDP Checksum to 0. */
        u2Offset = (UINT2) L2DS_UDP_CKSUM_OFFSET (pL2DhcpSnpPktInfo);
        pu1PktOffset = pu1RecvBuf + u2Offset;

        MEMSET (pu1PktOffset, L2DS_ZERO, L2DS_UDP_CKSUM_LEN);

        /* Calculate the new Checksum */
        u2CheckSum = L2dsUtilCalcUdpCkSum (OSIX_NTOHS (u2PktChgLen),
                                           pL2DhcpSnpPktInfo->u4SrcAddr,
                                           pL2DhcpSnpPktInfo->u4DstAddr,
                                           (pu1RecvBuf +
                                            pL2DhcpSnpPktInfo->u1EtherHdrLen +
                                            pL2DhcpSnpPktInfo->u1IpHdrLen));

        MEMCPY (pu1PktOffset, &u2CheckSum, L2DS_UDP_CKSUM_LEN);
    }
    /* Convert the packet to CRU-Buffer */
    pBuf = L2dsUtilLinearToCRU (pu1RecvBuf, u4NewPktLength);
    if (pBuf == NULL)
    {
        L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                  "L2dsPktHandlePktFromSrvr:Unable to convert the linear buffer"
                  " to CRU-buffer\r\n");
        return L2DS_FAILURE;
    }

    OSIX_BITLIST_SET_BIT (OutPortList, u4OutPort, sizeof (tPortList));

    /* Sent the packet out through the specified out-port */
    if (L2dsPortForwardPkt (pBuf, u4ContextId, VlanTag,
                            pL2DhcpSnpPktInfo->u4PortNumber,
                            pL2DhcpSnpPktInfo->DstMacAddress,
                            pL2DhcpSnpPktInfo->SrcMacAddress,
                            OutPortList) == L2DS_SUCCESS)
    {
        /* If the packet is correctly sent out, and the packet type is DHCP 
         * ACK or NAK, Update the IP binding database */
        if (pL2DhcpSnpPktInfo->u1PktType == L2DS_DHCP_ACK)
        {
            /* update the databse */
            if (L2dsPortUpdateEntry
                (pL2DhcpSnpPktInfo, u4ContextId, u4OutPort, CVlanId,
                 (pL2DhcpSnpPktInfo->L2DhcpSnpPacket).Chaddr,
                 (pL2DhcpSnpPktInfo->L2DhcpSnpPacket).u4Yiaddr,
                 L2DS_IPDB_CREATE) != L2DS_SUCCESS)
            {
                L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                          "L2dsPktHandlePktFromSrvr:updation of the binding "
                          "database failed.\r\n");
            }
        }
        else if (pL2DhcpSnpPktInfo->u1PktType == L2DS_DHCP_NACK)
        {
            /* update the databse */
            if (L2dsPortUpdateEntry
                (pL2DhcpSnpPktInfo, u4ContextId, u4OutPort, CVlanId,
                 (pL2DhcpSnpPktInfo->L2DhcpSnpPacket).Chaddr,
                 (pL2DhcpSnpPktInfo->L2DhcpSnpPacket).u4Yiaddr,
                 L2DS_IPDB_DELETE) != L2DS_SUCCESS)
            {
                L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                          "L2dsPktHandlePktFromSrvr:updation of the binding "
                          "database failed.\r\n");
            }
        }
    }
    else
    {
        L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                  "L2dsPktHandlePktFromSrvr:Failed to send the packet out."
                  "\r\n");

        /* Release the CRU Buffer */
        CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
        return L2DS_FAILURE;
    }
    L2DS_TRC_ARG1 (L2DS_TRC_FLAG, L2DS_FN_EXIT, L2DS_MODULE_NAME,
                   "L2dsPktHandlePktFromSrvr:Exiting the function %s \r\n",
                   __FUNCTION__);
    return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsPktFwdOnTrusted                                  */
/*                                                                           */
/* Description        : This function is to sent a packet out through the    */
/*                      trusted ports                                        */
/*                                                                           */
/* Input(s)           : pu1RecvBuf  - Incoming packet buffer                 */
/*                      u4ContextId   - Context Identifier                   */
/*                      u4InPort    - Incoming Port                          */
/*                      VlanTag       - VLAN tag informations of the packet  */
/*                      DstMacAddress - Destination MAC of the packet        */
/*                      SrcMacAddress - Source MAC of the packet             */
/*                      u4PktLength - total packet length                    */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsPktFwdOnTrusted (UINT1 *pu1RecvBuf, UINT4 u4ContextId, UINT4 u4InPort,
                     tVlanTag VlanTag, tMacAddr DstMacAddress,
                     tMacAddr SrcMacAddress, UINT4 u4PktLength)
{
    tPortList           TrustedPortList = { L2DS_ZERO };
    tVlanId             VlanId = L2DS_ZERO;
    tCRU_BUF_CHAIN_HEADER *pBuf = NULL;

    VlanId = VlanTag.OuterVlanTag.u2VlanId;

    pBuf = L2dsUtilLinearToCRU (pu1RecvBuf, u4PktLength);
    if (pBuf == NULL)
    {
        L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                  "L2dsPktFwdOnTrusted:Unable to convert the "
                  "linear buffer to CRU-buffer\r\n");

        return L2DS_FAILURE;
    }

    if (L2dsPortGetTrustedList (u4ContextId, VlanId, TrustedPortList)
        != L2DS_SUCCESS)
    {
        L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                  "L2dsPktFwdOnTrusted:Unable to get the trusted "
                  "portlist\r\n");
        /* Release the CRU Buffer */
        CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
        return L2DS_FAILURE;
    }

    /* Sent the packet out */
    if (L2dsPortForwardPkt (pBuf, u4ContextId, VlanTag, u4InPort, DstMacAddress,
                            SrcMacAddress, TrustedPortList) != L2DS_SUCCESS)
    {
        L2DS_TRC (L2DS_TRC_FLAG, L2DS_FAIL_TRC, L2DS_MODULE_NAME,
                  "L2dsPktFwdOnTrusted:Unable to send the packet " "out\r\n");

        /* Release the CRU Buffer */
        CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
        return L2DS_FAILURE;
    }
    return L2DS_SUCCESS;
}
#endif

/*****************************************************************************
 *
 *    Function Name        : CmL2dsGetIpv4Info
 *
 *    Description        :  Get Ipv4 pakcet info, the packet in this func is beginning 
 *						from ip layer.
 *
 *    Input(s)		   : pu1Ipv4Hdr 		- Incoming packet buffer
 * 						pL2DS              - DHCP packet information
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
CmL2dsGetIpv4Info(UINT1 *pu1Ipv4Hdr, tL2DhcpSnpPktInfo * pL2DS)
{
	UINT1	*pu1PktOffset = pu1Ipv4Hdr;

	/* Get the whole Ip header */
	MEMCPY(&pL2DS->IPv4Hdr, pu1PktOffset, sizeof(t_IP_HEADER));

	/* Get ip header length */
	pL2DS->u4IpHdrLen = (UINT4)(pL2DS->IPv4Hdr.u1Ver_hdrlen 
		& L2DS_IP_HDRLEN_BITMASK) * L2DS_BYTE_IN_WORD;

	/* Get packet length(from IP Header to the end of the packet)*/
	pL2DS->IPv4Hdr.u2Totlen = OSIX_NTOHS(pL2DS->IPv4Hdr.u2Totlen);

	/* Get packet length(from Eth Header to the end of the packet)*/
	pL2DS->u4PktLen = pL2DS->u2EthHdrLen + pL2DS->IPv4Hdr.u2Totlen;

	/* Get src ip */
	pL2DS->IPv4Hdr.u4Src = OSIX_NTOHL(pL2DS->IPv4Hdr.u4Src);

	/* Get dst ip */	
	pL2DS->IPv4Hdr.u4Dest = OSIX_NTOHL(pL2DS->IPv4Hdr.u4Dest);

	/* Copy the details in the DHCP Header */
	pu1PktOffset = pu1Ipv4Hdr + L2DS_DHCP_HDR_OFFSET(pL2DS);
	if(CmL2dsGetDhcp4Info(pu1PktOffset, 
		(pL2DS->IPv4Hdr.u2Totlen - L2DS_DHCP_HDR_OFFSET(pL2DS)),
		&(pL2DS->L2DSPkt)) != L2DS_SUCCESS)
	{
		return L2DS_FAILURE;
	}

	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsGetIpv6Info
 *
 *    Description        :  Get Ipv6 pakcet info
 *
 *    Input(s)		   : pu1Ipv6Hdr 		- Incoming packet buffer
 * 						pL2DS              - DHCP packet information
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
CmL2dsGetIpv6Info(UINT1 *pu1Ipv6Hdr, tL2DhcpSnpPktInfo * pL2DS)
{
	UINT1	*pu1PktOffset = pu1Ipv6Hdr;

	/* Get ipv6 header info */
	MEMCPY(&pL2DS->IPv6Hdr, pu1PktOffset, sizeof(tIp6Hdr));

	/* Get ip header length */
	pL2DS->u4IpHdrLen = IPV6_HEADER_LEN;

	/* This length does not include ip layer length */
	pL2DS->IPv6Hdr.u2Len = OSIX_NTOHS(pL2DS->IPv6Hdr.u2Len);

	/* Get packet length(from Eth Header to the end of the packet) */
	pL2DS->u4PktLen = pL2DS->u2EthHdrLen + pL2DS->u4IpHdrLen + 
		pL2DS->IPv6Hdr.u2Len;

	/* Move operation pointer to next protocol(UDP) layer */
	pu1PktOffset += pL2DS->u4IpHdrLen;

	/* Move operation pointer to next protocol(DHCP6) layer */
	pu1PktOffset += L2DS_UDP_HEADER_LEN;

	if(CmL2dsGetDhcp6Info(pu1PktOffset, 
		pL2DS->IPv6Hdr.u2Len - L2DS_UDP_HEADER_LEN,
		&pL2DS->L2DS6Pkt) != L2DS_SUCCESS)
	{
		return L2DS_FAILURE;
	}

	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsGetDhcp4Info
 *
 *    Description        :  This function is to copy DHCP header informations
 *
 *    Input(s)		   : pu1DhcpHdr 	- incoming packet buffer
 *						u4DSPktLen 	- Dhcp layer length
 *
 *    Output(s)            : pL2DSPkt		- Used to store DHCP packet info.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4
CmL2dsGetDhcp4Info (UINT1 *pu1DhcpHdr, UINT4 u4DSPktLen,
	tL2DhcpSnpPacket *pL2DSPkt)
{
    UINT2	u2Offset 		= L2DS_ZERO;
	UINT1	*pu1DhcpBuf	= pu1DhcpHdr;
    tL2DhcpSnpOption    	L2DSOpt;

	/* Verify Magic Cookie */
	if(L2dsUtilVerifyCookie(pu1DhcpBuf + L2DS_DHCP_FIXED_HDR_LEN) 
		!= L2DS_SUCCESS)
	{
		L2DS_TRC(L2DS_FAIL_TRC, "[L2dsPktGetPktInfo]"
			"Verify Magic Cookie failed.\r\n");
		return L2DS_FAILURE;
	}

    /* Copy the OP code */
    pL2DSPkt->u1Op = pu1DhcpBuf[u2Offset];

	/* option length */
    pL2DSPkt->u2OptLen = (UINT2)(u4DSPktLen - L2DS_HDR_LEN_WITH_COOKIE);

    /* Copy the transaction ID */
    MEMCPY (&(pL2DSPkt->u4Xid), (pu1DhcpBuf + L2DS_XID_OFFSET),
            L2DS_XID_LEN);

    pL2DSPkt->u4Xid = L2DS_NTOHL (pL2DSPkt->u4Xid);

    /* Copy the client address */
    MEMCPY (&(pL2DSPkt->u4Ciaddr), (pu1DhcpBuf + L2DS_CIADDR_OFFSET),
            L2DS_IP_ADDR_LEN);

    pL2DSPkt->u4Ciaddr = L2DS_NTOHL (pL2DSPkt->u4Ciaddr);

    /* copy the 'your' client address field */
    MEMCPY (&(pL2DSPkt->u4Yiaddr), (pu1DhcpBuf + L2DS_YIADDR_OFFSET),
            L2DS_IP_ADDR_LEN);

    pL2DSPkt->u4Yiaddr = L2DS_NTOHL (pL2DSPkt->u4Yiaddr);

    /* copy the server address field */
    MEMCPY (&(pL2DSPkt->u4Siaddr), (pu1DhcpBuf + L2DS_SIADDR_OFFSET),
            L2DS_IP_ADDR_LEN);

    pL2DSPkt->u4Siaddr = L2DS_NTOHL (pL2DSPkt->u4Siaddr);

    /* Copy the relay agent address field */
    MEMCPY (&(pL2DSPkt->u4Giaddr), (pu1DhcpBuf + L2DS_GIADDR_OFFSET),
            L2DS_IP_ADDR_LEN);

    pL2DSPkt->u4Giaddr = L2DS_NTOHL (pL2DSPkt->u4Giaddr);

    /* Copy the client hardware address field */
    MEMCPY (&(pL2DSPkt->Chaddr), (pu1DhcpBuf + L2DS_CHADDR_OFFSET),
            sizeof (tMacAddr));

    /* Copy the sname field */
    MEMCPY (&(pL2DSPkt->u1Sname), (pu1DhcpBuf + L2DS_SNAME_OFFSET),
            L2DS_SNAME_LEN);

    /* copy the file field */
    MEMCPY (&(pL2DSPkt->u1File), (pu1DhcpBuf + L2DS_FILE_OFFSET),
            L2DS_FILE_LEN);

    /* copy the options */
    MEMCPY (&(pL2DSPkt->au1Options),
            (pu1DhcpBuf + L2DS_HDR_LEN_WITH_COOKIE),
            (u4DSPktLen - L2DS_HDR_LEN_WITH_COOKIE));

	/* Get option info */
	if(L2dsUtilGetOption(pL2DSPkt, &L2DSOpt, L2DS_OPT_MSG_TYPE) 
		!= L2DS_FOUND)
	{
		L2DS_TRC(L2DS_FAIL_TRC, "[L2dsPktGetPktInfo]"
			"Option 53 does not exist.\r\n");
		return L2DS_FAILURE;
	}

	/* Get DHCP message type, it's very important */
	pL2DSPkt->u1MsgType = L2DSOpt.pu1Val[L2DS_ZERO];
	MEMSET (L2DSOpt.pu1Val, L2DS_ZERO, L2DS_OPTION_LEN);

    return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsGetDhcp4Info
 *
 *    Description        :  This function is to copy DHCP header informations
 *
 *    Input(s)		   : pu1DhcpHdr 	- incoming packet buffer
 *						u4DSPktLen 	- Dhcp layer length
 *
 *    Output(s)            : pL2DS6Pkt		- Used to store DHCP6 packet info.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4
CmL2dsGetDhcp6Info (UINT1 *pu1DhcpHdr, UINT4 u4DSPktLen,
	tL2DhcpSnpPktInfo6Pkt *pL2DS6Pkt)
{
	UINT4	u4PktOffSetLen	= 0;
	UINT2	u2OptionType	= 0;
	UINT2	u2OptionLen		= 0;
	UINT1	*pu1PktOffset 	= pu1DhcpHdr;

	/* Get packet type */
	pL2DS6Pkt->u1MsgType = pu1PktOffset[L2DS_ZERO];

	/* Get trans id */
	MEMCPY((UINT1 *)&pL2DS6Pkt->u4TransID, pu1PktOffset, L2DS6_XID_LEN);
	pL2DS6Pkt->u4TransID &= 0x00FFFFFFL;
	pu1PktOffset += L2DS6_XID_LEN;

	if((pL2DS6Pkt->u1MsgType == L2DS_DHCP6_SOLICIT) ||
	   (pL2DS6Pkt->u1MsgType == L2DS_DHCP6_REQUEST))
	{
		while (u4DSPktLen > u4PktOffSetLen)
		{
			MEMCPY((UINT1 *)&u2OptionType, pu1PktOffset, L2DS6_OPTION_TYPE_LEN);
			u2OptionType = OSIX_HTONS(u2OptionType);
		
			if (u2OptionType == L2DS6_CI_OPTION_TYPE)
			{
				MEMCPY(&u2OptionLen, pu1PktOffset + L2DS6_OPTION_TYPE_LEN, 2);
				u2OptionLen = OSIX_HTONS(u2OptionLen);

				/*get client mac*/
				MEMCPY(pL2DS6Pkt->Chaddr, pu1PktOffset + 4 + (u2OptionLen - L2DS6_CIADDR_LEN), sizeof(tMacAddr));
			}
			else
			{
				MEMCPY(&u2OptionLen, pu1PktOffset + L2DS6_OPTION_TYPE_LEN, 2);
				u2OptionLen = OSIX_HTONS(u2OptionLen);
			}

			pu1PktOffset += (4 + u2OptionLen);
			u4PktOffSetLen += (4 + u2OptionLen);
		}
	}
	else if((pL2DS6Pkt->u1MsgType == L2DS_DHCP6_ADVERTISE) ||
			(pL2DS6Pkt->u1MsgType == L2DS_DHCP6_REPLY) ||
			(pL2DS6Pkt->u1MsgType == L2DS_DHCP6_RELEASE))
	{		
		while (u4DSPktLen > u4PktOffSetLen)
		{
			MEMCPY((UINT1 *)&u2OptionType, pu1PktOffset, L2DS6_OPTION_TYPE_LEN);
			u2OptionType = OSIX_HTONS(u2OptionType);

			if (u2OptionType == L2DS6_CI_OPTION_TYPE)
			{
				MEMCPY(&u2OptionLen, pu1PktOffset + L2DS6_OPTION_TYPE_LEN, 2);
				u2OptionLen = OSIX_HTONS(u2OptionLen);

				/*get client mac*/
				MEMCPY(pL2DS6Pkt->Chaddr, pu1PktOffset + 4 + (u2OptionLen - L2DS6_CIADDR_LEN), sizeof(tMacAddr));
			}
			else if(u2OptionType == L2DS6_IA_OPTION_TYPE)
			{
				MEMCPY(&u2OptionLen, pu1PktOffset + L2DS6_OPTION_TYPE_LEN, 2);
				u2OptionLen = OSIX_HTONS(u2OptionLen);

				/*get host ip*/
				MEMCPY(pL2DS6Pkt->HostIPv6.u1_addr, pu1PktOffset + 4 + L2DS6_IA_OFFSET, 
					   sizeof(tIp6Addr));

				/*get lease time*/
				MEMCPY((UINT1 *)&pL2DS6Pkt->u4LeaseTime, pu1PktOffset + 4 + L2DS6_IA_OFFSET + L2DS6_IP6ADDR_LEN, L2DS6_LIFETIME_LEN);
				pL2DS6Pkt->u4LeaseTime = L2DS_NTOHL (pL2DS6Pkt->u4LeaseTime);
			}
			else
			{
				MEMCPY(&u2OptionLen, pu1PktOffset + L2DS6_OPTION_TYPE_LEN, 2);
				u2OptionLen = OSIX_HTONS(u2OptionLen);
			}

			pu1PktOffset += (4 + u2OptionLen);
			u4PktOffSetLen += (4 + u2OptionLen);
		}
	}

	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsGetLTime
 *
 *    Description        :  This function is used to get lease time
 *
 *    Input(s)		   : pL2DS 			- dhcp packet information
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4 CmL2dsGetLTime(tL2DhcpSnpPktInfo * pL2DS)
{
	tL2DhcpSnpOption	L2DSOpt;

	MEMSET(&L2DSOpt, L2DS_ZERO, sizeof (tL2DhcpSnpOption));
	
	/* The packet is an DHCP ACK Packet, Get the lease time */
	if(L2dsUtilGetOption(&(pL2DS->L2DSPkt), &L2DSOpt, 
		L2DS_OPT_LEASE_TIME) == L2DS_FOUND)
	{
		MEMCPY(&pL2DS->L2DSPkt.u4LeaseTime, L2DSOpt.pu1Val, 
			sizeof (UINT4));
		pL2DS->L2DSPkt.u4LeaseTime = OSIX_NTOHL(pL2DS->L2DSPkt.u4LeaseTime);
	}
	else
	{
		/* Default value */
		pL2DS->L2DSPkt.u4LeaseTime = L2DS_LEASE_TIME_DFT;
	}

	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsGetGw
 *
 *    Description        :  This function is used to get gateway
 *
 *    Input(s)		   : pL2DS 			- dhcp packet information
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4 CmL2dsGetGw(tL2DhcpSnpPktInfo * pL2DS)
{
	tL2DhcpSnpOption	L2DSOpt;

	MEMSET(&L2DSOpt, L2DS_ZERO, sizeof (tL2DhcpSnpOption));
	/* get the default gateway IP address */
	if(L2dsUtilGetOption (&(pL2DS->L2DSPkt), &L2DSOpt, 
		L2DS_OPT_ROUTER_OPTION) == L2DS_FOUND)
	{
		MEMCPY(&pL2DS->L2DSPkt.u4DftGwIP, L2DSOpt.pu1Val, 
			sizeof(UINT4));
		pL2DS->L2DSPkt.u4DftGwIP = OSIX_NTOHL(pL2DS->L2DSPkt.u4DftGwIP);
	}
	else
	{
		/* Default value */
		pL2DS->L2DSPkt.u4DftGwIP = L2DS_ZERO;
	}

	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsPktMod
 *
 *    Description        :  This function modify incoming packet's information after add op82 into it
 *
 *    Input(s)		   : pu1RecvBuf   	- Incoming packet buffer   
 * 						pL2DS 	- dhcp packet information
 *						u1ModType  	- add or delete options from the packet
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4 CmL2dsPktMod(UINT1 *pu1RecvBuf,  tL2DhcpSnpPktInfo * pL2DS, UINT1 u1ModType)
{
    UINT2		u2NewOptLen 	= L2DS_ZERO;
    UINT2		u2CheckSum 	= L2DS_ZERO;
	UINT2		u2PktChgLen	= L2DS_ZERO;
	UINT2		u2AddLen 		= L2DS_ZERO;
	UINT1		u1RmvLen 		= L2DS_ZERO;
    UINT1		*pu1PktOffset 	= NULL;
    UINT1		*pu1Options 	= NULL;

	if(u1ModType == L2DS_OPTION_ADD)
	{
		/* Add option-82 and vendor specific options.
		* Even if the addition of options fail, we will send the packet out 
		*/
		if(L2dsUtilAddOptions(pL2DS, &u2AddLen) != L2DS_SUCCESS)
		{
			L2DS_TRC(L2DS_FAIL_TRC, "[CmL2dsPktMod]add op82 failed!\r\n");
			return L2DS_FAILURE;
		}
	}
	else/* delete */
	{
		/* Remove the options added by us */
		if(L2dsUtilRemoveOptions(pL2DS, &u1RmvLen) != L2DS_SUCCESS)
		{
			L2DS_TRC(L2DS_FAIL_TRC, "[CmL2dsPktMod]"
				"Unable to remove the options added by us\r\n");
			return L2DS_FAILURE;
		}
	}

    /* Copy the new options on to the packet */
    pu1Options = pL2DS->L2DSPkt.au1Options;
    u2NewOptLen = pL2DS->L2DSPkt.u2OptLen;
    pu1PktOffset = pu1RecvBuf + L2DS_DHCP_OPT_OFFSET(pL2DS);
    MEMCPY(pu1PktOffset, pu1Options, u2NewOptLen);
	if(u1ModType == L2DS_OPTION_ADD)
		pL2DS->u4PktLen += (UINT4)u2AddLen;
	else
		pL2DS->u4PktLen += (UINT4)u1RmvLen;

    /* Update the ip length field */
    pu1PktOffset = pu1RecvBuf + L2DS_IP_LENGTH_OFFSET(pL2DS);
	if(u1ModType == L2DS_OPTION_ADD)
		pL2DS->IPv4Hdr.u2Totlen += u2AddLen;
	else
		pL2DS->IPv4Hdr.u2Totlen -= u1RmvLen;
    u2PktChgLen = OSIX_HTONS(pL2DS->IPv4Hdr.u2Totlen);
    MEMCPY(pu1PktOffset, &u2PktChgLen, L2DS_IP_DGRAM_LEN);

    /* Calculate the new IP Header checksum */
    pu1PktOffset = pu1RecvBuf + L2DS_IP_HDR_CKSUM_OFFSET(pL2DS);
    MEMSET(pu1PktOffset, L2DS_ZERO, L2DS_IP_HDR_CKSUM_LEN);
    u2CheckSum = L2dsPortCalcIpCkSum(pu1RecvBuf + pL2DS->u2EthHdrLen,
		pL2DS->u4IpHdrLen);
    u2CheckSum = OSIX_HTONS(u2CheckSum);
    MEMCPY(pu1PktOffset, &u2CheckSum, L2DS_IP_HDR_CKSUM_LEN);

    /* Update the udp header length field */
    pu1PktOffset = pu1RecvBuf + L2DS_UDP_HDR_LEN_OFFSET(pL2DS);
    MEMCPY(&u2PktChgLen, pu1PktOffset, L2DS_UDP_DGRAM_LEN);
    u2PktChgLen = OSIX_NTOHS (u2PktChgLen);
	if(u1ModType == L2DS_OPTION_ADD)
    	u2PktChgLen += u2AddLen;
	else
		u2PktChgLen -= u1RmvLen;
    u2PktChgLen = OSIX_HTONS (u2PktChgLen);
    MEMCPY(pu1PktOffset, &u2PktChgLen, L2DS_UDP_DGRAM_LEN);

    /* Set the UDP Checksum to 0 and Calculate the new Checksum */
    pu1PktOffset = pu1RecvBuf + L2DS_UDP_CKSUM_OFFSET(pL2DS);
    MEMSET(pu1PktOffset, L2DS_ZERO, L2DS_UDP_CKSUM_LEN);
    u2CheckSum = L2dsUtilCalcUdpCkSum(OSIX_NTOHS(u2PktChgLen),
		pL2DS->IPv4Hdr.u4Src, pL2DS->IPv4Hdr.u4Dest,
		pu1RecvBuf + pL2DS->u2EthHdrLen + pL2DS->u4IpHdrLen);
    MEMCPY(pu1PktOffset, &u2CheckSum, L2DS_UDP_CKSUM_LEN);

	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsFwdPkt
 *
 *    Description        :  This function is used to send DHCP packet
 *
 *    Input(s)		   : u4ContextId  	- Context Identifier 
 * 						pu1RecvBuf   	- Incoming packet buffer  
 *						pL2DS 			- dhcp packet information
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4 CmL2dsFwdPkt(UINT4 u4CxtId, UINT1 *pu1RecvBuf,  
	tL2DhcpSnpPktInfo * pL2DS)
{
    tCRU_BUF_CHAIN_HEADER 	*pBuf 	= NULL;

    /* Convert the packet to a CRU-buffer */
    pBuf = L2dsUtilLinearToCRU(pu1RecvBuf, pL2DS->u4PktLen);
    if(pBuf == NULL)
    {
		L2DS_TRC(L2DS_FAIL_TRC, "[CmL2dsFwdPkt]"
			"Unable to convert the linear buffer to CRU-buffer.\r\n");

        return L2DS_FAILURE;
    }

	/* packet released when  L2DS_SUCCESS, or not keep original process */
	if (CmL2dsPktReleaseForMirrored(pBuf, pL2DS) == L2DS_SUCCESS)
	{
		return L2DS_SUCCESS;
	}

	/* send the packet out through assigned portlist */
	if(L2dsPortForwardPkt(pBuf, u4CxtId, pL2DS->VlanTag, 
		pL2DS->u2InPort, pL2DS->EthHdr.au1DstAddr, 
		pL2DS->EthHdr.au1SrcAddr, pL2DS->PortList) != L2DS_SUCCESS)
	{
		L2DS_TRC(L2DS_FAIL_TRC, "[CmL2dsFwdPkt]"
			"Unable to send the packet through portlist.\r\n");

		/* Release the CRU Buffer */
		CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
		return L2DS_FAILURE;
	}

	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsUpdIPDB
 *
 *    Description        :  This function is used to add or delete a entry in IPDB
 *
 *    Input(s)		   : u4ContextId  	- Context Identifier 
 *						pL2DS 			- dhcp packet information
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4 CmL2dsUpdIPDB(UINT4 u4CxtId, tL2DhcpSnpPktInfo * pL2DS, UINT1 *pu1RecvBuf)
{
	UINT1		u1Action;
	UINT4 		au4IP6Addr[4] = {0};
    tIPvXAddr   TempAddr;
    UINT4       u4TempIp;
	
	/* If the packet is DHCP RELEASE or DECLINE, 
	* or L2DS_DHCP_NACK, Delete the entry from database.
	* If the packet is ACK, Add the entry to database.
	*/
	if (pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV4)
	{
		if (pL2DS->L2DSPkt.u1MsgType == L2DS_DHCP_RELEASE	||
			pL2DS->L2DSPkt.u1MsgType == L2DS_DHCP_DECLINE	||
			pL2DS->L2DSPkt.u1MsgType == L2DS_DHCP_NACK)
		{
			u1Action = L2DS_IPDB_DELETE;
		}
		else if (pL2DS->L2DSPkt.u1MsgType == L2DS_DHCP_ACK)
		{
			u1Action = L2DS_IPDB_CREATE;
		}
		else
		{
			/* Forward the packet */
			if(CmL2dsFwdPkt(u4CxtId, pu1RecvBuf, pL2DS) == L2DS_FAILURE)
			{
				return L2DS_FAILURE;
			}
			
			return L2DS_SUCCESS;
		}
	}
	else if (pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV6)
	{
		if (pL2DS->L2DS6Pkt.u1MsgType == L2DS_DHCP6_RELEASE	||
			pL2DS->L2DS6Pkt.u1MsgType == L2DS_DHCP6_DECLINE)
		{
			u1Action = L2DS_IPDB_DELETE;
		}
		else if(pL2DS->L2DS6Pkt.u1MsgType == L2DS_DHCP6_REPLY)
		{
			u1Action = L2DS_IPDB_CREATE;
		}
		else
		{
			/* Forward the packet */
			if(CmL2dsFwdPkt(u4CxtId, pu1RecvBuf, pL2DS) == L2DS_FAILURE)
			{
				return L2DS_FAILURE;
			}
			
			return L2DS_SUCCESS;
		}
	}
	else
	{
		return L2DS_FAILURE;
	}

	if (L2dsPortUpdateEntry(u4CxtId, pL2DS, u1Action) == L2DS_SUCCESS)
	{
		/* Forward the packet */
		if(CmL2dsFwdPkt(u4CxtId, pu1RecvBuf, pL2DS) == L2DS_FAILURE)
		{
			return L2DS_FAILURE;
		}

		if (pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV4)
		{
			if (pL2DS->L2DSPkt.u1MsgType == L2DS_DHCP_ACK)
			{
			    u4TempIp = OSIX_NTOHL(pL2DS->L2DSPkt.u4Yiaddr);
                IPVX_ADDR_INIT_FROMV4(TempAddr, u4TempIp);

				if (pL2DS->L2DSPkt.u4Yiaddr != 0)
				{
					/*CAMEOTAG: add by youchao 2013-1-18 
					    add syslog for dhcpsnooping module */
					CMSYS_LOG_MSG(SYSLOG_INFO_LEVEL, gi4SysLogId,
						 SYSLOG_MSG_DHCPSNP_CLIENT_GET_IP_SUCC,
						 SYSLOG_KEY_MACA, pL2DS->L2DSPkt.Chaddr,
						 SYSLOG_KEY_IPVX, &TempAddr,
						 SYSLOG_KEY_PORT, &pL2DS->u2OutPort);

					/* CAMEOTAG:add by Jiane on 2013.4.3.
					  * DES:support getting ip address from untrust port trap
					  */
					  au4IP6Addr[0] = OSIX_NTOHL(pL2DS->L2DSPkt.u4Yiaddr);
					  CmAPIIpdbIpFromUntrustPortTrap(pL2DS->u2OutPort,
					  						pL2DS->VlanTag.OuterVlanTag.u2VlanId,
					  						pL2DS->L2DSPkt.Chaddr,
					  						IPVX_ADDR_FMLY_IPV4,
					  						au4IP6Addr);
					/* CAMEOTAG end */
				}
			}
		}
		else if (pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV6)
		{
			if(pL2DS->L2DS6Pkt.u1MsgType == L2DS_DHCP6_REPLY)
			{
				/*CAMEOTAG: add by youchao 2013-1-18 
				* add syslog for dhcpsnooping module 
				*/
				IPVX_ADDR_INIT_FROMV6(TempAddr, pL2DS->L2DS6Pkt.HostIPv6.u1_addr);
				CMSYS_LOG_MSG(SYSLOG_INFO_LEVEL, gi4SysLogId,
					 SYSLOG_MSG_DHCPSNP_CLIENT_GET_IP_SUCC,
					 SYSLOG_KEY_MACA, pL2DS->L2DS6Pkt.Chaddr,
					 SYSLOG_KEY_IPVX, &TempAddr,
					 SYSLOG_KEY_PORT, &pL2DS->u2OutPort);

				/* CAMEOTAG:add by Jiane on 2013.4.3.
				  * DES:support getting ip address from untrust port trap
				  */
				  CmAPIIpdbIpFromUntrustPortTrap(pL2DS->u2OutPort,
				  						pL2DS->VlanTag.OuterVlanTag.u2VlanId,
				  						pL2DS->L2DS6Pkt.Chaddr,
				  						IPVX_ADDR_FMLY_IPV6,
				  						pL2DS->L2DS6Pkt.HostIPv6.u1_addr);
			}
		}
		else
		{
			return L2DS_FAILURE;
		}
	
		return L2DS_SUCCESS;
	}
	else
	{
		return L2DS_FAILURE;
	}
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsGetBcPortlist
 *
 *    Description        :  This function is used to get trust port list exclude
 *						inport. The port list is used to forward dhcp pkts
 *						when receiving them.
 *
 *    Input(s)		   : u4CxtId 		- Context id
 * 						pu1RecvBuf - Incoming packet buffer
 * 						pL2DS		- DHCP Snp packet details info
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4 CmL2dsGetBcPortlist(UINT4 u4CxtId, UINT1 *pu1RecvBuf, 
	tL2DhcpSnpPktInfo * pL2DS)
{
	tVlanId		Vid	 = pL2DS->VlanTag.OuterVlanTag.u2VlanId;

	UNUSED_PARAM(pu1RecvBuf);
	
	if(L2dsPortGetUpStreamList(u4CxtId, Vid, pL2DS->PortList)
		!= L2DS_SUCCESS)
	{
		L2DS_TRC(L2DS_TRC_ERR, 
			"[CmL2dsGetBcPortlist]L2dsPortGetUpStreamList failed\r\n");
		return L2DS_FAILURE;
	}

	/* Exclude the InPort */
	OSIX_BITLIST_RESET_BIT(pL2DS->PortList, pL2DS->u2InPort, 
		sizeof (tPortList));

	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsGetUcPortlist
 *
 *    Description        :  This function is used to get an target port through
 *						looking up fdb. The port is used to forward dhcp 
 *						server pkts when receiving them.
 *
 *    Input(s)		   : u4CxtId 		- Context id
 * 						pu1RecvBuf - Incoming packet buffer
 * 						pL2DS		- DHCP Snp packet details info
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE
 *
 *****************************************************************************/
INT4 CmL2dsGetUcPortlist(UINT4 u4CxtId, UINT1 *pu1RecvBuf, 
	tL2DhcpSnpPktInfo * pL2DS)
{
	INT4		i4PortSecState;
	UINT4		u4PortIdx;
	tVlanId		Vid = pL2DS->VlanTag.OuterVlanTag.u2VlanId;

	/* After test, this u4OutPort and the pL2DS->u4InPort,
	* They are both logical ports.
	* So, no need to use CmAPILaLogicalPortlistGetWithLock to convert.
	*/
	if(pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV4)
	{
		CmL2dsFdbFind(Vid, pL2DS->L2DSPkt.Chaddr, &u4PortIdx);
	}
	else if(pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV6)
	{
		CmL2dsFdbFind(Vid, pL2DS->L2DS6Pkt.Chaddr, &u4PortIdx);
	}
	
	if(u4PortIdx == 0)
	{
		/* Forward these packets */
		CmL2dsFwdPkt(u4CxtId, pu1RecvBuf, pL2DS);
		return L2DS_FAILURE;
	}

	if(CmAPICfaGetIfSecState(u4PortIdx, &i4PortSecState) 
		== ISS_FAILURE)
	{
		return L2DS_FAILURE;
	}
	
	if(i4PortSecState != CFA_PORT_STATE_UNTRUSTED)
	{
		/* Forward these packets */
		CmL2dsFwdPkt(u4CxtId, pu1RecvBuf, pL2DS);
		return L2DS_FAILURE;
	}
	
	pL2DS->u2OutPort = u4PortIdx;
	MEMSET(pL2DS->PortList, L2DS_ZERO, sizeof(tPortList));
	OSIX_BITLIST_SET_BIT(pL2DS->PortList, pL2DS->u2OutPort, 
		sizeof(tPortList));

	return L2DS_SUCCESS;
}

/* CAMEOTAG: modify by linyu on 2013-03-20, fix packet duplicate when hw mirror packet  */
#undef CAMEO_MIRROR_SERVER_PACKET_RULE_CREATED
/*****************************************************************************
 *
 *    Function Name        : CmL2dsPktReleaseForMirrored
 *
 *    Description        :  This function is used to release packet which mirrored to cpu
 *
 *    Input(s)		   : pBuf 	- packet buffer
 * 						 pL2DS  -  DHCP Snp packet details info
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS, packet released
 *                         L2DS_FAILURE, packet not released
 *
 *****************************************************************************/
/*
--------------------------------------------------------------------------------
|					dhcp sever offer/ack packet								   |
--------------------------------------------------------------------------------
|						|				dhcp snooping enable				   |
|	DUT dhcp client		|------------------------------------------------------|
|						|			trust			|		untrust			   |
--------------------------------------------------------------------------------
|		copy			|			copy			|		trap			   |
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
|					dhcp client discover/request packet						   |
--------------------------------------------------------------------------------
|						|				dhcp snooping enable				   |
|	DUT server client	|------------------------------------------------------|
|						|			trust			|		untrust			   |
--------------------------------------------------------------------------------
|		copy			|			hw forward		|		trap			   |
--------------------------------------------------------------------------------
*/
INT4 CmL2dsPktReleaseForMirrored(tCRU_BUF_CHAIN_HEADER * pBuf, 
                                        tL2DhcpSnpPktInfo * pL2DS)
{
#if defined(NPAPI_WANTED) && defined(CAMEO_MIRROR_SERVER_PACKET_RULE_CREATED)
	if (pL2DS->u1InPortType == L2DS_UPSTREAM_PORT)
	{
		if(pL2DS->u4IpType == IPVX_ADDR_FMLY_IPV4)
		{
			switch(pL2DS->L2DSPkt.u1MsgType)
			{
				case L2DS_DHCP_OFFER:
				case L2DS_DHCP_ACK: 
				case L2DS_DHCP_NACK:
					CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
					return L2DS_SUCCESS;
					break;
				default: break;
			}
		}
		else
		{
			switch(pL2DS->L2DS6Pkt.u1MsgType)
			{
				case L2DS_DHCP6_ADVERTISE:
				case L2DS_DHCP6_REPLY:
				case L2DS_DHCP6_RECONFIG:
					CRU_BUF_Release_MsgBufChain (pBuf, FALSE);
					return L2DS_SUCCESS;
					break;
				default: break;
			}
		}
	}
#else
	UNUSED_PARAM(pBuf);
	UNUSED_PARAM(pL2DS);
#endif

    return L2DS_FAILURE;
}
#undef CAMEO_MIRROR_SERVER_PACKET_RULE_CREATED


/*                                                                           */
/***************************** END OF FILE ***********************************/
/*                                                                           */
