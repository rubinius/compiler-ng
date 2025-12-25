#pragma once

#include "llvm/Config/config.h"
#include "llvm/MC/MCContext.h"
#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class Target;

MCCodeEmitter *createRubiniusMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx);
MCCodeEmitter *createRubiniusbeMCCodeEmitter(const MCInstrInfo &MCII,
                                        MCContext &Ctx);

MCAsmBackend *createRubiniusAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                  const MCRegisterInfo &MRI,
                                  const MCTargetOptions &Options);
MCAsmBackend *createRubiniusbeAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);
std::unique_ptr<MCObjectTargetWriter> createRubiniusELFObjectWriter(uint8_t OSABI);
} // namespace llvm

// Defines symbolic names for Rubinius registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "RubiniusGenRegisterInfo.inc"

// Defines symbolic names for the Rubinius instructions.
//
#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_MC_HELPER_DECLS
#include "RubiniusGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "RubiniusGenSubtargetInfo.inc"
