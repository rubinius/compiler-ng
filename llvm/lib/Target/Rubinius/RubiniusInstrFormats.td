class RubiniusOpClass<bits<3> val> {
  bits<3> Value = val;
}

def Rubinius_LD    : RubiniusOpClass<0x0>;
def Rubinius_LDX   : RubiniusOpClass<0x1>;
def Rubinius_ST    : RubiniusOpClass<0x2>;
def Rubinius_STX   : RubiniusOpClass<0x3>;
def Rubinius_ALU   : RubiniusOpClass<0x4>;
def Rubinius_JMP   : RubiniusOpClass<0x5>;
def Rubinius_JMP32 : RubiniusOpClass<0x6>;
def Rubinius_ALU64 : RubiniusOpClass<0x7>;

class RubiniusSrcType<bits<1> val> {
  bits<1> Value = val;
}

def Rubinius_K : RubiniusSrcType<0x0>;
def Rubinius_X : RubiniusSrcType<0x1>;

class RubiniusArithOp<bits<4> val> {
  bits<4> Value = val;
}

def Rubinius_ADD  : RubiniusArithOp<0x0>;
def Rubinius_SUB  : RubiniusArithOp<0x1>;
def Rubinius_MUL  : RubiniusArithOp<0x2>;
def Rubinius_DIV  : RubiniusArithOp<0x3>;
def Rubinius_OR   : RubiniusArithOp<0x4>;
def Rubinius_AND  : RubiniusArithOp<0x5>;
def Rubinius_LSH  : RubiniusArithOp<0x6>;
def Rubinius_RSH  : RubiniusArithOp<0x7>;
def Rubinius_NEG  : RubiniusArithOp<0x8>;
def Rubinius_MOD  : RubiniusArithOp<0x9>;
def Rubinius_XOR  : RubiniusArithOp<0xa>;
def Rubinius_MOV  : RubiniusArithOp<0xb>;
def Rubinius_ARSH : RubiniusArithOp<0xc>;
def Rubinius_END  : RubiniusArithOp<0xd>;

def Rubinius_XCHG    : RubiniusArithOp<0xe>;
def Rubinius_CMPXCHG : RubiniusArithOp<0xf>;

class RubiniusAtomicOp<bits<5> val> {
  bits<5> Value = val;
}

def Rubinius_LOAD_ACQ : RubiniusAtomicOp<0x10>;
def Rubinius_STORE_REL : RubiniusAtomicOp<0x11>;

class RubiniusEndDir<bits<1> val> {
  bits<1> Value = val;
}

def Rubinius_TO_LE : RubiniusSrcType<0x0>;
def Rubinius_TO_BE : RubiniusSrcType<0x1>;

class RubiniusJumpOp<bits<4> val> {
  bits<4> Value = val;
}

def Rubinius_JA   : RubiniusJumpOp<0x0>;
def Rubinius_JEQ  : RubiniusJumpOp<0x1>;
def Rubinius_JGT  : RubiniusJumpOp<0x2>;
def Rubinius_JGE  : RubiniusJumpOp<0x3>;
def Rubinius_JSET : RubiniusJumpOp<0x4>;
def Rubinius_JNE  : RubiniusJumpOp<0x5>;
def Rubinius_JSGT : RubiniusJumpOp<0x6>;
def Rubinius_JSGE : RubiniusJumpOp<0x7>;
def Rubinius_CALL : RubiniusJumpOp<0x8>;
def Rubinius_EXIT : RubiniusJumpOp<0x9>;
def Rubinius_JLT  : RubiniusJumpOp<0xa>;
def Rubinius_JLE  : RubiniusJumpOp<0xb>;
def Rubinius_JSLT : RubiniusJumpOp<0xc>;
def Rubinius_JSLE : RubiniusJumpOp<0xd>;
def Rubinius_JCOND : RubiniusJumpOp<0xe>;

class RubiniusWidthModifer<bits<2> val> {
  bits<2> Value = val;
}

def Rubinius_W  : RubiniusWidthModifer<0x0>;
def Rubinius_H  : RubiniusWidthModifer<0x1>;
def Rubinius_B  : RubiniusWidthModifer<0x2>;
def Rubinius_DW : RubiniusWidthModifer<0x3>;

class RubiniusModeModifer<bits<3> val> {
  bits<3> Value = val;
}

def Rubinius_IMM  : RubiniusModeModifer<0x0>;
def Rubinius_ABS  : RubiniusModeModifer<0x1>;
def Rubinius_IND  : RubiniusModeModifer<0x2>;
def Rubinius_MEM  : RubiniusModeModifer<0x3>;
def Rubinius_MEMSX  : RubiniusModeModifer<0x4>;
def Rubinius_ATOMIC : RubiniusModeModifer<0x6>;

class RubiniusAtomicFlag<bits<4> val> {
  bits<4> Value = val;
}

def Rubinius_FETCH : RubiniusAtomicFlag<0x1>;

class InstRubinius<dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  field bits<64> Inst;
  let Size = 8;

  let Namespace = "Rubinius";
  let DecoderNamespace = "Rubinius";

  RubiniusOpClass RubiniusClass;
  let Inst{58-56} = RubiniusClass.Value;

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
  : InstRubinius<outs, ins, asmstr, pattern> {
  let Inst{63-0} = 0;
  let isPseudo = 1;
}
