#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target &getTheRubiniusTarget() {
  static Target T;
  return T;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRubiniusTargetInfo() {
  RegisterTarget<False>(getTheRubiniusTarget(), "rubinius", "Rubinius", "Rubinius");
}
