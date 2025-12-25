#pragma once

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class RubiniusSubtarget;

class RubiniusFrameLowering : public TargetFrameLowering {
public:
  explicit RubiniusFrameLowering(const RubiniusSubtarget &sti)
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(8), 0) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const override;

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator MI) const override {
    return MBB.erase(MI);
  }

protected:
  bool hasFPImpl(const MachineFunction &MF) const override;
};
}
