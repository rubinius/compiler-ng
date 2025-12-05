class RubiniusReg<string n, bits<5> num> : Register<n> {
  let HWEncoding = num;
}

def R0 : RubiniusReg<"r0", 0>;
def R1 : RubiniusReg<"r1", 1>;
def R2 : RubiniusReg<"r2", 2>;
def R3 : RubiniusReg<"r3", 3>;

def GPR : RegisterClass<"Rubinius", [i32], 32, (add R0, R1, R2, R3)>;
