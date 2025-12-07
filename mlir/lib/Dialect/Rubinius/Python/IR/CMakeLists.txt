add_mlir_dialect_library(MLIRRubiniusPython
  RubiniusPythonDialect.cpp

  ADDITIONAL_HEADER_DIRS
    ${MLIR_MAIN_INCLUDE_DIR}/mlir/Dialect/Rubinius

  DEPENDS
    MLIRRubiniusPythonOpsIncGen

  LINK_LIBS PUBLIC
    MLIRIR
    MLIRRubiniusCore
)
