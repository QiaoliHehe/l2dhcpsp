#ifndef _L2DSMACS_H
#define _L2DSMACS_H
/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsmacs.h,v 1.7 2012/05/03 13:00:54 siva Exp $               */
/*****************************************************************************/
/*    FILE  NAME            : l2dsmacs.h                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snoop                                   */
/*    MODULE NAME           : L2DHCP snoop Macros                            */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains macro definitions           */
/*                            for L2 DHCP Snooping module                    */
/*---------------------------------------------------------------------------*/

/* Trace related Macro */
#define L2DS_TRC_FLAG           gL2DhcpSnpGlobalInfo.u4DebugFlag
#define L2DS_CURR_CXT_ID        gL2DhcpSnpGlobalInfo.u4L2dsCurCxtId
                                             

/* Maximum number of VLAN entries */
#define L2DS_MAX_NUM_VLAN       VLAN_DEV_MAX_NUM_VLAN

#define L2DS_MAX_VLAN_ID        VLAN_DEV_MAX_VLAN_ID
#define L2DS_MIN_VLAN_ID        L2DS_MIN_NUM_VLAN

/* Vlan Protocol ID (0x8100) */
#define L2DS_VLAN_PROTOCOL_ID   VLAN_PROTOCOL_ID
/* Locks */
#define L2DS_LOCK               L2dsUtilTakeLock
#define L2DS_UNLOCK             L2dsUtilReleaseLock

/* Admin Status */
#define L2DS_ADMIN_STATUS(u4ContextId) \
    gaL2DhcpSnpGlobalConfig[u4ContextId].u1SnpAdminStatus

/* Mac verify status */
#define L2DS_MAC_VERIFY_STATUS(u4ContextId) \
    gaL2DhcpSnpGlobalConfig[u4ContextId].u1MacVerifyStatus

#define L2DS_IS_CONTEXT_EXIST(u4ContextId) \
    gaL2DhcpSnpGlobalConfig[u4ContextId].u1IsContextExist

/* CAMEOTAG: Add by Ricann,20120907
* Add following 4 fields to support Navigator and Phoenix projects.
*/
/* Option82 Status */
#define L2DS_ADD_OP82_STATUS(u4ContextId) \
	gaL2DhcpSnpGlobalConfig[u4ContextId].u1AddOp82St

/* Datebase backup to file interval */
#define L2DS_BACKUP_INTERVAL(u4ContextId) \
	gaL2DhcpSnpGlobalConfig[u4ContextId].u4BkupItvl

/* Whether forward packet with op82 option */
#define L2DS_FWD_OP82_STATUS(u4ContextId) \
	gaL2DhcpSnpGlobalConfig[u4ContextId].u1FwdOp82St

/* Whether backup Datebase backup to file */
#define L2DS_BACKUP_DB_STATUS(u4ContextId) \
	gaL2DhcpSnpGlobalConfig[u4ContextId].u1BkupDBSt



/* RBTree Related Macros */
#define L2DS_INTF_RBTREE        gL2DhcpSnpGlobalInfo.InterfaceEntry

#define L2DS_CREATE_RBTREE      RBTreeCreateEmbedded
#define L2DS_DELETE_RBTREE      RBTreeDestroy

/* DSL option Related macros */
#define L2DS_SUB_OPTION_DATA_LEN    (2 + L2DS_ENTERPRISE_ID_LEN)

/* Rowstatus Macros */
#define L2DS_ACTIVE               ACTIVE
#define L2DS_NOT_IN_SERVICE       NOT_IN_SERVICE
#define L2DS_NOT_READY            NOT_READY
#define L2DS_CREATE_AND_WAIT      CREATE_AND_WAIT
#define L2DS_CREATE_AND_GO        CREATE_AND_GO
#define L2DS_DESTROY              DESTROY

/* Task, Queue and Event related macros, OSIX related macros */
#define L2DS_SUCCESS            OSIX_SUCCESS
#define L2DS_FAILURE            OSIX_FAILURE
#define L2DS_TRUE               OSIX_TRUE
#define L2DS_FALSE              OSIX_FALSE

#define L2DS_NTOHL              OSIX_NTOHL

#define L2DS_CREATE_QUEUE       OsixQueCrt
#define L2DS_DELETE_QUEUE       OsixQueDel
#define L2DS_SEND_TO_QUEUE      OsixQueSend
#define L2DS_RECV_FROM_QUEUE    OsixQueRecv
#define L2DS_CREATE_SEM         OsixCreateSem
#define L2DS_DELETE_SEM         OsixDeleteSem
#define L2DS_TAKE_SEM           OsixTakeSem
#define L2DS_GIVE_SEM           OsixGiveSem
#define L2DS_RECEIVE_EVENT      OsixReceiveEvent
#define L2DS_SEND_EVENT         OsixSendEvent

#define L2DS_EVENT_WAIT_FLAG    OSIX_WAIT
#define L2DS_RECEIVE_Q_FLAG     OSIX_NO_WAIT
#define L2DS_SEM_FLAGS          OSIX_DEFAULT_SEM_MODE

#define L2DS_QUEUE_ID           gL2DhcpSnpGlobalInfo.L2DhcpSnpQId
#define L2DS_SEM_ID             gL2DhcpSnpGlobalInfo.L2DhcpSnpSemId
#define L2DS_DYN_POOL_ID        gL2DhcpSnpGlobalInfo.DynamicPoolId
#define L2DS_PKT_INFO_POOL_ID   gL2DhcpSnpGlobalInfo.L2DsPktInfoPoolId
#define L2DS_TMR_LIST          gL2DhcpSnpGlobalInfo.TmrListId
#define L2DS_TMR_DB_BLK		gL2DhcpSnpGlobalInfo.TmrDBBlk

#define L2DS_MAX_DYN_POOL       1

/* CRU_Buffer copy */
#define L2DS_COPY_FROM_BUF(pBuf, pu1Dst, u4Offset, u4Size) \
    CRU_BUF_Copy_FromBufChain ((pBuf), (UINT1 *)(pu1Dst), u4Offset, u4Size)

/* Memory related MACRO */
#define L2DS_CREATE_MEM_POOL    MemCreateMemPool

#define L2DS_DELETE_MEM_POOL(PoolId) \
{\
    if (PoolId != 0)\
    {\
        (VOID) MemDeleteMemPool(PoolId);\
    }\
}

#define  L2DS_ALLOC_MEM_BLOCK(PoolId) \
             MemAllocMemBlk (PoolId)

#define  L2DS_RELEASE_MEM_BLOCK(PoolId, pu1Msg) \
{\
    MemReleaseMemBlock(PoolId, (UINT1 *) pu1Msg);\
    pu1Msg = NULL;\
}

#define L2DS_MEMORY_TYPE        MEM_DEFAULT_MEMORY_TYPE
#define L2DS_INTF_MEMBLK_SIZE   sizeof(tL2DhcpSnpIfaceEntry)
#define L2DS_MEMBLK_SIZE        sizeof(tL2DhcpSnpQMsg)

#define L2DS_INTF_POOL_ID       gL2DhcpSnpGlobalInfo.IntfEntryPoolId
#define L2DS_Q_POOL_ID          gL2DhcpSnpGlobalInfo.QMemPoolId

/*                  *
 * VARIOUS OFFSETS  *
 *                  */

/* Offset to IP Datagram Length */
#define L2DS_IP_LENGTH_OFFSET(pL2DhcpSnpPktInfo) \
    (pL2DhcpSnpPktInfo->u2EthHdrLen + 2)

/* Offset to Ip header checksum */
#define L2DS_IP_HDR_CKSUM_OFFSET(pL2DhcpSnpPktInfo) \
    (pL2DhcpSnpPktInfo->u2EthHdrLen + 10)

/* Source IP address offset */
#define L2DS_IPSRC_ADDR_OFFSET(pL2DhcpSnpPktInfo) \
    (pL2DhcpSnpPktInfo->u2EthHdrLen + 12)

/* Destination IP address offset */
#define L2DS_IPDST_ADDR_OFFSET(pL2DhcpSnpPktInfo) \
    (pL2DhcpSnpPktInfo->u2EthHdrLen + 16)

/* UDP Header length offset */
#define L2DS_UDP_HDR_LEN_OFFSET(pL2DhcpSnpPktInfo) \
    (pL2DhcpSnpPktInfo->u2EthHdrLen + pL2DhcpSnpPktInfo->u4IpHdrLen + 4)

/* UDP checksum offset */
#define L2DS_UDP_CKSUM_OFFSET(pL2DhcpSnpPktInfo) \
    (pL2DhcpSnpPktInfo->u2EthHdrLen + pL2DhcpSnpPktInfo->u4IpHdrLen + 6)

/* Offset to DHCP Header */
#define L2DS_DHCP_HDR_OFFSET(pL2DhcpSnpPktInfo) \
    (pL2DhcpSnpPktInfo->u4IpHdrLen + L2DS_UDP_HEADER_LEN)

/* Magic cookie offset */
#define L2DS_MAGIC_COOKIE_OFFSET(pL2DhcpSnpPktInfo) \
    (pL2DhcpSnpPktInfo->u4IpHdrLen + \
     L2DS_UDP_HEADER_LEN + L2DS_DHCP_FIXED_HDR_LEN)

/* Offset to DHCP Options */
#define L2DS_DHCP_OPT_OFFSET(pL2DhcpSnpPktInfo) \
    (pL2DhcpSnpPktInfo->u2EthHdrLen + pL2DhcpSnpPktInfo->u4IpHdrLen + \
     L2DS_UDP_HEADER_LEN + L2DS_DHCP_FIXED_HDR_LEN + L2DS_MAGIC_COOKIE_LEN)


/* The interface statistics macros. */
#define L2DS_STAT_RX_DISC(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfRxDiscovers

#define L2DS_STAT_RX_REQS(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfRxRequests

#define L2DS_STAT_RX_RELS(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfRxReleases

#define L2DS_STAT_RX_DECL(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfRxDeclines

#define L2DS_STAT_RX_INF(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfRxInforms

#define L2DS_STAT_TX_OFFR(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfTxOffers

#define L2DS_STAT_TX_ACKS(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfTxAcks

#define L2DS_STAT_TX_NAKS(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfTxNaks

#define L2DS_STAT_TTL_DISC(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfTotalDiscards

#define L2DS_STAT_MAC_DISC(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfMacDiscards

#define L2DS_STAT_SRV_DISC(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfServerDiscards

#define L2DS_STAT_OPT_DISC(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u4NoOfOptionDiscards

#define L2DS_INT_ROWSTATUS(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u1RowStatus

#define L2DS_INTF_SNP_STATUS(pL2DhcpSnpIfaceEntry) \
    pL2DhcpSnpIfaceEntry->u1VlanSnpStatus


/* Macros to increment interface statitics */
#define L2DS_INCR_STAT_RX_DISC(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfRxDiscovers == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxDiscovers = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxDiscovers++; \
    } \
}

#define L2DS_INCR_STAT_RX_REQS(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfRxRequests == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxRequests = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxRequests++; \
    } \
}

#define L2DS_INCR_STAT_RX_RELS(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfRxReleases == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxReleases = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxReleases++; \
    } \
}

#define L2DS_INCR_STAT_RX_DECL(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfRxDeclines == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxDeclines = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxDeclines++; \
    } \
}

#define L2DS_INCR_STAT_RX_INTF(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfRxInforms == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxInforms = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfRxInforms++; \
    } \
}

#define L2DS_INCR_STAT_TX_OFFR(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfTxOffers == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfTxOffers = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfTxOffers++; \
    } \
}

#define L2DS_INCR_STAT_TX_ACKS(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfTxAcks == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfTxAcks = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfTxAcks++; \
    } \
}

#define L2DS_INCR_STAT_TX_NAKS(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfTxNaks == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfTxNaks = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfTxNaks++; \
    } \
}

#define L2DS_INCR_STAT_TOTAL_DISC(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfTotalDiscards == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfTotalDiscards = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfTotalDiscards++; \
    } \
}

#define L2DS_INCR_STAT_MAC_DISC(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfMacDiscards == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfMacDiscards = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfMacDiscards++; \
    } \
}

#define L2DS_INCR_STAT_SRV_DISC(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfServerDiscards == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfServerDiscards = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfServerDiscards++; \
    } \
}

#define L2DS_INCR_STAT_OPT_DISC(pL2DhcpSnpIfaceEntry) \
{\
    if (pL2DhcpSnpIfaceEntry->u4NoOfOptionDiscards == 0xFFFFFFFF) \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfOptionDiscards = 0; \
    } \
    else \
    { \
        pL2DhcpSnpIfaceEntry->u4NoOfOptionDiscards++; \
    } \
}

/*CAMEOTAG:Ricann,20130228*/
#define L2DS_OFFSET(x, y)		((UINT4)(&(((x *)0)->y)))
#define L2DS_FDB_TMR_OFFSET	L2DS_OFFSET(tL2DhcpSnpFdb, TmrBlk)
#define L2DS_FDB_ENTRY_IDX(pFdbEntry)		\
	(((UINT1 *)pFdbEntry - (UINT1 *)(&gL2DSFdbGlob.L2DSFdb))/sizeof(tL2DhcpSnpFdb))

#endif /* _L2DSMACS_H */
