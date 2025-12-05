#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/Ruby/Ruby.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/DialectConversion.h"

using namespace mlir;
using namespace mlir::ruby;

namespace {
class AddOpLowering : public OpConversionPattern<Ruby_AddOp> {
public:
  using OpConversionPattern::OpConversionPattern;
  LogicalResult
  matchAndRewrite(Ruby_AddOp op, Ruby_AddOp::Adaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    auto llvmI32Ty = IntegerType::get(getContext(), 32);
    auto llvmType =
        LLVM::LLVMType::getInt32Ty(getContext()); // for LLVM dialect

    Value lhs = adaptor.getLhs();
    Value rhs = adaptor.getRhs();

    auto loc = op.getLoc();
    Value sum = rewriter.create<LLVM::AddOp>(loc, lhs, rhs);
    rewriter.replaceOp(op, sum);
    return success();
  }
};

struct ConvertRubyToLLVM
    : public PassWrapper<ConvertRubyToLLVM, OperationPass<ModuleOp>> {
  void runOnOperation() override {
    MLIRContext &ctx = getContext();
    LLVM::LLVMDialect *llvmDialect = ctx.getOrLoadDialect<LLVM::LLVMDialect>();
    (void)llvmDialect;

    RewritePatternSet patterns(&ctx);
    patterns.add<AddOpLowering>(&ctx);

    ConversionTarget target(ctx);
    target.addLegalDialect<LLVM::LLVMDialect>();
    target.addIllegalDialect<RubyDialect>();

    if (failed(applyPartialConversion(getOperation(), target,
                                      std::move(patterns)))) {
      signalPassFailure();
    }
  }
};
} // namespace

std::unique_ptr<Pass> createConvertRubyToLLVMPass() {
  return std::make_unique<ConvertRubyToLLVM>();
}

static PassRegistration<ConvertRubyToLLVM> Pass(
    "convert-ruby-to-llvm", "Lower ruby dialect to LLVM dialect");
