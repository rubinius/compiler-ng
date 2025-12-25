#include "MCTargetDesc/RubiniusMCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCValue.h"
#include <memory>

using namespace llvm;

namespace {

class RubiniusELFObjectWriter final : public MCELFObjectTargetWriter {
public:
  explicit RubiniusELFObjectWriter(uint8_t OSABI)
      : MCELFObjectTargetWriter(/*Is64Bit=*/true,
                               /*OSABI=*/OSABI,
                               /*EMachine=*/ELF::EM_NONE,
                               /*HasRelocationAddend=*/true) {}

  unsigned getRelocType(const MCFixup & /*Fixup*/,
                        const MCValue & /*Target*/,
                        bool /*IsPCRel*/) const override {
    // Stub: target has no real relocations yet.
    return 0;
  }
};

} // namespace

std::unique_ptr<MCObjectTargetWriter>
llvm::createRubiniusELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<RubiniusELFObjectWriter>(OSABI);
}
