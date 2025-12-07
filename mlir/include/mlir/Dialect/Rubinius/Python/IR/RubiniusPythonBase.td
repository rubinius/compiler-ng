#ifndef MLIR_DIALECT_RUBINIUS_PYTHON_IR_RUBINIUSPYTHONBASE_TD
#define MLIR_DIALECT_RUBINIUS_PYTHON_IR_RUBINIUSPYTHONBASE_TD

include "mlir/IR/OpBase.td"

def RubiniusPython_Dialect : Dialect {
  let name = "rubinius_python";
  let cppNamespace = "::mlir::rubinius::python";
}

class RubiniusPython_Op<string mnemonic, list<Trait> traits = []> :
    Op<RubiniusPython_Dialect, mnemonic, traits>;

#endif // MLIR_DIALECT_RUBINIUS_PYTHON_IR_RUBINIUSPYTHONBASE_TD
