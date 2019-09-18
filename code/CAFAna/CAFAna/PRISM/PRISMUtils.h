#pragma once

#include "CAFAna/Core/Var.h"

#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "CAFAna/PRISM/PredictionPRISM.h"

#include "StandardRecord/StandardRecord.h"

#include "TFile.h"
#include "TH1D.h"

#include <memory>
#include <vector>

namespace ana {

class FVMassCorrection {
  size_t fOverflow;

public:
  TH1D *fvmasscor;

  FVMassCorrection();
  double GetWeight(double vtx_x_cm);
};

inline double FD_ND_FVRatio(double x_slice_cm) {
  double NDSliceFV = x_slice_cm * 200 * (350 - 50);
  double FDFV = 620 * 1100 * (1244 - 50);

  return FDFV / NDSliceFV;
}

extern const ana::Var kMassCorrection;

struct PRISMStateBlob {
  std::unique_ptr<PredictionPRISM> PRISM;
  std::unique_ptr<PredictionInterp> NDMatchInterp;
  std::unique_ptr<PredictionInterp> FDMatchInterp;
  std::unique_ptr<PredictionNoExtrap> FarDet;
};

PRISMStateBlob LoadPRISMState(TFile &f, std::string const &varname,
                              bool IsRHC = false);

} // namespace ana
