#include "RubiniusGenInstrInfo.inc"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
using namespace llvm;

namespace {
class RubiniusAsmPrinter : public AsmPrinter {
public:
  using AsmPrinter::AsmPrinter;

  StringRef getPassName() const override { return "Rubinius Assembly Printer"; }

  void emitInstruction(const MachineInstr *MI) override {
    OutStreamer->emitInstruction(*MI, getSubtargetInfo());
  }
};
} // namespace

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRubiniusAsmPrinter() {
  RegisterAsmPrinter<RubiniusAsmPrinter> X(getTheRubiniusTarget());
}
