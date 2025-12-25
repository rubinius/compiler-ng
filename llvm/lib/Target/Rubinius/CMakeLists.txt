add_llvm_component_group(Rubinius)

set(LLVM_TARGET_DEFINITIONS Rubinius.td)

tablegen(LLVM RubiniusGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM RubiniusGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM RubiniusGenCallingConv.inc -gen-callingconv)
tablegen(LLVM RubiniusGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM RubiniusGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM RubiniusGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM RubiniusGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM RubiniusGenSDNodeInfo.inc -gen-sd-node-info)
tablegen(LLVM RubiniusGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM RubiniusGenGlobalISel.inc -gen-global-isel)
tablegen(LLVM RubiniusGenRegisterBank.inc -gen-register-bank)

add_public_tablegen_target(RubiniusCommonTableGen)

add_llvm_target(RubiniusCodeGen
  GISel/RubiniusCallLowering.cpp
  GISel/RubiniusInstructionSelector.cpp
  GISel/RubiniusRegisterBankInfo.cpp
  GISel/RubiniusLegalizerInfo.cpp
  RubiniusAsmPrinter.cpp
  RubiniusFrameLowering.cpp
  RubiniusInstrInfo.cpp
  RubiniusISelDAGToDAG.cpp
  RubiniusISelLowering.cpp
  RubiniusMCInstLower.cpp
  RubiniusRegisterInfo.cpp
  RubiniusSelectionDAGInfo.cpp
  RubiniusSubtarget.cpp
  RubiniusTargetLoweringObjectFile.cpp
  RubiniusTargetMachine.cpp
  BTFDebug.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  RubiniusDesc
  RubiniusInfo
  CodeGen
  CodeGenTypes
  Core
  GlobalISel
  IPO
  MC
  Passes
  Scalar
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils

  ADD_TO_COMPONENT
  Rubinius
  )

add_subdirectory(AsmParser)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
