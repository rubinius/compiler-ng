#pragma once
#include "RubiniusSubtarget.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class RubiniusTargetMachine : public LLVMTargetMachine {
  RubiniusSubtarget Subtarget;

public:
  RubiniusTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                    StringRef FS, const TargetOptions &Options,
                    Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                    CodeGenOptLevel OL);

  const RubiniusSubtarget *getSubtargetImpl(const Function &) const override;
};

} // namespace llvm
