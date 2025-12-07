add_llvm_component_library(LLVMRubiniusInfo
  RubiniusTargetInfo.cpp

  LINK_COMPONENTS
    Support

  ADD_TO_COMPONENT
    Rubinius
)
