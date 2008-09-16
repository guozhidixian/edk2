/** @file
  TCG Service Protocol as defined in TCG_EFI_Protocol_1_20_Final
  See http://trustedcomputinggroup.org for the latest specification

  Copyright (c) 2007 - 2008, Intel Corporation 
  All rights reserved. This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _TCG_SERVICE_PROTOCOL_H_
#define _TCG_SERVICE_PROTOCOL_H_

#include <Uefi/UefiTcgPlatform.h>

#define EFI_TCG_PROTOCOL_GUID  \
  {0xf541796d, 0xa62e, 0x4954, { 0xa7, 0x75, 0x95, 0x84, 0xf6, 0x1b, 0x9c, 0xdd } } 

typedef struct _EFI_TCG_PROTOCOL EFI_TCG_PROTOCOL;

//
// Set structure alignment to 1-byte
//
#pragma pack (1)

typedef struct {
  UINT8  Major;
  UINT8  Minor;
  UINT8  RevMajor;
  UINT8  RevMinor;
} TCG_VERSION;

typedef struct _TCG_EFI_BOOT_SERVICE_CAPABILITY {
  UINT8          Size;                // Size of this structure
  TCG_VERSION    StructureVersion;    
  TCG_VERSION    ProtocolSpecVersion;
  UINT8          HashAlgorithmBitmap; // Hash algorithms  
                                      // this protocol is capable of : 01=SHA-1
  BOOLEAN        TPMPresentFlag;      // 00h = TPM not present
  BOOLEAN        TPMDeactivatedFlag;  // 01h = TPM currently deactivated
} TCG_EFI_BOOT_SERVICE_CAPABILITY;

typedef UINT32   TCG_ALGORITHM_ID;

//
// Restore original structure alignment
//
#pragma pack ()

/**
  This service provides EFI protocol capability information, state information 
  about the TPM, and Event Log state information.

  @param  This                   Indicates the calling context
  @param  ProtocolCapability     The callee allocates memory for a TCG_BOOT_SERVICE_CAPABILITY 
                                 structure and fills in the fields with the EFI protocol 
                                 capability information and the current TPM state information.
  @param  TCGFeatureFlags        This is a pointer to the feature flags. No feature 
                                 flags are currently defined so this parameter 
                                 MUST be set to 0. However, in the future, 
                                 feature flags may be defined that, for example, 
                                 enable hash algorithm agility.
  @param  EventLogLocation       This is a pointer to the address of the event log in memory.
  @param  EventLogLastEntry      If the Event Log contains more than one entry, 
                                 this is a pointer to the address of the start of 
                                 the last entry in the event log in memory. 

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.
  @retval EFI_INVALID_PARAMETER  One or more of the parameters are incorrect.
  @retval EFI_BUFFER_TOO_SMALL   The receive buffer is too small.
  @retval EFI_NOT_FOUND          The component was not running

**/
typedef
EFI_STATUS
(EFIAPI *EFI_TCG_STATUS_CHECK)(
  IN      EFI_TCG_PROTOCOL          *This,
  OUT     TCG_EFI_BOOT_SERVICE_CAPABILITY
                                    *ProtocolCapability,
  OUT     UINT32                    *TCGFeatureFlags,
  OUT     EFI_PHYSICAL_ADDRESS      *EventLogLocation,
  OUT     EFI_PHYSICAL_ADDRESS      *EventLogLastEntry
  );

/**
  This service abstracts the capability to do a hash operation on a data buffer.
  
  @param  This                   Indicates the calling context
  @param  HashData               Pointer to the data buffer to be hashed
  @param  HashDataLen            Length of the data buffer to be hashed
  @param  AlgorithmId            Identification of the Algorithm to use for the hashing operation
  @param  HashedDataLen          Resultant length of the hashed data
  @param  HashedDataResult       Resultant buffer of the hashed data  
  
  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.
  @retval EFI_INVALID_PARAMETER  One or more of the parameters are incorrect.
  @retval EFI_BUFFER_TOO_SMALL   The receive buffer is too small.
  @retval EFI_NOT_FOUND          The component was not running

**/
typedef
EFI_STATUS
(EFIAPI *EFI_TCG_HASH_ALL)(
  IN      EFI_TCG_PROTOCOL          *This,
  IN      UINT8                     *HashData,
  IN      UINT64                    HashDataLen,
  IN      TCG_ALGORITHM_ID          AlgorithmId,
  IN OUT  UINT64                    *HashedDataLen,
  IN OUT  UINT8                     **HashedDataResult
  );

/**
  This service abstracts the capability to add an entry to the Event Log.

  @param  This                   Indicates the calling context
  @param  TCGLogData             Pointer to the start of the data buffer containing 
                                 the TCG_PCR_EVENT data structure. All fields in 
                                 this structure are properly filled by the caller.
  @param  EventNumber            The event number of the event just logged
  @param  Flags                  Indicate additional flags. Only one flag has been 
                                 defined at this time, which is 0x01 and means the 
                                 extend operation should not be performed. All 
                                 other bits are reserved. 
 
  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.
  @retval EFI_INVALID_PARAMETER  One or more of the parameters are incorrect.
  @retval EFI_BUFFER_TOO_SMALL   The receive buffer is too small.
  @retval EFI_NOT_FOUND          The component was not running
**/
typedef
EFI_STATUS
(EFIAPI *EFI_TCG_LOG_EVENT)(
  IN      EFI_TCG_PROTOCOL          *This,
  IN      TCG_PCR_EVENT             *TCGLogData,
  IN OUT  UINT32                    *EventNumber,
  IN      UINT32                    Flags
  );

/**
  This service is a proxy for commands to the TPM.

  @param  This                        Indicates the calling context
  @param  TpmInputParameterBlockSize  Size of the TPM input parameter block
  @param  TpmInputParameterBlock      Pointer to the TPM input parameter block
  @param  TpmOutputParameterBlockSize Size of the TPM output parameter block
  @param  TpmOutputParameterBlock     Pointer to the TPM output parameter block

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.
  @retval EFI_INVALID_PARAMETER  One or more of the parameters are incorrect.
  @retval EFI_BUFFER_TOO_SMALL   The receive buffer is too small.
  @retval EFI_NOT_FOUND          The component was not running
**/
typedef
EFI_STATUS
(EFIAPI *EFI_TCG_PASS_THROUGH_TO_TPM)(
  IN      EFI_TCG_PROTOCOL          *This,
  IN      UINT32                    TpmInputParamterBlockSize,
  IN      UINT8                     *TpmInputParamterBlock,
  IN      UINT32                    TpmOutputParameterBlockSize,
  IN      UINT8                     *TpmOutputParameterBlock
  );

/**
  This service abstracts the capability to do a hash operation on a data buffer, extend a specific TPM PCR with the hash result, and add an entry to the Event Log

  @param  This                   Indicates the calling context
  @param  HashData               Physical address of the start of the data buffer 
                                 to be hashed, extended, and logged.
  @param  HashDataLen            The length, in bytes, of the buffer referenced by HashData
  @param  AlgorithmId            Identification of the Algorithm to use for the hashing operation
  @param  TCGLogData             The physical address of the start of the data 
                                 buffer containing the TCG_PCR_EVENT data structure.
  @param  EventNumber            The event number of the event just logged.
  @param  EventLogLastEntry      Physical address of the first byte of the entry 
                                 just placed in the Event Log. If the Event Log was 
                                 empty when this function was called then this physical 
                                 address will be the same as the physical address of 
                                 the start of the Event Log.

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.
  @retval EFI_INVALID_PARAMETER  One or more of the parameters are incorrect.
  @retval EFI_BUFFER_TOO_SMALL   The receive buffer is too small.
  @retval EFI_NOT_FOUND          The component was not running
**/
typedef
EFI_STATUS
(EFIAPI *EFI_TCG_HASH_LOG_EXTEND_EVENT)(
  IN      EFI_TCG_PROTOCOL          *This,
  IN      EFI_PHYSICAL_ADDRESS      HashData,
  IN      UINT64                    HashDataLen,
  IN      TCG_ALGORITHM_ID          AlgorithmId,
  IN OUT  TCG_PCR_EVENT             *TCGLogData,
  IN OUT  UINT32                    *EventNumber,
     OUT  EFI_PHYSICAL_ADDRESS      *EventLogLastEntry
  );

/**
  @par Protocol Description:
  The EFI_TCG Protocol abstracts TCG activity.

  @param  StatusCheck        This service provides information on the TPM.
  @param  HashAll            This service abstracts the capability to do a hash 
                             operation on a data buffer.
  @param  LogEvent           This service abstracts the capability to add 
                             an entry to the Event Log.
  @param  PassThroughToTPM   This service provides a pass-through capability 
                             from the caller to the system's TPM.
  @param  HashLogExtendEvent This service abstracts the capability to do a hash 
                             operation on a data buffer, extend a specific TPM PCR 
                             with the hash result, and add an entry to the Event Log. 

**/
struct _EFI_TCG_PROTOCOL {
  EFI_TCG_STATUS_CHECK              StatusCheck;
  EFI_TCG_HASH_ALL                  HashAll;
  EFI_TCG_LOG_EVENT                 LogEvent;
  EFI_TCG_PASS_THROUGH_TO_TPM       PassThroughToTpm;
  EFI_TCG_HASH_LOG_EXTEND_EVENT     HashLogExtendEvent;
};

extern EFI_GUID gEfiTcgProtocolGuid;

#endif
