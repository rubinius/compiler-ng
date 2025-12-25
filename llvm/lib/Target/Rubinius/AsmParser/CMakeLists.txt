add_llvm_component_library(LLVMRubiniusAsmParser
  RubiniusAsmParser.cpp

  LINK_COMPONENTS
  MC
  MCParser
  RubiniusDesc
  RubiniusInfo
  Support

  ADD_TO_COMPONENT
  Rubinius
  )
