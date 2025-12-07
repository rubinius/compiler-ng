set(LLVM_TARGET_DEFINITIONS Passes.td)

mlir_tablegen(Passes.h.inc -gen-pass-decls -name RubiniusCore)
mlir_tablegen(Passes.cpp.inc -gen-pass-defs -name RubiniusCore)
add_public_tablegen_target(MLIRRubiniusCoreTransformsIncGen)

add_mlir_library(MLIRRubiniusCoreTransforms
  Passes.cpp
  RubiniusCoreTransforms.cpp

  LINK_LIBS PUBLIC
    MLIRRubiniusCore
    MLIRIR
    MLIRPass
    MLIRTransforms

  DEPENDS
    MLIRRubiniusCoreTransformsIncGen
)
