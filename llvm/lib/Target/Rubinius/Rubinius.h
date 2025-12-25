#pragma once

#include "MCTargetDesc/RubiniusMCTargetDesc.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class RubiniusRegisterBankInfo;
class RubiniusSubtarget;
class RubiniusTargetMachine;
class InstructionSelector;
class PassRegistry;

#define Rubinius_TRAP "__bpf_trap"

FunctionPass *createRubiniusISelDag(RubiniusTargetMachine &TM);

InstructionSelector *createRubiniusInstructionSelector(const RubiniusTargetMachine &,
                                                  const RubiniusSubtarget &,
                                                  const RubiniusRegisterBankInfo &);

void initializeRubiniusAsmPrinterPass(PassRegistry &);

class RubiniusIRPeepholePass : public PassInfoMixin<RubiniusIRPeepholePass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }
};

class RubiniusASpaceCastSimplifyPass
    : public PassInfoMixin<RubiniusASpaceCastSimplifyPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }
};
} // namespace llvm
