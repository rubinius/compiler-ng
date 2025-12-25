#include "RubiniusSubtarget.h"
#include "Rubinius.h"
#include "RubiniusTargetMachine.h"
#include "GISel/RubiniusCallLowering.h"
#include "GISel/RubiniusLegalizerInfo.h"
#include "GISel/RubiniusRegisterBankInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/TargetParser/Host.h"

using namespace llvm;

#define DEBUG_TYPE "rbx-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "RubiniusGenSubtargetInfo.inc"

static cl::opt<bool> Disable_ldsx("disable-ldsx", cl::Hidden, cl::init(false),
  cl::desc("Disable ldsx insns"));
static cl::opt<bool> Disable_movsx("disable-movsx", cl::Hidden, cl::init(false),
  cl::desc("Disable movsx insns"));
static cl::opt<bool> Disable_bswap("disable-bswap", cl::Hidden, cl::init(false),
  cl::desc("Disable bswap insns"));
static cl::opt<bool> Disable_sdiv_smod("disable-sdiv-smod", cl::Hidden,
  cl::init(false), cl::desc("Disable sdiv/smod insns"));
static cl::opt<bool> Disable_gotol("disable-gotol", cl::Hidden, cl::init(false),
  cl::desc("Disable gotol insn"));
static cl::opt<bool>
    Disable_StoreImm("disable-storeimm", cl::Hidden, cl::init(false),
                     cl::desc("Disable Rubinius_ST (immediate store) insn"));
static cl::opt<bool> Disable_load_acq_store_rel(
    "disable-load-acq-store-rel", cl::Hidden, cl::init(false),
    cl::desc("Disable load-acquire and store-release insns"));
static cl::opt<bool> Disable_gotox("disable-gotox", cl::Hidden, cl::init(false),
                                   cl::desc("Disable gotox insn"));

void RubiniusSubtarget::anchor() {}

RubiniusSubtarget &RubiniusSubtarget::initializeSubtargetDependencies(StringRef CPU,
                                                            StringRef FS) {
  initializeEnvironment();
  initSubtargetFeatures(CPU, FS);
  ParseSubtargetFeatures(CPU, /*TuneCPU*/ CPU, FS);
  return *this;
}

void RubiniusSubtarget::initializeEnvironment() {
  HasJmpExt = false;
  HasJmp32 = false;
  HasAlu32 = false;
  UseDwarfRIS = false;
  HasLdsx = false;
  HasMovsx = false;
  HasBswap = false;
  HasSdivSmod = false;
  HasGotol = false;
  HasStoreImm = false;
  HasLoadAcqStoreRel = false;
  HasGotox = false;
  AllowsMisalignedMemAccess = false;
  AllowBuiltinCalls = false;
}

void RubiniusSubtarget::initSubtargetFeatures(StringRef CPU, StringRef FS) {
  // TODO: Rubinius
  CPU = "rbx";
}

RubiniusSubtarget::RubiniusSubtarget(const Triple &TT, const std::string &CPU,
                           const std::string &FS, const TargetMachine &TM)
    : RubiniusGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)), FrameLowering(*this),
      TLInfo(TM, *this) {
  IsLittleEndian = TT.isLittleEndian();

  CallLoweringInfo.reset(new RubiniusCallLowering(*getTargetLowering()));
  Legalizer.reset(new RubiniusLegalizerInfo(*this));
  auto *RBI = new RubiniusRegisterBankInfo(*getRegisterInfo());
  RegBankInfo.reset(RBI);

  InstSelector.reset(createRubiniusInstructionSelector(
      *static_cast<const RubiniusTargetMachine *>(&TM), *this, *RBI));
}

const CallLowering *RubiniusSubtarget::getCallLowering() const {
  return CallLoweringInfo.get();
}

InstructionSelector *RubiniusSubtarget::getInstructionSelector() const {
  return InstSelector.get();
}

const LegalizerInfo *RubiniusSubtarget::getLegalizerInfo() const {
  return Legalizer.get();
}

const RegisterBankInfo *RubiniusSubtarget::getRegBankInfo() const {
  return RegBankInfo.get();
}
