add_llvm_component_library(LLVMRubiniusDesc
  RubiniusMCTargetDesc.cpp
  RubiniusMCAsmInfo.cpp
  RubiniusMCCodeEmitter.cpp

  DEPENDS
    RubiniusCommonTableGen

  LINK_COMPONENTS
    MC
    Support

  ADD_TO_COMPONENT
    Rubinius
)
