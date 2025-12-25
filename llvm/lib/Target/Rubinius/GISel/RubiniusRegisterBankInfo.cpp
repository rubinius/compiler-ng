#include "RubiniusRegisterBankInfo.h"

#define DEBUG_TYPE "rbx-reg-bank-info"

#define GET_TARGET_REGBANK_IMPL
#include "RubiniusGenRegisterBank.inc"

using namespace llvm;

RubiniusRegisterBankInfo::RubiniusRegisterBankInfo(const TargetRegisterInfo &TRI)
    : RubiniusGenRegisterBankInfo() {}
