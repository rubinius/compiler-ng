#pragma once
#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class RubiniusMCAsmInfo : public MCAsmInfoELF {
public:
  RubiniusMCAsmInfo();
};

} // namespace llvm
