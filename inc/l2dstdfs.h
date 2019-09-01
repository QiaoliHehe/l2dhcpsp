#ifndef _L2DSTDFS_H
#define _L2DSTDFS_H
/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dstdfs.h,v 1.6 2012/05/03 13:00:54 siva Exp $               */ 
/*****************************************************************************/
/*    FILE  NAME            : l2dstdfs.h                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snoop                                   */
/*    MODULE NAME           : L2DHCP snoop Data Structures                   */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains Data Structures             */
/*                            for L2 DHCP Snooping module                    */
/*---------------------------------------------------------------------------*/

/* Global Configurations */
typedef struct _L2DhcpSnpGlobalConfig 
{
    UINT1       u1SnpAdminStatus;         /* Global DHCP snooping status */
    UINT1       u1MacVerifyStatus;        /* Global MAC Verification status */
    UINT1       u1IsContextExist;         /* To indicate whether the current 
                                             context is exist or not */
	/* CAMEOTAG: Add by Ricann,20120907
	* Add following 4 fields to support Navigator and Phoenix projects.
	*/
	UINT1		u1AddOp82St;	/* Global DHCP Snooping insert Option82 Status */
	UINT4		u4BkupItvl;	/* Datebase backup to file interval */
	UINT1		u1FwdOp82St;	/* Whether forward packet with op82 option */
	UINT1		u1BkupDBSt;	/* Whether backup Datebase backup to file */
	UINT1		u1Reserved[2];	/* Padding */
} tL2DhcpSnpGlobalConfig;


/* Global Inforamtions */
typedef struct _L2DhcpSnpGlobalInfo
{
    tRBTree     InterfaceEntry;           /* RBTree for Interface entry */
    tMemPoolId  IntfEntryPoolId;          /* Memory pool ID for interface
                                             entries */
    tMemPoolId  QMemPoolId;               /* Memory pool ID for Packet Q */
    tMemPoolId  DynamicPoolId;            /* Memory Pool ID for Dynamic 
                                             allocation*/
    tMemPoolId  L2DsPktInfoPoolId;        /* Memory PoolID for tL2DhcpSnpPktInfo*/   
    tOsixSemId  L2DhcpSnpSemId;           /* Semapore for the module */
    tOsixQId    L2DhcpSnpQId;             /* Id of Queue */
	tTimerListId TmrListId;				   /* Timer list */
	tTmrBlk		TmrDBBlk;				   /* refresh flsh db timer block */
    UINT4       u4DebugFlag;              /* Flag used for Trace */
    UINT4       u4L2dsCurCxtId;            /* Current Context Identifier*/
} tL2DhcpSnpGlobalInfo;


/* Layer 2 VLAN Entry */
typedef struct _L2DhcpSnpIfaceEntry
{
    tRBNodeEmbd VlanRbNode;
    UINT4       u4NoOfRxDiscovers;     /* total number of DHCP DISCOVERs 
                                          received from the hosts connected 
                                          to this VLAN */
    UINT4       u4NoOfRxRequests;      /* total number of DHCP REQUESTs 
                                          received from the hosts connected 
                                          to this VLAN */
    UINT4       u4NoOfRxReleases;      /* total number of DHCP RELEASEs 
                                          received from the hosts 
                                          connected to this VLAN. */
    UINT4       u4NoOfRxDeclines;      /* total number of DHCP DECLINEs 
                                          received from the hosts connected 
                                          to this VLAN */
    UINT4       u4NoOfRxInforms;       /* total number of DHCP INFORMs 
                                          received from the hosts connected 
                                          to this VLAN */
    UINT4       u4NoOfTxOffers;        /* Total number of DHCP OFFERs sent 
                                          out on this VLAN to hosts */
    UINT4       u4NoOfTxAcks;          /* Total number of DHCP ACKs sent out 
                                          on this VLAN to hosts */
    UINT4       u4NoOfTxNaks;          /* Total number of DHCP NAKs sent out 
                                          on this VLAN to hosts */
    UINT4       u4NoOfTotalDiscards;   /* Total number of DHCP packets from 
                                          hosts dropped on this VLAN */
    UINT4       u4NoOfMacDiscards;     /* Total number of DHCP packets from 
                                          hosts dropped due to failure in 
                                          MAC Address verification */
    UINT4       u4NoOfServerDiscards;  /* Total number of DHCP packets from 
                                          hosts dropped because they are 
                                          supposed to be the messages from a 
                                          server. The DHCP packets so dropped 
                                          will include DHCP ACKs, NAKs and 
                                          OFFERs received from host ports*/
    UINT4       u4NoOfOptionDiscards;  /* Total number of DHCP packets from 
                                          hosts dropped because either 
                                          option-82 is present in them 
                                          or giaddr field is set.*/
    UINT4       u4L2dsCxtId;           /* Context Identifier*/
    tVlanId     VlanId;                /* Vlan ID */ 
    UINT1       u1VlanSnpStatus;       /* L2 DHCP Snooping interface status */
    UINT1       u1RowStatus;           /* Row status */
} tL2DhcpSnpIfaceEntry;


/* Used for handling informations about incoming Events */
typedef struct _L2DhcpSnpQMsg 
{
    tCRU_BUF_CHAIN_HEADER   *pInQMsg;     /* Packet received */
    tVlanTag                VlanTag;      /* Incoming VLAN tags */
    UINT4                   u4EventType;  /* Type of the event */
    UINT4                   u4InPort;     /* Incoming port */
    UINT4                   u4L2dsCxtId;  /* Context Identifier*/
    tVlanId                 VlanId;       /* VlanId */    
    UINT2                   u2Padding;    /* Padding */
} tL2DhcpSnpQMsg;


/* Used to keep informations about the incoming DHCP Paket */
typedef struct _L2DhcpSnpPacket {
    UINT4     u4Xid;                       /* Transaction ID */
    UINT4     u4Ciaddr;                    /* Client IP address */ 
    UINT4     u4Yiaddr;                    /* 'Your' (client) Ip Address */
    UINT4     u4Siaddr;                    /* IP address of next server to 
                                              talk to */
    UINT4     u4Giaddr;                    /* DHCP relay agent IP address */
    UINT1     u1Sname[L2DS_SNAME_LEN];     /* Optional server host name */
    UINT1     u1File[L2DS_FILE_LEN];       /* Boot file Name */
    UINT1     au1Options[L2DS_OPTION_LEN]; /* Optional parameters
                                              (actual length dependent 
                                              on MTU) */
    tMacAddr  Chaddr;                      /* Client hardware address */
    UINT2     u2OptLen;                    /* Length of options */
    UINT1     u1Op;                        /* Message opcode/type */

	/*CAMEOTAG:Add by Ricann,20121026*/
	UINT1	u1MsgType;		/* packet message type (option 53 contents) */
	UINT4	u4LeaseTime;	/* Lease durations specified */
    UINT4	u4DftGwIP;		/* Default gateway (option 3 contents) */

    /*UINT1     u1Padding; */               /* Padding */
}tL2DhcpSnpPacket;

/* Used to keep informations about the incoming DHCP6 Paket */
typedef struct _L2DS6Pkt{
	UINT4		u4TransID;				/* Transaction-ID */
	tMacAddr  	Chaddr;					/* Client hardware address */
	tIp6Addr	HostIPv6;				/* Host ipv6 address */
	UINT4		u4LeaseTime;			/* Lease durations specified */
	UINT1		u1MsgType;				/* DHCP6 message type */
	UINT1	  	u1Pad[1];				/* Padding */
}tL2DhcpSnpPktInfo6Pkt;

/* Used to keep informations about the incoming DHCP Paket */
typedef struct _L2DhcpSnpPktInfo {
	/* L2 info */
	UINT2             	u2EthHdrLen;      	/* Ethernet header length including VLAN tags */
	tEnetV2Header		EthHdr;				/* We only use mac in it */
	tVlanTag			VlanTag;			/* VlanId is OuterVlanTag, CVlanId is InnerVlanTag */

	/* L3 info */
	UINT4				u4IpType;			/* IPv4 or IPv6 */
	UINT4				u4IpHdrLen;		/* IP Header length */
	t_IP_HEADER			IPv4Hdr;			/* IPv4 Header */
	tIp6Hdr				IPv6Hdr;			/* IPv6 Header */

	/* L4 info */
	tL2DhcpSnpPacket  		L2DSPkt;		/* DHCP Packet informations */ 
	tL2DhcpSnpPktInfo6Pkt	L2DS6Pkt;		/* DHCP6 Packet informations */ 

	/* Other info */
	UINT1             	u1InPortType;		/* Trust or un-trust(downstream or upstream) */
	UINT2             	u2InPort;			/* Port no of recvd Pkt */
	UINT2             	u2OutPort;			/* Only use when a trust port forward a ACK or NACK pkt to DHCP client */
	UINT4				u4PktLen;			/* Packet length */
	tPortList			PortList;			/* Trusted post list */
}tL2DhcpSnpPktInfo;


/* used for adding/extracting options to/from dhcp message */
typedef struct _L2DhcpSnpOption {
    UINT1       u1Type;                   /* Option Type */  
    UINT1       u1Len;                    /* Option length */
    UINT2       u2Padding;                /* Padding */
    UINT1       *pu1Val;                  /* Value of the option */ 
} tL2DhcpSnpOption;

/*CAMEOTAG:Ricann,20130228*/
typedef struct _L2DhcpSnpFdb{
	tTmrBlk		TmrBlk;
    tVlanId		Vid;
	tMacAddr	Mac;
	UINT4		u4Port;
}tL2DhcpSnpFdb;

typedef struct _L2DhcpSnpFdbGlob{
    INT4			i4UsedNum;
	tTmrBlk			PrintFdbTmrBlk;
	tL2DhcpSnpFdb	L2DSFdb[L2DS_MAX_FDB_ENTRY];
}tL2DhcpSnpFdbGlob;

#endif /* _L2DSTDFS_H */

