#pragma once

#include "MCTargetDesc/RubiniusMCTargetDesc.h"
#include "llvm/CodeGen/RegisterBankInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGBANK_DECLARATIONS
#include "RubiniusGenRegisterBank.inc"

namespace llvm {
class TargetRegisterInfo;

class RubiniusGenRegisterBankInfo : public RegisterBankInfo {
protected:
#define GET_TARGET_REGBANK_CLASS
#include "RubiniusGenRegisterBank.inc"
};

class RubiniusRegisterBankInfo final : public RubiniusGenRegisterBankInfo {
public:
  RubiniusRegisterBankInfo(const TargetRegisterInfo &TRI);
};
} // namespace llvm
