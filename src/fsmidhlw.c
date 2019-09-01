/********************************************************************
* Copyright (C) 2006 Aricent Inc . All Rights Reserved
*
* $Id: fsmidhlw.c,v 1.3 2010/09/17 06:24:55 prabuc Exp $
*
* Description: Protocol Low Level Routines
*********************************************************************/
#include "fssnmp.h"
#include "l2dsinc.h"
#include "l2dsextn.h"

/* LOW LEVEL Routines for Table : FsMIDhcpSnpGlobalConfigTable. */

/****************************************************************************
 Function    :  nmhValidateIndexInstanceFsMIDhcpSnpGlobalConfigTable
 Input       :  The Indices
                FsMIDhcpSnpContextId
 Output      :  The Routines Validates the Given Indices.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
/* GET_EXACT Validate Index Instance Routine. */

INT1
nmhValidateIndexInstanceFsMIDhcpSnpGlobalConfigTable (INT4
                                                      i4FsMIDhcpSnpContextId)
{
    if (L2dsVcmIsVcExist (i4FsMIDhcpSnpContextId) == L2DS_TRUE)
    {
        if ((i4FsMIDhcpSnpContextId >= L2DS_DEFAULT_CXT_ID) &&
            (i4FsMIDhcpSnpContextId <= L2DS_MAX_CONTEXTS))
        {
            return SNMP_SUCCESS;
        }
    }
    return SNMP_FAILURE;
}

/****************************************************************************
 Function    :  nmhGetFirstIndexFsMIDhcpSnpGlobalConfigTable
 Input       :  The Indices
                FsMIDhcpSnpContextId
 Output      :  The Get First Routines gets the Lexicographicaly
                First Entry from the Table.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
/* GET_FIRST Routine. */

INT1
nmhGetFirstIndexFsMIDhcpSnpGlobalConfigTable (INT4 *pi4FsMIDhcpSnpContextId)
{
    UINT4               u4ContextId = L2DS_ZERO;

    for (u4ContextId = L2DS_DEFAULT_CXT_ID;
         u4ContextId <= L2DS_MAX_CONTEXTS; u4ContextId++)
    {
        if (L2DS_IS_CONTEXT_EXIST (u4ContextId) == L2DS_TRUE)
        {
            *pi4FsMIDhcpSnpContextId = (INT4) u4ContextId;
            return SNMP_SUCCESS;
        }
    }

    return SNMP_FAILURE;
}

/****************************************************************************
 Function    :  nmhGetNextIndexFsMIDhcpSnpGlobalConfigTable
 Input       :  The Indices
                FsMIDhcpSnpContextId
                nextFsMIDhcpSnpContextId
 Output      :  The Get Next function gets the Next Index for
                the Index Value given in the Index Values. The
                Indices are stored in the next_varname variables.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
/* GET_NEXT Routine.  */
INT1
nmhGetNextIndexFsMIDhcpSnpGlobalConfigTable (INT4 i4FsMIDhcpSnpContextId,
                                             INT4 *pi4NextFsMIDhcpSnpContextId)
{
    UINT4               u4ContextId = L2DS_ZERO;

    for (u4ContextId = (i4FsMIDhcpSnpContextId + L2DS_ONE);
         u4ContextId <= L2DS_MAX_CONTEXTS; u4ContextId++)
    {
        if (L2DS_IS_CONTEXT_EXIST (u4ContextId) == L2DS_TRUE)
        {
            *pi4NextFsMIDhcpSnpContextId = (INT4) u4ContextId;
            return SNMP_SUCCESS;
        }
    }

    return SNMP_FAILURE;
}

/* Low Level GET Routine for All Objects  */

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpSnoopingAdminStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId

                The Object 
                retValFsMIDhcpSnpSnoopingAdminStatus
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpSnoopingAdminStatus (INT4 i4FsMIDhcpSnpContextId,
                                      INT4
                                      *pi4RetValFsMIDhcpSnpSnoopingAdminStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpSnoopingAdminStatus
        (pi4RetValFsMIDhcpSnpSnoopingAdminStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpMacVerifyStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId

                The Object 
                retValFsMIDhcpSnpMacVerifyStatus
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpMacVerifyStatus (INT4 i4FsMIDhcpSnpContextId,
                                  INT4 *pi4RetValFsMIDhcpSnpMacVerifyStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpMacVerifyStatus
        (pi4RetValFsMIDhcpSnpMacVerifyStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/* Low Level SET Routine for All Objects  */

/****************************************************************************
 Function    :  nmhSetFsMIDhcpSnpSnoopingAdminStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId

                The Object 
                setValFsMIDhcpSnpSnoopingAdminStatus
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhSetFsMIDhcpSnpSnoopingAdminStatus (INT4 i4FsMIDhcpSnpContextId,
                                      INT4
                                      i4SetValFsMIDhcpSnpSnoopingAdminStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhSetFsDhcpSnpSnoopingAdminStatus
        (i4SetValFsMIDhcpSnpSnoopingAdminStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhSetFsMIDhcpSnpMacVerifyStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId

                The Object 
                setValFsMIDhcpSnpMacVerifyStatus
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhSetFsMIDhcpSnpMacVerifyStatus (INT4 i4FsMIDhcpSnpContextId,
                                  INT4 i4SetValFsMIDhcpSnpMacVerifyStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return =
        nmhSetFsDhcpSnpMacVerifyStatus (i4SetValFsMIDhcpSnpMacVerifyStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/* Low Level TEST Routines for All Objects  */

/****************************************************************************
 Function    :  nmhTestv2FsMIDhcpSnpSnoopingAdminStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId

                The Object 
                testValFsMIDhcpSnpSnoopingAdminStatus
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhTestv2FsMIDhcpSnpSnoopingAdminStatus (UINT4 *pu4ErrorCode,
                                         INT4 i4FsMIDhcpSnpContextId,
                                         INT4
                                         i4TestValFsMIDhcpSnpSnoopingAdminStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhTestv2FsDhcpSnpSnoopingAdminStatus
        (pu4ErrorCode, i4TestValFsMIDhcpSnpSnoopingAdminStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhTestv2FsMIDhcpSnpMacVerifyStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId

                The Object 
                testValFsMIDhcpSnpMacVerifyStatus
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhTestv2FsMIDhcpSnpMacVerifyStatus (UINT4 *pu4ErrorCode,
                                     INT4 i4FsMIDhcpSnpContextId,
                                     INT4 i4TestValFsMIDhcpSnpMacVerifyStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhTestv2FsDhcpSnpMacVerifyStatus
        (pu4ErrorCode, i4TestValFsMIDhcpSnpMacVerifyStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/* Low Level Dependency Routines for All Objects  */

/****************************************************************************
 Function    :  nmhDepv2FsMIDhcpSnpGlobalConfigTable
 Input       :  The Indices
                FsMIDhcpSnpContextId
 Output      :  The Dependency Low Lev Routine Take the Indices &
                check whether dependency is met or not.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  SNMP_SUCCESS or SNMP_FAILURE
****************************************************************************/
INT1
nmhDepv2FsMIDhcpSnpGlobalConfigTable (UINT4 *pu4ErrorCode,
                                      tSnmpIndexList * pSnmpIndexList,
                                      tSNMP_VAR_BIND * pSnmpVarBind)
{
    UNUSED_PARAM (pu4ErrorCode);
    UNUSED_PARAM (pSnmpIndexList);
    UNUSED_PARAM (pSnmpVarBind);
    return SNMP_SUCCESS;
}

/* LOW LEVEL Routines for Table : FsMIDhcpSnpInterfaceTable. */

/****************************************************************************
 Function    :  nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId
 Output      :  The Routines Validates the Given Indices.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
/* GET_EXACT Validate Index Instance Routine. */

INT1
nmhValidateIndexInstanceFsMIDhcpSnpInterfaceTable (INT4 i4FsMIDhcpSnpContextId,
                                                   INT4 i4FsMIDhcpSnpVlanId)
{
    if ((i4FsMIDhcpSnpContextId < L2DS_DEFAULT_CXT_ID) &&
        (i4FsMIDhcpSnpContextId >= L2DS_MAX_CONTEXTS))
    {
        return SNMP_SUCCESS;
    }

    if (L2dsVcmIsVcExist (i4FsMIDhcpSnpContextId) == L2DS_FALSE)
    {
        return SNMP_SUCCESS;
    }

    return (nmhValidateIndexInstanceFsDhcpSnpInterfaceTable
            (i4FsMIDhcpSnpVlanId));
}

/****************************************************************************
 Function    :  nmhGetFirstIndexFsMIDhcpSnpInterfaceTable
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId
 Output      :  The Get First Routines gets the Lexicographicaly
                First Entry from the Table.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
/* GET_FIRST Routine. */

INT1
nmhGetFirstIndexFsMIDhcpSnpInterfaceTable (INT4 *pi4FsMIDhcpSnpContextId,
                                           INT4 *pi4FsMIDhcpSnpVlanId)
{
    UINT4               u4ContextId = L2DS_DEFAULT_CXT_ID;

    do
    {
        if (L2dsSelectContext (u4ContextId) != L2DS_SUCCESS)
        {
            return SNMP_FAILURE;
        }
        *pi4FsMIDhcpSnpContextId = u4ContextId;

        if ((nmhGetFirstIndexFsDhcpSnpInterfaceTable
             (pi4FsMIDhcpSnpVlanId)) == SNMP_SUCCESS)
        {
            L2dsReleaseContext ();
            return SNMP_SUCCESS;
        }
    }
    while (L2dsGetNextActiveContext (*pi4FsMIDhcpSnpContextId,
                                     &u4ContextId) == L2DS_SUCCESS);

    L2dsReleaseContext ();
    return SNMP_FAILURE;
}

/****************************************************************************
 Function    :  nmhGetNextIndexFsMIDhcpSnpInterfaceTable
 Input       :  The Indices
                FsMIDhcpSnpContextId
                nextFsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId
                nextFsMIDhcpSnpVlanId
 Output      :  The Get Next function gets the Next Index for
                the Index Value given in the Index Values. The
                Indices are stored in the next_varname variables.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
/* GET_NEXT Routine.  */
INT1
nmhGetNextIndexFsMIDhcpSnpInterfaceTable (INT4 i4FsMIDhcpSnpContextId,
                                          INT4 *pi4NextFsMIDhcpSnpContextId,
                                          INT4 i4FsMIDhcpSnpVlanId,
                                          INT4 *pi4NextFsMIDhcpSnpVlanId)
{
    UINT4               u4ContextId = L2DS_ZERO;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_SUCCESS)
    {
        if (nmhGetNextIndexFsDhcpSnpInterfaceTable (i4FsMIDhcpSnpVlanId,
                                                    pi4NextFsMIDhcpSnpVlanId)
            == SNMP_SUCCESS)
        {
            *pi4NextFsMIDhcpSnpContextId = i4FsMIDhcpSnpContextId;
            L2dsReleaseContext ();
            return SNMP_SUCCESS;
        }
    }

    do
    {
        L2dsReleaseContext ();

        if (L2dsGetNextActiveContext (i4FsMIDhcpSnpContextId,
                                      &u4ContextId) == L2DS_FAILURE)
        {
            return SNMP_FAILURE;
        }

        if (L2dsSelectContext (u4ContextId) == L2DS_FAILURE)
        {
            return SNMP_FAILURE;
        }
        i4FsMIDhcpSnpContextId = (INT4) u4ContextId;
        *pi4NextFsMIDhcpSnpContextId = (INT4) u4ContextId;

    }
    while (nmhGetFirstIndexFsDhcpSnpInterfaceTable (pi4NextFsMIDhcpSnpVlanId)
           == SNMP_FAILURE);

    L2dsReleaseContext ();
    return SNMP_SUCCESS;

}

/* Low Level GET Routine for All Objects  */

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpVlanSnpStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpVlanSnpStatus
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpVlanSnpStatus (INT4 i4FsMIDhcpSnpContextId,
                                INT4 i4FsMIDhcpSnpVlanId,
                                INT4 *pi4RetValFsMIDhcpSnpVlanSnpStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpVlanSnpStatus
        (i4FsMIDhcpSnpVlanId, pi4RetValFsMIDhcpSnpVlanSnpStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpRxDiscovers
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpRxDiscovers
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpRxDiscovers (INT4 i4FsMIDhcpSnpContextId,
                              INT4 i4FsMIDhcpSnpVlanId,
                              UINT4 *pu4RetValFsMIDhcpSnpRxDiscovers)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpRxDiscovers
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpRxDiscovers);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpRxRequests
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpRxRequests
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpRxRequests (INT4 i4FsMIDhcpSnpContextId,
                             INT4 i4FsMIDhcpSnpVlanId,
                             UINT4 *pu4RetValFsMIDhcpSnpRxRequests)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpRxRequests
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpRxRequests);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpRxReleases
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpRxReleases
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpRxReleases (INT4 i4FsMIDhcpSnpContextId,
                             INT4 i4FsMIDhcpSnpVlanId,
                             UINT4 *pu4RetValFsMIDhcpSnpRxReleases)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpRxReleases
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpRxReleases);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpRxDeclines
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpRxDeclines
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpRxDeclines (INT4 i4FsMIDhcpSnpContextId,
                             INT4 i4FsMIDhcpSnpVlanId,
                             UINT4 *pu4RetValFsMIDhcpSnpRxDeclines)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpRxDeclines
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpRxDeclines);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpRxInforms
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpRxInforms
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpRxInforms (INT4 i4FsMIDhcpSnpContextId,
                            INT4 i4FsMIDhcpSnpVlanId,
                            UINT4 *pu4RetValFsMIDhcpSnpRxInforms)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpRxInforms
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpRxInforms);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpTxOffers
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpTxOffers
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpTxOffers (INT4 i4FsMIDhcpSnpContextId,
                           INT4 i4FsMIDhcpSnpVlanId,
                           UINT4 *pu4RetValFsMIDhcpSnpTxOffers)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpTxOffers
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpTxOffers);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpTxAcks
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpTxAcks
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpTxAcks (INT4 i4FsMIDhcpSnpContextId,
                         INT4 i4FsMIDhcpSnpVlanId,
                         UINT4 *pu4RetValFsMIDhcpSnpTxAcks)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpTxAcks
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpTxAcks);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpTxNaks
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpTxNaks
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpTxNaks (INT4 i4FsMIDhcpSnpContextId,
                         INT4 i4FsMIDhcpSnpVlanId,
                         UINT4 *pu4RetValFsMIDhcpSnpTxNaks)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpTxNaks
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpTxNaks);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpNoOfDiscards
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpNoOfDiscards
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpNoOfDiscards (INT4 i4FsMIDhcpSnpContextId,
                               INT4 i4FsMIDhcpSnpVlanId,
                               UINT4 *pu4RetValFsMIDhcpSnpNoOfDiscards)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpNoOfDiscards
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpNoOfDiscards);
    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpMacDiscards
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpMacDiscards
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpMacDiscards (INT4 i4FsMIDhcpSnpContextId,
                              INT4 i4FsMIDhcpSnpVlanId,
                              UINT4 *pu4RetValFsMIDhcpSnpMacDiscards)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpMacDiscards
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpMacDiscards);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpServerDiscards
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpServerDiscards
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpServerDiscards (INT4 i4FsMIDhcpSnpContextId,
                                 INT4 i4FsMIDhcpSnpVlanId,
                                 UINT4 *pu4RetValFsMIDhcpSnpServerDiscards)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpServerDiscards
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpServerDiscards);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpOptionDiscards
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpOptionDiscards
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpOptionDiscards (INT4 i4FsMIDhcpSnpContextId,
                                 INT4 i4FsMIDhcpSnpVlanId,
                                 UINT4 *pu4RetValFsMIDhcpSnpOptionDiscards)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpOptionDiscards
        (i4FsMIDhcpSnpVlanId, pu4RetValFsMIDhcpSnpOptionDiscards);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhGetFsMIDhcpSnpInterfaceStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                retValFsMIDhcpSnpInterfaceStatus
 Output      :  The Get Low Lev Routine Take the Indices &
                store the Value requested in the Return val.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhGetFsMIDhcpSnpInterfaceStatus (INT4 i4FsMIDhcpSnpContextId,
                                  INT4 i4FsMIDhcpSnpVlanId,
                                  INT4 *pi4RetValFsMIDhcpSnpInterfaceStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhGetFsDhcpSnpInterfaceStatus
        (i4FsMIDhcpSnpVlanId, pi4RetValFsMIDhcpSnpInterfaceStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/* Low Level SET Routine for All Objects  */

/****************************************************************************
 Function    :  nmhSetFsMIDhcpSnpVlanSnpStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                setValFsMIDhcpSnpVlanSnpStatus
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhSetFsMIDhcpSnpVlanSnpStatus (INT4 i4FsMIDhcpSnpContextId,
                                INT4 i4FsMIDhcpSnpVlanId,
                                INT4 i4SetValFsMIDhcpSnpVlanSnpStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhSetFsDhcpSnpVlanSnpStatus
        (i4FsMIDhcpSnpVlanId, i4SetValFsMIDhcpSnpVlanSnpStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhSetFsMIDhcpSnpInterfaceStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                setValFsMIDhcpSnpInterfaceStatus
 Output      :  The Set Low Lev Routine Take the Indices &
                Sets the Value accordingly.
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhSetFsMIDhcpSnpInterfaceStatus (INT4 i4FsMIDhcpSnpContextId,
                                  INT4 i4FsMIDhcpSnpVlanId,
                                  INT4 i4SetValFsMIDhcpSnpInterfaceStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return = nmhSetFsDhcpSnpInterfaceStatus
        (i4FsMIDhcpSnpVlanId, i4SetValFsMIDhcpSnpInterfaceStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/* Low Level TEST Routines for All Objects  */

/****************************************************************************
 Function    :  nmhTestv2FsMIDhcpSnpVlanSnpStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                testValFsMIDhcpSnpVlanSnpStatus
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhTestv2FsMIDhcpSnpVlanSnpStatus (UINT4 *pu4ErrorCode,
                                   INT4 i4FsMIDhcpSnpContextId,
                                   INT4 i4FsMIDhcpSnpVlanId,
                                   INT4 i4TestValFsMIDhcpSnpVlanSnpStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return =
        nmhTestv2FsDhcpSnpVlanSnpStatus (pu4ErrorCode,
                                         i4FsMIDhcpSnpVlanId,
                                         i4TestValFsMIDhcpSnpVlanSnpStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/****************************************************************************
 Function    :  nmhTestv2FsMIDhcpSnpInterfaceStatus
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId

                The Object 
                testValFsMIDhcpSnpInterfaceStatus
 Output      :  The Test Low Lev Routine Take the Indices &
                Test whether that Value is Valid Input for Set.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhTestv2FsMIDhcpSnpInterfaceStatus (UINT4 *pu4ErrorCode,
                                     INT4 i4FsMIDhcpSnpContextId,
                                     INT4 i4FsMIDhcpSnpVlanId,
                                     INT4 i4TestValFsMIDhcpSnpInterfaceStatus)
{
    INT1                i1Return = SNMP_FAILURE;

    if (L2dsSelectContext (i4FsMIDhcpSnpContextId) == L2DS_FAILURE)
    {
        return SNMP_FAILURE;
    }

    i1Return =
        nmhTestv2FsDhcpSnpInterfaceStatus (pu4ErrorCode,
                                           i4FsMIDhcpSnpVlanId,
                                           i4TestValFsMIDhcpSnpInterfaceStatus);

    L2dsReleaseContext ();
    return i1Return;
}

/* Low Level Dependency Routines for All Objects  */

/****************************************************************************
 Function    :  nmhDepv2FsMIDhcpSnpInterfaceTable
 Input       :  The Indices
                FsMIDhcpSnpContextId
                FsMIDhcpSnpVlanId
 Output      :  The Dependency Low Lev Routine Take the Indices &
                check whether dependency is met or not.
                Stores the value of error code in the Return val
 Error Codes :  The following error codes are to be returned
                SNMP_ERR_WRONG_LENGTH ref:(4 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_WRONG_VALUE ref:(6 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_NO_CREATION ref:(7 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_NAME ref:(8 of Sect 4.2.5 of rfc1905)
                SNMP_ERR_INCONSISTENT_VALUE ref:(10 of Sect 4.2.5 of rfc1905)
 Returns     :  i1Return or SNMP_FAILURE
****************************************************************************/
INT1
nmhDepv2FsMIDhcpSnpInterfaceTable (UINT4 *pu4ErrorCode,
                                   tSnmpIndexList * pSnmpIndexList,
                                   tSNMP_VAR_BIND * pSnmpVarBind)
{
    UNUSED_PARAM (pu4ErrorCode);
    UNUSED_PARAM (pSnmpIndexList);
    UNUSED_PARAM (pSnmpVarBind);
    return SNMP_SUCCESS;
}
