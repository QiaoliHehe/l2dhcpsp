/********************************************************************
* Copyright (C) 2006 Aricent Inc . All Rights Reserved
*
* $Id: fsdhcsdb.h,v 1.2 2010/06/17 07:00:16 prabuc Exp $
*
* Description: Protocol Mib Data base
*********************************************************************/
#ifndef _FSDHCSDB_H
#define _FSDHCSDB_H

UINT1 FsDhcpSnpInterfaceTableINDEX [] = {SNMP_DATA_TYPE_INTEGER32};

UINT4 fsdhcs [] ={1,3,6,1,4,1,29601,2,3};
tSNMP_OID_TYPE fsdhcsOID = {9, fsdhcs};


UINT4 FsDhcpSnpSnoopingAdminStatus [ ] ={1,3,6,1,4,1,29601,2,3,1,1};
UINT4 FsDhcpSnpMacVerifyStatus [ ] ={1,3,6,1,4,1,29601,2,3,1,2};
/* CAMEOTAG : add by Ricann 20120904,
*   extend internal mib with following four items.
*/
UINT4 CmDhcpSnpInsertOption82Status [ ] ={1,3,6,1,4,1,29601,2,3,1,100};
UINT4 CmDhcpSnpFwdOption82Status [ ] ={1,3,6,1,4,1,29601,2,3,1,101};
UINT4 CmDhcpSnpBackupDBStatus [ ] ={1,3,6,1,4,1,29601,2,3,1,102};
UINT4 CmDhcpSnpBackupDBInterval [ ] ={1,3,6,1,4,1,29601,2,3,1,103};
UINT4 FsDhcpSnpVlanId [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,1};
UINT4 FsDhcpSnpVlanSnpStatus [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,2};
UINT4 FsDhcpSnpRxDiscovers [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,3};
UINT4 FsDhcpSnpRxRequests [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,4};
UINT4 FsDhcpSnpRxReleases [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,5};
UINT4 FsDhcpSnpRxDeclines [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,6};
UINT4 FsDhcpSnpRxInforms [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,7};
UINT4 FsDhcpSnpTxOffers [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,8};
UINT4 FsDhcpSnpTxAcks [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,9};
UINT4 FsDhcpSnpTxNaks [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,10};
UINT4 FsDhcpSnpNoOfDiscards [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,11};
UINT4 FsDhcpSnpMacDiscards [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,12};
UINT4 FsDhcpSnpServerDiscards [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,13};
UINT4 FsDhcpSnpOptionDiscards [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,14};
UINT4 FsDhcpSnpInterfaceStatus [ ] ={1,3,6,1,4,1,29601,2,3,2,1,1,15};




tMbDbEntry fsdhcsMibEntry[]= {

{{11,FsDhcpSnpSnoopingAdminStatus}, NULL, FsDhcpSnpSnoopingAdminStatusGet, FsDhcpSnpSnoopingAdminStatusSet, FsDhcpSnpSnoopingAdminStatusTest, FsDhcpSnpSnoopingAdminStatusDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, NULL, 0, 0, 0, "2"},

{{11,FsDhcpSnpMacVerifyStatus}, NULL, FsDhcpSnpMacVerifyStatusGet, FsDhcpSnpMacVerifyStatusSet, FsDhcpSnpMacVerifyStatusTest, FsDhcpSnpMacVerifyStatusDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, NULL, 0, 0, 0, "1"},

/* CAMEOTAG : add by Ricann 20120904,
*   extend internal mib with following four items.
*/
{{11,CmDhcpSnpInsertOption82Status}, NULL, CmDhcpSnpInsertOption82StatusGet, CmDhcpSnpInsertOption82StatusSet, CmDhcpSnpInsertOption82StatusTest, CmDhcpSnpInsertOption82StatusDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, NULL, 0, 0, 0, "2"},

{{11,CmDhcpSnpFwdOption82Status}, NULL, CmDhcpSnpFwdOption82StatusGet, CmDhcpSnpFwdOption82StatusSet, CmDhcpSnpFwdOption82StatusTest, CmDhcpSnpFwdOption82StatusDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, NULL, 0, 0, 0, "2"},

{{11,CmDhcpSnpBackupDBStatus}, NULL, CmDhcpSnpBackupDBStatusGet, CmDhcpSnpBackupDBStatusSet, CmDhcpSnpBackupDBStatusTest, CmDhcpSnpBackupDBStatusDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, NULL, 0, 0, 0, "2"},

{{11,CmDhcpSnpBackupDBInterval}, NULL, CmDhcpSnpBackupDBIntervalGet, CmDhcpSnpBackupDBIntervalSet, CmDhcpSnpBackupDBIntervalTest, CmDhcpSnpBackupDBIntervalDep, SNMP_DATA_TYPE_INTEGER32, SNMP_READWRITE, NULL, 0, 0, 0, "1200"},

/* CAMEOTAG: Add by zhangming 2014-05-07, Support vlan id index get function */
{{13,FsDhcpSnpVlanId}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpVlanIdGet, NULL, NULL, NULL, SNMP_DATA_TYPE_INTEGER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpVlanSnpStatus}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpVlanSnpStatusGet, FsDhcpSnpVlanSnpStatusSet, FsDhcpSnpVlanSnpStatusTest, FsDhcpSnpInterfaceTableDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, "2"},

{{13,FsDhcpSnpRxDiscovers}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpRxDiscoversGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpRxRequests}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpRxRequestsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpRxReleases}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpRxReleasesGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpRxDeclines}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpRxDeclinesGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpRxInforms}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpRxInformsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpTxOffers}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpTxOffersGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpTxAcks}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpTxAcksGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpTxNaks}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpTxNaksGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpNoOfDiscards}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpNoOfDiscardsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpMacDiscards}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpMacDiscardsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpServerDiscards}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpServerDiscardsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpOptionDiscards}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpOptionDiscardsGet, NULL, NULL, NULL, SNMP_DATA_TYPE_COUNTER32, SNMP_READONLY, FsDhcpSnpInterfaceTableINDEX, 1, 0, 0, NULL},

{{13,FsDhcpSnpInterfaceStatus}, GetNextIndexFsDhcpSnpInterfaceTable, FsDhcpSnpInterfaceStatusGet, FsDhcpSnpInterfaceStatusSet, FsDhcpSnpInterfaceStatusTest, FsDhcpSnpInterfaceTableDep, SNMP_DATA_TYPE_INTEGER, SNMP_READWRITE, FsDhcpSnpInterfaceTableINDEX, 1, 0, 1, NULL},
};
tMibData fsdhcsEntry = { 21, fsdhcsMibEntry };

#endif /* _FSDHCSDB_H */

