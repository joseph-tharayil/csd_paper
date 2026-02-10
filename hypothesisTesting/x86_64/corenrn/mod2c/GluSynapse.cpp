/*********************************************************
Model Name      : GluSynapse
Filename        : GluSynapse.mod
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


    /** constants used in nmodl from UNITS */
    static const double FARADAY = 0x1.78e555060882cp+16;
    static const double PI = 0x1.921fb54442d18p+1;
    static const double R = 0x1.0a1013e8990bep+3;
    #ifndef NRN_PRCELLSTATE
    #define NRN_PRCELLSTATE 0
    #endif


    /** channel information */
    static const char *mechanism_info[] = {
        "7.7.0",
        "GluSynapse",
        "tau_d_AMPA",
        "gmax0_AMPA",
        "gmax_d_AMPA",
        "gmax_p_AMPA",
        "gmax_NMDA",
        "Use",
        "Dep",
        "Fac",
        "Nrrp",
        "Use_d",
        "Use_p",
        "volume_CR",
        "theta_d_GB",
        "theta_p_GB",
        "rho0_GB",
        "synapseID",
        "verbose",
        "selected_for_report",
        "conductance",
        0,
        "g_AMPA",
        "g_NMDA",
        "ica_NMDA",
        "ica_VDCC",
        "dep_GB",
        "pot_GB",
        "vsyn",
        "i",
        "next_delay",
        0,
        "A_AMPA",
        "B_AMPA",
        "gmax_AMPA",
        "A_NMDA",
        "B_NMDA",
        "Use_GB",
        "m_VDCC",
        "h_VDCC",
        "cai_CR",
        "rho_GB",
        "effcai_GB",
        0,
        "rng",
        "delay_times",
        "delay_weights",
        0
    };


    /** all global variables */
    struct GluSynapse_Store {
        int point_type{};
        double A_AMPA0{};
        double B_AMPA0{};
        double gmax_AMPA0{};
        double A_NMDA0{};
        double B_NMDA0{};
        double Use_GB0{};
        double m_VDCC0{};
        double h_VDCC0{};
        double cai_CR0{};
        double rho_GB0{};
        double effcai_GB0{};
        int reset{};
        int mech_type{};
        double tau_r_AMPA{0.2};
        double E_AMPA{0};
        double scale_NMDA{2.552};
        double slope_NMDA{0.072};
        double tau_r_NMDA{0.29};
        double tau_d_NMDA{70};
        double E_NMDA{-3};
        double mg{1};
        double ljp_VDCC{0};
        double vhm_VDCC{-5.9};
        double km_VDCC{9.5};
        double mtau_VDCC{1};
        double vhh_VDCC{-39};
        double kh_VDCC{-9.2};
        double htau_VDCC{27};
        double gca_bar_VDCC{0.0744};
        double gamma_ca_CR{0.04};
        double tau_ca_CR{12};
        double min_ca_CR{7e-05};
        double cao_CR{2};
        double rho_star_GB{0.5};
        double tau_ind_GB{70};
        double tau_exp_GB{100};
        double tau_effca_GB{200};
        double gamma_d_GB{100};
        double gamma_p_GB{450};
        double nc_type_param{5};
        double minis_single_vesicle{0};
        double init_depleted{0};
        int slist1[11]{28, 29, 30, 31, 32, 33, 34, 35, 36, 38, 37};
        int dlist1[11]{40, 41, 42, 43, 44, 45, 46, 47, 48, 50, 49};
    };
    static_assert(std::is_trivially_copy_constructible_v<GluSynapse_Store>);
    static_assert(std::is_trivially_move_constructible_v<GluSynapse_Store>);
    static_assert(std::is_trivially_copy_assignable_v<GluSynapse_Store>);
    static_assert(std::is_trivially_move_assignable_v<GluSynapse_Store>);
    static_assert(std::is_trivially_destructible_v<GluSynapse_Store>);
    GluSynapse_Store GluSynapse_global;


    /** all mechanism instance variables and global variables */
    struct GluSynapse_Instance  {
        double* celsius{&coreneuron::celsius};
        const double* tau_d_AMPA{};
        const double* gmax0_AMPA{};
        const double* gmax_d_AMPA{};
        const double* gmax_p_AMPA{};
        const double* gmax_NMDA{};
        const double* Use{};
        const double* Dep{};
        const double* Fac{};
        const double* Nrrp{};
        const double* Use_d{};
        const double* Use_p{};
        const double* volume_CR{};
        const double* theta_d_GB{};
        const double* theta_p_GB{};
        const double* rho0_GB{};
        const double* synapseID{};
        const double* verbose{};
        const double* selected_for_report{};
        const double* conductance{};
        double* g_AMPA{};
        double* g_NMDA{};
        double* ica_NMDA{};
        double* ica_VDCC{};
        double* dep_GB{};
        double* pot_GB{};
        double* vsyn{};
        double* i{};
        double* next_delay{};
        double* A_AMPA{};
        double* B_AMPA{};
        double* gmax_AMPA{};
        double* A_NMDA{};
        double* B_NMDA{};
        double* Use_GB{};
        double* m_VDCC{};
        double* h_VDCC{};
        double* cai_CR{};
        double* rho_GB{};
        double* effcai_GB{};
        double* usingR123{};
        double* DA_AMPA{};
        double* DB_AMPA{};
        double* Dgmax_AMPA{};
        double* DA_NMDA{};
        double* DB_NMDA{};
        double* DUse_GB{};
        double* Dm_VDCC{};
        double* Dh_VDCC{};
        double* Dcai_CR{};
        double* Drho_GB{};
        double* Deffcai_GB{};
        double* v_unused{};
        double* g_unused{};
        double* tsave{};
        const double* node_area{};
        const int* point_process{};
        void** rng{};
        void** delay_times{};
        void** delay_weights{};
        const int* tqitem{};
        int* watch0{};
        int* watch1{};
        int* watch2{};
        int* watch3{};
        int* watch4{};
        GluSynapse_Store* global{&GluSynapse_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"tau_r_AMPA_GluSynapse", &GluSynapse_global.tau_r_AMPA},
        {"E_AMPA_GluSynapse", &GluSynapse_global.E_AMPA},
        {"scale_NMDA_GluSynapse", &GluSynapse_global.scale_NMDA},
        {"slope_NMDA_GluSynapse", &GluSynapse_global.slope_NMDA},
        {"tau_r_NMDA_GluSynapse", &GluSynapse_global.tau_r_NMDA},
        {"tau_d_NMDA_GluSynapse", &GluSynapse_global.tau_d_NMDA},
        {"E_NMDA_GluSynapse", &GluSynapse_global.E_NMDA},
        {"mg_GluSynapse", &GluSynapse_global.mg},
        {"ljp_VDCC_GluSynapse", &GluSynapse_global.ljp_VDCC},
        {"vhm_VDCC_GluSynapse", &GluSynapse_global.vhm_VDCC},
        {"km_VDCC_GluSynapse", &GluSynapse_global.km_VDCC},
        {"mtau_VDCC_GluSynapse", &GluSynapse_global.mtau_VDCC},
        {"vhh_VDCC_GluSynapse", &GluSynapse_global.vhh_VDCC},
        {"kh_VDCC_GluSynapse", &GluSynapse_global.kh_VDCC},
        {"htau_VDCC_GluSynapse", &GluSynapse_global.htau_VDCC},
        {"gca_bar_VDCC_GluSynapse", &GluSynapse_global.gca_bar_VDCC},
        {"gamma_ca_CR_GluSynapse", &GluSynapse_global.gamma_ca_CR},
        {"tau_ca_CR_GluSynapse", &GluSynapse_global.tau_ca_CR},
        {"min_ca_CR_GluSynapse", &GluSynapse_global.min_ca_CR},
        {"cao_CR_GluSynapse", &GluSynapse_global.cao_CR},
        {"rho_star_GB_GluSynapse", &GluSynapse_global.rho_star_GB},
        {"tau_ind_GB_GluSynapse", &GluSynapse_global.tau_ind_GB},
        {"tau_exp_GB_GluSynapse", &GluSynapse_global.tau_exp_GB},
        {"tau_effca_GB_GluSynapse", &GluSynapse_global.tau_effca_GB},
        {"gamma_d_GB_GluSynapse", &GluSynapse_global.gamma_d_GB},
        {"gamma_p_GB_GluSynapse", &GluSynapse_global.gamma_p_GB},
        {"nc_type_param_GluSynapse", &GluSynapse_global.nc_type_param},
        {"minis_single_vesicle_GluSynapse", &GluSynapse_global.minis_single_vesicle},
        {"init_depleted_GluSynapse", &GluSynapse_global.init_depleted},
        {nullptr, nullptr}
    };


    /** connect global (array) variables to hoc -- */
    static DoubVec hoc_vector_double[] = {
        {nullptr, nullptr, 0}
    };


    static inline int first_pointer_var_index() {
        return 2;
    }


    static inline int first_random_var_index() {
        return -1;
    }


    static inline int num_net_receive_args() {
        return 6;
    }


    static inline int float_variables_size() {
        return 54;
    }


    static inline int int_variables_size() {
        return 11;
    }


    static inline int get_mech_type() {
        return GluSynapse_global.mech_type;
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
    static void nrn_private_constructor_GluSynapse(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new GluSynapse_Instance{};
        assert(inst->global == &GluSynapse_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(GluSynapse_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_GluSynapse(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GluSynapse_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GluSynapse_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &GluSynapse_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(GluSynapse_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->tau_d_AMPA = ml->data+0*pnodecount;
        inst->gmax0_AMPA = ml->data+1*pnodecount;
        inst->gmax_d_AMPA = ml->data+2*pnodecount;
        inst->gmax_p_AMPA = ml->data+3*pnodecount;
        inst->gmax_NMDA = ml->data+4*pnodecount;
        inst->Use = ml->data+5*pnodecount;
        inst->Dep = ml->data+6*pnodecount;
        inst->Fac = ml->data+7*pnodecount;
        inst->Nrrp = ml->data+8*pnodecount;
        inst->Use_d = ml->data+9*pnodecount;
        inst->Use_p = ml->data+10*pnodecount;
        inst->volume_CR = ml->data+11*pnodecount;
        inst->theta_d_GB = ml->data+12*pnodecount;
        inst->theta_p_GB = ml->data+13*pnodecount;
        inst->rho0_GB = ml->data+14*pnodecount;
        inst->synapseID = ml->data+15*pnodecount;
        inst->verbose = ml->data+16*pnodecount;
        inst->selected_for_report = ml->data+17*pnodecount;
        inst->conductance = ml->data+18*pnodecount;
        inst->g_AMPA = ml->data+19*pnodecount;
        inst->g_NMDA = ml->data+20*pnodecount;
        inst->ica_NMDA = ml->data+21*pnodecount;
        inst->ica_VDCC = ml->data+22*pnodecount;
        inst->dep_GB = ml->data+23*pnodecount;
        inst->pot_GB = ml->data+24*pnodecount;
        inst->vsyn = ml->data+25*pnodecount;
        inst->i = ml->data+26*pnodecount;
        inst->next_delay = ml->data+27*pnodecount;
        inst->A_AMPA = ml->data+28*pnodecount;
        inst->B_AMPA = ml->data+29*pnodecount;
        inst->gmax_AMPA = ml->data+30*pnodecount;
        inst->A_NMDA = ml->data+31*pnodecount;
        inst->B_NMDA = ml->data+32*pnodecount;
        inst->Use_GB = ml->data+33*pnodecount;
        inst->m_VDCC = ml->data+34*pnodecount;
        inst->h_VDCC = ml->data+35*pnodecount;
        inst->cai_CR = ml->data+36*pnodecount;
        inst->rho_GB = ml->data+37*pnodecount;
        inst->effcai_GB = ml->data+38*pnodecount;
        inst->usingR123 = ml->data+39*pnodecount;
        inst->DA_AMPA = ml->data+40*pnodecount;
        inst->DB_AMPA = ml->data+41*pnodecount;
        inst->Dgmax_AMPA = ml->data+42*pnodecount;
        inst->DA_NMDA = ml->data+43*pnodecount;
        inst->DB_NMDA = ml->data+44*pnodecount;
        inst->DUse_GB = ml->data+45*pnodecount;
        inst->Dm_VDCC = ml->data+46*pnodecount;
        inst->Dh_VDCC = ml->data+47*pnodecount;
        inst->Dcai_CR = ml->data+48*pnodecount;
        inst->Drho_GB = ml->data+49*pnodecount;
        inst->Deffcai_GB = ml->data+50*pnodecount;
        inst->v_unused = ml->data+51*pnodecount;
        inst->g_unused = ml->data+52*pnodecount;
        inst->tsave = ml->data+53*pnodecount;
        inst->node_area = nt->_data;
        inst->point_process = ml->pdata;
        inst->rng = nt->_vdata;
        inst->delay_times = nt->_vdata;
        inst->delay_weights = nt->_vdata;
        inst->tqitem = ml->pdata;
        inst->watch0 = ml->pdata;
        inst->watch1 = ml->pdata;
        inst->watch2 = ml->pdata;
        inst->watch3 = ml->pdata;
        inst->watch4 = ml->pdata;
    }



    static void nrn_alloc_GluSynapse(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_GluSynapse(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_GluSynapse(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);

        #endif
    }


    inline double nernst_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double ci, double co, double z);
    inline double urand_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline double brand_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double n, double p);
    inline double bbsavestate_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int setup_delay_vecs_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int setRNG_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int clearRNG_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
}


using namespace coreneuron;


/**
 * This Verbatim block is needed to generate random numbers from a uniform
 * distribution U(0, 1).
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifndef NRN_VERSION_GTEQ_8_2_0
#include "nrnran123.h"
double nrn_random_pick(void* r);
void* nrn_random_arg(int argpos);
#ifndef CORENEURON_BUILD
extern int ifarg(int iarg);
extern void* vector_arg(int iarg);
extern double* vector_vec(void* vv);
extern int vector_capacity(void* vv);
#endif
#define RANDCAST
#else
#define RANDCAST (Rand*)
#endif


static void bbcore_write(double* dArray, int* iArray, int* doffset, int* ioffset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
    IvocVect *vv_delay_times = *((IvocVect**)(&nt->_vdata[indexes[3*pnodecount + id]]));
    IvocVect *vv_delay_weights = *((IvocVect**)(&nt->_vdata[indexes[4*pnodecount + id]]));
    if (iArray) {
        nrnran123_State** pv = (nrnran123_State**)(&nt->_vdata[indexes[2*pnodecount + id]]);
        uint32_t* ia = ((uint32_t*)iArray) + *ioffset;
        nrnran123_getids3(*pv, ia, ia+1, ia+2);
        char which;
        nrnran123_getseq(*pv, ia+3, &which);
        ia[4] = (int)which;
    }
    *ioffset += 5;
    *doffset += 0;
    if (vv_delay_times && vv_delay_weights &&
       (vector_capacity(vv_delay_times) >= 1) && (vector_capacity(vv_delay_weights) >= 1)) {
        if (iArray) {
            uint32_t* di = ((uint32_t*)iArray) + *ioffset;
            di[0] = vector_capacity(vv_delay_times);
            di[1] = vector_capacity(vv_delay_weights);
        }
        if (dArray) {
            double* delay_times_el = vector_vec(vv_delay_times);
            double* delay_weights_el = vector_vec(vv_delay_weights);
            double* x_i = dArray + *doffset;
            int delay_vecs_idx;
            int x_idx = 0;
            for(delay_vecs_idx = 0; delay_vecs_idx < vector_capacity(vv_delay_times); ++delay_vecs_idx) {
                 x_i[x_idx++] = delay_times_el[delay_vecs_idx];
                 x_i[x_idx++] = delay_weights_el[delay_vecs_idx];
            }
        }
        *doffset += vector_capacity(vv_delay_times) + vector_capacity(vv_delay_weights);
    } else {
        if (iArray) {
            uint32_t* di = ((uint32_t*)iArray) + *ioffset;
            di[0] = 0;
            di[1] = 0;
        }
    }
    *ioffset += 2;
}
static void bbcore_read(double* dArray, int* iArray, int* doffset, int* ioffset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
    uint32_t* ia = ((uint32_t*)iArray) + *ioffset;
    if (ia[0] != 0 || ia[1] != 0 || ia[2] != 0) {
        nrnran123_State** pv = (nrnran123_State**)(&nt->_vdata[indexes[2*pnodecount + id]]);
#if !NRNBBCORE
        if(*pv) {
            nrnran123_deletestream(*pv);
        }
#endif
        *pv = nrnran123_newstream3(ia[0], ia[1], ia[2]);
        nrnran123_setseq(*pv, ia[3], (char)ia[4]);
    }
    *ioffset += 5;
    int delay_times_sz = iArray[5];
    int delay_weights_sz = iArray[6];
    *ioffset += 2;
    if ((delay_times_sz > 0) && (delay_weights_sz > 0)) {
        double* x_i = dArray + *doffset;
        if (!nt->_vdata[indexes[3*pnodecount + id]]) {
            nt->_vdata[indexes[3*pnodecount + id]] = (double*)vector_new1(delay_times_sz);
        }
        assert(delay_times_sz == vector_capacity((IvocVect*)nt->_vdata[indexes[3*pnodecount + id]]));
        if (!nt->_vdata[indexes[4*pnodecount + id]]) {
            nt->_vdata[indexes[4*pnodecount + id]] = (double*)vector_new1(delay_weights_sz);
        }
        assert(delay_weights_sz == vector_capacity((IvocVect*)nt->_vdata[indexes[4*pnodecount + id]]));
        double* delay_times_el = vector_vec((IvocVect*)nt->_vdata[indexes[3*pnodecount + id]]);
        double* delay_weights_el = vector_vec((IvocVect*)nt->_vdata[indexes[4*pnodecount + id]]);
        int x_idx;
        int vec_idx = 0;
        for(x_idx = 0; x_idx < delay_times_sz + delay_weights_sz; x_idx += 2) {
            delay_times_el[vec_idx] = x_i[x_idx];
            delay_weights_el[vec_idx++] = x_i[x_idx+1];
        }
        *doffset += delay_times_sz + delay_weights_sz;
    }
}


namespace coreneuron {


    inline int setup_delay_vecs_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_setup_delay_vecs = 0;
        #ifndef CORENEURON_BUILD
            void** vv_delay_times = (void**)(&inst->delay_times[indexes[3*pnodecount + id]]);
            void** vv_delay_weights = (void**)(&inst->delay_weights[indexes[4*pnodecount + id]]);
            *vv_delay_times = (void*)NULL;
            *vv_delay_weights = (void*)NULL;
            if (ifarg(1)) {
                *vv_delay_times = vector_arg(1);
            }
            if (ifarg(2)) {
                *vv_delay_weights = vector_arg(2);
            }
        #endif

        return ret_setup_delay_vecs;
    }


    inline int setRNG_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_setRNG = 0;
            #ifndef CORENEURON_BUILD
            inst->usingR123[id] = 0;
            if( ifarg(1) && hoc_is_double_arg(1) ) {
                nrnran123_State** pv = (nrnran123_State**)(&inst->rng[indexes[2*pnodecount + id]]);
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
                void** pv = (void**)(&inst->rng[indexes[2*pnodecount + id]]);
                *pv = nrn_random_arg(1);
            } else {  
                void** pv = (void**)(&inst->rng[indexes[2*pnodecount + id]]);
                *pv = (void*)0;
            }
            #endif

        return ret_setRNG;
    }


    inline int clearRNG_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_clearRNG = 0;
            #ifndef CORENEURON_BUILD
            if (inst->usingR123[id]) {
                nrnran123_State** pv = (nrnran123_State**)(&inst->rng[indexes[2*pnodecount + id]]);
                if (*pv) {
                    nrnran123_deletestream(*pv);
                    *pv = (nrnran123_State*)0;
                }
            } else {
                void** pv = (void**)(&inst->rng[indexes[2*pnodecount + id]]);
                if (*pv) {
                    *pv = (void*)0;
                }
            }
            #endif

        return ret_clearRNG;
    }


    inline double nernst_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double ci, double co, double z) {
        double ret_nernst = 0.0;
        ret_nernst = (1000.0) * R * (*(inst->celsius) + 273.15) / (z * FARADAY) * log(co / ci);
        if (inst->verbose[id] > 1.0) {
            printf("nernst:%g R:%g temperature (c):%g \n", ret_nernst, R, *(inst->celsius));
        }
        return ret_nernst;
    }


    inline double urand_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_urand = 0.0;
            double value;
            if ( inst->usingR123[id] ) {
                value = nrnran123_dblpick((nrnran123_State*)inst->rng[indexes[2*pnodecount + id]]);
            } else if (inst->rng[indexes[2*pnodecount + id]]) {
                #ifndef CORENEURON_BUILD
                value = nrn_random_pick(RANDCAST inst->rng[indexes[2*pnodecount + id]]);
                #endif
            } else {
                value = 0.0;
            }
            ret_urand = value;

        return ret_urand;
    }


    inline double brand_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double n, double p) {
        double ret_brand = 0.0;
        double result, count, success;
        success = 0.0;
        for (int count = 0; count <= (n - 1); count++) {
            double urand_in_0;
            {
                    double value;
                    if ( inst->usingR123[id] ) {
                        value = nrnran123_dblpick((nrnran123_State*)inst->rng[indexes[2*pnodecount + id]]);
                    } else if (inst->rng[indexes[2*pnodecount + id]]) {
                        #ifndef CORENEURON_BUILD
                        value = nrn_random_pick(RANDCAST inst->rng[indexes[2*pnodecount + id]]);
                        #endif
                    } else {
                        value = 0.0;
                    }
                    urand_in_0 = value;

            }
            result = urand_in_0;
            if (result <= p) {
                success = success + 1.0;
            }
        }
        ret_brand = success;
        return ret_brand;
    }


    inline double bbsavestate_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_bbsavestate = 0.0;
        ret_bbsavestate = 0.0;
            #ifndef CORENEURON_BUILD
                /* first arg is direction (0 save, 1 restore), second is array*/
                /* if first arg is -1, fill xdir with the size of the array */
                double *xdir, *xval;
                #ifndef NRN_VERSION_GTEQ_8_2_0
                double *hoc_pgetarg();
                long nrn_get_random_sequence(void* r);
                void nrn_set_random_sequence(void* r, int val);
                #endif
                xdir = hoc_pgetarg(1);
                xval = hoc_pgetarg(2);
                if (inst->rng[indexes[2*pnodecount + id]]) {
                    if (*xdir == -1) {  
                        if( inst->usingR123[id] ) {
                            *xdir = 2.0;
                        } else {
                            *xdir = 1.0;
                        }
                        return 0.0;
                    } else if(*xdir ==0 ) {  
                        if( inst->usingR123[id] ) {
                            uint32_t seq;
                            char which;
                            nrnran123_getseq( (nrnran123_State*)inst->rng[indexes[2*pnodecount + id]], &seq, &which );
                            xval[0] = (double) seq;
                            xval[1] = (double) which;
                        } else {
                            xval[0] = (double)nrn_get_random_sequence(RANDCAST inst->rng[indexes[2*pnodecount + id]]);
                        }
                    } else {  
                        if( inst->usingR123[id] ) {
                            nrnran123_setseq( (nrnran123_State*)inst->rng[indexes[2*pnodecount + id]], (uint32_t)xval[0], (char)xval[1] );
                        } else {
                            nrn_set_random_sequence(RANDCAST inst->rng[indexes[2*pnodecount + id]], (long)(xval[0]));
                        }
                    }
                }
            #endif

        return ret_bbsavestate;
    }


    static inline void net_send_buffering(const NrnThread* nt, NetSendBuffer_t* nsb, int type, int vdata_index, int weight_index, int point_index, double t, double flag) {
        int i = 0;
        i = nsb->_cnt++;
        if (i >= nsb->_size) {
            nsb->grow();
        }
        if (i < nsb->_size) {
            nsb->_sendtype[i] = type;
            nsb->_vdata_index[i] = vdata_index;
            nsb->_weight_index[i] = weight_index;
            nsb->_pnt_index[i] = point_index;
            nsb->_nsb_t[i] = t;
            nsb->_nsb_flag[i] = flag;
        }
    }


    /** initialize block for net receive */
    static void net_init(Point_process* pnt, int weight_index, double flag) {
        int tid = pnt->_tid;
        int id = pnt->_i_instance;
        double v = 0;
        NrnThread* nt = nrn_threads + tid;
        Memb_list* ml = nt->_ml_list[pnt->_type];
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        double* data = ml->data;
        double* weights = nt->weights;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);

        double* weight = weights + weight_index + 0;
        double* u = weights + weight_index + 1;
        double* tsyn = weights + weight_index + 2;
        double* recovered = weights + weight_index + 3;
        double* unrecovered = weights + weight_index + 4;
        double* nc_type = weights + weight_index + 5;
        (*weight) = 1.0;
        (*u) = 0.0;
        (*tsyn) = 0.0;
        if (inst->global->init_depleted) {
            (*recovered) = 0.0;
            (*unrecovered) = inst->Nrrp[id];
        } else {
            (*recovered) = inst->Nrrp[id];
            (*unrecovered) = 0.0;
        }
        if ((*nc_type) == 0.0) {
                        IvocVect *vv_delay_times = *((IvocVect**)(&inst->delay_times[indexes[3*pnodecount + id]]));
                        IvocVect *vv_delay_weights = *((IvocVect**)(&inst->delay_weights[indexes[4*pnodecount + id]]));
                        if (vv_delay_times && vector_capacity(vv_delay_times)>=1) {
                            double* deltm_el = vector_vec(vv_delay_times);
                            int delay_times_idx;
                            inst->next_delay[id] = 0;
                            for(delay_times_idx = 0; delay_times_idx < vector_capacity(vv_delay_times); ++delay_times_idx) {
                                double next_delay_t = deltm_el[delay_times_idx];

            net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[5*pnodecount+id], weight_index, inst->point_process[1*pnodecount+id], nt->_t+next_delay_t, 10.0);
                            }
                        }

        }
        auto& nsb = ml->_net_send_buffer;
    }


    static void nrn_watch_activate(GluSynapse_Instance* inst, int id, int pnodecount, int watch_id, double v, bool &watch_remove) {
        if (watch_remove == false) {
            inst->watch1[7*pnodecount+id] = 0;
            inst->watch2[8*pnodecount+id] = 0;
            inst->watch3[9*pnodecount+id] = 0;
            inst->watch4[10*pnodecount+id] = 0;
            watch_remove = true;
        }
        if (watch_id == 0) {
            inst->watch1[7*pnodecount+id] = 2 + (inst->effcai_GB[id]>inst->theta_d_GB[id]);
        }
        if (watch_id == 1) {
            inst->watch2[8*pnodecount+id] = 2 + (inst->effcai_GB[id]<inst->theta_d_GB[id]);
        }
        if (watch_id == 2) {
            inst->watch3[9*pnodecount+id] = 2 + (inst->effcai_GB[id]>inst->theta_p_GB[id]);
        }
        if (watch_id == 3) {
            inst->watch4[10*pnodecount+id] = 2 + (inst->effcai_GB[id]<inst->theta_p_GB[id]);
        }
    }


    /** routine to check watch activation */
    void nrn_watch_check_GluSynapse(NrnThread* nt, Memb_list* ml) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            bool watch_untriggered = true;
            if (inst->watch1[7*pnodecount+id]&2 && watch_untriggered) {
                if (inst->effcai_GB[id] > inst->theta_d_GB[id]) {
                    if ((inst->watch1[7*pnodecount+id]&1) == 0) {
                        watch_untriggered = false;
                        net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[5*pnodecount+id], -1, inst->point_process[1*pnodecount+id], nt->_t+0.0, 2.0);
                    }
                    inst->watch1[7*pnodecount+id] = 3;
                } else {
                    inst->watch1[7*pnodecount+id] = 2;
                }
            }
            if (inst->watch2[8*pnodecount+id]&2 && watch_untriggered) {
                if (inst->effcai_GB[id] < inst->theta_d_GB[id]) {
                    if ((inst->watch2[8*pnodecount+id]&1) == 0) {
                        watch_untriggered = false;
                        net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[5*pnodecount+id], -1, inst->point_process[1*pnodecount+id], nt->_t+0.0, 3.0);
                    }
                    inst->watch2[8*pnodecount+id] = 3;
                } else {
                    inst->watch2[8*pnodecount+id] = 2;
                }
            }
            if (inst->watch3[9*pnodecount+id]&2 && watch_untriggered) {
                if (inst->effcai_GB[id] > inst->theta_p_GB[id]) {
                    if ((inst->watch3[9*pnodecount+id]&1) == 0) {
                        watch_untriggered = false;
                        net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[5*pnodecount+id], -1, inst->point_process[1*pnodecount+id], nt->_t+0.0, 4.0);
                    }
                    inst->watch3[9*pnodecount+id] = 3;
                } else {
                    inst->watch3[9*pnodecount+id] = 2;
                }
            }
            if (inst->watch4[10*pnodecount+id]&2 && watch_untriggered) {
                if (inst->effcai_GB[id] < inst->theta_p_GB[id]) {
                    if ((inst->watch4[10*pnodecount+id]&1) == 0) {
                        watch_untriggered = false;
                        net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[5*pnodecount+id], -1, inst->point_process[1*pnodecount+id], nt->_t+0.0, 5.0);
                    }
                    inst->watch4[10*pnodecount+id] = 3;
                } else {
                    inst->watch4[10*pnodecount+id] = 2;
                }
            }
        }

        NetSendBuffer_t* nsb = ml->_net_send_buffer;
        for (int i=0; i < nsb->_cnt; i++) {
            int type = nsb->_sendtype[i];
            int tid = nt->id;
            double t = nsb->_nsb_t[i];
            double flag = nsb->_nsb_flag[i];
            int vdata_index = nsb->_vdata_index[i];
            int weight_index = nsb->_weight_index[i];
            int point_index = nsb->_pnt_index[i];
            net_sem_from_gpu(type, vdata_index, weight_index, tid, point_index, t, flag);
        }
        nsb->_cnt = 0;
    }


    static inline void net_receive_kernel_GluSynapse(double t, Point_process* pnt, GluSynapse_Instance* inst, NrnThread* nt, Memb_list* ml, int weight_index, double flag) {
        int tid = pnt->_tid;
        int id = pnt->_i_instance;
        double v = 0;
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        double* data = ml->data;
        double* weights = nt->weights;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        double* weight = weights + weight_index + 0;
        double* u = weights + weight_index + 1;
        double* tsyn = weights + weight_index + 2;
        double* recovered = weights + weight_index + 3;
        double* unrecovered = weights + weight_index + 4;
        double* nc_type = weights + weight_index + 5;
        inst->tsave[id] = t;
        bool watch_remove = false;
        {
            double p_rec, released, tp, factor, rec;
            if (inst->verbose[id] > 0.0) {
                printf("Time = %g ms, incoming spike at synapse %g\n", t, inst->synapseID[id]);
            }
            if (flag == 0.0) {
                if ((*weight) <= 0.0) {
                    if (inst->verbose[id] > 0.0) {
                        printf("Inactive synapse, weight = %g\n", (*weight));
                    }
                } else {
                    double brand_in_0, brand_in_1;
                    if (inst->verbose[id] > 0.0) {
                        printf("Flag 0, Regular spike\n");
                    }
                    (*u) = inst->Use_GB[id] + (*u) * (1.0 - inst->Use_GB[id]) * exp( -(t - (*tsyn)) / inst->Fac[id]);
                    if (inst->verbose[id] > 0.0) {
                        printf("\tVesicle release probability = %g\n", (*u));
                    }
                    p_rec = 1.0 - exp( -(t - (*tsyn)) / inst->Dep[id]);
                    if (inst->verbose[id] > 0.0) {
                        printf("\tVesicle recovery probability = %g\n", p_rec);
                    }
                    if (inst->verbose[id] > 0.0) {
                        printf("\tVesicle available before recovery = %g\n", (*recovered));
                    }
                    {
                        double result, count, success, n_in_0, p_in_0;
                        n_in_0 = (*unrecovered);
                        p_in_0 = p_rec;
                        success = 0.0;
                        for (int count = 0; count <= (n_in_0 - 1); count++) {
                            double urand_in_0;
                            {
                                    double value;
                                    if ( inst->usingR123[id] ) {
                                        value = nrnran123_dblpick((nrnran123_State*)inst->rng[indexes[2*pnodecount + id]]);
                                    } else if (inst->rng[indexes[2*pnodecount + id]]) {
                                        #ifndef CORENEURON_BUILD
                                        value = nrn_random_pick(RANDCAST inst->rng[indexes[2*pnodecount + id]]);
                                        #endif
                                    } else {
                                        value = 0.0;
                                    }
                                    urand_in_0 = value;

                            }
                            result = urand_in_0;
                            if (result <= p_in_0) {
                                success = success + 1.0;
                            }
                        }
                        brand_in_0 = success;
                    }
                    (*recovered) = (*recovered) + brand_in_0;
                    if (inst->verbose[id] > 0.0) {
                        printf("\tVesicles available after recovery = %g\n", (*recovered));
                    }
                    rec = (*recovered);
                    if (rec > 1.0 && inst->global->minis_single_vesicle && (*nc_type) == 1.0) {
                        rec = 1.0;
                    }
                    {
                        double result, count, success, n_in_1, p_in_1;
                        n_in_1 = rec;
                        p_in_1 = (*u);
                        success = 0.0;
                        for (int count = 0; count <= (n_in_1 - 1); count++) {
                            double urand_in_0;
                            {
                                    double value;
                                    if ( inst->usingR123[id] ) {
                                        value = nrnran123_dblpick((nrnran123_State*)inst->rng[indexes[2*pnodecount + id]]);
                                    } else if (inst->rng[indexes[2*pnodecount + id]]) {
                                        #ifndef CORENEURON_BUILD
                                        value = nrn_random_pick(RANDCAST inst->rng[indexes[2*pnodecount + id]]);
                                        #endif
                                    } else {
                                        value = 0.0;
                                    }
                                    urand_in_0 = value;

                            }
                            result = urand_in_0;
                            if (result <= p_in_1) {
                                success = success + 1.0;
                            }
                        }
                        brand_in_1 = success;
                    }
                    released = brand_in_1;
                    if (inst->verbose[id] > 0.0) {
                        printf("\tReleased %g vesicles out of %g\n", released, (*recovered));
                    }
                    (*recovered) = (*recovered) - released;
                    (*unrecovered) = inst->Nrrp[id] - (*recovered);
                    if (inst->verbose[id] > 0.0) {
                        printf("\tFinal vesicle count, Recovered = %g, Unrecovered = %g, Nrrp = %g\n", (*recovered), (*unrecovered), inst->Nrrp[id]);
                    }
                    tp = (inst->global->tau_r_AMPA * inst->tau_d_AMPA[id]) / (inst->tau_d_AMPA[id] - inst->global->tau_r_AMPA) * log(inst->tau_d_AMPA[id] / inst->global->tau_r_AMPA);
                    factor = 1.0 / ( -exp( -tp / inst->global->tau_r_AMPA) + exp( -tp / inst->tau_d_AMPA[id]));
                    inst->A_AMPA[id] = inst->A_AMPA[id] + released / inst->Nrrp[id] * factor;
                    inst->B_AMPA[id] = inst->B_AMPA[id] + released / inst->Nrrp[id] * factor;
                    tp = (inst->global->tau_r_NMDA * inst->global->tau_d_NMDA) / (inst->global->tau_d_NMDA - inst->global->tau_r_NMDA) * log(inst->global->tau_d_NMDA / inst->global->tau_r_NMDA);
                    factor = 1.0 / ( -exp( -tp / inst->global->tau_r_NMDA) + exp( -tp / inst->global->tau_d_NMDA));
                    inst->A_NMDA[id] = inst->A_NMDA[id] + released / inst->Nrrp[id] * factor;
                    inst->B_NMDA[id] = inst->B_NMDA[id] + released / inst->Nrrp[id] * factor;
                    (*tsyn) = t;
                }
            } else if (flag == 1.0) {
                if (inst->verbose[id] > 0.0) {
                    printf("Flag 1, Initialize watchers\n");
                }
                nrn_watch_activate(inst, id, pnodecount, 0, v, watch_remove);
                nrn_watch_activate(inst, id, pnodecount, 1, v, watch_remove);
                nrn_watch_activate(inst, id, pnodecount, 2, v, watch_remove);
                nrn_watch_activate(inst, id, pnodecount, 3, v, watch_remove);
            } else if (flag == 2.0) {
                if (inst->verbose[id] > 0.0) {
                    printf("Flag 2, Activate depression mechanisms\n");
                }
                inst->dep_GB[id] = 1.0;
            } else if (flag == 3.0) {
                if (inst->verbose[id] > 0.0) {
                    printf("Flag 3, Deactivate depression mechanisms\n");
                }
                inst->dep_GB[id] = 0.0;
            } else if (flag == 4.0) {
                if (inst->verbose[id] > 0.0) {
                    printf("Flag 4, Activate potentiation mechanisms\n");
                }
                inst->pot_GB[id] = 1.0;
            } else if (flag == 5.0) {
                if (inst->verbose[id] > 0.0) {
                    printf("Flag 5, Deactivate potentiation mechanisms\n");
                }
                inst->pot_GB[id] = 0.0;
            } else if (flag == 10.0) {
                        IvocVect *vv_delay_weights = *((IvocVect**)(&inst->delay_weights[indexes[4*pnodecount + id]]));
                        if (vv_delay_weights && vector_capacity(vv_delay_weights)>=inst->next_delay[id]) {
                            double* weights_v = vector_vec(vv_delay_weights);
                            double next_delay_weight = weights_v[(int)inst->next_delay[id]];

                (*weight) = inst->conductance[id] * next_delay_weight;
                inst->next_delay[id] = inst->next_delay[id] + 1.0;
                        }

            }
        }
    }


    static void net_receive_GluSynapse(Point_process* pnt, int weight_index, double flag) {
        NrnThread* nt = nrn_threads + pnt->_tid;
        Memb_list* ml = get_memb_list(nt);
        NetReceiveBuffer_t* nrb = ml->_net_receive_buffer;
        if (nrb->_cnt >= nrb->_size) {
            realloc_net_receive_buffer(nt, ml);
        }
        int id = nrb->_cnt;
        nrb->_pnt_index[id] = pnt-nt->pntprocs;
        nrb->_weight_index[id] = weight_index;
        nrb->_nrb_t[id] = nt->_t;
        nrb->_nrb_flag[id] = flag;
        nrb->_cnt++;
    }


    void net_buf_receive_GluSynapse(NrnThread* nt) {
        Memb_list* ml = get_memb_list(nt);
        if (!ml) {
            return;
        }

        NetReceiveBuffer_t* nrb = ml->_net_receive_buffer;
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);
        int count = nrb->_displ_cnt;
        #pragma omp simd
        #pragma ivdep
        for (int i = 0; i < count; i++) {
            int start = nrb->_displ[i];
            int end = nrb->_displ[i+1];
            for (int j = start; j < end; j++) {
                int index = nrb->_nrb_index[j];
                int offset = nrb->_pnt_index[index];
                double t = nrb->_nrb_t[index];
                int weight_index = nrb->_weight_index[index];
                double flag = nrb->_nrb_flag[index];
                Point_process* point_process = nt->pntprocs + offset;
                net_receive_kernel_GluSynapse(t, point_process, inst, nt, ml, weight_index, flag);
            }
        }
        nrb->_displ_cnt = 0;
        nrb->_cnt = 0;

        NetSendBuffer_t* nsb = ml->_net_send_buffer;
        for (int i=0; i < nsb->_cnt; i++) {
            int type = nsb->_sendtype[i];
            int tid = nt->id;
            double t = nsb->_nsb_t[i];
            double flag = nsb->_nsb_flag[i];
            int vdata_index = nsb->_vdata_index[i];
            int weight_index = nsb->_weight_index[i];
            int point_index = nsb->_pnt_index[i];
            net_sem_from_gpu(type, vdata_index, weight_index, tid, point_index, t, flag);
        }
        nsb->_cnt = 0;
    }


    /** initialize channel */
    void nrn_init_GluSynapse(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                inst->tsave[id] = -1e20;
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->A_AMPA[id] = inst->global->A_AMPA0;
                inst->B_AMPA[id] = inst->global->B_AMPA0;
                inst->gmax_AMPA[id] = inst->global->gmax_AMPA0;
                inst->A_NMDA[id] = inst->global->A_NMDA0;
                inst->B_NMDA[id] = inst->global->B_NMDA0;
                inst->Use_GB[id] = inst->global->Use_GB0;
                inst->m_VDCC[id] = inst->global->m_VDCC0;
                inst->h_VDCC[id] = inst->global->h_VDCC0;
                inst->cai_CR[id] = inst->global->cai_CR0;
                inst->rho_GB[id] = inst->global->rho_GB0;
                inst->effcai_GB[id] = inst->global->effcai_GB0;
                inst->A_AMPA[id] = 0.0;
                inst->B_AMPA[id] = 0.0;
                inst->gmax_AMPA[id] = inst->gmax0_AMPA[id];
                inst->A_NMDA[id] = 0.0;
                inst->B_NMDA[id] = 0.0;
                inst->Use_GB[id] = inst->Use[id];
                inst->cai_CR[id] = inst->global->min_ca_CR;
                inst->rho_GB[id] = inst->rho0_GB[id];
                inst->effcai_GB[id] = 0.0;
                inst->dep_GB[id] = 0.0;
                inst->pot_GB[id] = 0.0;
                inst->next_delay[id] =  -1.0;
                net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[5*pnodecount+id], 0, inst->point_process[1*pnodecount+id], nt->_t+0.0, 1.0);
            }
        }

        NetSendBuffer_t* nsb = ml->_net_send_buffer;
        for (int i=0; i < nsb->_cnt; i++) {
            int type = nsb->_sendtype[i];
            int tid = nt->id;
            double t = nsb->_nsb_t[i];
            double flag = nsb->_nsb_flag[i];
            int vdata_index = nsb->_vdata_index[i];
            int weight_index = nsb->_weight_index[i];
            int point_index = nsb->_pnt_index[i];
            net_sem_from_gpu(type, vdata_index, weight_index, tid, point_index, t, flag);
        }
        nsb->_cnt = 0;
    }


    inline double nrn_current_GluSynapse(int id, int pnodecount, GluSynapse_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        double Eca_syn, mggate, i_AMPA, i_NMDA, Pf_NMDA, gca_bar_abs_VDCC, gca_VDCC, nernst_in_0;
        inst->g_AMPA[id] = (1e-3) * inst->gmax_AMPA[id] * (inst->B_AMPA[id] - inst->A_AMPA[id]);
        i_AMPA = inst->g_AMPA[id] * (v - inst->global->E_AMPA);
        mggate = 1.0 / (1.0 + exp( -inst->global->slope_NMDA * v) * (inst->global->mg / inst->global->scale_NMDA));
        inst->g_NMDA[id] = (1e-3) * inst->gmax_NMDA[id] * mggate * (inst->B_NMDA[id] - inst->A_NMDA[id]);
        i_NMDA = inst->g_NMDA[id] * (v - inst->global->E_NMDA);
        Pf_NMDA = (4.0 * inst->global->cao_CR) / (4.0 * inst->global->cao_CR + (1.0 / 1.38) * 120.0) * 0.6;
        inst->ica_NMDA[id] = Pf_NMDA * inst->g_NMDA[id] * (v - 40.0);
        gca_bar_abs_VDCC = inst->global->gca_bar_VDCC * 4.0 * PI * pow((3.0 / 4.0 * inst->volume_CR[id] * 1.0 / PI), (2.0 / 3.0));
        gca_VDCC = (1e-3) * gca_bar_abs_VDCC * inst->m_VDCC[id] * inst->m_VDCC[id] * inst->h_VDCC[id];
        {
            double ci_in_0, co_in_0, z_in_0;
            ci_in_0 = inst->cai_CR[id];
            co_in_0 = inst->global->cao_CR;
            z_in_0 = 2.0;
            nernst_in_0 = (1000.0) * R * (*(inst->celsius) + 273.15) / (z_in_0 * FARADAY) * log(co_in_0 / ci_in_0);
            if (inst->verbose[id] > 1.0) {
                printf("nernst:%g R:%g temperature (c):%g \n", nernst_in_0, R, *(inst->celsius));
            }
        }
        Eca_syn = nernst_in_0;
        inst->ica_VDCC[id] = gca_VDCC * (v - Eca_syn);
        inst->vsyn[id] = v;
        inst->i[id] = i_AMPA + i_NMDA + inst->ica_VDCC[id];
        current += inst->i[id];
        return current;
    }


    /** update current */
    void nrn_cur_GluSynapse(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        double* shadow_rhs = nt->_shadow_rhs;
        double* shadow_d = nt->_shadow_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            double g = nrn_current_GluSynapse(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double rhs = nrn_current_GluSynapse(id, pnodecount, inst, data, indexes, thread, nt, v);
            g = (g-rhs)/0.001;
            double mfactor = 1.e2/inst->node_area[indexes[0*pnodecount + id]];
            g = g*mfactor;
            rhs = rhs*mfactor;
            #if NRN_PRCELLSTATE
            inst->g_unused[id] = g;
            #endif
            shadow_rhs[id] = rhs;
            shadow_d[id] = g;
        }
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            vec_rhs[node_id] -= shadow_rhs[id];
            vec_d[node_id] += shadow_d[id];
        }
    }


    /** update state */
    void nrn_state_GluSynapse(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<GluSynapse_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            double minf_VDCC, hinf_VDCC;
            inst->DA_AMPA[id] =  -inst->A_AMPA[id] / inst->global->tau_r_AMPA;
            inst->DB_AMPA[id] =  -inst->B_AMPA[id] / inst->tau_d_AMPA[id];
            inst->Dgmax_AMPA[id] = (inst->gmax_d_AMPA[id] + inst->rho_GB[id] * (inst->gmax_p_AMPA[id] - inst->gmax_d_AMPA[id]) - inst->gmax_AMPA[id]) / ((1e3) * inst->global->tau_exp_GB);
            inst->DA_NMDA[id] =  -inst->A_NMDA[id] / inst->global->tau_r_NMDA;
            inst->DB_NMDA[id] =  -inst->B_NMDA[id] / inst->global->tau_d_NMDA;
            inst->DUse_GB[id] = (inst->Use_d[id] + inst->rho_GB[id] * (inst->Use_p[id] - inst->Use_d[id]) - inst->Use_GB[id]) / ((1e3) * inst->global->tau_exp_GB);
            minf_VDCC = 1.0 / (1.0 + exp(((inst->global->vhm_VDCC - inst->global->ljp_VDCC) - v) / inst->global->km_VDCC));
            hinf_VDCC = 1.0 / (1.0 + exp(((inst->global->vhh_VDCC - inst->global->ljp_VDCC) - v) / inst->global->kh_VDCC));
            inst->Dm_VDCC[id] = (minf_VDCC - inst->m_VDCC[id]) / inst->global->mtau_VDCC;
            inst->Dh_VDCC[id] = (hinf_VDCC - inst->h_VDCC[id]) / inst->global->htau_VDCC;
            inst->Dcai_CR[id] =  -(1e-9) * (inst->ica_NMDA[id] + inst->ica_VDCC[id]) * inst->global->gamma_ca_CR / ((1e-15) * inst->volume_CR[id] * 2.0 * FARADAY) - (inst->cai_CR[id] - inst->global->min_ca_CR) / inst->global->tau_ca_CR;
            inst->Deffcai_GB[id] =  -inst->effcai_GB[id] / inst->global->tau_effca_GB + (inst->cai_CR[id] - inst->global->min_ca_CR);
            inst->Drho_GB[id] = ( -inst->rho_GB[id] * (1.0 - inst->rho_GB[id]) * (inst->global->rho_star_GB - inst->rho_GB[id]) + inst->pot_GB[id] * inst->global->gamma_p_GB * (1.0 - inst->rho_GB[id]) - inst->dep_GB[id] * inst->global->gamma_d_GB * inst->rho_GB[id]) / ((1e3) * inst->global->tau_ind_GB);
            inst->A_AMPA[id] = inst->A_AMPA[id] + nt->_dt * inst->DA_AMPA[id];
            inst->B_AMPA[id] = inst->B_AMPA[id] + nt->_dt * inst->DB_AMPA[id];
            inst->gmax_AMPA[id] = inst->gmax_AMPA[id] + nt->_dt * inst->Dgmax_AMPA[id];
            inst->A_NMDA[id] = inst->A_NMDA[id] + nt->_dt * inst->DA_NMDA[id];
            inst->B_NMDA[id] = inst->B_NMDA[id] + nt->_dt * inst->DB_NMDA[id];
            inst->Use_GB[id] = inst->Use_GB[id] + nt->_dt * inst->DUse_GB[id];
            inst->m_VDCC[id] = inst->m_VDCC[id] + nt->_dt * inst->Dm_VDCC[id];
            inst->h_VDCC[id] = inst->h_VDCC[id] + nt->_dt * inst->Dh_VDCC[id];
            inst->cai_CR[id] = inst->cai_CR[id] + nt->_dt * inst->Dcai_CR[id];
            inst->effcai_GB[id] = inst->effcai_GB[id] + nt->_dt * inst->Deffcai_GB[id];
            inst->rho_GB[id] = inst->rho_GB[id] + nt->_dt * inst->Drho_GB[id];
        }
    }


    /** register channel with the simulator */
    void _GluSynapse_reg() {

        int mech_type = nrn_get_mechtype("GluSynapse");
        GluSynapse_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        point_register_mech(mechanism_info, nrn_alloc_GluSynapse, nrn_cur_GluSynapse, nullptr, nrn_state_GluSynapse, nrn_init_GluSynapse, nrn_private_constructor_GluSynapse, nrn_private_destructor_GluSynapse, first_pointer_var_index(), nullptr, nullptr, 1);

        hoc_reg_bbcore_read(mech_type, bbcore_read);
        hoc_reg_bbcore_write(mech_type, bbcore_write);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "area");
        hoc_register_dparam_semantics(mech_type, 1, "pntproc");
        hoc_register_dparam_semantics(mech_type, 2, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 3, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 4, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 5, "netsend");
        hoc_register_dparam_semantics(mech_type, 6, "watch");
        hoc_register_dparam_semantics(mech_type, 7, "watch");
        hoc_register_dparam_semantics(mech_type, 8, "watch");
        hoc_register_dparam_semantics(mech_type, 9, "watch");
        hoc_register_dparam_semantics(mech_type, 10, "watch");
        hoc_register_watch_check(nrn_watch_check_GluSynapse, mech_type);
        hoc_register_net_receive_buffering(net_buf_receive_GluSynapse, mech_type);
        set_pnt_receive(mech_type, net_receive_GluSynapse, net_init, num_net_receive_args());
        hoc_register_net_send_buffering(mech_type);
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
