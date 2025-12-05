add_mlir_conversion_library(MLIRRubyToLLVM
  RubyToLLVM.cpp

  LINK_COMPONENTS
  Core

  LINK_LIBS PUBLIC
  MLIRIR
  MLIRLLVMIR
  MLIRRuby
)
