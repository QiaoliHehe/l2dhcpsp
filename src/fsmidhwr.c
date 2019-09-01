# include  "lr.h"
# include  "fssnmp.h"
# include  "fsmidhlw.h"
# include  "fsmidhwr.h"
# include  "fsmidhdb.h"

INT4
GetNextIndexFsMIDhcpSnpGlobalConfigTable (tSnmpIndex * pFirstMultiIndex,
                                          tSnmpIndex * pNextMultiIndex)
{
    if (pFirstMultiIndex == NULL)
    {
        if (nmhGetFirstIndexFsMIDhcpSnpGlobalConfigTable
            (&(pNextMultiIndex->pIndex[0].i4_SLongValue)) == SNMP_FAILURE)
        {
            return SNMP_FAILURE;
        }
    }
    else
    {
        if (nmhGetNextIndexFsMIDhcpSnpGlobalConfigTable
            (pFirstMultiIndex->pIndex[0].i4_SLongValue,
             &(pNextMultiIndex->pIndex[0].i4_SLongValue)) == SNMP_FAILURE)
        {
            return SNMP_FAILURE;
        }
    }

    return SNMP_SUCCESS;
}

VOID
RegisterFSMIDH ()
{
    SNMPRegisterMibWithLock (&fsmidhOID, &fsmidhEntry, NULL, NULL,
                             SNMP_MSR_TGR_FALSE);
    SNMPAddSysorEntry (&fsmidhOID, (const UINT1 *) "fsmidhcsnp");
}

VOID
UnRegisterFSMIDH ()
{
    SNMPUnRegisterMib (&fsmidhOID, &fsmidhEntry);
    SNMPDelSysorEntry (&fsmidhOID, (const UINT1 *) "fsmidhcsnp");
}

INT4
FsMIDhcpSnpSnoopingAdminStatusGet (tSnmpIndex * pMultiIndex,
                                   tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpGlobalConfigTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpSnoopingAdminStatus
            (pMultiIndex->pIndex[0].i4_SLongValue,
             &(pMultiData->i4_SLongValue)));

}

INT4
FsMIDhcpSnpMacVerifyStatusGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpGlobalConfigTable
        (pMultiIndex->pIndex[0].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpMacVerifyStatus
            (pMultiIndex->pIndex[0].i4_SLongValue,
             &(pMultiData->i4_SLongValue)));

}

INT4
FsMIDhcpSnpSnoopingAdminStatusSet (tSnmpIndex * pMultiIndex,
                                   tRetVal * pMultiData)
{
    return (nmhSetFsMIDhcpSnpSnoopingAdminStatus
            (pMultiIndex->pIndex[0].i4_SLongValue, pMultiData->i4_SLongValue));

}

INT4
FsMIDhcpSnpMacVerifyStatusSet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    return (nmhSetFsMIDhcpSnpMacVerifyStatus
            (pMultiIndex->pIndex[0].i4_SLongValue, pMultiData->i4_SLongValue));

}

INT4
FsMIDhcpSnpSnoopingAdminStatusTest (UINT4 *pu4Error, tSnmpIndex * pMultiIndex,
                                    tRetVal * pMultiData)
{
    return (nmhTestv2FsMIDhcpSnpSnoopingAdminStatus (pu4Error,
                                                     pMultiIndex->pIndex[0].
                                                     i4_SLongValue,
                                                     pMultiData->
                                                     i4_SLongValue));

}

INT4
FsMIDhcpSnpMacVerifyStatusTest (UINT4 *pu4Error, tSnmpIndex * pMultiIndex,
                                tRetVal * pMultiData)
{
    return (nmhTestv2FsMIDhcpSnpMacVerifyStatus (pu4Error,
                                                 pMultiIndex->pIndex[0].
                                                 i4_SLongValue,
                                                 pMultiData->i4_SLongValue));

}

INT4
FsMIDhcpSnpGlobalConfigTableDep (UINT4 *pu4Error,
                                 tSnmpIndexList * pSnmpIndexList,
                                 tSNMP_VAR_BIND * pSnmpvarbinds)
{
    return (nmhDepv2FsMIDhcpSnpGlobalConfigTable (pu4Error, pSnmpIndexList,
                                                  pSnmpvarbinds));
}

INT4
GetNextIndexFsMIDhcpSnpInterfaceTable (tSnmpIndex * pFirstMultiIndex,
                                       tSnmpIndex * pNextMultiIndex)
{
    if (pFirstMultiIndex == NULL)
    {
        if (nmhGetFirstIndexFsMIDhcpSnpInterfaceTable
            (&(pNextMultiIndex->pIndex[0].i4_SLongValue),
             &(pNextMultiIndex->pIndex[1].i4_SLongValue)) == SNMP_FAILURE)
        {
            return SNMP_FAILURE;
        }
    }
    else
    {
        if (nmhGetNextIndexFsMIDhcpSnpInterfaceTable
            (pFirstMultiIndex->pIndex[0].i4_SLongValue,
             &(pNextMultiIndex->pIndex[0].i4_SLongValue),
             pFirstMultiIndex->pIndex[1].i4_SLongValue,
             &(pNextMultiIndex->pIndex[1].i4_SLongValue)) == SNMP_FAILURE)
        {
            return SNMP_FAILURE;
        }
    }

    return SNMP_SUCCESS;
}

INT4
FsMIDhcpSnpVlanSnpStatusGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpVlanSnpStatus
            (pMultiIndex->pIndex[0].i4_SLongValue,
             pMultiIndex->pIndex[1].i4_SLongValue,
             &(pMultiData->i4_SLongValue)));

}

INT4
FsMIDhcpSnpRxDiscoversGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpRxDiscovers (pMultiIndex->pIndex[0].i4_SLongValue,
                                          pMultiIndex->pIndex[1].i4_SLongValue,
                                          &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpRxRequestsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpRxRequests (pMultiIndex->pIndex[0].i4_SLongValue,
                                         pMultiIndex->pIndex[1].i4_SLongValue,
                                         &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpRxReleasesGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpRxReleases (pMultiIndex->pIndex[0].i4_SLongValue,
                                         pMultiIndex->pIndex[1].i4_SLongValue,
                                         &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpRxDeclinesGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpRxDeclines (pMultiIndex->pIndex[0].i4_SLongValue,
                                         pMultiIndex->pIndex[1].i4_SLongValue,
                                         &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpRxInformsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpRxInforms (pMultiIndex->pIndex[0].i4_SLongValue,
                                        pMultiIndex->pIndex[1].i4_SLongValue,
                                        &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpTxOffersGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpTxOffers (pMultiIndex->pIndex[0].i4_SLongValue,
                                       pMultiIndex->pIndex[1].i4_SLongValue,
                                       &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpTxAcksGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpTxAcks (pMultiIndex->pIndex[0].i4_SLongValue,
                                     pMultiIndex->pIndex[1].i4_SLongValue,
                                     &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpTxNaksGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpTxNaks (pMultiIndex->pIndex[0].i4_SLongValue,
                                     pMultiIndex->pIndex[1].i4_SLongValue,
                                     &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpNoOfDiscardsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpNoOfDiscards (pMultiIndex->pIndex[0].i4_SLongValue,
                                           pMultiIndex->pIndex[1].i4_SLongValue,
                                           &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpMacDiscardsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpMacDiscards (pMultiIndex->pIndex[0].i4_SLongValue,
                                          pMultiIndex->pIndex[1].i4_SLongValue,
                                          &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpServerDiscardsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpServerDiscards
            (pMultiIndex->pIndex[0].i4_SLongValue,
             pMultiIndex->pIndex[1].i4_SLongValue,
             &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpOptionDiscardsGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpOptionDiscards
            (pMultiIndex->pIndex[0].i4_SLongValue,
             pMultiIndex->pIndex[1].i4_SLongValue,
             &(pMultiData->u4_ULongValue)));

}

INT4
FsMIDhcpSnpInterfaceStatusGet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    if (nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
        (pMultiIndex->pIndex[0].i4_SLongValue,
         pMultiIndex->pIndex[1].i4_SLongValue) == SNMP_FAILURE)
    {
        return SNMP_FAILURE;
    }
    return (nmhGetFsMIDhcpSnpInterfaceStatus
            (pMultiIndex->pIndex[0].i4_SLongValue,
             pMultiIndex->pIndex[1].i4_SLongValue,
             &(pMultiData->i4_SLongValue)));

}

INT4
FsMIDhcpSnpVlanSnpStatusSet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    return (nmhSetFsMIDhcpSnpVlanSnpStatus
            (pMultiIndex->pIndex[0].i4_SLongValue,
             pMultiIndex->pIndex[1].i4_SLongValue, pMultiData->i4_SLongValue));

}

INT4
FsMIDhcpSnpInterfaceStatusSet (tSnmpIndex * pMultiIndex, tRetVal * pMultiData)
{
    return (nmhSetFsMIDhcpSnpInterfaceStatus
            (pMultiIndex->pIndex[0].i4_SLongValue,
             pMultiIndex->pIndex[1].i4_SLongValue, pMultiData->i4_SLongValue));

}

INT4
FsMIDhcpSnpVlanSnpStatusTest (UINT4 *pu4Error, tSnmpIndex * pMultiIndex,
                              tRetVal * pMultiData)
{
    return (nmhTestv2FsMIDhcpSnpVlanSnpStatus (pu4Error,
                                               pMultiIndex->pIndex[0].
                                               i4_SLongValue,
                                               pMultiIndex->pIndex[1].
                                               i4_SLongValue,
                                               pMultiData->i4_SLongValue));

}

INT4
FsMIDhcpSnpInterfaceStatusTest (UINT4 *pu4Error, tSnmpIndex * pMultiIndex,
                                tRetVal * pMultiData)
{
    return (nmhTestv2FsMIDhcpSnpInterfaceStatus (pu4Error,
                                                 pMultiIndex->pIndex[0].
                                                 i4_SLongValue,
                                                 pMultiIndex->pIndex[1].
                                                 i4_SLongValue,
                                                 pMultiData->i4_SLongValue));

}

INT4
FsMIDhcpSnpInterfaceTableDep (UINT4 *pu4Error, tSnmpIndexList * pSnmpIndexList,
                              tSNMP_VAR_BIND * pSnmpvarbinds)
{
    return (nmhDepv2FsMIDhcpSnpInterfaceTable (pu4Error, pSnmpIndexList,
                                               pSnmpvarbinds));
}
