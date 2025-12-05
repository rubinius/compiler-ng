# TableGen setup: all Rubinius .td defs should be rooted at Rubinius.td
set(LLVM_TARGET_DEFINITIONS Rubinius.td)

# Generate the minimal set of backend inc files we actually use.
tablegen(LLVM RubiniusGenRegisterInfo.inc   -gen-register-info)
tablegen(LLVM RubiniusGenInstrInfo.inc      -gen-instr-info)
tablegen(LLVM RubiniusGenSubtargetInfo.inc  -gen-subtarget)
tablegen(LLVM RubiniusGenAsmWriter.inc      -gen-asm-writer)
tablegen(LLVM RubiniusGenMCCodeEmitter.inc  -gen-emitter)

# Make these TableGen outputs visible to other pieces (MCTargetDesc, etc.).
add_public_tablegen_target(RubiniusCommonTableGen)

# The core codegen library for the Rubinius backend.
# Name must match the component in llvm/lib/Target/Rubinius/LLVMBuild.txt
# (RubiniusCodeGen).
add_llvm_target(RubiniusCodeGen
  RubiniusAsmPrinter.cpp
  RubiniusInstrInfo.cpp
  RubiniusISelDAGToDAG.cpp
  RubiniusISelLowering.cpp
  RubiniusMCInstLower.cpp
  RubiniusRegisterInfo.cpp
  RubiniusSubtarget.cpp
  RubiniusTargetMachine.cpp
)

add_llvm_component_library(LLVMRubiniusCodeGen
  RubiniusTargetMachine.cpp
  RubiniusISelLowering.cpp
  RubiniusSubtarget.cpp
  RubiniusAsmPrinter.cpp

  DEPENDS
  RubiniusCommonTableGen

  LINK_COMPONENTS
  AsmPrinter
  Core
  CodeGen
  MC
  SelectionDAG
  Support
  Target
)

add_llvm_component_library(LLVMRubiniusDesc
  MCTargetDesc/RubiniusMCTargetDesc.cpp
  MCTargetDesc/RubiniusMCAsmInfo.cpp

  DEPENDS
  RubiniusCommonTableGen

  LINK_COMPONENTS
  MC
  MCDisassembler
  Support
)

# These must exist and have their own CMakeLists.txt files.
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
