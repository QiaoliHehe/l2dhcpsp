#ifndef _L2DSDEFN_H
#define _L2DSDEFN_H
/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsdefn.h,v 1.9 2013/02/09 08:21:04 siva Exp $                                                                    */
/*****************************************************************************/
/*    FILE  NAME            : l2dsdefn.h                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snoop                                   */
/*    MODULE NAME           : L2DHCP snoop definitions                       */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains type definitions            */
/*                            for L2 DHCP Snooping module                    */
/*---------------------------------------------------------------------------*/

/* Osix, Task, Queue and Event related definitions */
#define L2DS_TASK_NAME              (const UINT1 *) "L2DS"
#define L2DS_SEM_NAME               (const UINT1 *) "L2DS"
#define L2DS_QUEUE_NAME             (UINT1 *) "L2DQ"
#define L2DS_MODULE_NAME            "L2DS"
#define L2DS_SEM_COUNT              1
#define L2DS_QUEUE_DEPTH            50

/* CREATE/DELETE */
#define L2DS_IPDB_CREATE            1
#define L2DS_IPDB_DELETE            2

/* Minimum VLAN ID Allowed */
#define L2DS_MIN_NUM_VLAN           0
#define L2DS_MAX_CONTEXTS           SYS_DEF_MAX_NUM_CONTEXTS
#define L2DS_INVALID_CXT_ID         0xFFFFFFFF
#define L2DS_DEFAULT_CXT_ID         0
#define L2DS_MINUS_ONE             -1

/*CAMEOTAG: Added by zhaopan on 2015/11/26. use enum definition instead*/
enum 
{
	L2DS_ZERO = 0,
	L2DS_ONE = 1,
	L2DS_TWO = 2,
	L2DS_THREE = 3,
	L2DS_FOUR = 4,
	L2DS_SIX = 6,
	L2DS_SEVEN = 7,
	L2DS_EIGHT = 8,
	L2DS_SIXTEEN = 16
};


#define L2DS_UDP_PROTO_ID           17
/* MTU size */
#define L2DS_MAX_MTU                1600
/* CAMEOTAG: add by maoqin 2012-6-7 */
/* DHCP Packet Flag */
#define DHCP_SERVER_PKT 0
#define DHCP_CLIENT_PKT 1
/* Various header lengths */
#define L2DS_ETHERNET_HDR_LEN       14
#define L2DS_IP_HEADER_LEN          20 /* Without including IP options */
#define L2DS_UDP_HEADER_LEN         8
#define L2DS_DHCP_FIXED_HDR_LEN     236
#define L2DS_HDR_LEN_WITH_COOKIE    240

#define L2DS_FIXED_LEN              \
    (L2DS_ETHERNET_HDR_LEN + L2DS_IP_HEADER_LEN + L2DS_UDP_HEADER_LEN + \
     L2DS_HDR_LEN_WITH_COOKIE)

/* MAGIC Cookie length */
#define L2DS_MAGIC_COOKIE_LEN       4

/* Various Field lengths of DHCP Header */
#define L2DS_HRDW_ADD_LEN           16
#define L2DS_SNAME_LEN              64
#define L2DS_FILE_LEN               128

/* Option 82 related lengths */
#define L2DS_ACCESS_NODE_LEN        24
#define L2DS_ACCESS_NODE_ARRAY_LEN  L2DS_ACCESS_NODE_LEN + 4

/* Option 82 related fields */
#define L2DS_CIRCUIT_SUBOPT         1
#define L2DS_CIRCUIT_SUBOPT_LEN     6
#define L2DS_CIRCUIT_ID_TYPE        0
#define L2DS_CIRCUIT_LEN            4

/* Circuit Id header format is having "sub option type(1 Byte), 
 * total length(1 Byte) and then circuit id 
 * string(e.g Access-Node-Identifier atm slot/port:vpi.vci). In that 
 * circuit id string is variable one, so to get full length of circuit id, 
 * needs to add "L2DS_CIRCUIT_ID_HEADER_LEN " with the access node identifier 
 * string */

#define L2DS_CIRCUIT_ID_HEADER_LEN   2 /* sub option type(1 Byte) + 
                                          total length(1 Byte) */
#define L2DS_REMOTE_SUBOPT          2
#define L2DS_REMOTE_SUBOPT_LEN      8
#define L2DS_REMOTE_ID_TYPE         0
#define L2DS_REMOTE_LEN             6

/* TYPE + LENGTH field Size */
#define L2DS_OPTION_VAL_OFFSET      2 

/* Ethernet source mac offset */
#define L2DS_SRCMAC_OFFSET          6

/* DHCP Header offsets */ 
#define L2DS_XID_OFFSET             4
#define L2DS_CIADDR_OFFSET          12
#define L2DS_YIADDR_OFFSET          16 
#define L2DS_SIADDR_OFFSET          20 
#define L2DS_GIADDR_OFFSET          24
#define L2DS_CHADDR_OFFSET          28
#define L2DS_SNAME_OFFSET           34
#define L2DS_FILE_OFFSET            98

#define L2DS_IP_DGRAM_LEN           2
#define L2DS_IP_HDR_CKSUM_LEN       2

#define L2DS_UDP_DGRAM_LEN          2

#define L2DS_OPTION_LEN             (L2DS_MAX_MTU - L2DS_FIXED_LEN)
/* Length of IP Address */
#define L2DS_IP_ADDR_LEN            4
/* Length of transaction ID */
#define L2DS_XID_LEN                4

/* dhcpv6: Length of transaction ID */
#define L2DS6_XID_LEN               4

/* dhcpv6: ciaddr length */
#define L2DS6_CIADDR_LEN         	6

/* dhcpv6: identity association offsets */
#define L2DS6_IA_OFFSET 			16

/* dhcpv6: ipv6 address length */
#define L2DS6_IP6ADDR_LEN         	16

/* dhcpv6: lifetime length */
#define L2DS6_LIFETIME_LEN         	4

/* dhcpv6: option type length */
#define L2DS6_OPTION_TYPE_LEN       2

/* dhcpv6: CI option type  */
#define L2DS6_CI_OPTION_TYPE        1

/* dhcpv6: IA option type  */
#define L2DS6_IA_OPTION_TYPE        3

/* Mask used to get IP header length */
#define L2DS_IP_IPTYPE_BITMASK      0xf0
#define L2DS_IP_HDRLEN_BITMASK      0x0f
#define L2DS_BYTE_IN_WORD           4

/* Way of sending packet */
#define L2DS_UNICAST				 	1
#define L2DS_BROADCAST					2

/* Whether update binding database */
#define L2DS_UPDATE_DB_NO			0
#define L2DS_UPDATE_DB_YES			1

/* Add options or delete options */
#define L2DS_OPTION_ADD			1
#define L2DS_OPTION_DEL			2

/* Magic cookie */
#define L2DS_OPTIONS_COOKIE         0x63825363
/* Definition of DHCP message types */
#define L2DS_DHCP_DISCOVER				1
#define L2DS_DHCP_OFFER				2
#define L2DS_DHCP_REQUEST				3
#define L2DS_DHCP_DECLINE				4
#define L2DS_DHCP_ACK					5
#define L2DS_DHCP_NACK					6 
#define L2DS_DHCP_RELEASE				7
#define L2DS_DHCP_INFORM				8

/* Definition of DHCP6 message types */
#define L2DS_DHCP6_SOLICIT				1
#define L2DS_DHCP6_ADVERTISE			2
#define L2DS_DHCP6_REQUEST				3
#define L2DS_DHCP6_CONFIRM				4
#define L2DS_DHCP6_RENEW				5
#define L2DS_DHCP6_REBIND				6 
#define L2DS_DHCP6_REPLY				7
#define L2DS_DHCP6_RELEASE				8
#define L2DS_DHCP6_DECLINE				9
#define L2DS_DHCP6_RECONFIG			10
#define L2DS_DHCP6_INFO_REQUEST		11
#define L2DS_DHCP6_RELAY_FWD			12
#define L2DS_DHCP6_RELAY_REPLY		13

/* DHCP OPTIONS */
#define L2DS_OPT_PAD                0
#define L2DS_OPT_SUBNET_MASK        1
#define L2DS_OPT_ROUTER_OPTION      3
#define L2DS_OPT_DNS_NS             6
#define L2DS_OPT_DNS_NAME           15
#define L2DS_OPT_VENDOR_SPEC        43
#define L2DS_OPT_REQUESTED_IP       50
#define L2DS_OPT_LEASE_TIME         51
#define L2DS_OPT_OVERLOAD           52
#define L2DS_OPT_MSG_TYPE           53
#define L2DS_OPT_SERVER_ID          54
#define L2DS_OPT_PARAMETER_LIST     55
#define L2DS_OPT_MAX_MESSAGE_SIZE   57
#define L2DS_OPT_CLIENT_ID          61
#define L2DS_OPT_TFTP_SNAME         66
#define L2DS_OPT_BOOT_FILE_NAME     67
#define L2DS_OPT_USER_CLASS         77
#define L2DS_OPT_RAI                82
#define L2DS_OPT_END                255
#define L2DS_MAX_OPT_LEN            255
#define L2DS_CIRCUIT_MAX_LEN         27
/* DHCP DSL SUB-OPTIONS */
#define L2DS_SUBOPT_DSL             9
#define L2DS_SUBOPT_ENTERPRISE_ID   3561
#define L2DS_DSL_ACTUAL_UPRATE      0x81
#define L2DS_DSL_ACTUAL_DWRATE      0x82
#define L2DS_DSL_MIN_UPRATE         0x83
#define L2DS_DSL_MIN_DWRATE         0x84
   
#define L2DS_DSL_OPTION_LEN         100
#define L2DS_ENTERPRISE_ID_LEN      4
#define L2DS_VENDOR_OPTION_LEN      104 
/* Different events to be handled */
#define L2DS_EVENT_ARRIVED          0x01
#define L2DS_VLAN_INTERFACE_EVENT   0x02
#define L2DS_PKT_RECEIVE_EVENT      0x04
#define L2DS_TIMER_EVENT      		  0x08
#define L2DS_MIB_RESTORE_OVER		  MSR_RESTORE_COMPLETE_EVENT
#define L2DS_TMR_DB      	          1

#define L2DS_EVENTS  				(L2DS_EVENT_ARRIVED | \
									L2DS_TIMER_EVENT	| \
									L2DS_MIB_RESTORE_OVER)

/* Packet sending modes */
#define L2DS_VLAN_UNICAST           1 
#define L2DS_VLAN_BROADCAST         2
#define L2DS_VLAN_MULTICAST         3

/* Port properties */
#define L2DS_UPSTREAM_PORT          1
#define L2DS_DOWNSTREAM_PORT        2

#define L2DS_TRUSTED_PORT           1
#define L2DS_UNTRUSTED_PORT         0

/* Miscellaneous definitions */
#define L2DS_FOUND                  1
#define L2DS_NOT_FOUND              2

/* UDP Checksum length */
#define L2DS_UDP_CKSUM_LEN          2

/* Broadcast mac address */
#define L2DS_BCAST_MAC_ADDR         0xffffffffffff

/* destination type of packet */
#define L2DS_MAC_BCAST              1
#define L2DS_MAC_UCAST              2

#define  CFA_PVC                    70  /* channel(std) */
#define  CFA_XDSL2                  251 /* not standard */

#define  DCS_ACCESS_NODE_LEN        24 /* Access Node Id used in
                                          circuit id string */
#define  DCS_CIRCUIT_SUBOPT         0x01
#define  DCS_CIRCUIT_ID_HEADER_LEN  2 /* TYPE + LENGTH*/
#define  DCS_REMOTE_ID_HEADER_LEN   2 /* TYPE + LENGTH*/
#define  DCS_REMOTE_SUBOPT          0x02
#define  DCS_DSL_OPTION_LEN         89
#define  DCS_CIRCUIT_ID_LEN         63
#define  DCS_REMOTE_ID_LEN          63
#define  DCS_SUCCESS          OSIX_SUCCESS
#define  DCS_FAILURE          OSIX_FAILURE
#define  DCS_ENABLE                 1
#define  DCS_DISABLE                2

/* Time related macros */
#define  DCS_BACKUP_DB_ITVL_MIN		600
#define  DCS_BACKUP_DB_ITVL_DFT		1200
#define  DCS_BACKUP_DB_ITVL_MAX		86400
#define  DCS_BACKUP_DB_FILE			FLASH "dhcpsnoop" /* /mnt/flash/dhcpsnoop */		
#define  DCS_BACKUP_DB_RWBUF_LEN		256

/* Maximum number of physical downlink/user ports */
#define SYS_DEF_MAX_PHYSICAL_USER_INTERFACES 0
#define SYS_DEF_MAX_PVCS_PER_PORT 0

#define SYS_DEF_MAX_PVCS 				\
	(SYS_DEF_MAX_PHYSICAL_USER_INTERFACES * SYS_DEF_MAX_PVCS_PER_PORT)

/*CAMEOTAG:Ricann,20130228*/
/*CAMEOTAG: Added by zhaopan on 2013/6/8. L2DS_MAX_FDB_ENTRY updated correspondingly according
   to max ipdb binding entries num*/
#define L2DS_MAX_FDB_ENTRY 				CM_IPDB_BINDING_MAX_ENTRIES
#define L2DS_INVALID_FDB_IDX 			L2DS_MAX_FDB_ENTRY
#define L2DS_TMR_FDB      	          	2
#define L2DS_FDB_EXPIRE_TIME			300		/*5 minutes*/
#define L2DS_TMR_PRINT_FDB	          	3
#define L2DS_PRINT_FDB_TIME	        	(5*60)	/*5 minutes*/


/* CAMEOTAG:Add by Ricann, 20120925 
* Add for debug and trace.
*/
#define L2DS_TRC_NAME				"[DS:TRC] "
#define L2DS_TRC_NONE				0x0
#define L2DS_TRC_ERR				0x0001
#define L2DS_TRC_FDB				0x0010
/*#define	L2DS_DBG_FDB*/

#ifdef TRACE_WANTED
#define L2DS_TRC(Flag, Fmt) \
    UtlTrcLog(L2DS_TRC_FLAG, Flag, L2DS_TRC_NAME, Fmt)

#define L2DS_TRC_ARG1(Flag, Fmt, Arg1) \
    UtlTrcLog(L2DS_TRC_FLAG, Flag, L2DS_TRC_NAME, Fmt, Arg1)

#define L2DS_TRC_ARG2(Flag, Fmt, Arg1, Arg2) \
    UtlTrcLog(L2DS_TRC_FLAG, Flag, L2DS_TRC_NAME, Fmt, Arg1, Arg2)

#define L2DS_TRC_ARG3(Flag, Fmt, Arg1, Arg2, Arg3) \
    UtlTrcLog(L2DS_TRC_FLAG, Flag, L2DS_TRC_NAME, Fmt, Arg1, Arg2, Arg3)

#define L2DS_TRC_ARG4(Flag, Fmt, Arg1, Arg2, Arg3, Arg4) \
    UtlTrcLog(L2DS_TRC_FLAG, Flag, L2DS_TRC_NAME, Fmt, Arg1, Arg2, Arg3, Arg4)

#define L2DS_TRC_ARG5(Flag, Fmt, Arg1, Arg2, Arg3, Arg4, Arg5) \
    UtlTrcLog(L2DS_TRC_FLAG, Flag, L2DS_TRC_NAME, Fmt, Arg1, Arg2, Arg3, Arg4, Arg5)

#define L2DS_TRC_ARG6(Flag, Fmt, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) \
    UtlTrcLog(L2DS_TRC_FLAG, Flag, L2DS_TRC_NAME, Fmt, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)

#else
#define L2DS_TRC(Flag, Fmt)
#define L2DS_TRC_ARG1(Flag, Fmt, Arg1)
#define L2DS_TRC_ARG2(Flag, Fmt, Arg1, Arg2)
#define L2DS_TRC_ARG3(Flag, Fmt, Arg1, Arg2, Arg3)
#define L2DS_TRC_ARG4(Flag, Fmt, Arg1, Arg2, Arg3, Arg4)
#define L2DS_TRC_ARG5(Flag, Fmt, Arg1, Arg2, Arg3, Arg4, Arg5)
#define L2DS_TRC_ARG6(Flag, Fmt, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)
#endif

#endif /* _L2DSDEFN_H */
