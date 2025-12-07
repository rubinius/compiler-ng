add_mlir_dialect_library(MLIRRubiniusCore
  RubiniusCoreDialect.cpp

  ADDITIONAL_HEADER_DIRS
    ${MLIR_MAIN_INCLUDE_DIR}/mlir/Dialect/Rubinius

  DEPENDS
    MLIRRubiniusCoreOpsIncGen

  LINK_LIBS PUBLIC
    MLIRIR
)
