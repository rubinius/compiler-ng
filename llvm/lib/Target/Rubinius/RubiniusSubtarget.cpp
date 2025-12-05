#include "RubiniusSubtarget.h"
#include "RubiniusGenSubtargetInfo.inc"
using namespace llvm;

RubiniusSubtarget::RubiniusSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                             const TargetMachine &TM)
    : TargetSubtargetInfo(TT, CPU, FS, /*ProcSchedModels*/ None,
                          /*ProcItinData*/ nullptr, nullptr, nullptr, nullptr) {}
