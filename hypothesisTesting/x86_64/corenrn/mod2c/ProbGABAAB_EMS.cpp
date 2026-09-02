/*********************************************************
Model Name      : ProbGABAAB_EMS
Filename        : ProbGABAAB_EMS.mod
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
        "ProbGABAAB_EMS",
        "tau_r_GABAA",
        "tau_d_GABAA",
        "tau_r_GABAB",
        "tau_d_GABAB",
        "Use",
        "Dep",
        "Fac",
        "e_GABAA",
        "e_GABAB",
        "u0",
        "Nrrp",
        "synapseID",
        "verboseLevel",
        "selected_for_report",
        "GABAB_ratio",
        "conductance",
        0,
        "i",
        "i_GABAA",
        "i_GABAB",
        "g_GABAA",
        "g_GABAB",
        "g",
        "unoccupied",
        "occupied",
        "tsyn",
        "u",
        "next_delay",
        0,
        "A_GABAA",
        "B_GABAA",
        "A_GABAB",
        "B_GABAB",
        0,
        "rng",
        "delay_times",
        "delay_weights",
        0
    };


    /** all global variables */
    struct ProbGABAAB_EMS_Store {
        int point_type{};
        double A_GABAA0{};
        double B_GABAA0{};
        double A_GABAB0{};
        double B_GABAB0{};
        int reset{};
        int mech_type{};
        double nc_type_param{4};
        double minis_single_vesicle{0};
        double init_depleted{0};
        double gmax{0.001};
        int slist1[4]{27, 28, 29, 30};
        int dlist1[4]{34, 35, 36, 37};
    };
    static_assert(std::is_trivially_copy_constructible_v<ProbGABAAB_EMS_Store>);
    static_assert(std::is_trivially_move_constructible_v<ProbGABAAB_EMS_Store>);
    static_assert(std::is_trivially_copy_assignable_v<ProbGABAAB_EMS_Store>);
    static_assert(std::is_trivially_move_assignable_v<ProbGABAAB_EMS_Store>);
    static_assert(std::is_trivially_destructible_v<ProbGABAAB_EMS_Store>);
    ProbGABAAB_EMS_Store ProbGABAAB_EMS_global;


    /** all mechanism instance variables and global variables */
    struct ProbGABAAB_EMS_Instance  {
        const double* tau_r_GABAA{};
        const double* tau_d_GABAA{};
        const double* tau_r_GABAB{};
        const double* tau_d_GABAB{};
        const double* Use{};
        const double* Dep{};
        const double* Fac{};
        const double* e_GABAA{};
        const double* e_GABAB{};
        const double* u0{};
        const double* Nrrp{};
        const double* synapseID{};
        double* verboseLevel{};
        const double* selected_for_report{};
        const double* GABAB_ratio{};
        const double* conductance{};
        double* i{};
        double* i_GABAA{};
        double* i_GABAB{};
        double* g_GABAA{};
        double* g_GABAB{};
        double* g{};
        double* unoccupied{};
        double* occupied{};
        double* tsyn{};
        double* u{};
        double* next_delay{};
        double* A_GABAA{};
        double* B_GABAA{};
        double* A_GABAB{};
        double* B_GABAB{};
        double* factor_GABAA{};
        double* factor_GABAB{};
        double* usingR123{};
        double* DA_GABAA{};
        double* DB_GABAA{};
        double* DA_GABAB{};
        double* DB_GABAB{};
        double* v_unused{};
        double* g_unused{};
        double* tsave{};
        const double* node_area{};
        const int* point_process{};
        void** rng{};
        void** delay_times{};
        void** delay_weights{};
        const int* tqitem{};
        ProbGABAAB_EMS_Store* global{&ProbGABAAB_EMS_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"nc_type_param_ProbGABAAB_EMS", &ProbGABAAB_EMS_global.nc_type_param},
        {"minis_single_vesicle_ProbGABAAB_EMS", &ProbGABAAB_EMS_global.minis_single_vesicle},
        {"init_depleted_ProbGABAAB_EMS", &ProbGABAAB_EMS_global.init_depleted},
        {"gmax_ProbGABAAB_EMS", &ProbGABAAB_EMS_global.gmax},
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
        return 5;
    }


    static inline int float_variables_size() {
        return 41;
    }


    static inline int int_variables_size() {
        return 6;
    }


    static inline int get_mech_type() {
        return ProbGABAAB_EMS_global.mech_type;
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
    static void nrn_private_constructor_ProbGABAAB_EMS(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new ProbGABAAB_EMS_Instance{};
        assert(inst->global == &ProbGABAAB_EMS_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(ProbGABAAB_EMS_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_ProbGABAAB_EMS(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<ProbGABAAB_EMS_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &ProbGABAAB_EMS_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(ProbGABAAB_EMS_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<ProbGABAAB_EMS_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &ProbGABAAB_EMS_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(ProbGABAAB_EMS_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->tau_r_GABAA = ml->data+0*pnodecount;
        inst->tau_d_GABAA = ml->data+1*pnodecount;
        inst->tau_r_GABAB = ml->data+2*pnodecount;
        inst->tau_d_GABAB = ml->data+3*pnodecount;
        inst->Use = ml->data+4*pnodecount;
        inst->Dep = ml->data+5*pnodecount;
        inst->Fac = ml->data+6*pnodecount;
        inst->e_GABAA = ml->data+7*pnodecount;
        inst->e_GABAB = ml->data+8*pnodecount;
        inst->u0 = ml->data+9*pnodecount;
        inst->Nrrp = ml->data+10*pnodecount;
        inst->synapseID = ml->data+11*pnodecount;
        inst->verboseLevel = ml->data+12*pnodecount;
        inst->selected_for_report = ml->data+13*pnodecount;
        inst->GABAB_ratio = ml->data+14*pnodecount;
        inst->conductance = ml->data+15*pnodecount;
        inst->i = ml->data+16*pnodecount;
        inst->i_GABAA = ml->data+17*pnodecount;
        inst->i_GABAB = ml->data+18*pnodecount;
        inst->g_GABAA = ml->data+19*pnodecount;
        inst->g_GABAB = ml->data+20*pnodecount;
        inst->g = ml->data+21*pnodecount;
        inst->unoccupied = ml->data+22*pnodecount;
        inst->occupied = ml->data+23*pnodecount;
        inst->tsyn = ml->data+24*pnodecount;
        inst->u = ml->data+25*pnodecount;
        inst->next_delay = ml->data+26*pnodecount;
        inst->A_GABAA = ml->data+27*pnodecount;
        inst->B_GABAA = ml->data+28*pnodecount;
        inst->A_GABAB = ml->data+29*pnodecount;
        inst->B_GABAB = ml->data+30*pnodecount;
        inst->factor_GABAA = ml->data+31*pnodecount;
        inst->factor_GABAB = ml->data+32*pnodecount;
        inst->usingR123 = ml->data+33*pnodecount;
        inst->DA_GABAA = ml->data+34*pnodecount;
        inst->DB_GABAA = ml->data+35*pnodecount;
        inst->DA_GABAB = ml->data+36*pnodecount;
        inst->DB_GABAB = ml->data+37*pnodecount;
        inst->v_unused = ml->data+38*pnodecount;
        inst->g_unused = ml->data+39*pnodecount;
        inst->tsave = ml->data+40*pnodecount;
        inst->node_area = nt->_data;
        inst->point_process = ml->pdata;
        inst->rng = nt->_vdata;
        inst->delay_times = nt->_vdata;
        inst->delay_weights = nt->_vdata;
        inst->tqitem = ml->pdata;
    }



    static void nrn_alloc_ProbGABAAB_EMS(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_ProbGABAAB_EMS(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<ProbGABAAB_EMS_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_ProbGABAAB_EMS(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<ProbGABAAB_EMS_Instance*>(ml->instance);

        #endif
    }


    inline double urand_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline double bbsavestate_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int setup_delay_vecs_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int setRNG_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int clearRNG_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int toggleVerbose_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
}


using namespace coreneuron;


#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#ifndef NRN_VERSION_GTEQ_8_2_0
#include "nrnran123.h"
#ifndef CORENEURON_BUILD
extern int ifarg(int iarg);
extern void* vector_arg(int iarg);
extern double* vector_vec(void* vv);
extern int vector_capacity(void* vv);
#endif
double nrn_random_pick(void* r);
void* nrn_random_arg(int argpos);
#define RANDCAST
#else
#define RANDCAST (Rand*)
#endif


static void bbcore_write(double* x, int* d, int* x_offset, int* d_offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
  IvocVect *vv_delay_times = *((IvocVect**)(&nt->_vdata[indexes[3*pnodecount + id]]));
  IvocVect *vv_delay_weights = *((IvocVect**)(&nt->_vdata[indexes[4*pnodecount + id]]));
  if (d) {
    uint32_t* di = ((uint32_t*)d) + *d_offset;
    nrnran123_State** pv = (nrnran123_State**)(&nt->_vdata[indexes[2*pnodecount + id]]);
    nrnran123_getids3(*pv, di, di+1, di+2);
    char which;
    nrnran123_getseq(*pv, di+3, &which);
    di[4] = (int)which;
  }
  *d_offset += 5;
  if (vv_delay_times && vv_delay_weights &&
     (vector_capacity(vv_delay_times) >= 1) && (vector_capacity(vv_delay_weights) >= 1)) {
    if (d) {
      uint32_t* di = ((uint32_t*)d) + *d_offset;
      di[0] = vector_capacity(vv_delay_times);
      di[1] = vector_capacity(vv_delay_weights);
    }
    if (x) {
      double* delay_times_el = vector_vec(vv_delay_times);
      double* delay_weights_el = vector_vec(vv_delay_weights);
      double* x_i = x + *x_offset;
      int delay_vecs_idx;
      int x_idx = 0;
      for(delay_vecs_idx = 0; delay_vecs_idx < vector_capacity(vv_delay_times); ++delay_vecs_idx) {
         x_i[x_idx++] = delay_times_el[delay_vecs_idx];
         x_i[x_idx++] = delay_weights_el[delay_vecs_idx];
      }
    }
    *x_offset += vector_capacity(vv_delay_times) + vector_capacity(vv_delay_weights);
  } else {
    if (d) {
      uint32_t* di = ((uint32_t*)d) + *d_offset;
      di[0] = 0;
      di[1] = 0;
    }
  }
  *d_offset += 2;
}
static void bbcore_read(double* x, int* d, int* x_offset, int* d_offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
  uint32_t* di = ((uint32_t*)d) + *d_offset;
  if (di[0] != 0 || di[1] != 0 || di[2] != 0) {
      nrnran123_State** pv = (nrnran123_State**)(&nt->_vdata[indexes[2*pnodecount + id]]);
#if !NRNBBCORE
      if(*pv) {
          nrnran123_deletestream(*pv);
      }
#endif
      *pv = nrnran123_newstream3(di[0], di[1], di[2]);
      char which = (char)di[4];
      nrnran123_setseq(*pv, di[3], which);
  }
  int delay_times_sz = di[5];
  int delay_weights_sz = di[6];
  *d_offset += 7;
  if ((delay_times_sz > 0) && (delay_weights_sz > 0)) {
    double* x_i = x + *x_offset;
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
    *x_offset += delay_times_sz + delay_weights_sz;
  }
}


namespace coreneuron {


    inline int setup_delay_vecs_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
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


    inline int setRNG_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
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


    inline int clearRNG_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
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


    inline int toggleVerbose_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_toggleVerbose = 0;
        inst->verboseLevel[id] = 1.0 - inst->verboseLevel[id];
        return ret_toggleVerbose;
    }


    inline double urand_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_urand = 0.0;
            double value = 0.0;
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


    inline double bbsavestate_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
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
        auto* const inst = static_cast<ProbGABAAB_EMS_Instance*>(ml->instance);

        double* nc_type = weights + weight_index + 4;
        if ((*nc_type) == 0.0) {
                        IvocVect *vv_delay_times = *((IvocVect**)(&inst->delay_times[indexes[3*pnodecount + id]]));
                        IvocVect *vv_delay_weights = *((IvocVect**)(&inst->delay_weights[indexes[4*pnodecount + id]]));
                        if (vv_delay_times && vector_capacity(vv_delay_times)>=1) {
                          double* deltm_el = vector_vec(vv_delay_times);
                          int delay_times_idx;
                          inst->next_delay[id] = 0;
                          for (delay_times_idx = 0; delay_times_idx < vector_capacity(vv_delay_times); ++delay_times_idx) {
                            double next_delay_t = deltm_el[delay_times_idx];

            net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[5*pnodecount+id], weight_index, inst->point_process[1*pnodecount+id], nt->_t+next_delay_t, 1.0);
                          }
                        }

        }
        auto& nsb = ml->_net_send_buffer;
    }


    static inline void net_receive_kernel_ProbGABAAB_EMS(double t, Point_process* pnt, ProbGABAAB_EMS_Instance* inst, NrnThread* nt, Memb_list* ml, int weight_index, double flag) {
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
        double* weight_GABAA = weights + weight_index + 1;
        double* weight_GABAB = weights + weight_index + 2;
        double* Psurv = weights + weight_index + 3;
        double* nc_type = weights + weight_index + 4;
        inst->tsave[id] = t;
        {
            double result, ves, occu;
            (*weight_GABAA) = (*weight);
            (*weight_GABAB) = (*weight) * inst->GABAB_ratio[id];
            if (flag == 1.0) {
                        IvocVect *vv_delay_weights = *((IvocVect**)(&inst->delay_weights[indexes[4*pnodecount + id]]));
                        if (vv_delay_weights && vector_capacity(vv_delay_weights)>=inst->next_delay[id]) {
                          double* weights_v = vector_vec(vv_delay_weights);
                          double next_delay_weight = weights_v[(int)inst->next_delay[id]];

                (*weight) = inst->conductance[id] * next_delay_weight;
                inst->next_delay[id] = inst->next_delay[id] + 1.0;
                        }
                        return;

            }
            if ((*weight) <= 0.0 || t < 0.0) {
                        return;

            }
            if (inst->Fac[id] > 0.0) {
                inst->u[id] = inst->u[id] * exp( -(t - inst->tsyn[id]) / inst->Fac[id]);
            } else {
                inst->u[id] = inst->Use[id];
            }
            if (inst->Fac[id] > 0.0) {
                inst->u[id] = inst->u[id] + inst->Use[id] * (1.0 - inst->u[id]);
            }
            for (int counter = 0; counter <= (inst->unoccupied[id] - 1); counter++) {
                double urand_in_0;
                (*Psurv) = exp( -(t - inst->tsyn[id]) / inst->Dep[id]);
                {
                        double value = 0.0;
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
                if (result > (*Psurv)) {
                    inst->occupied[id] = inst->occupied[id] + 1.0;
                    if (inst->verboseLevel[id] > 0.0) {
                        printf("[Syn %.0f] Recovered! t = %g, Psurv = %g, urand = %g\n", inst->synapseID[id], t, (*Psurv), result);
                    }
                }
            }
            ves = 0.0;
            occu = inst->occupied[id];
            if (occu > 1.0 && inst->global->minis_single_vesicle && (*nc_type) == 1.0) {
                occu = 1.0;
            }
            for (int counter = 0; counter <= (occu - 1); counter++) {
                double urand_in_1;
                {
                        double value = 0.0;
                        if ( inst->usingR123[id] ) {
                            value = nrnran123_dblpick((nrnran123_State*)inst->rng[indexes[2*pnodecount + id]]);
                        } else if (inst->rng[indexes[2*pnodecount + id]]) {
                            #ifndef CORENEURON_BUILD
                            value = nrn_random_pick(RANDCAST inst->rng[indexes[2*pnodecount + id]]);
                            #endif
                        } else {
                            value = 0.0;
                        }
                        urand_in_1 = value;

                }
                result = urand_in_1;
                if (result < inst->u[id]) {
                    inst->occupied[id] = inst->occupied[id] - 1.0;
                    ves = ves + 1.0;
                }
            }
            inst->unoccupied[id] = inst->Nrrp[id] - inst->occupied[id];
            inst->tsyn[id] = t;
            if (ves > 0.0) {
                inst->A_GABAA[id] = inst->A_GABAA[id] + ves / inst->Nrrp[id] * (*weight_GABAA) * inst->factor_GABAA[id];
                inst->B_GABAA[id] = inst->B_GABAA[id] + ves / inst->Nrrp[id] * (*weight_GABAA) * inst->factor_GABAA[id];
                inst->A_GABAB[id] = inst->A_GABAB[id] + ves / inst->Nrrp[id] * (*weight_GABAB) * inst->factor_GABAB[id];
                inst->B_GABAB[id] = inst->B_GABAB[id] + ves / inst->Nrrp[id] * (*weight_GABAB) * inst->factor_GABAB[id];
                if (inst->verboseLevel[id] > 0.0) {
                    printf("[Syn %.0f] Release! t = %g, vals: %g %g %g %g\n", inst->synapseID[id], t, inst->A_GABAA[id], (*weight_GABAA), inst->factor_GABAA[id], (*weight));
                }
            } else {
                if (inst->verboseLevel[id] > 0.0) {
                    printf("[Syn %.0f] Failure! t = %g: urand = %g\n", inst->synapseID[id], t, result);
                }
            }
        }
    }


    static void net_receive_ProbGABAAB_EMS(Point_process* pnt, int weight_index, double flag) {
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


    void net_buf_receive_ProbGABAAB_EMS(NrnThread* nt) {
        Memb_list* ml = get_memb_list(nt);
        if (!ml) {
            return;
        }

        NetReceiveBuffer_t* nrb = ml->_net_receive_buffer;
        auto* const inst = static_cast<ProbGABAAB_EMS_Instance*>(ml->instance);
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
                net_receive_kernel_ProbGABAAB_EMS(t, point_process, inst, nt, ml, weight_index, flag);
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
    void nrn_init_ProbGABAAB_EMS(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<ProbGABAAB_EMS_Instance*>(ml->instance);

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
                inst->A_GABAA[id] = inst->global->A_GABAA0;
                inst->B_GABAA[id] = inst->global->B_GABAA0;
                inst->A_GABAB[id] = inst->global->A_GABAB0;
                inst->B_GABAB[id] = inst->global->B_GABAB0;
                double tp_GABAA, tp_GABAB;
                inst->tsyn[id] = 0.0;
                inst->u[id] = inst->u0[id];
                if (inst->global->init_depleted) {
                    inst->unoccupied[id] = inst->Nrrp[id];
                    inst->occupied[id] = 0.0;
                } else {
                    inst->unoccupied[id] = 0.0;
                    inst->occupied[id] = inst->Nrrp[id];
                }
                inst->A_GABAA[id] = 0.0;
                inst->B_GABAA[id] = 0.0;
                inst->A_GABAB[id] = 0.0;
                inst->B_GABAB[id] = 0.0;
                tp_GABAA = (inst->tau_r_GABAA[id] * inst->tau_d_GABAA[id]) / (inst->tau_d_GABAA[id] - inst->tau_r_GABAA[id]) * log(inst->tau_d_GABAA[id] / inst->tau_r_GABAA[id]);
                tp_GABAB = (inst->tau_r_GABAB[id] * inst->tau_d_GABAB[id]) / (inst->tau_d_GABAB[id] - inst->tau_r_GABAB[id]) * log(inst->tau_d_GABAB[id] / inst->tau_r_GABAB[id]);
                inst->factor_GABAA[id] =  -exp( -tp_GABAA / inst->tau_r_GABAA[id]) + exp( -tp_GABAA / inst->tau_d_GABAA[id]);
                inst->factor_GABAA[id] = 1.0 / inst->factor_GABAA[id];
                inst->factor_GABAB[id] =  -exp( -tp_GABAB / inst->tau_r_GABAB[id]) + exp( -tp_GABAB / inst->tau_d_GABAB[id]);
                inst->factor_GABAB[id] = 1.0 / inst->factor_GABAB[id];
                        if( inst->usingR123[id] ) {
                            nrnran123_setseq((nrnran123_State*)inst->rng[indexes[2*pnodecount + id]], 0, 0);
                        }

                inst->next_delay[id] =  -1.0;
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


    inline double nrn_current_ProbGABAAB_EMS(int id, int pnodecount, ProbGABAAB_EMS_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->g_GABAA[id] = inst->global->gmax * (inst->B_GABAA[id] - inst->A_GABAA[id]);
        inst->g_GABAB[id] = inst->global->gmax * (inst->B_GABAB[id] - inst->A_GABAB[id]);
        inst->g[id] = inst->g_GABAA[id] + inst->g_GABAB[id];
        inst->i_GABAA[id] = inst->g_GABAA[id] * (v - inst->e_GABAA[id]);
        inst->i_GABAB[id] = inst->g_GABAB[id] * (v - inst->e_GABAB[id]);
        inst->i[id] = inst->i_GABAA[id] + inst->i_GABAB[id];
        current += inst->i[id];
        return current;
    }


    /** update current */
    void nrn_cur_ProbGABAAB_EMS(NrnThread* nt, Memb_list* ml, int type) {
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
        auto* const inst = static_cast<ProbGABAAB_EMS_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            double g = nrn_current_ProbGABAAB_EMS(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double rhs = nrn_current_ProbGABAAB_EMS(id, pnodecount, inst, data, indexes, thread, nt, v);
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
    void nrn_state_ProbGABAAB_EMS(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<ProbGABAAB_EMS_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->A_GABAA[id] = inst->A_GABAA[id] + (1.0 - exp(nt->_dt * (( -1.0) / inst->tau_r_GABAA[id]))) * ( -(0.0) / (( -1.0) / inst->tau_r_GABAA[id]) - inst->A_GABAA[id]);
            inst->B_GABAA[id] = inst->B_GABAA[id] + (1.0 - exp(nt->_dt * (( -1.0) / inst->tau_d_GABAA[id]))) * ( -(0.0) / (( -1.0) / inst->tau_d_GABAA[id]) - inst->B_GABAA[id]);
            inst->A_GABAB[id] = inst->A_GABAB[id] + (1.0 - exp(nt->_dt * (( -1.0) / inst->tau_r_GABAB[id]))) * ( -(0.0) / (( -1.0) / inst->tau_r_GABAB[id]) - inst->A_GABAB[id]);
            inst->B_GABAB[id] = inst->B_GABAB[id] + (1.0 - exp(nt->_dt * (( -1.0) / inst->tau_d_GABAB[id]))) * ( -(0.0) / (( -1.0) / inst->tau_d_GABAB[id]) - inst->B_GABAB[id]);
        }
    }


    /** register channel with the simulator */
    void _ProbGABAAB_EMS_reg() {

        int mech_type = nrn_get_mechtype("ProbGABAAB_EMS");
        ProbGABAAB_EMS_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        point_register_mech(mechanism_info, nrn_alloc_ProbGABAAB_EMS, nrn_cur_ProbGABAAB_EMS, nullptr, nrn_state_ProbGABAAB_EMS, nrn_init_ProbGABAAB_EMS, nrn_private_constructor_ProbGABAAB_EMS, nrn_private_destructor_ProbGABAAB_EMS, first_pointer_var_index(), nullptr, nullptr, 1);

        hoc_reg_bbcore_read(mech_type, bbcore_read);
        hoc_reg_bbcore_write(mech_type, bbcore_write);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "area");
        hoc_register_dparam_semantics(mech_type, 1, "pntproc");
        hoc_register_dparam_semantics(mech_type, 2, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 3, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 4, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 5, "netsend");
        hoc_register_net_receive_buffering(net_buf_receive_ProbGABAAB_EMS, mech_type);
        set_pnt_receive(mech_type, net_receive_ProbGABAAB_EMS, net_init, num_net_receive_args());
        hoc_register_net_send_buffering(mech_type);
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
