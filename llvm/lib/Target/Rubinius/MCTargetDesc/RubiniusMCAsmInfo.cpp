#include "RubiniusMCAsmInfo.h"
using namespace llvm;

RubiniusMCAsmInfo::RubiniusMCAsmInfo(const Triple &TT,
                                     const MCTargetOptions &)
    : MCAsmInfoELF() {
  CommentString = "#";
  MinInstAlignment = 4;
}
