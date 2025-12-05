add_llvm_library(LLVMRubiniusInfo
  RubiniusMCTargetDesc.cpp
  RubiniusMCAsmInfo.cpp

  LINK_COMPONENTS
  MC
  Support
)
