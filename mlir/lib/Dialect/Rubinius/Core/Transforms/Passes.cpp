#include "mlir/Dialect/Rubinius/Core/Transforms/Passes.h"

namespace mlir::rubinius::core {

#define GEN_PASS_REGISTRATION
#include "mlir/Dialect/Rubinius/Core/Transforms/Passes.h.inc"

} // namespace mlir::rubinius::core
