add_mlir_dialect_library(MLIRRubiniusRuby
  RubiniusRubyDialect.cpp

  ADDITIONAL_HEADER_DIRS
    ${MLIR_MAIN_INCLUDE_DIR}/mlir/Dialect/Rubinius

  DEPENDS
    MLIRRubiniusRubyOpsIncGen

  LINK_LIBS PUBLIC
    MLIRIR
    MLIRRubiniusCore
)
