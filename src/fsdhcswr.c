# include  "lr.h"
# include  "fssnmp.h"
# include  "fsdhcslw.h"
# include  "fsdhcswr.h"
# include  "fsdhcsdb.h"

VOID
RegisterFSDHCS ()
{
    /*CAMEOTAG: Modified by Aiyong, 2013-12-19, Change SNMP_MSR_TGR_FALSE to SNMP_MSR_TGR_TRUE
     * If we use "SNMP_MSR_TGR_FALSE" to register mib, the funtion changed OID value in master 
     * will not be synchronized to slave through SNMP module's dynamic update
     */
    SNMPRegisterMibWithLock (&fsdhcsOID, &fsdhcsEntry,
                             NULL, NULL, SNMP_MSR_TGR_TRUE);
    SNMPAddSysorEntry (&fsdhcsOID, (const UINT1 *) "fsdhcsnp");
}

VOID
UnRegisterFSDHCS ()
{
    SNMPUnRegisterMib (&fsdhcsOID, &fsdhcsEntry);
    SNMPDelSysorEntry (&fsdhcsOID, (const UINT1 *) "fsdhcsnp");
}

INT4
FsDhcpSnpSnoopingAdminStatusGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    UNUSED_PARAM (pMultiIndex);
    return (nmhGetFsDhcpSnpSnoopingAdminStatus (&(pMultiData->i4_SLongValue)));
}

INT4
FsDhcpSnpMacVerifyStatusGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    UNUSED_PARAM (pMultiIndex);
    return (nmhGetFsDhcpSnpMacVerifyStatus (&(pMultiData->i4_SLongValue)));
}
INT4 CmDhcpSnpInsertOption82StatusGet(tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhGetCmDhcpSnpInsertOption82Status(&(pMultiData->i4_SLongValue)));
}
INT4 CmDhcpSnpFwdOption82StatusGet(tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhGetCmDhcpSnpFwdOption82Status(&(pMultiData->i4_SLongValue)));
}
INT4 CmDhcpSnpBackupDBStatusGet(tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhGetCmDhcpSnpBackupDBStatus(&(pMultiData->i4_SLongValue)));
}
INT4 CmDhcpSnpBackupDBIntervalGet(tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhGetCmDhcpSnpBackupDBInterval(&(pMultiData->i4_SLongValue)));
}
INT4 FsDhcpSnpSnoopingAdminStatusSet(tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    UNUSED_PARAM (pMultiIndex);
    return (nmhSetFsDhcpSnpSnoopingAdminStatus (pMultiData->i4_SLongValue));
}

INT4
FsDhcpSnpMacVerifyStatusSet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    UNUSED_PARAM (pMultiIndex);
    return (nmhSetFsDhcpSnpMacVerifyStatus (pMultiData->i4_SLongValue));
}


INT4 CmDhcpSnpInsertOption82StatusSet(tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhSetCmDhcpSnpInsertOption82Status(pMultiData->i4_SLongValue));
}


INT4 CmDhcpSnpFwdOption82StatusSet(tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhSetCmDhcpSnpFwdOption82Status(pMultiData->i4_SLongValue));
}


INT4 CmDhcpSnpBackupDBStatusSet(tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhSetCmDhcpSnpBackupDBStatus(pMultiData->i4_SLongValue));
}


INT4 CmDhcpSnpBackupDBIntervalSet(tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhSetCmDhcpSnpBackupDBInterval(pMultiData->i4_SLongValue));
}

INT4
FsDhcpSnpSnoopingAdminStatusTest (UINT4 *pu4Error,
                                  tSnmpIndex * pMultiIndex,
                                  tRetVal * pMultiData)
{
    UNUSED_PARAM (pMultiIndex);
    return (nmhTestv2FsDhcpSnpSnoopingAdminStatus (pu4Error,
                                                   pMultiData->i4_SLongValue));
}

INT4
FsDhcpSnpMacVerifyStatusTest (UINT4 *pu4Error,
                              tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    UNUSED_PARAM (pMultiIndex);
    return (nmhTestv2FsDhcpSnpMacVerifyStatus (pu4Error,
                                               pMultiData->i4_SLongValue));
}
INT4 CmDhcpSnpInsertOption82StatusTest(UINT4 *pu4Error ,tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhTestv2CmDhcpSnpInsertOption82Status(pu4Error, pMultiData->i4_SLongValue));
}


INT4 CmDhcpSnpFwdOption82StatusTest(UINT4 *pu4Error ,tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhTestv2CmDhcpSnpFwdOption82Status(pu4Error, pMultiData->i4_SLongValue));
}


INT4 CmDhcpSnpBackupDBStatusTest(UINT4 *pu4Error ,tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhTestv2CmDhcpSnpBackupDBStatus(pu4Error, pMultiData->i4_SLongValue));
}


INT4 CmDhcpSnpBackupDBIntervalTest(UINT4 *pu4Error ,tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
	UNUSED_PARAM(pMultiIndex);
	return(nmhTestv2CmDhcpSnpBackupDBInterval(pu4Error, pMultiData->i4_SLongValue));
}

INT4
FsDhcpSnpSnoopingAdminStatusDep (UINT4 *pu4Error,
                                 tSnmpIndexList * pSnmpIndexList,
                                 tSNMP_VAR_BIND * pSnmpvarbinds)
{
    return (nmhDepv2FsDhcpSnpSnoopingAdminStatus (pu4Error,
                                                  pSnmpIndexList,
                                                  pSnmpvarbinds));
}

INT4
FsDhcpSnpMacVerifyStatusDep (UINT4 *pu4Error,
                             tSnmpIndexList * pSnmpIndexList,
                             tSNMP_VAR_BIND * pSnmpvarbinds)
{
    return (nmhDepv2FsDhcpSnpMacVerifyStatus (pu4Error,
                                              pSnmpIndexList, pSnmpvarbinds));
}
INT4 CmDhcpSnpInsertOption82StatusDep(UINT4 *pu4Error,tSnmpIndexList *pSnmpIndexList, tSNMP_VAR_BIND *pSnmpvarbinds)
{
	return(nmhDepv2CmDhcpSnpInsertOption82Status(pu4Error, pSnmpIndexList, pSnmpvarbinds));
}

INT4 CmDhcpSnpFwdOption82StatusDep(UINT4 *pu4Error,tSnmpIndexList *pSnmpIndexList, tSNMP_VAR_BIND *pSnmpvarbinds)
{
	return(nmhDepv2CmDhcpSnpFwdOption82Status(pu4Error, pSnmpIndexList, pSnmpvarbinds));
}

INT4 CmDhcpSnpBackupDBStatusDep(UINT4 *pu4Error,tSnmpIndexList *pSnmpIndexList, tSNMP_VAR_BIND *pSnmpvarbinds)
{
	return(nmhDepv2CmDhcpSnpBackupDBStatus(pu4Error, pSnmpIndexList, pSnmpvarbinds));
}

INT4 CmDhcpSnpBackupDBIntervalDep(UINT4 *pu4Error,tSnmpIndexList *pSnmpIndexList, tSNMP_VAR_BIND *pSnmpvarbinds)
{
	return(nmhDepv2CmDhcpSnpBackupDBInterval(pu4Error, pSnmpIndexList, pSnmpvarbinds));
}

INT4
GetNextIndexFsDhcpSnpInterfaceTable (tSnmpIndex * pFirstMultiIndex,
                                     tSnmpIndex * pNextMultiIndex)
{
    if (pFirstMultiIndex == NULL)
    {
        if (nmhGetFirstIndexFsDhcpSnpInterfaceTable
            (&(pNextMultiIndex->pIndex[0].i4_SLongValue)) == SNMP_FAILURE)
        {
            return SNMP_FAILURE;
        }
    }
    else
    {
        if (nmhGetNextIndexFsDhcpSnpInterfaceTable
            (pFirstMultiIndex->pIndex[0].i4_SLongValue,
             &(pNextMultiIndex->pIndex[0].i4_SLongValue)) == SNMP_FAILURE)
        {
            return SNMP_FAILURE;
        }
    }

    return SNMP_SUCCESS;
}

INT4
FsDhcpSnpVlanIdGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpVlanId (pMultiIndex->pIndex[0].i4_SLongValue,
                                          &(pMultiData->i4_SLongValue)));

}

INT4
FsDhcpSnpVlanSnpStatusGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpVlanSnpStatus (pMultiIndex->pIndex[0].i4_SLongValue,
                                          &(pMultiData->i4_SLongValue)));

}

INT4
FsDhcpSnpRxDiscoversGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpRxDiscovers (pMultiIndex->pIndex[0].i4_SLongValue,
                                        &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpRxRequestsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpRxRequests (pMultiIndex->pIndex[0].i4_SLongValue,
                                       &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpRxReleasesGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpRxReleases (pMultiIndex->pIndex[0].i4_SLongValue,
                                       &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpRxDeclinesGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpRxDeclines (pMultiIndex->pIndex[0].i4_SLongValue,
                                       &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpRxInformsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpRxInforms (pMultiIndex->pIndex[0].i4_SLongValue,
                                      &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpTxOffersGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpTxOffers (pMultiIndex->pIndex[0].i4_SLongValue,
                                     &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpTxAcksGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpTxAcks (pMultiIndex->pIndex[0].i4_SLongValue,
                                   &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpTxNaksGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpTxNaks (pMultiIndex->pIndex[0].i4_SLongValue,
                                   &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpNoOfDiscardsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpNoOfDiscards (pMultiIndex->pIndex[0].i4_SLongValue,
                                         &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpMacDiscardsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpMacDiscards (pMultiIndex->pIndex[0].i4_SLongValue,
                                        &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpServerDiscardsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpServerDiscards (pMultiIndex->pIndex[0].i4_SLongValue,
                                           &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpOptionDiscardsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpOptionDiscards (pMultiIndex->pIndex[0].i4_SLongValue,
                                           &(pMultiData->u4_ULongValue)));

}

INT4
FsDhcpSnpInterfaceStatusGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsDhcpSnpInterfaceStatus
            (pMultiIndex->pIndex[0].i4_SLongValue,
             &(pMultiData->i4_SLongValue)));

}

INT4
FsDhcpSnpVlanSnpStatusSet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    return (nmhSetFsDhcpSnpVlanSnpStatus (pMultiIndex->pIndex[0].i4_SLongValue,
                                          pMultiData->i4_SLongValue));

}

INT4
FsDhcpSnpInterfaceStatusSet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    return (nmhSetFsDhcpSnpInterfaceStatus
            (pMultiIndex->pIndex[0].i4_SLongValue, pMultiData->i4_SLongValue));

}

INT4
FsDhcpSnpVlanSnpStatusTest (UINT4 *pu4Error,
                            tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    return (nmhTestv2FsDhcpSnpVlanSnpStatus (pu4Error,
                                             pMultiIndex->pIndex[0].
                                             i4_SLongValue,
                                             pMultiData->i4_SLongValue));

}

INT4
FsDhcpSnpInterfaceStatusTest (UINT4 *pu4Error,
                              tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    return (nmhTestv2FsDhcpSnpInterfaceStatus (pu4Error,
                                               pMultiIndex->pIndex[0].
                                               i4_SLongValue,
                                               pMultiData->i4_SLongValue));

}

INT4
FsDhcpSnpInterfaceTableDep (UINT4 *pu4Error,
                            tSnmpIndexList * pSnmpIndexList,
                            tSNMP_VAR_BIND * pSnmpvarbinds)
{
    return (nmhDepv2FsDhcpSnpInterfaceTable (pu4Error,
                                             pSnmpIndexList, pSnmpvarbinds));
}
