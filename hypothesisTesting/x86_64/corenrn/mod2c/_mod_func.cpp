#include <cstdio>
namespace coreneuron {
extern int nrnmpi_myid;
extern int nrn_nobanner_;
extern int
  _ALU_reg(void),
  _CaDynamics_DC0_reg(void),
  _Ca_HVA2_reg(void),
  _Ca_LVAst_reg(void),
  _ConductanceSource_reg(void),
  _CoreNEURONArtificialCell_reg(void),
  _DetAMPANMDA_reg(void),
  _DetGABAAB_reg(void),
  _GluSynapse_reg(void),
  _Ih_reg(void),
  _K_Pst_reg(void),
  _K_Tst_reg(void),
  _KdShu2007_reg(void),
  _Kv7_reg(void),
  _MembraneCurrentSource_reg(void),
  _NaTg_reg(void),
  _NaTg_axon_reg(void),
  _NaTg_persistent_axon_reg(void),
  _NaTg_persistent_somadend_reg(void),
  _NaTg_somadend_reg(void),
  _Nap_Et2_reg(void),
  _ProbAMPANMDA_EMS_reg(void),
  _ProbGABAAB_EMS_reg(void),
  _SK_E2_reg(void),
  _SKv3_1_reg(void),
  _SonataReportHelper_reg(void),
  _SonataReports_reg(void),
  _StochKv3_reg(void),
  _TTXDynamicsSwitch_reg(void),
  _exp2syn_reg(void),
  _expsyn_reg(void),
  _gap_reg(void),
  _hh_reg(void),
  _netstim_reg(void),
  _netstim_inhpoisson_reg(void),
  _passive_reg(void),
  _pattern_reg(void),
  _stim_reg(void),
  _svclmp_reg(void),
  _vecevent_reg(void);

void modl_reg() {
    if (!nrn_nobanner_ && nrnmpi_myid < 1) {
        fprintf(stderr, " Additional mechanisms from files\n");
        fprintf(stderr, " ALU.mod");
        fprintf(stderr, " CaDynamics_DC0.mod");
        fprintf(stderr, " Ca_HVA2.mod");
        fprintf(stderr, " Ca_LVAst.mod");
        fprintf(stderr, " ConductanceSource.mod");
        fprintf(stderr, " CoreNEURONArtificialCell.mod");
        fprintf(stderr, " DetAMPANMDA.mod");
        fprintf(stderr, " DetGABAAB.mod");
        fprintf(stderr, " GluSynapse.mod");
        fprintf(stderr, " Ih.mod");
        fprintf(stderr, " K_Pst.mod");
        fprintf(stderr, " K_Tst.mod");
        fprintf(stderr, " KdShu2007.mod");
        fprintf(stderr, " Kv7.mod");
        fprintf(stderr, " MembraneCurrentSource.mod");
        fprintf(stderr, " NaTg.mod");
        fprintf(stderr, " NaTg_axon.mod");
        fprintf(stderr, " NaTg_persistent_axon.mod");
        fprintf(stderr, " NaTg_persistent_somadend.mod");
        fprintf(stderr, " NaTg_somadend.mod");
        fprintf(stderr, " Nap_Et2.mod");
        fprintf(stderr, " ProbAMPANMDA_EMS.mod");
        fprintf(stderr, " ProbGABAAB_EMS.mod");
        fprintf(stderr, " SK_E2.mod");
        fprintf(stderr, " SKv3_1.mod");
        fprintf(stderr, " SonataReportHelper.mod");
        fprintf(stderr, " SonataReports.mod");
        fprintf(stderr, " StochKv3.mod");
        fprintf(stderr, " TTXDynamicsSwitch.mod");
        fprintf(stderr, " exp2syn.mod");
        fprintf(stderr, " expsyn.mod");
        fprintf(stderr, " gap.mod");
        fprintf(stderr, " hh.mod");
        fprintf(stderr, " netstim.mod");
        fprintf(stderr, " netstim_inhpoisson.mod");
        fprintf(stderr, " passive.mod");
        fprintf(stderr, " pattern.mod");
        fprintf(stderr, " stim.mod");
        fprintf(stderr, " svclmp.mod");
        fprintf(stderr, " vecevent.mod");
        fprintf(stderr, "\n\n");
    }
    _ALU_reg();
    _CaDynamics_DC0_reg();
    _Ca_HVA2_reg();
    _Ca_LVAst_reg();
    _ConductanceSource_reg();
    _CoreNEURONArtificialCell_reg();
    _DetAMPANMDA_reg();
    _DetGABAAB_reg();
    _GluSynapse_reg();
    _Ih_reg();
    _K_Pst_reg();
    _K_Tst_reg();
    _KdShu2007_reg();
    _Kv7_reg();
    _MembraneCurrentSource_reg();
    _NaTg_reg();
    _NaTg_axon_reg();
    _NaTg_persistent_axon_reg();
    _NaTg_persistent_somadend_reg();
    _NaTg_somadend_reg();
    _Nap_Et2_reg();
    _ProbAMPANMDA_EMS_reg();
    _ProbGABAAB_EMS_reg();
    _SK_E2_reg();
    _SKv3_1_reg();
    _SonataReportHelper_reg();
    _SonataReports_reg();
    _StochKv3_reg();
    _TTXDynamicsSwitch_reg();
    _exp2syn_reg();
    _expsyn_reg();
    _gap_reg();
    _hh_reg();
    _netstim_reg();
    _netstim_inhpoisson_reg();
    _passive_reg();
    _pattern_reg();
    _stim_reg();
    _svclmp_reg();
    _vecevent_reg();
}
} //namespace coreneuron
