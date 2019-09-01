#ifndef _FSMIDHWR_H
#define _FSMIDHWR_H
INT4 GetNextIndexFsMIDhcpSnpGlobalConfigTable(tSnmpIndex *, tSnmpIndex *);

VOID RegisterFSMIDH(VOID);

VOID UnRegisterFSMIDH(VOID);
INT4 FsMIDhcpSnpSnoopingAdminStatusGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpMacVerifyStatusGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpSnoopingAdminStatusSet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpMacVerifyStatusSet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpSnoopingAdminStatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpMacVerifyStatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpGlobalConfigTableDep(UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*);
INT4 GetNextIndexFsMIDhcpSnpInterfaceTable(tSnmpIndex *, tSnmpIndex *);
INT4 FsMIDhcpSnpVlanSnpStatusGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpRxDiscoversGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpRxRequestsGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpRxReleasesGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpRxDeclinesGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpRxInformsGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpTxOffersGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpTxAcksGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpTxNaksGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpNoOfDiscardsGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpMacDiscardsGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpServerDiscardsGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpOptionDiscardsGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpInterfaceStatusGet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpVlanSnpStatusSet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpInterfaceStatusSet(tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpVlanSnpStatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpInterfaceStatusTest(UINT4 *, tSnmpIndex *, tRetVal *);
INT4 FsMIDhcpSnpInterfaceTableDep(UINT4 *, tSnmpIndexList*, tSNMP_VAR_BIND*);
#endif /* _FSMIDHWR_H */
