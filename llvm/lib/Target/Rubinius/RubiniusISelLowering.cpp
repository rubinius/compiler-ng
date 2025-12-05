#include "RubiniusISelLowering.h"
#include "RubiniusSubtarget.h"
using namespace llvm;

RubiniusTargetLowering::RubiniusTargetLowering(const TargetMachine &TM,
                                       const RubiniusSubtarget &)
    : TargetLowering(TM) {
  // We accept the default legalization for i32 add, which is fine for the demo.
}
