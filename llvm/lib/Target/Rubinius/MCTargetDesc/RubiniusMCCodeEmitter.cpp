// RubiniusMCCodeEmitter.cpp
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class RubiniusMCCodeEmitter : public MCCodeEmitter {
public:
  RubiniusMCCodeEmitter(const MCInstrInfo &, MCContext &) {}
  void encodeInstruction(const MCInst &, raw_ostream &, SmallVectorImpl<MCFixup> &,
                         const MCSubtargetInfo &) const override {
    llvm_unreachable("RubiniusMCCodeEmitter not implemented yet");
  }
};
} // namespace

MCCodeEmitter *createRubiniusMCCodeEmitter(const MCInstrInfo &II,
                                           MCContext &Ctx) {
  return new RubiniusMCCodeEmitter(II, Ctx);
}
