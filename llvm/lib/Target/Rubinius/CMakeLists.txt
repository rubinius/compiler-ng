add_llvm_component_group(Rubinius)

# TableGen setup: all Rubinius .td defs should be rooted at Rubinius.td
set(LLVM_TARGET_DEFINITIONS Rubinius.td)

# Generate the backend's TableGen outputs.
tablegen(LLVM RubiniusGenRegisterInfo.inc   -gen-register-info)
tablegen(LLVM RubiniusGenInstrInfo.inc      -gen-instr-info)
tablegen(LLVM RubiniusGenSubtargetInfo.inc  -gen-subtarget)
tablegen(LLVM RubiniusGenAsmWriter.inc      -gen-asm-writer)
tablegen(LLVM RubiniusGenMCCodeEmitter.inc  -gen-emitter)

# Make these TableGen outputs visible to other pieces (MCTargetDesc, etc.).
add_public_tablegen_target(RubiniusCommonTableGen)

# The core codegen library for the Rubinius backend.
# Name must match the component in llvm/lib/Target/Rubinius/LLVMBuild.txt (RubiniusCodeGen).
add_llvm_target(RubiniusCodeGen
  RubiniusAsmPrinter.cpp
  RubiniusISelDAGToDAG.cpp
  RubiniusISelLowering.cpp
  RubiniusInstrInfo.cpp
  RubiniusMCInstLower.cpp
  RubiniusRegisterInfo.cpp
  RubiniusSubtarget.cpp
  RubiniusTargetInfo.cpp
  RubiniusTargetMachine.cpp

  LINK_COMPONENTS
    AsmPrinter
    CodeGen
    Core
    MC
    SelectionDAG
    Support
    Target
    RubiniusDesc
    RubiniusInfo

  ADD_TO_COMPONENT
    Rubinius
)

# These subdirectories define the MC-layer and target-info libraries.
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
