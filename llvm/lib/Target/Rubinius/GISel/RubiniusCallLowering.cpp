#include "RubiniusCallLowering.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"

#define DEBUG_TYPE "rbx-call-lowering"

using namespace llvm;

RubiniusCallLowering::RubiniusCallLowering(const RubiniusTargetLowering &TLI)
    : CallLowering(&TLI) {}

bool RubiniusCallLowering::lowerReturn(MachineIRBuilder &MIRBuilder,
                                  const Value *Val, ArrayRef<Register> VRegs,
                                  FunctionLoweringInfo &FLI,
                                  Register SwiftErrorVReg) const {
  if (!VRegs.empty())
    return false;
  MIRBuilder.buildInstr(Rubinius::RET);
  return true;
}

bool RubiniusCallLowering::lowerFormalArguments(MachineIRBuilder &MIRBuilder,
                                           const Function &F,
                                           ArrayRef<ArrayRef<Register>> VRegs,
                                           FunctionLoweringInfo &FLI) const {
  return VRegs.empty();
}

bool RubiniusCallLowering::lowerCall(MachineIRBuilder &MIRBuilder,
                                CallLoweringInfo &Info) const {
  return false;
}
