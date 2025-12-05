#pragma once
#include "llvm/Target/TargetSubtargetInfo.h"

namespace llvm {

class RubiniusSubtarget : public TargetSubtargetInfo {
public:
  RubiniusSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                const TargetMachine &TM);

  // No extra features, keep default.
};

} // namespace llvm
