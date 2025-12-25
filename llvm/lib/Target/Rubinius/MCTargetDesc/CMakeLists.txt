add_llvm_component_library(LLVMRubiniusDesc
  RubiniusMCTargetDesc.cpp
  RubiniusAsmBackend.cpp
  RubiniusInstPrinter.cpp
  RubiniusMCCodeEmitter.cpp
  RubiniusELFObjectWriter.cpp

  LINK_COMPONENTS
  MC
  RubiniusInfo
  Support
  TargetParser

  ADD_TO_COMPONENT
  Rubinius
  )
