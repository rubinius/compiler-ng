#include "MCTargetDesc/RubiniusMCTargetDesc.h"
#include "MCTargetDesc/RubiniusInstPrinter.h"
#include "MCTargetDesc/RubiniusMCAsmInfo.h"
#include "TargetInfo/RubiniusTargetInfo.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Host.h"

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "RubiniusGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "RubiniusGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "RubiniusGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createRubiniusMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitRubiniusMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createRubiniusMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitRubiniusMCRegisterInfo(X, Rubinius::R11 /* RAReg doesn't exist */);
  return X;
}

static MCSubtargetInfo *createRubiniusMCSubtargetInfo(const Triple &TT,
                                                 StringRef CPU, StringRef FS) {
  return createRubiniusMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCStreamer *
createRubiniusMCStreamer(const Triple &T, MCContext &Ctx,
                    std::unique_ptr<MCAsmBackend> &&MAB,
                    std::unique_ptr<MCObjectWriter> &&OW,
                    std::unique_ptr<MCCodeEmitter> &&Emitter) {
  return createELFStreamer(Ctx, std::move(MAB), std::move(OW),
                           std::move(Emitter));
}

static MCInstPrinter *createRubiniusMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new RubiniusInstPrinter(MAI, MII, MRI);
  return nullptr;
}

namespace {

class RubiniusMCInstrAnalysis : public MCInstrAnalysis {
public:
  explicit RubiniusMCInstrAnalysis(const MCInstrInfo *Info)
      : MCInstrAnalysis(Info) {}

  bool evaluateBranch(const MCInst &Inst, uint64_t Addr, uint64_t Size,
                      uint64_t &Target) const override {
    // The target is the 3rd operand of cond inst and the 1st of uncond inst.
    int32_t Imm;
    if (isConditionalBranch(Inst)) {
      if (Inst.getOpcode() == Rubinius::JCOND)
        Imm = (short)Inst.getOperand(0).getImm();
      else
        Imm = (short)Inst.getOperand(2).getImm();
    } else if (isUnconditionalBranch(Inst)) {
      if (Inst.getOpcode() == Rubinius::JMP)
        Imm = (short)Inst.getOperand(0).getImm();
      else
        Imm = (int)Inst.getOperand(0).getImm();
    } else
      return false;

    Target = Addr + Size + Imm * Size;
    return true;
  }
};

} // end anonymous namespace

static MCInstrAnalysis *createRubiniusInstrAnalysis(const MCInstrInfo *Info) {
  return new RubiniusMCInstrAnalysis(Info);
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRubiniusTargetMC() {
  for (Target *T :
       {&getTheRubiniusleTarget(), &getTheRubiniusbeTarget(), &getTheRubiniusTarget()}) {
    // Register the MC asm info.
    RegisterMCAsmInfo<RubiniusMCAsmInfo> X(*T);

    // Register the MC instruction info.
    TargetRegistry::RegisterMCInstrInfo(*T, createRubiniusMCInstrInfo);

    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(*T, createRubiniusMCRegisterInfo);

    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(*T,
                                            createRubiniusMCSubtargetInfo);

    // Register the object streamer
    TargetRegistry::RegisterELFStreamer(*T, createRubiniusMCStreamer);

    // Register the MCInstPrinter.
    TargetRegistry::RegisterMCInstPrinter(*T, createRubiniusMCInstPrinter);

    // Register the MC instruction analyzer.
    TargetRegistry::RegisterMCInstrAnalysis(*T, createRubiniusInstrAnalysis);
  }

  // Register the MC code emitter
  TargetRegistry::RegisterMCCodeEmitter(getTheRubiniusleTarget(),
                                        createRubiniusMCCodeEmitter);
  TargetRegistry::RegisterMCCodeEmitter(getTheRubiniusbeTarget(),
                                        createRubiniusbeMCCodeEmitter);

  // Register the ASM Backend
  TargetRegistry::RegisterMCAsmBackend(getTheRubiniusleTarget(),
                                       createRubiniusAsmBackend);
  TargetRegistry::RegisterMCAsmBackend(getTheRubiniusbeTarget(),
                                       createRubiniusbeAsmBackend);

  if (sys::IsLittleEndianHost) {
    TargetRegistry::RegisterMCCodeEmitter(getTheRubiniusTarget(),
                                          createRubiniusMCCodeEmitter);
    TargetRegistry::RegisterMCAsmBackend(getTheRubiniusTarget(),
                                         createRubiniusAsmBackend);
  } else {
    TargetRegistry::RegisterMCCodeEmitter(getTheRubiniusTarget(),
                                          createRubiniusbeMCCodeEmitter);
    TargetRegistry::RegisterMCAsmBackend(getTheRubiniusTarget(),
                                         createRubiniusbeAsmBackend);
  }
}
