#pragma once

#include "RubiniusTargetMachine.h"
#include "BTFDebug.h"
#include "llvm/CodeGen/AsmPrinter.h"

namespace llvm {

class RubiniusAsmPrinter : public AsmPrinter {
public:
  explicit RubiniusAsmPrinter(TargetMachine &TM,
                         std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer), ID), BTF(nullptr), TM(TM) {}

  StringRef getPassName() const override { return "Rubinius Assembly Printer"; }
  bool doInitialization(Module &M) override;
  bool doFinalization(Module &M) override;
  void printOperand(const MachineInstr *MI, int OpNum, raw_ostream &O);
  bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                       const char *ExtraCode, raw_ostream &O) override;
  bool PrintAsmMemoryOperand(const MachineInstr *MI, unsigned OpNum,
                             const char *ExtraCode, raw_ostream &O) override;

  void emitInstruction(const MachineInstr *MI) override;
  MCSymbol *getJTPublicSymbol(unsigned JTI);
  void emitJumpTableInfo() override;

  static char ID;

private:
  BTFDebug *BTF;
  TargetMachine &TM;
  bool SawTrapCall = false;

  const RubiniusTargetMachine &getBTM() const;
};

} // namespace llvm
