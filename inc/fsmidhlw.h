/********************************************************************
* Copyright (C) 2006 Aricent Inc . All Rights Reserved
*
* $Id: fsmidhlw.h,v 1.2 2010/08/17 12:26:24 prabuc Exp $
*
* Description: Proto types for Low Level  Routines
*********************************************************************/

#ifndef _FSMIDHLW_H
#define _FSMIDHLW_H
/* Proto Validate Index Instance for FsMIDhcpSnpGlobalConfigTable. */
INT1
nmhValidateIndexInstanceFsMIDhcpSnpGlobalConfigTable ARG_LIST((INT4 ));

/* Proto Type for Low Level GET FIRST fn for FsMIDhcpSnpGlobalConfigTable  */

INT1
nmhGetFirstIndexFsMIDhcpSnpGlobalConfigTable ARG_LIST((INT4 *));

/* Proto type for GET_NEXT Routine.  */

INT1
nmhGetNextIndexFsMIDhcpSnpGlobalConfigTable ARG_LIST((INT4 , INT4 *));

/* Proto type for Low Level GET Routine All Objects.  */

INT1
nmhGetFsMIDhcpSnpSnoopingAdminStatus ARG_LIST((INT4 ,INT4 *));

INT1
nmhGetFsMIDhcpSnpMacVerifyStatus ARG_LIST((INT4 ,INT4 *));

/* Low Level SET Routine for All Objects.  */

INT1
nmhSetFsMIDhcpSnpSnoopingAdminStatus ARG_LIST((INT4  ,INT4 ));

INT1
nmhSetFsMIDhcpSnpMacVerifyStatus ARG_LIST((INT4  ,INT4 ));

/* Low Level TEST Routines for.  */

INT1
nmhTestv2FsMIDhcpSnpSnoopingAdminStatus ARG_LIST((UINT4 *  ,INT4  ,INT4 ));

INT1
nmhTestv2FsMIDhcpSnpMacVerifyStatus ARG_LIST((UINT4 *  ,INT4  ,INT4 ));

/* Low Level DEP Routines for.  */

INT1
nmhDepv2FsMIDhcpSnpGlobalConfigTable ARG_LIST((UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*));

/* Proto Validate Index Instance for FsMIDhcpSnpInterfaceTable. */
INT1
nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable ARG_LIST((INT4  , INT4 ));

/* Proto Type for Low Level GET FIRST fn for FsMIDhcpSnpInterfaceTable  */

INT1
nmhGetFirstIndexFsMIDhcpSnpInterfaceTable ARG_LIST((INT4 * , INT4 *));

/* Proto type for GET_NEXT Routine.  */

INT1
nmhGetNextIndexFsMIDhcpSnpInterfaceTable ARG_LIST((INT4 , INT4 * , INT4 , INT4 *));

/* Proto type for Low Level GET Routine All Objects.  */

INT1
nmhGetFsMIDhcpSnpVlanSnpStatus ARG_LIST((INT4  , INT4 ,INT4 *));

INT1
nmhGetFsMIDhcpSnpRxDiscovers ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpRxRequests ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpRxReleases ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpRxDeclines ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpRxInforms ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpTxOffers ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpTxAcks ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpTxNaks ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpNoOfDiscards ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpMacDiscards ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpServerDiscards ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpOptionDiscards ARG_LIST((INT4  , INT4 ,UINT4 *));

INT1
nmhGetFsMIDhcpSnpInterfaceStatus ARG_LIST((INT4  , INT4 ,INT4 *));

/* Low Level SET Routine for All Objects.  */

INT1
nmhSetFsMIDhcpSnpVlanSnpStatus ARG_LIST((INT4  , INT4  ,INT4 ));

INT1
nmhSetFsMIDhcpSnpInterfaceStatus ARG_LIST((INT4  , INT4  ,INT4 ));

/* Low Level TEST Routines for.  */

INT1
nmhTestv2FsMIDhcpSnpVlanSnpStatus ARG_LIST((UINT4 *  ,INT4  , INT4  ,INT4 ));

INT1
nmhTestv2FsMIDhcpSnpInterfaceStatus ARG_LIST((UINT4 *  ,INT4  , INT4  ,INT4 ));

/* Low Level DEP Routines for.  */

INT1
nmhDepv2FsMIDhcpSnpInterfaceTable ARG_LIST((UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*));
#endif 
