 /*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsstub.c,v 1.4 2010/08/04 08:51:03 prabuc Exp $       */
/*****************************************************************************/
/*    FILE  NAME            : l2dsstub.c                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snooping                                */
/*    MODULE NAME           : L2DHCP Snooping Stub Module                    */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains the stub funtions        */
/*                            for L2DHCP Snooping module                     */
/*---------------------------------------------------------------------------*/

#include "l2dsinc.h"
#include "l2dsextn.h"

/*****************************************************************************/
/* Function Name      : DcsUtilGetDslLineCharacteristics                     */
/*                                                                           */
/* Description        : This routine is to add DSL LineCharacteristics in the*/
/*                      incoming PPPoE Discovery packets                     */
/*                                                                           */
/* Input(s)           : u4Port - The incoming port                           */
/*                                                                           */
/* Output(s)          : pu1DslOptions - DSL Line Characteristics             */
/*                      pu1DslLen     - Added Length                         */
/*                                                                           */
/* Return Value(s)    : DCS_SUCCESS/DCS_FAILURE                              */
/*****************************************************************************/
INT4
DcsUtilGetDslLineCharacteristics (UINT4 u4Port, UINT1 *pu1DslOptions,
                                  UINT1 *pu1DslLen, tVlanTag VlanTag)
{
    UNUSED_PARAM (u4Port);
    UNUSED_PARAM (pu1DslOptions);
    UNUSED_PARAM (pu1DslLen);
    UNUSED_PARAM (VlanTag);
    return (DCS_SUCCESS);
}

/*****************************************************************************/
/* Function Name      : DcsUtilGetFreeStringACI                              */
/*                                                                           */
/* Description        : This routine is to add Agent Circuit ID Free String  */
/*                      in the incoming Packet                               */
/*                                                                           */
/* Input(s)           : u4PortNumber - Incoming Port                         */
/*                      VlanId       - Vlan Identifier                       */
/*                                                                           */
/* Output(s)          : pu1CircuitId - Pointer to Agent Circuit ID           */
/*                      pu1CircuitIDLen -Total circuit id Length             */
/*                                                                           */
/* Return Value(s)    : DCS_SUCCESS/DCS_FAILURE                              */
/*****************************************************************************/
INT4
DcsUtilGetFreeStringACI (INT4 i4IfIndex, UINT1 *pu1CircuitId,
                         UINT1 *pu1CircuitIDLen)
{
    UNUSED_PARAM (i4IfIndex);
    UNUSED_PARAM (pu1CircuitId);
    UNUSED_PARAM (pu1CircuitIDLen);

    return DCS_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/*    Function Name        : DcsGetRemoteAgentIdStatus                       */
/*                                                                           */
/*    Description          : This function is used to get configured Remote  */
/*                           Agent Identifier status.if the status is enable */
/*                           then the configured Remote ID value will be     */
/*                           added in PPPoE Intermediate Agent and DHCP      */
/*                           packets as a sub-option type odf (0x02)         */
/*                                                                           */
/*    Input(s)             : i4DcsPortCtrlIndex - Interface Index            */
/*                                                                           */
/*    Output(s)            : pu1AgentStaus - pointer to the Remoteid status  */
/*                                                                           */
/*    Return Value(s)      : DCS_SUCCESS/DCS_FAILURE                         */
/*****************************************************************************/
INT4
DcsGetRemoteAgentIdStatus (INT4 i4DcsPortCtrlIndex, UINT1 *pu1AgentStaus)
{
    UNUSED_PARAM (i4DcsPortCtrlIndex);

	/*CAMEOTAG: Added by zhaopan on 2015/11/26. Enable Remote ID status, the default content of which shouldn't be null*/
#ifdef CAMEO_DSP_DEFAULT_OPTION82_FORMAT_WANTED
	*pu1AgentStaus = DCS_ENABLE;
#else
    *pu1AgentStaus = DCS_DISABLE;
#endif

    return DCS_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/*    Function Name        : DcsGetAccessLoopStatus                          */
/*                                                                           */
/*    Description          : This function is used to get configured Access  */
/*                           Loop Charateristics status.If the status is     */
/*                           enable then the DSL line charateristics will be */
/*                           added in PPPoE Intermediate Agent and DHCP      */
/*                           packets.                                        */
/*                                                                           */
/*    Input(s)             : i4DcsPortCtrlIndex - Interface Index            */
/*                                                                           */
/*    Output(s)            : pu1AgentStaus - pointer to the AccessLoop status*/
/*                                                                           */
/*    Return Value(s)      : DCS_SUCCESS/DCS_FAILURE                         */
/*****************************************************************************/
INT4
DcsGetAccessLoopStatus (INT4 i4DcsPortCtrlIndex, UINT1 *pu1AgentStaus)
{
    UNUSED_PARAM (i4DcsPortCtrlIndex);
    UNUSED_PARAM (pu1AgentStaus);
    return DCS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CfaGetPvcIfIndexFromDslIndexVpiVci
 *
 *    Description          : This function is used to get PVC interface Index
 *                           from VCI,VPI and DslInterface Index value 
 *
 *    Input(s)             :  u4DslIfIndex - DSL Interface Index
 *                            i4Vpi- VPI value
 *                            i4Vci- VCI Value
 *
 *    Output(s)            :  pu4PvcIfIndex - PVC Interface Index
 *
 *    Returns              : CFA_SUCCESS / CFA_FAILURE
 *
 *****************************************************************************/

INT4
CfaGetPvcIfIndexFromDslIndexVpiVci (UINT4 u4DslIfIndex, INT4 i4Vpi, INT4 i4Vci,
                                    UINT4 *pu4PvcIfIndex)
{
    UNUSED_PARAM (u4DslIfIndex);
    UNUSED_PARAM (i4Vpi);
    UNUSED_PARAM (i4Vci);
    UNUSED_PARAM (pu4PvcIfIndex);
    return CFA_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/*    Function Name        : DcsGetFreeStringACI                             */
/*                                                                           */
/*    Description          : This function is used to get configured Circuit */
/*                           Agent Identifier.Allocate memory to             */
/*                           pu1AgentCircuitId before calling this function. */
/*                                                                           */
/*    Input(s)             : i4DcsPortCtrlIndex - Interface Index            */
/*                                                                           */
/*    Output(s)            : pu1AgentCircuitId - pointer to the Agent        */
/*                                               circuit id                  */
/*                           pi4Len           -  AgentCircuitId Length       */
/*                                                                           */
/*    Return Value(s)      : DCS_SUCCESS/DCS_FAILURE                         */
/*****************************************************************************/
INT4
DcsGetFreeStringACI (INT4 i4DcsPortCtrlIndex, UINT1 *pu1AgentCircuitId,
                     INT4 *pi4Len)
{
    UNUSED_PARAM (i4DcsPortCtrlIndex);
    UNUSED_PARAM (pi4Len);
    UNUSED_PARAM (pu1AgentCircuitId);
    return DCS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CfaGetVciVpiDslIfIndex
 *
 *    Description          : This function is used to get VCI,VPI and
 *                           DslInterface Index value from PVC interface
 *                           Index
 *
 *    Input(s)             : u4PvcIfIndex -PVC Interface Identifier
 *
 *
 *    Output(s)            : pu4DslIfIndex - DSL Interface Index
 *                           pi4Vpi- VPI value
 *                           pi4Vci- VCI Value
 *
 *
 *    Returns          : CFA_SUCCESS / CFA_FAILURE
 *
 *****************************************************************************/
INT4
CfaGetVciVpiDslIfIndex (UINT4 u4PvcIfIndex, UINT4 *pu4DslIfIndex, INT4 *pi4Vpi,
                        INT4 *pi4Vci)
{
    UNUSED_PARAM (u4PvcIfIndex);
    UNUSED_PARAM (pu4DslIfIndex);
    UNUSED_PARAM (pi4Vpi);
    UNUSED_PARAM (pi4Vci);

    return CFA_SUCCESS;
}
