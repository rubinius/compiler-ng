#pragma once
#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
class MCAsmInfo;
class MCSubtargetInfo;
class MCInstrInfo;
class MCRegisterInfo;
class MCContext;
class Target;
class Triple;
class StringRef;
class raw_ostream;

MCAsmInfo *createRubiniusMCAsmInfo(const Triple &TT, const MCTargetOptions &);
MCInstrInfo *createRubiniusMCInstrInfo();
MCRegisterInfo *createRubiniusMCRegisterInfo();
MCSubtargetInfo *createRubiniusMCSubtargetInfo(const Triple &TT,
                                           StringRef CPU, StringRef FS);

Target &getTheRubiniusTarget();
} // namespace llvm
