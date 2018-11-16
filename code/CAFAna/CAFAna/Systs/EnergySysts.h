#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

namespace ana
{
  /// 1% systematic on muon energy for energy deposition in liquid argon
  /// 100% correlated between near and far detectors for those ND events that stop in the LAr
  class eScaleMuLArSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleMuLAr";}
    std::string LatexName() const override {return "Muon Energy Scale LAr";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Elep_reco);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoLepEnNumu);

      const double scale = 1 + .01*sigma;
 
      // Checks if ND
      if(!sr->dune.isFD){
	// Select only CC muon neutrino events that stop in LAr
        if(abs(sr->dune.nuPDG) == 14 && sr->dune.isCC == 1 && sr->dune.muon_contained == 1){
          sr->dune.Ev_reco   = sr->dune.Ev_reco * ( (1 - sr->dune.Y) * scale + sr->dune.Y );
	  sr->dune.Elep_reco = sr->dune.Elep_reco * scale;
        }
	else { }
      }
      // Otherwise is FD
      else {
	if(sr->dune.isCC && abs(sr->dune.nuPDG) == 14){
	  sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu *( (1 - sr->dune.Y) * scale + sr->dune.Y);
	  sr->dune.RecoLepEnNumu = sr->dune.RecoLepEnNumu * scale;
	}
      }
    }
  };

  static const eScaleMuLArSyst keScaleMuLArSyst;

  /// 1% systematics on muon energy for those tracks that are measured by the magnetic field
  // Uncorrelated between ND and FD
  // This is a temporary solution - need some momentum dependent function
  class EnergyScaleMuSystND: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleMuND";}
    std::string LatexName() const override {return "Muon Energy Scale Near Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Elep_reco);

      const double scale = 1 + .01*sigma;

      // Is a numu CC and stops somewhere in the ND
      if(!sr->dune.isFD && abs(sr->dune.nuPDG)==14 && sr->dune.isCC==1 && (sr->dune.muon_contained==1 || sr->dune.muon_tracker==1 || sr->dune.muon_ecal==1)){ 
	sr->dune.Ev_reco = sr->dune.Ev_reco * ( (1 - sr->dune.Y) * scale + sr->dune.Y );
	sr->dune.Elep_reco = sr->dune.Elep_reco * scale;
      } 
    }
  };

  static const EnergyScaleMuSystND kEnergyScaleMuSystND;

  /// 2% energy scale systematic on electron energy
  /// 100% correlated between near and far detectors
  class EnergyScaleESyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleE";}
    std::string LatexName() const override {return "Electron Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);

      const double scale = 1 + .02*sigma;

      // Checks if ND
      if(!sr->dune.isFD){
        if(abs(sr->dune.nuPDG) == 12 && sr->dune.isCC){
          double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
          sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
        }
      }
      // Otherwise is FD
      else {
        if(sr->dune.isCC && abs(sr->dune.nuPDG) == 12){
          double Y = sr->dune.Y;
          sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
          sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * (1 - Y) * scale + (sr->dune.Ev_reco_nue * Y);
        }
      }
    }
  };

  static const EnergyScaleESyst kEnergyScaleESyst;


  /// 2% muon energy scale syst, applies to FD only
  class EnergyScaleMuSystFD: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleMuFD";}
    std::string LatexName() const override {return "Muon Energy Scale Far Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_numu);

      const double scale = 1 + .02*sigma;

      if(sr->dune.isFD){
	if(sr->dune.isCC && abs(sr->dune.nuPDG) == 14){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
	  sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (1 - Y) * scale + (sr->dune.Ev_reco_numu * Y);
	}
      }
    }
  };

  static const EnergyScaleMuSystFD kEnergyScaleMuSystFD;


  /// 2% electron energy scale systematic in FD only
  class EnergyScaleESystFD: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleEFD";}
    std::string LatexName() const override {return "Electron Energy Scale Far Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);

      const double scale = 1 + .02*sigma;

      if(sr->dune.isFD){
	if(sr->dune.isCC && abs(sr->dune.nuPDG) == 12){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
	  sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * (1 - Y) * scale + (sr->dune.Ev_reco_nue * Y);
	}
      }
    }
  };

  static const EnergyScaleESystFD kEnergyScaleESystFD;


  /// 2% electron energy scale systematic in ND only
  class EnergyScaleESystND: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleEND";}
    std::string LatexName() const override {return "Electron Energy Scale Near Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);

      const double scale = 1 + .02*sigma;

      if(!sr->dune.isFD && abs(sr->dune.nuPDG) == 12 && sr->dune.isCC){
	double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev); 
	sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
      }
    }
  };

  static const EnergyScaleESystND kEnergyScaleESystND;


  /// Energy scale systematics for hadronic final state particles
  // Assume 25% of the neutron energy is visible - this is fairly crude and should be changed later
  // Neutron energy scale
  class EnergyScaleNSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleN";}
    std::string LatexName() const override {return "Neutron Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      // TEMPORARY FIX: CHANGE BACK AFTER CAFs HAVE BEEN RERUN
      const double scale = 1 + .25*sigma;
      double fracN = 0;
      double fracNY = 0;
      if(sr->dune.isFD) {
        fracN = ((sr->dune.eN*.25) / sr->dune.Ev);
	fracNY = ((sr->dune.eN*.25) / (sr->dune.Ev*sr->dune.Y));
      }
      else {
        fracN = ((sr->dune.eN*.25) / (sr->dune.Ev*1000));
	fracNY = ((sr->dune.eN*.25) / (sr->dune.Ev*sr->dune.Y*1000));
      }
      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracN * scale + (1 - fracN));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracN * scale + (1 - fracN));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracN * scale + (1 - fracN));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracNY * scale + (1 - fracNY));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracNY * scale + (1 - fracNY));
    }
  };

  static const EnergyScaleNSyst kEnergyScaleNSyst;  

  // Proton energy scale
  class EnergyScalePSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleP";}
    std::string LatexName() const override {return "Proton Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      const double scale = 1 + .1 * sigma;
      double fracP = 0;
      double fracPY = 0;
      // TEMPORARY FIX: CHANGE BACK AFTER CAFs HAVE BEEN RERUN
      if(sr->dune.isFD) {
	fracP = (sr->dune.eP / sr->dune.Ev); 
	fracPY = (sr->dune.eP / (sr->dune.Ev * sr->dune.Y));
      }
      else {
	fracP = (sr->dune.eP / (sr->dune.Ev*1000)); 
	fracPY = (sr->dune.eP / (sr->dune.Ev*sr->dune.Y*1000));
      }

      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracP * scale + (1 - fracP));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracP * scale + (1 - fracP));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracP * scale + (1 - fracP));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPY * scale + (1 - fracPY));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPY * scale + (1 - fracPY));
    }
  };

  static const EnergyScalePSyst kEnergyScalePSyst;  

  // Pi plus energy scale
  class EnergyScalePipSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScalePip";}
    std::string LatexName() const override {return "Pi plus Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      const double scale = 1 + .05 * sigma;
      double fracPip = 0;
      double fracPipY = 0;
      // TEMPORARY FIX: CHANGE BACK AFTER CAFs HAVE BEEN RERUN
      if(sr->dune.isFD) {
	fracPip = (sr->dune.ePip / sr->dune.Ev);
	fracPipY = (sr->dune.ePip / (sr->dune.Ev*sr->dune.Y));
      }
      else {
	fracPip = (sr->dune.ePip / (sr->dune.Ev*1000));
	fracPipY = (sr->dune.ePip / (sr->dune.Ev*sr->dune.Y*1000));
      }
      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracPip * scale + (1 - fracPip));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracPip * scale + (1 - fracPip));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracPip * scale + (1 - fracPip));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPipY * scale + (1 - fracPipY));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPipY * scale + (1 - fracPipY));
    }
  };

  static const EnergyScalePipSyst kEnergyScalePipSyst;

  // Pi minus energy scale
  class EnergyScalePimSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScalePim";}
    std::string LatexName() const override {return "Pi minus Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      const double scale = 1 + .05 * sigma;
      double fracPim = 0;
      double fracPimY = 0;
      // TEMPORARY FIX: CHANGE BACK AFTER CAFs HAVE BEEN RERUN
      if(sr->dune.isFD) {
	fracPim = (sr->dune.ePim / sr->dune.Ev); 
	fracPimY = (sr->dune.ePim / (sr->dune.Ev*sr->dune.Y));
      }
      else {
	fracPim = (sr->dune.ePim / (sr->dune.Ev*1000)); 
	fracPimY = (sr->dune.ePim / (sr->dune.Ev*sr->dune.Y*1000));
      }
      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracPim * scale + (1 - fracPim));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracPim * scale + (1 - fracPim));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracPim * scale + (1 - fracPim));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPimY * scale + (1 - fracPimY));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPimY * scale + (1 - fracPimY));
    }
  };

  static const EnergyScalePimSyst kEnergyScalePimSyst;

  // Pi0 energy scale
  class EnergyScalePi0Syst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScalePi0";}
    std::string LatexName() const override {return "Pi0 Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      const double scale = 1 + .1 * sigma;
      double fracPi0 = 0;
      double fracPi0Y = 0;
      // TEMPORARY FIX: CHANGE BACK AFTER CAFs HAVE BEEN RERUN
      if(sr->dune.isFD) {
	fracPi0 = (sr->dune.ePi0 / sr->dune.Ev);
	fracPi0Y = (sr->dune.ePi0 / (sr->dune.Ev*sr->dune.Y));
      }
      else {
	fracPi0 = (sr->dune.ePi0 / (sr->dune.Ev*1000));
	fracPi0Y = (sr->dune.ePi0 / (sr->dune.Ev*sr->dune.Y*1000));
      }
      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracPi0 * scale + (1 - fracPi0));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracPi0 * scale + (1 - fracPi0));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracPi0 * scale + (1 - fracPi0));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPi0Y * scale + (1 - fracPi0Y));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPi0Y * scale + (1 - fracPi0Y));
    }
  };

  static const EnergyScalePi0Syst kEnergyScalePi0Syst;

  // Adjustable energy scale systematics which is correlated between all hadronic species
  class HadronCorr: public ISyst
  {
  public:
    std::string ShortName() const override {return "HadronCorr";}
    std::string LatexName() const override {return "Hadron Correlated energy";}

    HadronCorr(double Nfrac, double Pfrac, double Pimfrac, double Pipfrac, double Pi0frac)
      {
	fNfrac = Nfrac;
	fPfrac = Pfrac;
	fPimfrac = Pimfrac;
	fPipfrac = Pipfrac;
	fPi0frac = Pi0frac;
      }

    HadronCorr()
      {
	fNfrac = .25;
	fPfrac = .1;
	fPimfrac = .05;
	fPipfrac = .05;
	fPi0frac = .1;
      }

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override;


    void SetNFraction(const double Nfrac) { fNfrac = Nfrac; }
    void SetPFraction(const double Pfrac) { fPfrac = Pfrac; }
    void SetPimFraction(const double Pimfrac) { fPimfrac = Pimfrac; }
    void SetPipFraction(const double Pipfrac) { fPipfrac = Pipfrac; }
    void SetPi0Fraction(const double Pi0frac) { fPi0frac = Pi0frac;}

    void SetAllFraction(const double frac) 
    {
      fNfrac = frac;
      fPfrac = frac;
      fPimfrac = frac;
      fPipfrac = frac;
      fPi0frac = frac;
    }

    // Prints out the selected fractions for each particle
    void PrintFractions () {
      std::cout<<"Neutron, Proton, Pi plus, Pi minus, Pi zero "<<fNfrac<<", "<<fPfrac<<", "<<fPipfrac<<", "<<fPimfrac<<", "<<fPi0frac<<std::endl;
    }

  private:
    // For each species define the fraction of its energy that should be correlated
    double fNfrac;
    double fPfrac;
    double fPimfrac;
    double fPipfrac;
    double fPi0frac;
  };
}
