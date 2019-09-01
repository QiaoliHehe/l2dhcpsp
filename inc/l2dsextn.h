#ifndef _L2DSEXTN_H
#define _L2DSEXTN_H 
/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsextn.h,v 1.6 2010/09/17 06:24:52 prabuc Exp $                                                                    */
/*****************************************************************************/
/*    FILE  NAME            : l2dsextn.h                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snoop                                   */
/*    MODULE NAME           : L2DHCP snoop externs                           */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains external variables          */
/*                            for L2 DHCP Snooping module                    */
/*---------------------------------------------------------------------------*/

/* Instance of global Informations */
extern tL2DhcpSnpGlobalInfo     gL2DhcpSnpGlobalInfo;

/* Instance of global configurations */
extern tL2DhcpSnpGlobalConfig   gaL2DhcpSnpGlobalConfig [];

/* Broadcast Mac address */
extern tMacAddr                 BCastMacAddr;

/*  Data buffer for duplicating data information */
extern UINT1                   *gpau1L2dsDataBuffer;

extern INT4 			gi4SysLogId;

/* This is a global array for storing option value to be returned by
 * the function L2dsUtilGetOption.
 * The size is allocated to L2DS_OPTION_LEN, so that any variable length 
 * option also will be accommadated
 * */
PUBLIC UINT1               gau1OptionVal[L2DS_OPTION_LEN];

/*CAMEOTAG:Ricann,20130228*/
extern tL2DhcpSnpFdbGlob	gL2DSFdbGlob;

#endif /* _L2DSEXTN_H */
