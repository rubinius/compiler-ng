#include "RubiniusFrameLowering.h"
#include "RubiniusSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"

using namespace llvm;

bool RubiniusFrameLowering::hasFPImpl(const MachineFunction &MF) const {
  return true;
}

void RubiniusFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}

void RubiniusFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {}

void RubiniusFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                            BitVector &SavedRegs,
                                            RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  SavedRegs.reset(Rubinius::R6);
  SavedRegs.reset(Rubinius::R7);
  SavedRegs.reset(Rubinius::R8);
  SavedRegs.reset(Rubinius::R9);
}
