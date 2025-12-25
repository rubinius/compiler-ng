#include "RubiniusTargetMachine.h"
#include "Rubinius.h"
#include "RubiniusTargetLoweringObjectFile.h"
#include "RubiniusTargetTransformInfo.h"
#include "MCTargetDesc/RubiniusMCAsmInfo.h"
#include "TargetInfo/RubiniusTargetInfo.h"
#include "llvm/CodeGen/GlobalISel/IRTranslator.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelect.h"
#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/CodeGen/GlobalISel/RegBankSelect.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/PassManager.h"
#include "llvm/InitializePasses.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Utils/SimplifyCFGOptions.h"
#include <optional>
using namespace llvm;

static cl::
opt<bool> DisableMIPeephole("disable-rbx-peephole", cl::Hidden,
                            cl::desc("Disable machine peepholes for Rubinius"));

static cl::opt<bool>
    DisableCheckUnreachable("rbx-disable-trap-unreachable", cl::Hidden,
                            cl::desc("Disable Trap Unreachable for Rubinius"));

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRubiniusTarget() {
  // Register the target.
  RegisterTargetMachine<RubiniusTargetMachine> X(getTheRubiniusleTarget());
  RegisterTargetMachine<RubiniusTargetMachine> Y(getTheRubiniusbeTarget());
  RegisterTargetMachine<RubiniusTargetMachine> Z(getTheRubiniusTarget());

  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeGlobalISel(PR);
  initializeRubiniusAsmPrinterPass(PR);
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::PIC_);
}

RubiniusTargetMachine::RubiniusTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   std::optional<Reloc::Model> RM,
                                   std::optional<CodeModel::Model> CM,
                                   CodeGenOptLevel OL, bool JIT)
    : CodeGenTargetMachineImpl(T, TT.computeDataLayout(), TT, CPU, FS, Options,
                               getEffectiveRelocModel(RM),
                               getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<RubiniusTargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  if (!DisableCheckUnreachable) {
    this->Options.TrapUnreachable = true;
    this->Options.NoTrapAfterNoreturn = true;
  }

  initAsmInfo();

  RubiniusMCAsmInfo *MAI =
      static_cast<RubiniusMCAsmInfo *>(const_cast<MCAsmInfo *>(AsmInfo.get()));
  MAI->setDwarfUsesRelocationsAcrossSections(!Subtarget.getUseDwarfRIS());
}

namespace {
// Rubinius Code Generator Pass Configuration Options.
class RubiniusPassConfig : public TargetPassConfig {
public:
  RubiniusPassConfig(RubiniusTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  RubiniusTargetMachine &getRubiniusTargetMachine() const {
    return getTM<RubiniusTargetMachine>();
  }

  void addIRPasses() override;
  bool addInstSelector() override;
  void addMachineSSAOptimization() override;
  void addPreEmitPass() override;

  bool addIRTranslator() override;
  bool addLegalizeMachineIR() override;
  bool addRegBankSelect() override;
  bool addGlobalInstructionSelect() override;
};
}

TargetPassConfig *RubiniusTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new RubiniusPassConfig(*this, PM);
}

void RubiniusTargetMachine::registerPassBuilderCallbacks(PassBuilder &PB) {
// TODO: Rubinius
// #define GET_PASS_REGISTRY "RubiniusPassRegistry.def"
// #include "llvm/Passes/TargetPassRegistry.inc"

  PB.registerPipelineStartEPCallback(
      [=](ModulePassManager &MPM, OptimizationLevel) {
        FunctionPassManager FPM;
        MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
      });
  PB.registerPeepholeEPCallback([=](FunctionPassManager &FPM,
                                    OptimizationLevel Level) {
    FPM.addPass(SimplifyCFGPass(SimplifyCFGOptions().hoistCommonInsts(true)));
  });
}

void RubiniusPassConfig::addIRPasses() {
  addPass(createAtomicExpandLegacyPass());

  TargetPassConfig::addIRPasses();
}

TargetTransformInfo
RubiniusTargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(std::make_unique<RubiniusTTIImpl>(this, F));
}

// Install an instruction selector pass using
// the ISelDag to gen Rubinius code.
bool RubiniusPassConfig::addInstSelector() {

  return false;
}

void RubiniusPassConfig::addMachineSSAOptimization() {
  // The default implementation must be called first as we want eRubinius
  // Peephole ran at last.
  TargetPassConfig::addMachineSSAOptimization();
}

void RubiniusPassConfig::addPreEmitPass() {
}

bool RubiniusPassConfig::addIRTranslator() {
  addPass(new IRTranslator());
  return false;
}

bool RubiniusPassConfig::addLegalizeMachineIR() {
  addPass(new Legalizer());
  return false;
}

bool RubiniusPassConfig::addRegBankSelect() {
  addPass(new RegBankSelect());
  return false;
}

bool RubiniusPassConfig::addGlobalInstructionSelect() {
  addPass(new InstructionSelect(getOptLevel()));
  return false;
}
