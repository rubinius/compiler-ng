#include "RubiniusInstrInfo.h"
#include "Rubinius.h"
#include "RubiniusSubtarget.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/Support/ErrorHandling.h"
#include <cassert>
#include <iterator>

#define GET_INSTRINFO_CTOR_DTOR
#include "RubiniusGenInstrInfo.inc"

using namespace llvm;

RubiniusInstrInfo::RubiniusInstrInfo(const RubiniusSubtarget &STI)
    : RubiniusGenInstrInfo(STI, RI, Rubinius::ADJCALLSTACKDOWN, Rubinius::ADJCALLSTACKUP) {}

void RubiniusInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator I,
                               const DebugLoc &DL, Register DestReg,
                               Register SrcReg, bool KillSrc,
                               bool RenamableDest, bool RenamableSrc) const {
  if (Rubinius::GPRRegClass.contains(DestReg, SrcReg))
    BuildMI(MBB, I, DL, get(Rubinius::MOV_rr), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc));
  else if (Rubinius::GPR32RegClass.contains(DestReg, SrcReg))
    BuildMI(MBB, I, DL, get(Rubinius::MOV_rr_32), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc));
  else
    llvm_unreachable("Impossible reg-to-reg copy");
}

void RubiniusInstrInfo::expandMEMCPY(MachineBasicBlock::iterator MI) const {
  Register DstReg = MI->getOperand(0).getReg();
  Register SrcReg = MI->getOperand(1).getReg();
  uint64_t CopyLen = MI->getOperand(2).getImm();
  uint64_t Alignment = MI->getOperand(3).getImm();
  Register ScratchReg = MI->getOperand(4).getReg();
  MachineBasicBlock *BB = MI->getParent();
  DebugLoc dl = MI->getDebugLoc();
  unsigned LdOpc, StOpc;

  switch (Alignment) {
  case 1:
    LdOpc = Rubinius::LDB;
    StOpc = Rubinius::STB;
    break;
  case 2:
    LdOpc = Rubinius::LDH;
    StOpc = Rubinius::STH;
    break;
  case 4:
    LdOpc = Rubinius::LDW;
    StOpc = Rubinius::STW;
    break;
  case 8:
    LdOpc = Rubinius::LDD;
    StOpc = Rubinius::STD;
    break;
  default:
    llvm_unreachable("unsupported memcpy alignment");
  }

  unsigned IterationNum = CopyLen >> Log2_64(Alignment);
  for(unsigned I = 0; I < IterationNum; ++I) {
    BuildMI(*BB, MI, dl, get(LdOpc))
            .addReg(ScratchReg, RegState::Define).addReg(SrcReg)
            .addImm(I * Alignment);
    BuildMI(*BB, MI, dl, get(StOpc))
            .addReg(ScratchReg, RegState::Kill).addReg(DstReg)
            .addImm(I * Alignment);
  }

  unsigned BytesLeft = CopyLen & (Alignment - 1);
  unsigned Offset = IterationNum * Alignment;
  bool Hanging4Byte = BytesLeft & 0x4;
  bool Hanging2Byte = BytesLeft & 0x2;
  bool Hanging1Byte = BytesLeft & 0x1;
  if (Hanging4Byte) {
    BuildMI(*BB, MI, dl, get(Rubinius::LDW))
            .addReg(ScratchReg, RegState::Define).addReg(SrcReg).addImm(Offset);
    BuildMI(*BB, MI, dl, get(Rubinius::STW))
            .addReg(ScratchReg, RegState::Kill).addReg(DstReg).addImm(Offset);
    Offset += 4;
  }
  if (Hanging2Byte) {
    BuildMI(*BB, MI, dl, get(Rubinius::LDH))
            .addReg(ScratchReg, RegState::Define).addReg(SrcReg).addImm(Offset);
    BuildMI(*BB, MI, dl, get(Rubinius::STH))
            .addReg(ScratchReg, RegState::Kill).addReg(DstReg).addImm(Offset);
    Offset += 2;
  }
  if (Hanging1Byte) {
    BuildMI(*BB, MI, dl, get(Rubinius::LDB))
            .addReg(ScratchReg, RegState::Define).addReg(SrcReg).addImm(Offset);
    BuildMI(*BB, MI, dl, get(Rubinius::STB))
            .addReg(ScratchReg, RegState::Kill).addReg(DstReg).addImm(Offset);
  }

  BB->erase(MI);
}

bool RubiniusInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  if (MI.getOpcode() == Rubinius::MEMCPY) {
    expandMEMCPY(MI);
    return true;
  }

  return false;
}

void RubiniusInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator I,
                                       Register SrcReg, bool IsKill, int FI,
                                       const TargetRegisterClass *RC,
                                       Register VReg,
                                       MachineInstr::MIFlag Flags) const {
  DebugLoc DL;
  if (I != MBB.end())
    DL = I->getDebugLoc();

  if (RC == &Rubinius::GPRRegClass)
    BuildMI(MBB, I, DL, get(Rubinius::STD))
        .addReg(SrcReg, getKillRegState(IsKill))
        .addFrameIndex(FI)
        .addImm(0);
  else if (RC == &Rubinius::GPR32RegClass)
    BuildMI(MBB, I, DL, get(Rubinius::STW32))
        .addReg(SrcReg, getKillRegState(IsKill))
        .addFrameIndex(FI)
        .addImm(0);
  else
    llvm_unreachable("Can't store this register to stack slot");
}

void RubiniusInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator I,
                                        Register DestReg, int FI,
                                        const TargetRegisterClass *RC,
                                        Register VReg,
                                        MachineInstr::MIFlag Flags) const {
  DebugLoc DL;
  if (I != MBB.end())
    DL = I->getDebugLoc();

  if (RC == &Rubinius::GPRRegClass)
    BuildMI(MBB, I, DL, get(Rubinius::LDD), DestReg).addFrameIndex(FI).addImm(0);
  else if (RC == &Rubinius::GPR32RegClass)
    BuildMI(MBB, I, DL, get(Rubinius::LDW32), DestReg).addFrameIndex(FI).addImm(0);
  else
    llvm_unreachable("Can't load this register from stack slot");
}

bool RubiniusInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                 MachineBasicBlock *&TBB,
                                 MachineBasicBlock *&FBB,
                                 SmallVectorImpl<MachineOperand> &Cond,
                                 bool AllowModify) const {
  // Start from the bottom of the block and work up, examining the
  // terminator instructions.
  MachineBasicBlock::iterator I = MBB.end();
  while (I != MBB.begin()) {
    --I;
    if (I->isDebugInstr())
      continue;

    // Working from the bottom, when we see a non-terminator
    // instruction, we're done.
    if (!isUnpredicatedTerminator(*I))
      break;

    // From base method doc: ... returning true if it cannot be understood ...
    // Indirect branch has multiple destinations and no true/false concepts.
    if (I->isIndirectBranch())
      return true;

    // A terminator that isn't a branch can't easily be handled
    // by this analysis.
    if (!I->isBranch())
      return true;

    // Handle unconditional branches.
    if (I->getOpcode() == Rubinius::JMP) {
      if (!AllowModify) {
        TBB = I->getOperand(0).getMBB();
        continue;
      }

      // If the block has any instructions after a J, delete them.
      MBB.erase(std::next(I), MBB.end());
      Cond.clear();
      FBB = nullptr;

      // Delete the J if it's equivalent to a fall-through.
      if (MBB.isLayoutSuccessor(I->getOperand(0).getMBB())) {
        TBB = nullptr;
        I->eraseFromParent();
        I = MBB.end();
        continue;
      }

      // TBB is used to indicate the unconditinal destination.
      TBB = I->getOperand(0).getMBB();
      continue;
    }
    // Cannot handle conditional branches
    return true;
  }

  return false;
}

unsigned RubiniusInstrInfo::insertBranch(MachineBasicBlock &MBB,
                                    MachineBasicBlock *TBB,
                                    MachineBasicBlock *FBB,
                                    ArrayRef<MachineOperand> Cond,
                                    const DebugLoc &DL,
                                    int *BytesAdded) const {
  assert(!BytesAdded && "code size not handled");

  // Shouldn't be a fall through.
  assert(TBB && "insertBranch must not be told to insert a fallthrough");

  if (Cond.empty()) {
    // Unconditional branch
    assert(!FBB && "Unconditional branch with multiple successors!");
    BuildMI(&MBB, DL, get(Rubinius::JMP)).addMBB(TBB);
    return 1;
  }

  llvm_unreachable("Unexpected conditional branch");
}

unsigned RubiniusInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                    int *BytesRemoved) const {
  assert(!BytesRemoved && "code size not handled");

  MachineBasicBlock::iterator I = MBB.end();
  unsigned Count = 0;

  while (I != MBB.begin()) {
    --I;
    if (I->isDebugInstr())
      continue;
    if (I->getOpcode() != Rubinius::JMP)
      break;
    // Remove the branch.
    I->eraseFromParent();
    I = MBB.end();
    ++Count;
  }

  return Count;
}

int RubiniusInstrInfo::getJumpTableIndex(const MachineInstr &MI) const {
  if (MI.getOpcode() != Rubinius::JX)
    return -1;

  // The pattern looks like:
  // %0 = LD_imm64 %jump-table.0   ; load jump-table address
  // %1 = ADD_rr %0, $another_reg  ; address + offset
  // %2 = LDD %1, 0                ; load the actual label
  // JX %2
  const MachineFunction &MF = *MI.getParent()->getParent();
  const MachineRegisterInfo &MRI = MF.getRegInfo();

  Register Reg = MI.getOperand(0).getReg();
  if (!Reg.isVirtual())
    return -1;
  MachineInstr *Ldd = MRI.getUniqueVRegDef(Reg);
  if (Ldd == nullptr || Ldd->getOpcode() != Rubinius::LDD)
    return -1;

  Reg = Ldd->getOperand(1).getReg();
  if (!Reg.isVirtual())
    return -1;
  MachineInstr *Add = MRI.getUniqueVRegDef(Reg);
  if (Add == nullptr || Add->getOpcode() != Rubinius::ADD_rr)
    return -1;

  Reg = Add->getOperand(1).getReg();
  if (!Reg.isVirtual())
    return -1;
  MachineInstr *LDimm64 = MRI.getUniqueVRegDef(Reg);
  if (LDimm64 == nullptr || LDimm64->getOpcode() != Rubinius::LD_imm64)
    return -1;

  const MachineOperand &MO = LDimm64->getOperand(1);
  if (!MO.isJTI())
    return -1;

  return MO.getIndex();
}
