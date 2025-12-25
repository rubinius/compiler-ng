#include "MCTargetDesc/RubiniusMCFixups.h"
#include "MCTargetDesc/RubiniusMCTargetDesc.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/EndianStream.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

namespace {

class RubiniusMCCodeEmitter : public MCCodeEmitter {
  const MCRegisterInfo &MRI;
  bool IsLittleEndian;
  MCContext &Ctx;

public:
  RubiniusMCCodeEmitter(const MCInstrInfo &, const MCRegisterInfo &mri,
                   bool IsLittleEndian, MCContext &ctx)
      : MRI(mri), IsLittleEndian(IsLittleEndian), Ctx(ctx) {}
  RubiniusMCCodeEmitter(const RubiniusMCCodeEmitter &) = delete;
  void operator=(const RubiniusMCCodeEmitter &) = delete;
  ~RubiniusMCCodeEmitter() override = default;

  // getBinaryCodeForInstr - TableGen'erated function for getting the
  // binary encoding for an instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  // getMachineOpValue - Return binary encoding of operand. If the machin
  // operand requires relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  uint64_t getMemoryOpValue(const MCInst &MI, unsigned Op,
                            SmallVectorImpl<MCFixup> &Fixups,
                            const MCSubtargetInfo &STI) const;

  void encodeInstruction(const MCInst &MI, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;
};

} // end anonymous namespace

MCCodeEmitter *llvm::createRubiniusMCCodeEmitter(const MCInstrInfo &MCII,
                                            MCContext &Ctx) {
  return new RubiniusMCCodeEmitter(MCII, *Ctx.getRegisterInfo(), true, Ctx);
}

MCCodeEmitter *llvm::createRubiniusbeMCCodeEmitter(const MCInstrInfo &MCII,
                                              MCContext &Ctx) {
  return new RubiniusMCCodeEmitter(MCII, *Ctx.getRegisterInfo(), false, Ctx);
}

static void addFixup(SmallVectorImpl<MCFixup> &Fixups, uint32_t Offset,
                     const MCExpr *Value, uint16_t Kind, bool PCRel = false) {
  Fixups.push_back(MCFixup::create(Offset, Value, Kind, PCRel));
}

unsigned RubiniusMCCodeEmitter::getMachineOpValue(const MCInst &MI,
                                             const MCOperand &MO,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return MRI.getEncodingValue(MO.getReg());
  if (MO.isImm()) {
    uint64_t Imm = MO.getImm();
    uint64_t High32Bits = Imm >> 32, High33Bits = Imm >> 31;
    if (MI.getOpcode() != Rubinius::LD_imm64 && High32Bits != 0 &&
        High33Bits != 0x1FFFFFFFFULL) {
      Ctx.reportWarning(MI.getLoc(),
                        "immediate out of range, shall fit in 32 bits");
    }
    return static_cast<unsigned>(Imm);
  }

  assert(MO.isExpr());

  const MCExpr *Expr = MO.getExpr();

  assert(Expr->getKind() == MCExpr::SymbolRef);

  if (MI.getOpcode() == Rubinius::JAL)
    // func call name
    addFixup(Fixups, 0, Expr, FK_Data_4, true);
  else if (MI.getOpcode() == Rubinius::LD_imm64)
    addFixup(Fixups, 0, Expr, FK_SecRel_8);
  else if (MI.getOpcode() == Rubinius::JMPL)
    addFixup(Fixups, 0, Expr, Rubinius::FK_Rubinius_PCRel_4, true);
  else
    // bb label
    addFixup(Fixups, 0, Expr, FK_Data_2, true);

  return 0;
}

static uint8_t SwapBits(uint8_t Val)
{
  return (Val & 0x0F) << 4 | (Val & 0xF0) >> 4;
}

void RubiniusMCCodeEmitter::encodeInstruction(const MCInst &MI,
                                         SmallVectorImpl<char> &CB,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  unsigned Opcode = MI.getOpcode();
  raw_svector_ostream OS(CB);
  support::endian::Writer OSE(OS, IsLittleEndian ? llvm::endianness::little
                                                 : llvm::endianness::big);

  // TODO: Rubinius
}

// Encode Rubinius Memory Operand
uint64_t RubiniusMCCodeEmitter::getMemoryOpValue(const MCInst &MI, unsigned Op,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  // For CMPXCHG instructions, output is implicitly in R0/W0,
  // so memory operand starts from operand 0.
  int MemOpStartIndex = 1, Opcode = MI.getOpcode();
  if (Opcode == Rubinius::CMPXCHGW32 || Opcode == Rubinius::CMPXCHGD)
    MemOpStartIndex = 0;

  uint64_t Encoding;
  const MCOperand Op1 = MI.getOperand(MemOpStartIndex);
  assert(Op1.isReg() && "First operand is not register.");
  Encoding = MRI.getEncodingValue(Op1.getReg());
  Encoding <<= 16;
  MCOperand Op2 = MI.getOperand(MemOpStartIndex + 1);
  assert(Op2.isImm() && "Second operand is not immediate.");
  Encoding |= Op2.getImm() & 0xffff;
  return Encoding;
}

#include "RubiniusGenMCCodeEmitter.inc"
