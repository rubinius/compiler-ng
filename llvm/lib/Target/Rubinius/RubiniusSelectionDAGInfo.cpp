#include "RubiniusSelectionDAGInfo.h"
#include "RubiniusTargetMachine.h"
#include "llvm/CodeGen/SelectionDAG.h"

#define GET_SDNODE_DESC
#include "RubiniusGenSDNodeInfo.inc"

using namespace llvm;

#define DEBUG_TYPE "rbx-selectiondag-info"

RubiniusSelectionDAGInfo::RubiniusSelectionDAGInfo()
    : SelectionDAGGenTargetInfo(RubiniusGenSDNodeInfo) {}

SDValue RubiniusSelectionDAGInfo::EmitTargetCodeForMemcpy(
    SelectionDAG &DAG, const SDLoc &dl, SDValue Chain, SDValue Dst, SDValue Src,
    SDValue Size, Align Alignment, bool isVolatile, bool AlwaysInline,
    MachinePointerInfo DstPtrInfo, MachinePointerInfo SrcPtrInfo) const {
  // Requires the copy size to be a constant.
  ConstantSDNode *ConstantSize = dyn_cast<ConstantSDNode>(Size);
  if (!ConstantSize)
    return SDValue();

  unsigned CopyLen = ConstantSize->getZExtValue();
  unsigned StoresNumEstimate = alignTo(CopyLen, Alignment) >> Log2(Alignment);
  // Impose the same copy length limit as MaxStoresPerMemcpy.
  if (StoresNumEstimate > getCommonMaxStoresPerMemFunc())
    return SDValue();

  return DAG.getNode(RubiniusISD::MEMCPY, dl, MVT::Other, Chain, Dst, Src,
                     DAG.getConstant(CopyLen, dl, MVT::i64),
                     DAG.getConstant(Alignment.value(), dl, MVT::i64));
}
