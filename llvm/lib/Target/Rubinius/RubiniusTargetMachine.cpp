#include "RubiniusTargetMachine.h"
#include "RubiniusGenSubtargetInfo.inc"
#include "RubiniusISelLowering.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

static Target &getTheRubiniusTarget(); // from RubiniusTargetInfo.cpp

RubiniusTargetMachine::RubiniusTargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     Optional<Reloc::Model> RM,
                                     Optional<CodeModel::Model> CM,
                                     CodeGenOptLevel OL)
    : LLVMTargetMachine(T, "e", TT, CPU, FS, Options, RM.value_or(Reloc::Static),
                        CM.value_or(CodeModel::Small), OL),
      Subtarget(TT, CPU, FS, *this) {}

const RubiniusSubtarget *
RubiniusTargetMachine::getSubtargetImpl(const Function &) const {
  return &Subtarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRubiniusTarget() {
  RegisterTargetMachine<RubiniusTargetMachine> X(getTheRubiniusTarget());
}
