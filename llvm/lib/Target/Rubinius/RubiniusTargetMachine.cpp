#include "llvm/Target/TargetMachine.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

extern Target &getTheRubiniusTarget();

class RubiniusTargetMachine : public LLVMTargetMachine {
public:
  RubiniusTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                        StringRef FS, const TargetOptions &Options,
                        std::optional<Reloc::Model> RM,
                        std::optional<CodeModel::Model> CM,
                        CodeGenOptLevel OL)
      : LLVMTargetMachine(T, "e", TT, CPU, FS, Options, RM, CM, OL) {}
};

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRubiniusTarget() {
  Target &T = getTheRubiniusTarget();
  RegisterTargetMachine<RubiniusTargetMachine> X(T);
}
