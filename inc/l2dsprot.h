#ifndef _L2DSPROT_H
#define _L2DSPROT_H 
/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsprot.h,v 1.12 2012/04/16 14:12:07 siva Exp $               */
/*****************************************************************************/
/*    FILE  NAME            : l2dsprot.h                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snoop                                   */
/*    MODULE NAME           : L2DHCP snoop prototypes                        */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains function prototypes         */
/*                            for L2 DHCP Snooping module                    */
/*---------------------------------------------------------------------------*/


/* l2dsintf.c */
PUBLIC INT4 L2dsIntfDeleteIntfEntry PROTO ((UINT4, tVlanId));

PUBLIC tL2DhcpSnpIfaceEntry *L2dsIntfCreateIntfEntry PROTO ((UINT4, tVlanId));

/*CAMEOTAG:Ricann,20121229*/
PUBLIC INT4 CmL2dsIntfDelAll PROTO((VOID));

/* l2dsmain.c */
VOID L2dsMainProcessEvent (VOID);

VOID L2dsMainDeInit (VOID);

VOID L2dsMainAssignMempoolIds (VOID);

INT4 L2dsMainInit (VOID);

INT4 L2dsMainTmr(VOID);

/*CAMEOTAG:Ricann,20121229*/
INT4 CmL2dsMibRestoreOver(VOID);

/* l2dsutil.c */
INT4 L2dsUtilGetOption (tL2DhcpSnpPacket *, tL2DhcpSnpOption *, UINT1);

INT4 L2dsUtilGetDslOptions (UINT1 *, UINT1 *, UINT4, tVlanTag);

INT4 L2dsUtilAddOptions ( tL2DhcpSnpPktInfo *, UINT2 *);

INT4 L2dsUtilRemoveOptions (tL2DhcpSnpPktInfo *, UINT1 *);

INT4 L2dsUtilGetPortAndVlanFromOption (tL2DhcpSnpOption *, UINT4 *, tVlanId *);

INT4 L2dsUtilGetRemoteIdMac (tL2DhcpSnpOption *, tMacAddr);

INT4 L2dsUtilGetAccessNodeId (tL2DhcpSnpOption *, UINT1, UINT1 *);

INT4 L2dsUtilValidateFreeString (tL2DhcpSnpOption *, UINT4 *);

INT4 L2dsUtilVerifyCookie (UINT1 *);

INT2 L2DsUtilCalcIpCkSum (UINT1 *, UINT4);

INT2 L2dsUtilCalcUdpCkSum (UINT2, UINT4, UINT4, UINT1 *);

tCRU_BUF_CHAIN_HEADER *L2dsUtilLinearToCRU (UINT1 *, UINT4);
    
INT4 L2dsUtilRBTreeIntfEntryCmp (tRBElem *,tRBElem *);

VOID L2dsUtilRBTreeEntryFree (tRBElem *);

INT4 L2dsUtilTakeLock (VOID);
 
INT4 L2dsUtilReleaseLock (VOID);

VOID L2dsUtilRegisterFsDhcSnpMib (VOID);

INT4 L2dsUtilGetFreeString (tL2DhcpSnpOption *, UINT1, UINT1 *);

/* l2dspkt.c */
INT4 L2dsPktHdl (tCRU_BUF_CHAIN_HEADER *, UINT4, UINT4, tVlanTag);

INT4 L2dsPktHdlV4FromDown (UINT1 *, tL2DhcpSnpIfaceEntry *, tL2DhcpSnpPktInfo *);
INT4 L2dsPktHdlV6FromDown (UINT1 *, tL2DhcpSnpIfaceEntry *, tL2DhcpSnpPktInfo *);
INT4 L2dsPktHdlV4FromUp (UINT1 *, tL2DhcpSnpIfaceEntry *, tL2DhcpSnpPktInfo *);
INT4 L2dsPktHdlV6FromUp (UINT1 *, tL2DhcpSnpIfaceEntry *, tL2DhcpSnpPktInfo *);
VOID L2dsPktStatUpd (UINT1, tL2DhcpSnpIfaceEntry *);
INT4 L2dsPktGetPktInfo (UINT1 *, tL2DhcpSnpPktInfo *);

INT4 CmL2dsGetIpv4Info(UINT1 *, tL2DhcpSnpPktInfo *);
INT4 CmL2dsGetIpv6Info(UINT1 *, tL2DhcpSnpPktInfo *);
INT4 CmL2dsGetDhcp4Info (UINT1 *, UINT4, tL2DhcpSnpPacket *);
INT4 CmL2dsGetDhcp6Info (UINT1 *, UINT4, tL2DhcpSnpPktInfo6Pkt *);

INT4 CmL2dsGetLTime(tL2DhcpSnpPktInfo *);
INT4 CmL2dsGetGw(tL2DhcpSnpPktInfo *);
INT4 CmL2dsPktMod(UINT1 *,tL2DhcpSnpPktInfo * , UINT1);
INT4 CmL2dsFwdPkt(UINT4, UINT1 *, tL2DhcpSnpPktInfo *);
INT4 CmL2dsUpdIPDB(UINT4, tL2DhcpSnpPktInfo *, UINT1 *);
INT4 CmL2dsGetBcPortlist(UINT4, UINT1 *, tL2DhcpSnpPktInfo *);
INT4 CmL2dsGetUcPortlist(UINT4, UINT1 *, tL2DhcpSnpPktInfo *);
/* CAMEOTAG: modify by linyu on 2013-03-20, fix packet duplicate when hw mirror packet  */
INT4 CmL2dsPktReleaseForMirrored(tCRU_BUF_CHAIN_HEADER * pBuf, tL2DhcpSnpPktInfo * pL2DS);

/* l2dsport.c */
INT4 L2dsPortGetPortType (UINT2, UINT1 *);
    
INT4 L2dsPortGetTrustedList (UINT4, tVlanId, tPortList);

INT4 L2dsPortGetUpStreamList (UINT4, tVlanId, tPortList);

INT4 L2dsPortForwardPkt (tCRU_BUF_CHAIN_HEADER *, UINT4, tVlanTag, UINT4, tMacAddr, 
                         tMacAddr, tPortList);

INT4 L2dsPortGetEtherHdrLen (tCRU_BUF_CHAIN_HEADER *, UINT4, UINT2 *);

INT4 L2dsPortUpdateEntry (UINT4 u4ContextId, 
	tL2DhcpSnpPktInfo * pL2DSPktInfo, UINT1 u1Action);

INT4 L2dsPortDeleteEntries (UINT4, tVlanId);

INT4 L2dsPortGetDslOptions (UINT1 *, UINT1 *, UINT4, tVlanTag);

INT2 L2dsPortCalcIpCkSum (UINT1 *, UINT4);

INT4 L2dsPortGetCVlanTag (UINT1 *, UINT2, tVlanId *);

INT4 L2dsPortGetDefaultCTag (UINT4, tVlanId *);

INT4 L2dsPortGetPortState (UINT2, UINT1 *);

/* l2dscli.c */
INT4 L2dsCliSetSnoopingStatus (tCliHandle, INT4);

INT4 L2dsCliSetMacVerifyStatus (tCliHandle, INT4);

INT4 L2dsCliSetPortState (tCliHandle, UINT4, INT4);

INT4 L2dsCliSetSnpIfaceStatus (tCliHandle, INT4, INT4);

INT4 L2dsCliShowSnpGlobConf (tCliHandle,UINT1 *);

INT4 L2dsCliShowSnpVlanConf (tCliHandle, UINT1 *, INT4);

INT4 L2dsCliPrintVlanInformation (tCliHandle, INT4, UINT4 *);

INT4 L2dsShowRunningConfig (tCliHandle);

INT4 L2dsShowRunningConfigGetId (tCliHandle);

INT4 L2dsShowRunningConfigDetails (tCliHandle, INT4);

INT4 L2dsCliSetTrace (INT4);

UINT1 * DcsGetAccessNodeIdentifier PROTO ((UINT1 *pu1AccessNodeName));

INT4 DcsUtilGetDslLineCharacteristics (UINT4 , UINT1 *,UINT1 *, tVlanTag );

INT4 DcsUtilGetFreeStringACI (INT4 , UINT1*, UINT1 *);

INT4 DcsUtilGetCircuitIdString (UINT4 , tVlanId ,UINT1 *, UINT1 *);

INT4 DcsGetRemoteAgentIdStatus (INT4 , UINT1 *);

INT4 DcsUtilGetRemoteIdString ( UINT1 *, UINT1 *);

INT4 DcsGetAccessLoopStatus (INT4 , UINT1 *);

INT4 CfaGetPvcIfIndexFromDslIndexVpiVci (UINT4 , INT4, INT4 ,UINT4 *);

INT4 CfaGetVciVpiDslIfIndex (UINT4 , UINT4 * ,INT4 * ,INT4 *);

INT4 DcsGetFreeStringACI (INT4 , UINT1 *,INT4 *);

PUBLIC INT4 L2dsSelectContext PROTO  ((UINT4 ));

INT4 L2dsVcmIsSwitchExist (UINT4);

PUBLIC INT4 L2dsVcmIsSwitchNameExist (UINT1 *, UINT4 *);


PUBLIC VOID L2dsReleaseContext PROTO  ((VOID));

PUBLIC INT4 L2dsGetVcmSystemMode (UINT2);

PUBLIC INT4 L2dsGetVcmSystemModeExt (UINT2);

PUBLIC INT4 L2DSCliSelectContextOnMode (tCliHandle CliHandle, UINT4 u4Cmd,
                            UINT4 *pu4Context);
PUBLIC INT4 L2dsVcmIsVcExist(UINT4);

INT4 L2dsVcmGetContextInfoFromIfIndex (UINT4, UINT4 *, UINT2 *);

PUBLIC UINT4 L2dsGetContextId (UINT4);

PUBLIC INT4 L2dsGetNextActiveContext PROTO ((UINT4 , UINT4 *));

PUBLIC VOID L2dsPrintContextInfo PROTO ((tCliHandle CliHandle, 
                                         INT4 i4ContextId));

PUBLIC VOID L2dsPrintMacVerifyStatus PROTO ((tCliHandle CliHandle, 
                                             INT4 i4Status));

PUBLIC VOID L2dsPrintGlobalSnoopingStatus PROTO ((tCliHandle, INT4));

/* CAMEOTAG:Add by Ricann, 20120924 
* To support write and read binding database with flash.
*/
PUBLIC INT4 CmL2dsReadDB(VOID);

PUBLIC INT4 CmL2dsReadEntry(INT1 *);

extern INT4 ClomHexStr2Bitmap(CHR1 *, UINT4, UINT1 *, UINT4 , CHR1);

extern VOID RemoveChar(CHR1[], CHR1);

/* CAMEOTAG:Add by Ricann, 20130228 */
PUBLIC INT4 CmL2dsFdbAdd PROTO((tVlanId, tMacAddr, UINT4));
PUBLIC INT4 CmL2dsFdbDel PROTO((tVlanId, tMacAddr, UINT4));
PUBLIC INT4 CmL2dsFdbFind PROTO((tVlanId, tMacAddr, UINT4*));
PUBLIC INT4 CmL2dsFdbPrint PROTO((VOID));
INT4 CmDSPOption82DefCircuitIdGet (UINT4, tVlanId, UINT1 *, UINT1 *);
INT4 CmDSPOption82DefRemoteIdGet (UINT1 *, UINT1 *);

#endif /* _L2DSPROT_H */
