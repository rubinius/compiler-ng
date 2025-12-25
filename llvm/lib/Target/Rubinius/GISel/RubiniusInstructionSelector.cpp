#include "RubiniusInstrInfo.h"
#include "RubiniusRegisterBankInfo.h"
#include "RubiniusSubtarget.h"
#include "RubiniusTargetMachine.h"
#include "llvm/CodeGen/GlobalISel/GIMatchTableExecutorImpl.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"

#define DEBUG_TYPE "rbx-gisel"

using namespace llvm;

namespace {

#define GET_GLOBALISEL_PREDICATE_BITSET
#include "RubiniusGenGlobalISel.inc"
#undef GET_GLOBALISEL_PREDICATE_BITSET

class RubiniusInstructionSelector : public InstructionSelector {
public:
  RubiniusInstructionSelector(const RubiniusTargetMachine &TM, const RubiniusSubtarget &STI,
                         const RubiniusRegisterBankInfo &RBI);

  bool select(MachineInstr &I) override;
  static const char *getName() { return DEBUG_TYPE; }

private:
  /// tblgen generated 'select' implementation that is used as the initial
  /// selector for the patterns that do not require complex C++.
  bool selectImpl(MachineInstr &I, CodeGenCoverage &CoverageInfo) const;

  const RubiniusInstrInfo &TII;
  const RubiniusRegisterInfo &TRI;
  const RubiniusRegisterBankInfo &RBI;

#define GET_GLOBALISEL_PREDICATES_DECL
#include "RubiniusGenGlobalISel.inc"
#undef GET_GLOBALISEL_PREDICATES_DECL

#define GET_GLOBALISEL_TEMPORARIES_DECL
#include "RubiniusGenGlobalISel.inc"
#undef GET_GLOBALISEL_TEMPORARIES_DECL
};

} // namespace

#define GET_GLOBALISEL_IMPL
#include "RubiniusGenGlobalISel.inc"
#undef GET_GLOBALISEL_IMPL

RubiniusInstructionSelector::RubiniusInstructionSelector(const RubiniusTargetMachine &TM,
                                               const RubiniusSubtarget &STI,
                                               const RubiniusRegisterBankInfo &RBI)
    : TII(*STI.getInstrInfo()), TRI(*STI.getRegisterInfo()), RBI(RBI),
#define GET_GLOBALISEL_PREDICATES_INIT
#include "RubiniusGenGlobalISel.inc"
#undef GET_GLOBALISEL_PREDICATES_INIT
#define GET_GLOBALISEL_TEMPORARIES_INIT
#include "RubiniusGenGlobalISel.inc"
#undef GET_GLOBALISEL_TEMPORARIES_INIT
{
}

bool RubiniusInstructionSelector::select(MachineInstr &I) {
  if (!isPreISelGenericOpcode(I.getOpcode()))
    return true;
  if (selectImpl(I, *CoverageInfo))
    return true;
  return false;
}

namespace llvm {
InstructionSelector *
createRubiniusInstructionSelector(const RubiniusTargetMachine &TM,
                             const RubiniusSubtarget &Subtarget,
                             const RubiniusRegisterBankInfo &RBI) {
  return new RubiniusInstructionSelector(TM, Subtarget, RBI);
}
} // namespace llvm
