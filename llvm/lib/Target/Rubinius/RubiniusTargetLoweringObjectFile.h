#pragma once

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/Target/TargetLoweringObjectFile.h"

namespace llvm {
class RubiniusTargetLoweringObjectFileELF : public TargetLoweringObjectFileELF {

public:
  MCSection *
  getSectionForJumpTable(const Function &F, const TargetMachine &TM,
                         const MachineJumpTableEntry *JTE) const override;
};
} // namespace llvm
