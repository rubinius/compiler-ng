#include "RubiniusMCTargetDesc.h"
#include "RubiniusMCAsmInfo.h"
#include "RubiniusGenInstrInfo.inc"
#include "RubiniusGenRegisterInfo.inc"
#include "RubiniusGenSubtargetInfo.inc"

#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target &getTheRubiniusTarget(); // from TargetInfo

MCAsmInfo *llvm::createRubiniusMCAsmInfo(const Triple &TT,
    const MCTargetOptions &) {
  return new RubiniusMCAsmInfo();
}

MCInstrInfo *llvm::createRubiniusMCInstrInfo() {
  auto *X = new MCInstrInfo();
  InitRubiniusMCInstrInfo(*X);
  return X;
}

MCRegisterInfo *llvm::createRubiniusMCRegisterInfo() {
  auto *X = new MCRegisterInfo();
  InitRubiniusMCRegisterInfo(*X, /*RA=*/0);
  return X;
}

MCSubtargetInfo *llvm::createRubiniusMCSubtargetInfo(const Triple &TT,
    StringRef CPU, StringRef FS) {
  return createRubiniusSubtargetInfoImpl(TT, CPU, FS);
}

namespace {
  class RubiniusELFObjectWriter : public MCELFObjectTargetWriter {
    public:
      RubiniusELFObjectWriter()
        : MCELFObjectTargetWriter(/*Is64Bit=*/false, /*OSABI=*/0,
            /*EMachine=*/0x7777, /*HasRelocationAddend=*/false) {}

      unsigned getRelocType(MCContext &, const MCValue &,
          const MCFixup &, bool) const override {
        llvm_unreachable("No relocations supported in minimal Rubinius");
      }
  };
} // namespace

static std::unique_ptr<MCObjectTargetWriter>
createRubiniusELFObjectWriter() {
  return createELFObjectWriter(std::make_unique<RubiniusELFObjectWriter>());
}

namespace {
  class RubiniusAsmBackend : public MCAsmBackend {
    public:
      RubiniusAsmBackend(const Target &T, const MCSubtargetInfo &STI)
        : MCAsmBackend(SupportsRelaxation::None) {}

      unsigned getNumFixupKinds() const override { return 0; }
      bool writeNopData(raw_ostream &OS, uint64_t Count) const override {
        for (uint64_t i = 0; i < Count; ++i)
          OS << '\0';
        return true;
      }

      std::unique_ptr<MCObjectTargetWriter>
        createObjectTargetWriter() const override {
          return createRubiniusELFObjectWriter();
        }
  };
} // namespace

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRubiniusTargetMC() {
  Target &T = getTheRubiniusTarget();

  RegisterMCAsmInfoFn X(T, createRubiniusMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(T, createRubiniusMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(T, createRubiniusMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(T, createRubiniusMCSubtargetInfo);

  TargetRegistry::RegisterMCAsmBackend(
      T, [](const Target &T, const MCSubtargetInfo &STI, const MCRegisterInfo &,
        const MCTargetOptions &) -> std::unique_ptr<MCAsmBackend> {
      return std::make_unique<RubiniusAsmBackend>(T, STI);
      });
}
