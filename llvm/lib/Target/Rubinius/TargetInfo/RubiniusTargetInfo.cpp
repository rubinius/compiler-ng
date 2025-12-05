#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &getTheRubiniusTarget() {
  static Target TheRubiniusTarget;
  return TheRubiniusTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRubiniusTargetInfo() {
  RegisterTarget<Triple::rubinius> X(getTheRubiniusTarget(), "rubinius", "Rubinius Target");
}
