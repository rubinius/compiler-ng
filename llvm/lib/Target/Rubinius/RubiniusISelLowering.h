#pragma once
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class RubiniusSubtarget;

class RubiniusTargetLowering : public TargetLowering {
public:
  explicit RubiniusTargetLowering(const TargetMachine &TM, const RubiniusSubtarget &STI);
};

} // namespace llvm
