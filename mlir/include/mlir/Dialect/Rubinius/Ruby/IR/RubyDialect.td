include "mlir/IR/DialectBase.td"
include "mlir/IR/OpBase.td"

def Ruby_Dialect : Dialect {
  let name = "ruby";
  let cppNamespace = "::mlir::ruby";
}

def Ruby_AddOp : Op<Ruby_Dialect, "addi", [Pure]> {
  let summary = "add two i32";
  let arguments = (ins I32:$lhs, I32:$rhs);
  let results   = (outs I32:$res);

  let assemblyFormat = "$lhs `,` $rhs `:` type($res)";
}
