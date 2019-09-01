/********************************************************************
 * Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *
 * $Id: l2dssz.h,v 1.2 2012/05/03 13:00:54 siva Exp $
 *
 * Description: This file contains prototypes for functions
 *              defined in L2DHSCNP.
 *********************************************************************/


enum {
    MAX_L2DS_IFACE_ENTRIES_SIZING_ID,
    MAX_L2DS_PDU_COUNT_SIZING_ID,
    MAX_L2DS_PKT_INFO_SIZING_ID,
    MAX_L2DS_Q_MESG_SIZING_ID,
    L2DS_MAX_SIZING_ID
};


#ifdef  _L2DSSZ_C
tMemPoolId L2DSMemPoolIds[ L2DS_MAX_SIZING_ID];
INT4  L2dsSizingMemCreateMemPools(VOID);
VOID  L2dsSizingMemDeleteMemPools(VOID);
INT4  L2dsSzRegisterModuleSizingParams( CHR1 *pu1ModName);
#else  /*  _L2DSSZ_C  */
extern tMemPoolId L2DSMemPoolIds[ ];
extern INT4  L2dsSizingMemCreateMemPools(VOID);
extern VOID  L2dsSizingMemDeleteMemPools(VOID);
extern INT4  L2dsSzRegisterModuleSizingParams( CHR1 *pu1ModName); 
#endif /*  _L2DSSZ_C  */


#ifdef  _L2DSSZ_C
tFsModSizingParams FsL2DSSizingParams [] = {
{ "tL2DhcpSnpIfaceEntry", "MAX_L2DS_IFACE_ENTRIES", sizeof(tL2DhcpSnpIfaceEntry),MAX_L2DS_IFACE_ENTRIES, MAX_L2DS_IFACE_ENTRIES,0 },
{ "UINT1[1600]", "MAX_L2DS_PDU_COUNT", sizeof(UINT1[1600]),MAX_L2DS_PDU_COUNT, MAX_L2DS_PDU_COUNT,0 },
{ "tL2DhcpSnpPktInfo", "MAX_L2DS_PKT_INFO", sizeof(tL2DhcpSnpPktInfo),MAX_L2DS_PKT_INFO, MAX_L2DS_PKT_INFO,0 },
{ "tL2DhcpSnpQMsg", "MAX_L2DS_Q_MESG", sizeof(tL2DhcpSnpQMsg),MAX_L2DS_Q_MESG, MAX_L2DS_Q_MESG,0 },
{"\0","\0",0,0,0,0}
};
#else  /*  _L2DSSZ_C  */
extern tFsModSizingParams FsL2DSSizingParams [];
#endif /*  _L2DSSZ_C  */


