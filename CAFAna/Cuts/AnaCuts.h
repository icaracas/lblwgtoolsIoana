#pragma once

#include <cassert>
#include "CAFAna/Core/Cut.h"
#include "StandardRecord/StandardRecord.h"

namespace ana
{

  const Cut kPassFD_CVN_NUE({},
                  [](const caf::StandardRecord* sr)
                  {
                    return (sr->cvnnue > 0.85 && sr->cvnnumu < 0.5);
                  });

  const Cut kPassFD_CVN_NUMU({},
                  [](const caf::StandardRecord* sr)
                  {
                    return (sr->cvnnumu > 0.5 && sr->cvnnue < 0.85);
                  });

  const Cut kPassND_FHC_NUMU({},
                  [](const caf::StandardRecord* sr)
                  {
                    return (
			    sr->reco_numu && 
			    (sr->muon_contained || sr->muon_tracker) &&
			    sr->reco_q == -1 && 
			    sr->Ehad_veto<30);
		      });

    const Cut kPassND_RHC_NUMU({},
                  [](const caf::StandardRecord* sr)
                  {
                    return (
			    sr->reco_numu && 
			    (sr->muon_contained || sr->muon_tracker) &&
			    sr->reco_q == +1 && 
			    sr->Ehad_veto<30);
                  });


}
