/********************************************************************
* Copyright (C) 2006 Aricent Inc . All Rights Reserved
*
* $Id: fsdhcslw.h,v 1.2 2010/06/17 07:00:16 prabuc Exp $
*
* Description: This file contains the Proto types for 
*              Low Level Routines
*********************************************************************/

/* Proto type for Low Level GET Routine All Objects.  */

#ifndef _FSDHCLW_H
#define _FSDHCLW_H


INT1
nmhGetFsDhcpSnpSnoopingAdminStatus ARG_LIST((INT4 *));

INT1
nmhGetFsDhcpSnpMacVerifyStatus ARG_LIST((INT4 *));

INT1
nmhGetCmDhcpSnpInsertOption82Status ARG_LIST((INT4 *));

INT1
nmhGetCmDhcpSnpFwdOption82Status ARG_LIST((INT4 *));

INT1
nmhGetCmDhcpSnpBackupDBStatus ARG_LIST((INT4 *));

INT1
nmhGetCmDhcpSnpBackupDBInterval ARG_LIST((INT4 *));

/* Low Level SET Routine for All Objects.  */

INT1
nmhSetFsDhcpSnpSnoopingAdminStatus ARG_LIST((INT4 ));

INT1
nmhSetFsDhcpSnpMacVerifyStatus ARG_LIST((INT4 ));

INT1
nmhSetCmDhcpSnpInsertOption82Status ARG_LIST((INT4 ));

INT1
nmhSetCmDhcpSnpFwdOption82Status ARG_LIST((INT4 ));

INT1
nmhSetCmDhcpSnpBackupDBStatus ARG_LIST((INT4 ));

INT1
nmhSetCmDhcpSnpBackupDBInterval ARG_LIST((INT4 ));

/* Low Level TEST Routines for.  */

INT1
nmhTestv2FsDhcpSnpSnoopingAdminStatus ARG_LIST((UINT4 *  ,INT4 ));

INT1
nmhTestv2FsDhcpSnpMacVerifyStatus ARG_LIST((UINT4 *  ,INT4 ));

INT1
nmhTestv2CmDhcpSnpInsertOption82Status ARG_LIST((UINT4 *  ,INT4 ));

INT1
nmhTestv2CmDhcpSnpFwdOption82Status ARG_LIST((UINT4 *  ,INT4 ));

INT1
nmhTestv2CmDhcpSnpBackupDBStatus ARG_LIST((UINT4 *  ,INT4 ));

INT1
nmhTestv2CmDhcpSnpBackupDBInterval ARG_LIST((UINT4 *  ,INT4 ));

/* Low Level DEP Routines for.  */

INT1
nmhDepv2FsDhcpSnpSnoopingAdminStatus ARG_LIST((UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*));

INT1
nmhDepv2FsDhcpSnpMacVerifyStatus ARG_LIST((UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*));

INT1
nmhDepv2CmDhcpSnpInsertOption82Status ARG_LIST((UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*));

INT1
nmhDepv2CmDhcpSnpFwdOption82Status ARG_LIST((UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*));

INT1
nmhDepv2CmDhcpSnpBackupDBStatus ARG_LIST((UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*));

INT1
nmhDepv2CmDhcpSnpBackupDBInterval ARG_LIST((UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*));

/* Proto Validate Index Instance for FsDhcpSnpInterfaceTable. */
INT1
nmhValidateIndexInstanceFsDhcpSnpInterfaceTable ARG_LIST((INT4 ));

/* Proto Type for Low Level GET FIRST fn for FsDhcpSnpInterfaceTable  */

INT1
nmhGetFirstIndexFsDhcpSnpInterfaceTable ARG_LIST((INT4 *));

/* Proto type for GET_NEXT Routine.  */

INT1
nmhGetNextIndexFsDhcpSnpInterfaceTable ARG_LIST((INT4 , INT4 *));

/* Proto type for Low Level GET Routine All Objects.  */
INT1
nmhGetFsDhcpSnpVlanId ARG_LIST((INT4 ,INT4 *));

INT1
nmhGetFsDhcpSnpVlanSnpStatus ARG_LIST((INT4 ,INT4 *));

INT1
nmhGetFsDhcpSnpRxDiscovers ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpRxRequests ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpRxReleases ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpRxDeclines ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpRxInforms ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpTxOffers ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpTxAcks ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpTxNaks ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpNoOfDiscards ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpMacDiscards ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpServerDiscards ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpOptionDiscards ARG_LIST((INT4 ,UINT4 *));

INT1
nmhGetFsDhcpSnpInterfaceStatus ARG_LIST((INT4 ,INT4 *));

/* Low Level SET Routine for All Objects.  */

INT1
nmhSetFsDhcpSnpVlanSnpStatus ARG_LIST((INT4  ,INT4 ));

INT1
nmhSetFsDhcpSnpInterfaceStatus ARG_LIST((INT4  ,INT4 ));

/* Low Level TEST Routines for.  */

INT1
nmhTestv2FsDhcpSnpVlanSnpStatus ARG_LIST((UINT4 *  ,INT4  ,INT4 ));

INT1
nmhTestv2FsDhcpSnpInterfaceStatus ARG_LIST((UINT4 *  ,INT4  ,INT4 ));

/* Low Level DEP Routines for.  */

INT1
nmhDepv2FsDhcpSnpInterfaceTable ARG_LIST((UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*));

#endif
