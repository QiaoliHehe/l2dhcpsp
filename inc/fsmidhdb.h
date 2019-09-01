/********************************************************************
* Copyright (C) 2006 Aricent Inc . All Rights Reserved
*
* $Id: fsmidhdb.h,v 1.2 2010/08/17 12:26:24 prabuc Exp $
*
* Description: Protocol Mib Data base
*********************************************************************/
#ifndef _FSMIDHDB_H
#define _FSMIDHDB_H

UINT1 FsMIDhcpSnpGlobalConfigTableINDEX [] = {SNMP_DATA_TYPE_INTEGER32};
UINT1 FsMIDhcpSnpInterfaceTableINDEX [] = {SNMP_DATA_TYPE_INTEGER32 ,SNMP_DATA_TYPE_INTEGER32};

UINT4 fsmidh [] ={1,3,6,1,4,1,29601,2,49};
tSNMP_OID_TYPE fsmidhOID = {9, fsmidh};


UINT4 FsMIDhcpSnpContextId [ ] ={1,3,6,1,4,1,29601,2,49,1,1,1,1};
UINT4 FsMIDhcpSnpSnoopingAdminStatus [ ] ={1,3,6,1,4,1,29601,2,49,1,1,1,2};
UINT4 FsMIDhcpSnpMacVerifyStatus [ ] ={1,3,6,1,4,1,29601,2,49,1,1,1,3};
UINT4 FsMIDhcpSnpVlanId [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,2};
UINT4 FsMIDhcpSnpVlanSnpStatus [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,3};
UINT4 FsMIDhcpSnpRxDiscovers [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,4};
UINT4 FsMIDhcpSnpRxRequests [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,5};
UINT4 FsMIDhcpSnpRxReleases [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,6};
UINT4 FsMIDhcpSnpRxDeclines [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,7};
UINT4 FsMIDhcpSnpRxInforms [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,8};
UINT4 FsMIDhcpSnpTxOffers [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,9};
UINT4 FsMIDhcpSnpTxAcks [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,10};
UINT4 FsMIDhcpSnpTxNaks [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,11};
UINT4 FsMIDhcpSnpNoOfDiscards [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,12};
UINT4 FsMIDhcpSnpMacDiscards [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,13};
UINT4 FsMIDhcpSnpServerDiscards [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,14};
UINT4 FsMIDhcpSnpOptionDiscards [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,15};
UINT4 FsMIDhcpSnpInterfaceStatus [ ] ={1,3,6,1,4,1,29601,2,49,2,1,1,16};




tMbDbEntry fsmidhMibEntry[]= {

{{13,FsMIDhcpSnpContextId}, GetNextIndexFsMIDhcpSnpGlobalConfigTable, NULL, NULL, NULL, NULL, SNMP_DATA_TYPE_INTEGER32, SNMP_NOACCESS, FsMIDhcpSnpGlobalConfigTableINDEX, 1, 0, 0, NULL},

{{13,FsMIDhcpSnpSnoopingAdminStatus}, GetNextIndexFsMIDhcpSnpGlobalConfigTable, FsMIDhcpSnpSnoopingAdminStatusGet, FsMIDhcpSnpSnoopingAdminStatusSet, FsMIDhcpSnpSnoopingAdminStatusTest, FsMIDhcpSnpGlobalConfigTableDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, FsMIDhcpSnpGlobalConfigTableINDEX, 1, 0, 0, "2"},

{{13,FsMIDhcpSnpMacVerifyStatus}, GetNextIndexFsMIDhcpSnpGlobalConfigTable, FsMIDhcpSnpMacVerifyStatusGet, FsMIDhcpSnpMacVerifyStatusSet, FsMIDhcpSnpMacVerifyStatusTest, FsMIDhcpSnpGlobalConfigTableDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, FsMIDhcpSnpGlobalConfigTableINDEX, 1, 0, 0, "1"},

{{13,FsMIDhcpSnpVlanId}, GetNextIndexFsMIDhcpSnpInterfaceTable, NULL, NULL, NULL, NULL, SNMP_DATA_TYPE_INTEGER32, SNMP_NOACCESS, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpVlanSnpStatus}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpVlanSnpStatusGet, FsMIDhcpSnpVlanSnpStatusSet, FsMIDhcpSnpVlanSnpStatusTest, FsMIDhcpSnpInterfaceTableDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, "2"},

{{13,FsMIDhcpSnpRxDiscovers}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpRxDiscoversGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpRxRequests}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpRxRequestsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpRxReleases}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpRxReleasesGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpRxDeclines}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpRxDeclinesGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpRxInforms}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpRxInformsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpTxOffers}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpTxOffersGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpTxAcks}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpTxAcksGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpTxNaks}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpTxNaksGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpNoOfDiscards}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpNoOfDiscardsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpMacDiscards}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpMacDiscardsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpServerDiscards}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpServerDiscardsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpOptionDiscards}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpOptionDiscardsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 0, NULL},

{{13,FsMIDhcpSnpInterfaceStatus}, GetNextIndexFsMIDhcpSnpInterfaceTable, FsMIDhcpSnpInterfaceStatusGet, FsMIDhcpSnpInterfaceStatusSet, FsMIDhcpSnpInterfaceStatusTest, FsMIDhcpSnpInterfaceTableDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, FsMIDhcpSnpInterfaceTableINDEX, 2, 0, 1, NULL},
};
tMibData fsmidhEntry = { 18, fsmidhMibEntry };

#endif /* _FSMIDHDB_H */

