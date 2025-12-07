#include "mlir/Dialect/Rubinius/Core/Transforms/Passes.h"

namespace mlir::rubinius::core {

#define GEN_PASS_DEF_RUBINIUSCORECANONICALIZE
#define GEN_PASS_REGISTRATION
#include "mlir/Dialect/Rubinius/Core/Transforms/Passes.cpp.inc"

} // namespace mlir::rubinius::core
