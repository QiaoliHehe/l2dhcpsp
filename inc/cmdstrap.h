/********************************************************************
 * Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *
 * $Id: dhcpstrap.h,v 1.7 2010/07/28 10:30:41 prabuc Exp $
 *
 * Description:This file contains the variable definitions   
 *             for the DHCP Trap system.              
 *
 *******************************************************************/
#ifndef _CM_DHCPSNP_TRAP_H_
#define _CM_DHCPSNP_TRAP_H_

#define DHCP_SNP_UTIL_TRAPS_OID "1.3.6.1.4.1.29601.2.3.3"

#define DHCP_SNP_TRAPS_OID_LEN       12 
#define DHCP_SNP_MAX_OBJ_LEN         256

enum
{
    DHCP_SNP_UTIL_TRAP=1,
    DHCP_SNP_INVALID_TRAP
};

typedef struct
{
    INT4  i4TrapId;
    INT4  i4PortNum;
} tDhcpSNPUtlTrap;

VOID CmL2DhcpSNPSnmpSendTrap PROTO ((UINT1 u1TrapId, INT1 *pi1TrapOid, UINT1 u1OidLen,
                   VOID *pTrapInfo));
VOID
CmDhpSNPSnmpSendUntrustPortUtlTrap (INT4 i4PortNUM,
                         INT1 *pi1TrapsOid, UINT1 u1TrapOidLen);
tSNMP_OID_TYPE     *
CmDhcpSnpMakeObjIdFromDotNew (UINT1 *textStr);

#endif 

