add_mlir_conversion_library(MLIRRubyToLLVM
  RubyToLLVM.cpp

  DEPENDS
    MLIRLLVMIR
    MLIRRubiniusRubyOpsIncGen

  LINK_LIBS PUBLIC
    MLIRRubiniusRuby
    MLIRLLVMIR
    MLIRIR
)
