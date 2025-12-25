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

add_dependencies(obj.MLIRRubiniusCoreTransforms
  MLIRRubiniusCoreTransformsIncGen
)
