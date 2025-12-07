#pragma once

#include "mlir/Pass/Pass.h"

namespace mlir::rubinius::core {

#define GEN_PASS_DECL
#include "mlir/Dialect/Rubinius/Core/Transforms/Passes.h.inc"

} // namespace mlir::rubinius::core
