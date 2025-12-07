add_mlir_library(MLIRRubiniusPythonTransforms
  RubiniusPythonTransforms.cpp

  LINK_LIBS PUBLIC
    MLIRRubiniusPython
    MLIRPass
    MLIRIR
    MLIRTransforms
)
