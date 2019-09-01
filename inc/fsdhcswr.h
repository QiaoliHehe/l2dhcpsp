#ifndef _FSDHCSWR_H
#define _FSDHCSWR_H

VOID RegisterFSDHCS(VOID);

VOID UnRegisterFSDHCS(VOID);
INT4 FsDhcpSnpSnoopingAdminStatusGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpMacVerifyStatusGet(tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpInsertOption82StatusGet(tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpFwdOption82StatusGet(tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpBackupDBStatusGet(tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpBackupDBIntervalGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpSnoopingAdminStatusSet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpMacVerifyStatusSet(tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpInsertOption82StatusSet(tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpFwdOption82StatusSet(tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpBackupDBStatusSet(tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpBackupDBIntervalSet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpSnoopingAdminStatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpMacVerifyStatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpInsertOption82StatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpFwdOption82StatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpBackupDBStatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 CmDhcpSnpBackupDBIntervalTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpSnoopingAdminStatusDep(UINT4 *, tSnmpIndexList *, tSNMP_VAR_BIND *);
INT4 FsDhcpSnpMacVerifyStatusDep(UINT4 *, tSnmpIndexList *, tSNMP_VAR_BIND *);
INT4 CmDhcpSnpInsertOption82StatusDep(UINT4 *, tSnmpIndexList *, tSNMP_VAR_BIND *);
INT4 CmDhcpSnpFwdOption82StatusDep(UINT4 *, tSnmpIndexList *, tSNMP_VAR_BIND *);
INT4 CmDhcpSnpBackupDBStatusDep(UINT4 *, tSnmpIndexList *, tSNMP_VAR_BIND *);
INT4 CmDhcpSnpBackupDBIntervalDep(UINT4 *, tSnmpIndexList *, tSNMP_VAR_BIND *);
INT4 GetNextIndexFsDhcpSnpInterfaceTable(tSnmpIndex *, tSnmpIndex *);
INT4 FsDhcpSnpVlanIdGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpVlanSnpStatusGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpRxDiscoversGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpRxRequestsGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpRxReleasesGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpRxDeclinesGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpRxInformsGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpTxOffersGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpTxAcksGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpTxNaksGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpNoOfDiscardsGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpMacDiscardsGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpServerDiscardsGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpOptionDiscardsGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpInterfaceStatusGet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpVlanSnpStatusSet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpInterfaceStatusSet(tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpVlanSnpStatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpInterfaceStatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 FsDhcpSnpInterfaceTableDep(UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*);
#endif /* _FSDHCSWR_H */
