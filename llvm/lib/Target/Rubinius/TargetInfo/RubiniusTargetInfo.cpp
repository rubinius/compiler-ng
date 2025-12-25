#include "TargetInfo/RubiniusTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"

using namespace llvm;

Target &llvm::getTheRubiniusleTarget() {
  static Target TheRubiniusleTarget;
  return TheRubiniusleTarget;
}
Target &llvm::getTheRubiniusbeTarget() {
  static Target TheRubiniusbeTarget;
  return TheRubiniusbeTarget;
}
Target &llvm::getTheRubiniusTarget() {
  static Target TheRubiniusTarget;
  return TheRubiniusTarget;
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeRubiniusTargetInfo() {
  TargetRegistry::RegisterTarget(getTheRubiniusTarget(), "rbx", "Rubinius (host endian)",
                                 "Rubinius", [](Triple::ArchType) { return false; },
                                 true);
}
