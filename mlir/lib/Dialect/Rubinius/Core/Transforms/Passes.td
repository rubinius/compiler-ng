#ifndef MLIR_DIALECT_RUBINIUS_CORE_TRANSFORMS_PASSES_TD
#define MLIR_DIALECT_RUBINIUS_CORE_TRANSFORMS_PASSES_TD

include "mlir/Pass/PassBase.td"
include "mlir/Dialect/Rubinius/Core/IR/RubiniusCoreBase.td"

def RubiniusCoreCanonicalize
    : Pass<"rubinius-core-canonicalize", "mlir::Operation"> {
  let summary = "Canonicalize Rubinius core operations";
  let description = [{
    Placeholder pass for the Rubinius Core dialect. Currently performs no
    transformations; it exists to exercise the pass registration and pipeline.
  }];
}

#endif // MLIR_DIALECT_RUBINIUS_CORE_TRANSFORMS_PASSES_TD
