add_mlir_library(MLIRRubiniusRubyTransforms
  RubiniusRubyTransforms.cpp

  LINK_LIBS PUBLIC
    MLIRRubiniusRuby
    MLIRIR
    MLIRPass
    MLIRTransforms
)
