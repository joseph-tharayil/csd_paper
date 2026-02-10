/*********************************************************
Model Name      : StochKv3
Filename        : StochKv3.mod
NMODL Version   : 7.7.0
Vectorized      : true
Threadsafe      : true
Created         : Sat Jan 31 15:01:57 2026
Simulator       : CoreNEURON
Backend         : C++ (api-compatibility)
NMODL Compiler  : 0.6 [791014679 2024-07-05 16:14:29 +0200]
*********************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <coreneuron/gpu/nrn_acc_manager.hpp>
#include <coreneuron/mechanism/mech/mod2c_core_thread.hpp>
#include <coreneuron/mechanism/register_mech.hpp>
#include <coreneuron/nrnconf.h>
#include <coreneuron/nrniv/nrniv_decl.h>
#include <coreneuron/sim/multicore.hpp>
#include <coreneuron/sim/scopmath/newton_thread.hpp>
#include <coreneuron/utils/ivocvect.hpp>
#include <coreneuron/utils/nrnoc_aux.hpp>
#include <coreneuron/utils/randoms/nrnran123.h>


namespace coreneuron {
    #ifndef NRN_PRCELLSTATE
    #define NRN_PRCELLSTATE 0
    #endif


    /** channel information */
    static const char *mechanism_info[] = {
        "7.7.0",
        "StochKv3",
        "gamma_StochKv3",
        "eta_StochKv3",
        "gkbar_StochKv3",
        "deterministic_StochKv3",
        0,
        "an_StochKv3",
        "bn_StochKv3",
        "al_StochKv3",
        "bl_StochKv3",
        "ik_StochKv3",
        "gk_StochKv3",
        "ninf_StochKv3",
        "ntau_StochKv3",
        "linf_StochKv3",
        "ltau_StochKv3",
        "N_StochKv3",
        "P_an_StochKv3",
        "P_bn_StochKv3",
        "P_al_StochKv3",
        "P_bl_StochKv3",
        0,
        "n_StochKv3",
        "l_StochKv3",
        0,
        "rng_StochKv3",
        0
    };


    /** all global variables */
    struct StochKv3_Store {
        int k_type{};
        double n0{};
        double l0{};
        int reset{};
        int mech_type{};
        double vmin{-120};
        double vmax{100};
        int slist1[2]{20, 19};
        int dlist1[2]{25, 24};
        double usetable{1};
        double tmin_trates{};
        double mfac_trates{};
        double t_ninf[200]{};
        double t_linf[200]{};
        double t_ltau[200]{};
        double t_ntau[200]{};
        double t_an[200]{};
        double t_bn[200]{};
        double t_al[200]{};
        double t_bl[200]{};
    };
    static_assert(std::is_trivially_copy_constructible_v<StochKv3_Store>);
    static_assert(std::is_trivially_move_constructible_v<StochKv3_Store>);
    static_assert(std::is_trivially_copy_assignable_v<StochKv3_Store>);
    static_assert(std::is_trivially_move_assignable_v<StochKv3_Store>);
    static_assert(std::is_trivially_destructible_v<StochKv3_Store>);
    StochKv3_Store StochKv3_global;


    /** all mechanism instance variables and global variables */
    struct StochKv3_Instance  {
        const double* gamma{};
        double* eta{};
        const double* gkbar{};
        double* deterministic{};
        double* an{};
        double* bn{};
        double* al{};
        double* bl{};
        double* ik{};
        double* gk{};
        double* ninf{};
        double* ntau{};
        double* linf{};
        double* ltau{};
        double* N{};
        double* P_an{};
        double* P_bn{};
        double* P_al{};
        double* P_bl{};
        double* n{};
        double* l{};
        double* ek{};
        double* scale_dens{};
        double* usingR123{};
        double* Dn{};
        double* Dl{};
        double* N0L0{};
        double* N1L0{};
        double* N0L1{};
        double* N1L1{};
        double* n0l0_n1l0{};
        double* n0l0_n0l1{};
        double* n1l0_n1l1{};
        double* n1l0_n0l0{};
        double* n0l1_n1l1{};
        double* n0l1_n0l0{};
        double* n1l1_n0l1{};
        double* n1l1_n1l0{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ek{};
        double* ion_ik{};
        double* ion_dikdv{};
        void** rng{};
        double* area{};
        StochKv3_Store* global{&StochKv3_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"vmin_StochKv3", &StochKv3_global.vmin},
        {"vmax_StochKv3", &StochKv3_global.vmax},
        {"usetable_StochKv3", &StochKv3_global.usetable},
        {nullptr, nullptr}
    };


    /** connect global (array) variables to hoc -- */
    static DoubVec hoc_vector_double[] = {
        {nullptr, nullptr, 0}
    };


    static inline int first_pointer_var_index() {
        return 3;
    }


    static inline int first_random_var_index() {
        return -1;
    }


    static inline int float_variables_size() {
        return 40;
    }


    static inline int int_variables_size() {
        return 5;
    }


    static inline int get_mech_type() {
        return StochKv3_global.mech_type;
    }


    static inline Memb_list* get_memb_list(NrnThread* nt) {
        if (!nt->_ml_list) {
            return nullptr;
        }
        return nt->_ml_list[get_mech_type()];
    }


    static inline void* mem_alloc(size_t num, size_t size, size_t alignment = 16) {
        void* ptr;
        posix_memalign(&ptr, alignment, num*size);
        memset(ptr, 0, size);
        return ptr;
    }


    static inline void mem_free(void* ptr) {
        free(ptr);
    }


    static inline void coreneuron_abort() {
        abort();
    }

    // Allocate instance structure
    static void nrn_private_constructor_StochKv3(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new StochKv3_Instance{};
        assert(inst->global == &StochKv3_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(StochKv3_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_StochKv3(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<StochKv3_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &StochKv3_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(StochKv3_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<StochKv3_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &StochKv3_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(StochKv3_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gamma = ml->data+0*pnodecount;
        inst->eta = ml->data+1*pnodecount;
        inst->gkbar = ml->data+2*pnodecount;
        inst->deterministic = ml->data+3*pnodecount;
        inst->an = ml->data+4*pnodecount;
        inst->bn = ml->data+5*pnodecount;
        inst->al = ml->data+6*pnodecount;
        inst->bl = ml->data+7*pnodecount;
        inst->ik = ml->data+8*pnodecount;
        inst->gk = ml->data+9*pnodecount;
        inst->ninf = ml->data+10*pnodecount;
        inst->ntau = ml->data+11*pnodecount;
        inst->linf = ml->data+12*pnodecount;
        inst->ltau = ml->data+13*pnodecount;
        inst->N = ml->data+14*pnodecount;
        inst->P_an = ml->data+15*pnodecount;
        inst->P_bn = ml->data+16*pnodecount;
        inst->P_al = ml->data+17*pnodecount;
        inst->P_bl = ml->data+18*pnodecount;
        inst->n = ml->data+19*pnodecount;
        inst->l = ml->data+20*pnodecount;
        inst->ek = ml->data+21*pnodecount;
        inst->scale_dens = ml->data+22*pnodecount;
        inst->usingR123 = ml->data+23*pnodecount;
        inst->Dn = ml->data+24*pnodecount;
        inst->Dl = ml->data+25*pnodecount;
        inst->N0L0 = ml->data+26*pnodecount;
        inst->N1L0 = ml->data+27*pnodecount;
        inst->N0L1 = ml->data+28*pnodecount;
        inst->N1L1 = ml->data+29*pnodecount;
        inst->n0l0_n1l0 = ml->data+30*pnodecount;
        inst->n0l0_n0l1 = ml->data+31*pnodecount;
        inst->n1l0_n1l1 = ml->data+32*pnodecount;
        inst->n1l0_n0l0 = ml->data+33*pnodecount;
        inst->n0l1_n1l1 = ml->data+34*pnodecount;
        inst->n0l1_n0l0 = ml->data+35*pnodecount;
        inst->n1l1_n0l1 = ml->data+36*pnodecount;
        inst->n1l1_n1l0 = ml->data+37*pnodecount;
        inst->v_unused = ml->data+38*pnodecount;
        inst->g_unused = ml->data+39*pnodecount;
        inst->ion_ek = nt->_data;
        inst->ion_ik = nt->_data;
        inst->ion_dikdv = nt->_data;
        inst->rng = nt->_vdata;
        inst->area = nt->_data;
    }



    static void nrn_alloc_StochKv3(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_StochKv3(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<StochKv3_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_StochKv3(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<StochKv3_Instance*>(ml->instance);

        #endif
    }


    inline double strap_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double x);
    inline double urand_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline double brand_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double P, double N);
    inline double BnlDev_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double ppr, double nnr);
    inline double bbsavestate_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int trates_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double arg_v);
    inline int ChkProb_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double p);
    inline int setRNG_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
}


using namespace coreneuron;


#ifndef NRN_VERSION_GTEQ_8_2_0
#include "nrnran123.h"
extern int cvode_active_;
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifndef CORENEURON_BUILD
double nrn_random_pick(void* r);
void* nrn_random_arg(int argpos);
#endif
#define RANDCAST
#else
#define RANDCAST (Rand*)
#endif


static void bbcore_write(double* x, int* d, int* xx, int* offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
    if (d) {
        uint32_t* di = ((uint32_t*)d) + *offset;
      if (!nt->_vdata[indexes[3*pnodecount + id]]) {
        di[0] = 0; di[1] = 0, di[2] = 0;
      }else{
        nrnran123_State** pv = (nrnran123_State**)(&nt->_vdata[indexes[3*pnodecount + id]]);
        nrnran123_getids3(*pv, di, di+1, di+2);
        char which;
        nrnran123_getseq(*pv, di+3, &which);
        di[4] = (int)which;
      }
    }
    *offset += 5;
}
static void bbcore_read(double* x, int* d, int* xx, int* offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
    uint32_t* di = ((uint32_t*)d) + *offset;
        if (di[0] != 0 || di[1] != 0|| di[2] != 0)
        {
      nrnran123_State** pv = (nrnran123_State**)(&nt->_vdata[indexes[3*pnodecount + id]]);
#if !NRNBBCORE
      if(*pv) {
          nrnran123_deletestream(*pv);
      }
#endif
      *pv = nrnran123_newstream3(di[0], di[1], di[2]);
      nrnran123_setseq(*pv, di[3], (char)di[4]);
        }
    *offset += 5;
}


#define        PI 3.141592654
#define        r_ia     16807
#define        r_im     2147483647
#define        r_am     (1.0/r_im)
#define        r_iq     127773
#define        r_ir     2836
#define        r_ntab   32
#define        r_ndiv   (1+(r_im-1)/r_ntab)
#define        r_eps    1.2e-7
#define        r_rnmx   (1.0-r_eps)


/* ---------------------------------------------------------------- */
/* gammln - compute natural log of gamma function of xx */
static double
gammln(double xx)
{
    double x,tmp,ser;
    static double cof[6]={76.18009173,-86.50532033,24.01409822,
        -1.231739516,0.120858003e-2,-0.536382e-5};
    int j;
    x=xx-1.0;
    tmp=x+5.5;
    tmp -= (x+0.5)*log(tmp);
    ser=1.0;
    for (j=0;j<=5;j++) {
        x += 1.0;
        ser += cof[j]/x;
    }
    return -tmp+log(2.50662827465*ser);
}


namespace coreneuron {


    inline int f_trates_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double arg_v) {
        int ret_f_trates = 0;
        arg_v = arg_v + 10.0;
        inst->linf[id] = 1.0 / (1.0 + exp(( -30.0 - arg_v) / 10.0));
        inst->ltau[id] = 0.346 * exp( -arg_v / (18.272)) + 2.09;
        inst->ninf[id] = 1.0 / (1.0 + exp(0.0878 * (arg_v + 55.1)));
        inst->ntau[id] = 2.1 * exp( -arg_v / 21.2) + 4.627;
        arg_v = arg_v - 10.0;
        inst->al[id] = inst->linf[id] / inst->ltau[id];
        inst->bl[id] = 1.0 / inst->ltau[id] - inst->al[id];
        inst->an[id] = inst->ninf[id] / inst->ntau[id];
        inst->bn[id] = 1.0 / inst->ntau[id] - inst->an[id];
        return ret_f_trates;
    }


    void lazy_update_trates_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        if (inst->global->usetable == 0) {
            return;
        }
        static bool make_table = true;
        static double save_dt;
        if (save_dt != nt->_dt) {
            make_table = true;
        }
        if (make_table) {
            make_table = false;
            inst->global->tmin_trates = inst->global->vmin;
            double tmax = inst->global->vmax;
            double dx = (tmax-inst->global->tmin_trates) / 199.;
            inst->global->mfac_trates = 1./dx;
            double x = inst->global->tmin_trates;
            for (std::size_t i = 0; i < 200; x += dx, i++) {
                f_trates_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, x);
                inst->global->t_ntau[i] = inst->ntau[id];
                inst->global->t_ltau[i] = inst->ltau[id];
                inst->global->t_ninf[i] = inst->ninf[id];
                inst->global->t_linf[i] = inst->linf[id];
                inst->global->t_al[i] = inst->al[id];
                inst->global->t_bl[i] = inst->bl[id];
                inst->global->t_an[i] = inst->an[id];
                inst->global->t_bn[i] = inst->bn[id];
            }
            save_dt = nt->_dt;
        }
    }


    inline int trates_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double arg_v){
        if (inst->global->usetable == 0) {
            f_trates_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, arg_v);
            return 0;
        }
        double xi = inst->global->mfac_trates * (arg_v - inst->global->tmin_trates);
        if (isnan(xi)) {
            inst->ntau[id] = xi;
            inst->ltau[id] = xi;
            inst->ninf[id] = xi;
            inst->linf[id] = xi;
            inst->al[id] = xi;
            inst->bl[id] = xi;
            inst->an[id] = xi;
            inst->bn[id] = xi;
            return 0;
        }
        if (xi <= 0. || xi >= 199.) {
            int index = (xi <= 0.) ? 0 : 199;
            inst->ntau[id] = inst->global->t_ntau[index];
            inst->ltau[id] = inst->global->t_ltau[index];
            inst->ninf[id] = inst->global->t_ninf[index];
            inst->linf[id] = inst->global->t_linf[index];
            inst->al[id] = inst->global->t_al[index];
            inst->bl[id] = inst->global->t_bl[index];
            inst->an[id] = inst->global->t_an[index];
            inst->bn[id] = inst->global->t_bn[index];
            return 0;
        }
        int i = int(xi);
        double theta = xi - double(i);
        inst->ntau[id] = inst->global->t_ntau[i] + theta*(inst->global->t_ntau[i+1]-inst->global->t_ntau[i]);
        inst->ltau[id] = inst->global->t_ltau[i] + theta*(inst->global->t_ltau[i+1]-inst->global->t_ltau[i]);
        inst->ninf[id] = inst->global->t_ninf[i] + theta*(inst->global->t_ninf[i+1]-inst->global->t_ninf[i]);
        inst->linf[id] = inst->global->t_linf[i] + theta*(inst->global->t_linf[i+1]-inst->global->t_linf[i]);
        inst->al[id] = inst->global->t_al[i] + theta*(inst->global->t_al[i+1]-inst->global->t_al[i]);
        inst->bl[id] = inst->global->t_bl[i] + theta*(inst->global->t_bl[i+1]-inst->global->t_bl[i]);
        inst->an[id] = inst->global->t_an[i] + theta*(inst->global->t_an[i+1]-inst->global->t_an[i]);
        inst->bn[id] = inst->global->t_bn[i] + theta*(inst->global->t_bn[i+1]-inst->global->t_bn[i]);
        return 0;
    }


    inline int ChkProb_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double p) {
        int ret_ChkProb = 0;
        if (p < 0.0 || p > 1.0) {
                fprintf(stderr, "StochKv2.mod:ChkProb: argument not a probability.\n");

        }
        return ret_ChkProb;
    }


    inline int setRNG_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_setRNG = 0;
        #ifndef CORENEURON_BUILD
            inst->usingR123[id] = 0;
            if( ifarg(1) && hoc_is_double_arg(1) ) {
                nrnran123_State** pv = (nrnran123_State**)(&inst->rng[indexes[3*pnodecount + id]]);
                uint32_t a2 = 0;
                uint32_t a3 = 0;
                if (*pv) {
                    nrnran123_deletestream(*pv);
                    *pv = (nrnran123_State*)0;
                }
                if (ifarg(2)) {
                    a2 = (uint32_t)*getarg(2);
                }
                if (ifarg(3)) {
                    a3 = (uint32_t)*getarg(3);
                }
                *pv = nrnran123_newstream3((uint32_t)*getarg(1), a2, a3);
                inst->usingR123[id] = 1;
            } else if( ifarg(1) ) {
                void** pv = (void**)(&inst->rng[indexes[3*pnodecount + id]]);
                *pv = nrn_random_arg(1);
            } else {
                void** pv = (void**)(&inst->rng[indexes[3*pnodecount + id]]);
                *pv = (void*)0;
            }
        #endif

        return ret_setRNG;
    }


    inline double strap_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double x) {
        double ret_strap = 0.0;
        if (x < 0.0) {
            ret_strap = 0.0;
        } else {
            ret_strap = x;
        }
        return ret_strap;
    }


    inline double urand_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_urand = 0.0;
            double value = 0.0;
            if( inst->usingR123[id] ) {
                value = nrnran123_dblpick((nrnran123_State*)inst->rng[indexes[3*pnodecount + id]]);
            } else if (inst->rng[indexes[3*pnodecount + id]]) {
        #ifndef CORENEURON_BUILD
                value = nrn_random_pick(RANDCAST inst->rng[indexes[3*pnodecount + id]]);
        #endif
            } else {
                value = 0.0;
            }
            ret_urand = value;

        return ret_urand;
    }


    inline double brand_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double P, double N) {
        double ret_brand = 0.0;
                /*
                :Supports separate independent but reproducible streams for
                : each instance. However, the corresponding hoc Random
                : distribution MUST be set to Random.uniform(0,1)
                */
                double value = 0.0;
                int i;
                for (i = 0; i < inst->N[id]; i++) {
                   if (urand_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v) < P) {
                      value = value + 1;
                   }
                }
                return(value);

        ret_brand = value;
        return ret_brand;
    }


    inline double BnlDev_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double ppr, double nnr) {
        double ret_BnlDev = 0.0;
                int j;
                double am,em,g,angle,p,bnl,sq,bt,y;
                double pc,plog,pclog,en,oldg;
                /* prepare to always ignore errors within this routine */
                p=(ppr <= 0.5 ? ppr : 1.0-ppr);
                am=nnr*p;
                if (nnr < 25) {
                    bnl=0.0;
                    for (j=1;j<=nnr;j++)
                        if (urand_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v) < p) bnl += 1.0;
                }
                else if (am < 1.0) {
                    g=exp(-am);
                    bt=1.0;
                    for (j=0;j<=nnr;j++) {
                        bt *= urand_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v);
                        if (bt < g) break;
                    }
                    bnl=(j <= nnr ? j : nnr);
                }
                else {
                    {
                        en=nnr;
                        oldg=gammln(en+1.0);
                    }
                    {
                        pc=1.0-p;
                        plog=log(p);
                        pclog=log(pc);
                    }
                    sq=sqrt(2.0*am*pc);
                    do {
                        do {
                            angle=PI*urand_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v);
                            angle=PI*urand_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v);
                            y=tan(angle);
                            em=sq*y+am;
                        } while (em < 0.0 || em >= (en+1.0));
                        em=floor(em);
                            bt=1.2*sq*(1.0+y*y)*exp(oldg-gammln(em+1.0) -
                            gammln(en-em+1.0)+em*plog+(en-em)*pclog);
                    } while (urand_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v) > bt);
                    bnl=em;
                }
                if (p != ppr) bnl=nnr-bnl;
                /* recover error if changed during this routine, thus ignoring
                    any errors during this routine */
                return bnl;

        ret_BnlDev = bnl;
        return ret_BnlDev;
    }


    inline double bbsavestate_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_bbsavestate = 0.0;
        ret_bbsavestate = 0.0;
         #ifndef CORENEURON_BUILD
                /* first arg is direction (-1 get info, 0 save, 1 restore), second is value*/
                double *xdir, *xval;
                #ifndef NRN_VERSION_GTEQ_8_2_0
                double *hoc_pgetarg();
                long nrn_get_random_sequence(void* r);
                void nrn_set_random_sequence(void* r, int val);
                #endif
                xdir = hoc_pgetarg(1);
                xval = hoc_pgetarg(2);
                if (inst->rng[indexes[3*pnodecount + id]]) {
                        if (*xdir == -1.) {
                            if( inst->usingR123[id] ) {
                                *xdir = 2.0;
                            } else {
                                *xdir = 1.0;
                            }
                            return 0.0;
                        }
                        else if (*xdir == 0.) {
                            if( inst->usingR123[id] ) {
                                uint32_t seq;
                                char which;
                                nrnran123_getseq( (nrnran123_State*)inst->rng[indexes[3*pnodecount + id]], &seq, &which );
                                xval[0] = (double) seq;
                                xval[1] = (double) which;
                            } else {
                                xval[0] = (double)nrn_get_random_sequence(RANDCAST inst->rng[indexes[3*pnodecount + id]]);
                            }
                        } else{
                            if( inst->usingR123[id] ) {
                                nrnran123_setseq( (nrnran123_State*)inst->rng[indexes[3*pnodecount + id]], (uint32_t)xval[0], (char)xval[1] );
                            } else {
                                nrn_set_random_sequence(RANDCAST inst->rng[indexes[3*pnodecount + id]], (long)(xval[0]));
                            }
                        }
                }
        #endif

        return ret_bbsavestate;
    }


    /** initialize channel */
    void nrn_init_StochKv3(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<StochKv3_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->ek[id] = inst->ion_ek[indexes[0*pnodecount + id]];
                inst->n[id] = inst->global->n0;
                inst->l[id] = inst->global->l0;
                    if (cvode_active_ && !inst->deterministic[id]) {
                        hoc_execerror("StochKv2 with deterministic=0", "cannot be used with cvode");
                    }
                    if( inst->usingR123[id] ) {
                        nrnran123_setseq((nrnran123_State*)inst->rng[indexes[3*pnodecount + id]], 0, 0);
                    }

                inst->eta[id] = (inst->gkbar[id] / inst->gamma[id]) * (10000.0);
                trates_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, v);
                inst->n[id] = inst->ninf[id];
                inst->l[id] = inst->linf[id];
                inst->scale_dens[id] = inst->gamma[id] / inst->area[indexes[4*pnodecount + id]];
                inst->N[id] = floor(inst->eta[id] * inst->area[indexes[4*pnodecount + id]] + 0.5);
                inst->N1L1[id] = inst->n[id] * inst->l[id] * inst->N[id];
                inst->N1L0[id] = inst->n[id] * (1.0 - inst->l[id]) * inst->N[id];
                inst->N0L1[id] = (1.0 - inst->n[id]) * inst->l[id] * inst->N[id];
                if ( !inst->deterministic[id]) {
                    inst->N1L1[id] = floor(inst->N1L1[id] + 0.5);
                    inst->N1L0[id] = floor(inst->N1L0[id] + 0.5);
                    inst->N0L1[id] = floor(inst->N0L1[id] + 0.5);
                }
                inst->N0L0[id] = inst->N[id] - inst->N1L1[id] - inst->N1L0[id] - inst->N0L1[id];
                inst->n0l0_n1l0[id] = 0.0;
                inst->n0l0_n0l1[id] = 0.0;
                inst->n1l0_n1l1[id] = 0.0;
                inst->n1l0_n0l0[id] = 0.0;
                inst->n0l1_n1l1[id] = 0.0;
                inst->n0l1_n0l0[id] = 0.0;
                inst->n1l1_n0l1[id] = 0.0;
                inst->n1l1_n1l0[id] = 0.0;
            }
        }
    }


    inline double nrn_current_StochKv3(int id, int pnodecount, StochKv3_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        double strap_in_0;
        {
            double x_in_0;
            x_in_0 = inst->N1L1[id];
            if (x_in_0 < 0.0) {
                strap_in_0 = 0.0;
            } else {
                strap_in_0 = x_in_0;
            }
        }
        inst->gk[id] = (strap_in_0 * inst->scale_dens[id]) * (0.0001);
        inst->ik[id] = inst->gk[id] * (v - inst->ek[id]);
        current += inst->ik[id];
        return current;
    }


    /** update current */
    void nrn_cur_StochKv3(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<StochKv3_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ek[id] = inst->ion_ek[indexes[0*pnodecount + id]];
            double g = nrn_current_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double dik = inst->ik[id];
            double rhs = nrn_current_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v);
            g = (g-rhs)/0.001;
            inst->ion_dikdv[indexes[2*pnodecount + id]] += (dik-inst->ik[id])/0.001;
            inst->ion_ik[indexes[1*pnodecount + id]] += inst->ik[id];
            #if NRN_PRCELLSTATE
            inst->g_unused[id] = g;
            #endif
            vec_rhs[node_id] -= rhs;
            vec_d[node_id] += g;
        }
    }


    /** update state */
    void nrn_state_StochKv3(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<StochKv3_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ek[id] = inst->ion_ek[indexes[0*pnodecount + id]];
            trates_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, v);
            inst->l[id] = inst->l[id] + (1.0 - exp(nt->_dt * (( -((inst->al[id] + inst->bl[id])) * (1.0))))) * ( -(inst->al[id]) / (( -((inst->al[id] + inst->bl[id])) * (1.0))) - inst->l[id]);
            inst->n[id] = inst->n[id] + (1.0 - exp(nt->_dt * (( -((inst->an[id] + inst->bn[id])) * (1.0))))) * ( -(inst->an[id]) / (( -((inst->an[id] + inst->bn[id])) * (1.0))) - inst->n[id]);
            if (inst->deterministic[id] || nt->_dt > 1.0) {
                inst->N1L1[id] = inst->n[id] * inst->l[id] * inst->N[id];
                inst->N1L0[id] = inst->n[id] * (1.0 - inst->l[id]) * inst->N[id];
                inst->N0L1[id] = (1.0 - inst->n[id]) * inst->l[id] * inst->N[id];
            } else {
                double strap_in_1, strap_in_2, strap_in_3, strap_in_4, strap_in_5, strap_in_6, strap_in_7, strap_in_8, strap_in_9, strap_in_10, strap_in_11, strap_in_12;
                inst->N1L1[id] = floor(inst->N1L1[id] + 0.5);
                inst->N1L0[id] = floor(inst->N1L0[id] + 0.5);
                inst->N0L1[id] = floor(inst->N0L1[id] + 0.5);
                inst->N0L0[id] = inst->N[id] - inst->N1L1[id] - inst->N1L0[id] - inst->N0L1[id];
                {
                    double x_in_1;
                    x_in_1 = inst->an[id] * nt->_dt;
                    if (x_in_1 < 0.0) {
                        strap_in_1 = 0.0;
                    } else {
                        strap_in_1 = x_in_1;
                    }
                }
                inst->P_an[id] = strap_in_1;
                {
                    double x_in_2;
                    x_in_2 = inst->bn[id] * nt->_dt;
                    if (x_in_2 < 0.0) {
                        strap_in_2 = 0.0;
                    } else {
                        strap_in_2 = x_in_2;
                    }
                }
                inst->P_bn[id] = strap_in_2;
                {
                    double p_in_0;
                    p_in_0 = inst->P_an[id];
                    if (p_in_0 < 0.0 || p_in_0 > 1.0) {
                            fprintf(stderr, "StochKv2.mod:ChkProb: argument not a probability.\n");

                    }
                }
                {
                    double p_in_1;
                    p_in_1 = inst->P_bn[id];
                    if (p_in_1 < 0.0 || p_in_1 > 1.0) {
                            fprintf(stderr, "StochKv2.mod:ChkProb: argument not a probability.\n");

                    }
                }
                inst->n0l0_n1l0[id] = BnlDev_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, inst->P_an[id], inst->N0L0[id]);
                inst->n0l1_n1l1[id] = BnlDev_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, inst->P_an[id], inst->N0L1[id]);
                inst->n1l1_n0l1[id] = BnlDev_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, inst->P_bn[id], inst->N1L1[id]);
                inst->n1l0_n0l0[id] = BnlDev_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, inst->P_bn[id], inst->N1L0[id]);
                {
                    double x_in_3;
                    x_in_3 = inst->N0L0[id] - inst->n0l0_n1l0[id] + inst->n1l0_n0l0[id];
                    if (x_in_3 < 0.0) {
                        strap_in_3 = 0.0;
                    } else {
                        strap_in_3 = x_in_3;
                    }
                }
                inst->N0L0[id] = strap_in_3;
                {
                    double x_in_4;
                    x_in_4 = inst->N1L0[id] - inst->n1l0_n0l0[id] + inst->n0l0_n1l0[id];
                    if (x_in_4 < 0.0) {
                        strap_in_4 = 0.0;
                    } else {
                        strap_in_4 = x_in_4;
                    }
                }
                inst->N1L0[id] = strap_in_4;
                {
                    double x_in_5;
                    x_in_5 = inst->N0L1[id] - inst->n0l1_n1l1[id] + inst->n1l1_n0l1[id];
                    if (x_in_5 < 0.0) {
                        strap_in_5 = 0.0;
                    } else {
                        strap_in_5 = x_in_5;
                    }
                }
                inst->N0L1[id] = strap_in_5;
                {
                    double x_in_6;
                    x_in_6 = inst->N1L1[id] - inst->n1l1_n0l1[id] + inst->n0l1_n1l1[id];
                    if (x_in_6 < 0.0) {
                        strap_in_6 = 0.0;
                    } else {
                        strap_in_6 = x_in_6;
                    }
                }
                inst->N1L1[id] = strap_in_6;
                {
                    double x_in_7;
                    x_in_7 = inst->al[id] * nt->_dt;
                    if (x_in_7 < 0.0) {
                        strap_in_7 = 0.0;
                    } else {
                        strap_in_7 = x_in_7;
                    }
                }
                inst->P_al[id] = strap_in_7;
                {
                    double x_in_8;
                    x_in_8 = inst->bl[id] * nt->_dt;
                    if (x_in_8 < 0.0) {
                        strap_in_8 = 0.0;
                    } else {
                        strap_in_8 = x_in_8;
                    }
                }
                inst->P_bl[id] = strap_in_8;
                {
                    double p_in_2;
                    p_in_2 = inst->P_al[id];
                    if (p_in_2 < 0.0 || p_in_2 > 1.0) {
                            fprintf(stderr, "StochKv2.mod:ChkProb: argument not a probability.\n");

                    }
                }
                {
                    double p_in_3;
                    p_in_3 = inst->P_bl[id];
                    if (p_in_3 < 0.0 || p_in_3 > 1.0) {
                            fprintf(stderr, "StochKv2.mod:ChkProb: argument not a probability.\n");

                    }
                }
                inst->n0l0_n0l1[id] = BnlDev_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, inst->P_al[id], inst->N0L0[id] - inst->n0l0_n1l0[id]);
                inst->n1l0_n1l1[id] = BnlDev_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, inst->P_al[id], inst->N1L0[id] - inst->n1l0_n0l0[id]);
                inst->n0l1_n0l0[id] = BnlDev_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, inst->P_bl[id], inst->N0L1[id] - inst->n0l1_n1l1[id]);
                inst->n1l1_n1l0[id] = BnlDev_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v, inst->P_bl[id], inst->N1L1[id] - inst->n1l1_n0l1[id]);
                {
                    double x_in_9;
                    x_in_9 = inst->N0L0[id] - inst->n0l0_n0l1[id] + inst->n0l1_n0l0[id];
                    if (x_in_9 < 0.0) {
                        strap_in_9 = 0.0;
                    } else {
                        strap_in_9 = x_in_9;
                    }
                }
                inst->N0L0[id] = strap_in_9;
                {
                    double x_in_10;
                    x_in_10 = inst->N1L0[id] - inst->n1l0_n1l1[id] + inst->n1l1_n1l0[id];
                    if (x_in_10 < 0.0) {
                        strap_in_10 = 0.0;
                    } else {
                        strap_in_10 = x_in_10;
                    }
                }
                inst->N1L0[id] = strap_in_10;
                {
                    double x_in_11;
                    x_in_11 = inst->N0L1[id] - inst->n0l1_n0l0[id] + inst->n0l0_n0l1[id];
                    if (x_in_11 < 0.0) {
                        strap_in_11 = 0.0;
                    } else {
                        strap_in_11 = x_in_11;
                    }
                }
                inst->N0L1[id] = strap_in_11;
                {
                    double x_in_12;
                    x_in_12 = inst->N1L1[id] - inst->n1l1_n1l0[id] + inst->n1l0_n1l1[id];
                    if (x_in_12 < 0.0) {
                        strap_in_12 = 0.0;
                    } else {
                        strap_in_12 = x_in_12;
                    }
                }
                inst->N1L1[id] = strap_in_12;
            }
            inst->N0L0[id] = inst->N[id] - inst->N1L1[id] - inst->N1L0[id] - inst->N0L1[id];
        }
    }


    static void check_table_thread_StochKv3 (int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, int tml_id) {
        setup_instance(nt, ml);
        auto* const inst = static_cast<StochKv3_Instance*>(ml->instance);
        double v = 0;
        lazy_update_trates_StochKv3(id, pnodecount, inst, data, indexes, thread, nt, v);
    }


    /** register channel with the simulator */
    void _StochKv3_reg() {

        int mech_type = nrn_get_mechtype("StochKv3");
        StochKv3_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_StochKv3, nrn_cur_StochKv3, nullptr, nrn_state_StochKv3, nrn_init_StochKv3, nrn_private_constructor_StochKv3, nrn_private_destructor_StochKv3, first_pointer_var_index(), 1);
        StochKv3_global.k_type = nrn_get_mechtype("k_ion");

        _nrn_thread_table_reg(mech_type, check_table_thread_StochKv3);
        hoc_reg_bbcore_read(mech_type, bbcore_read);
        hoc_reg_bbcore_write(mech_type, bbcore_write);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "k_ion");
        hoc_register_dparam_semantics(mech_type, 1, "k_ion");
        hoc_register_dparam_semantics(mech_type, 2, "k_ion");
        hoc_register_dparam_semantics(mech_type, 3, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 4, "area");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
