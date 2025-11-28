#!/bin/bash

set -x

"$1"/bin/mlir-opt --version
"$1"/bin/mlir-translate --version
"$1"/bin/llc --version
"$1"/bin/llvm-as --version
"$1"/bin/llvm-dis --version
cat > hello.mlir <<'EOF'
module {
  func.func @add(%a: i32, %b: i32) -> i32 {
    %c = arith.addi %a, %b : i32
    return %c : i32
  }
}
EOF
"$1"/bin/mlir-opt hello.mlir \
  -convert-func-to-llvm \
  -convert-arith-to-llvm \
  -reconcile-unrealized-casts \
| "$1"/bin/mlir-translate --mlir-to-llvmir \
| "$1"/bin/llc -filetype=asm -o - > /dev/null
