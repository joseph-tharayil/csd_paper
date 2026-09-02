/*********************************************************
Model Name      : InhPoissonStim
Filename        : netstim_inhpoisson.mod
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
        "InhPoissonStim",
        "duration",
        0,
        "rmax",
        0,
        0,
        "uniform_rng",
        "exp_rng",
        "vecRate",
        "vecTbins",
        0
    };


    /** all global variables */
    struct InhPoissonStim_Store {
        int point_type{};
        int reset{};
        int mech_type{};
        double interval_min{1};
    };
    static_assert(std::is_trivially_copy_constructible_v<InhPoissonStim_Store>);
    static_assert(std::is_trivially_move_constructible_v<InhPoissonStim_Store>);
    static_assert(std::is_trivially_copy_assignable_v<InhPoissonStim_Store>);
    static_assert(std::is_trivially_move_assignable_v<InhPoissonStim_Store>);
    static_assert(std::is_trivially_destructible_v<InhPoissonStim_Store>);
    InhPoissonStim_Store InhPoissonStim_global;


    /** all mechanism instance variables and global variables */
    struct InhPoissonStim_Instance  {
        double* duration{};
        double* rmax{};
        double* index{};
        double* curRate{};
        double* start{};
        double* event{};
        double* usingR123{};
        double* activeFlag{};
        double* v_unused{};
        double* tsave{};
        const double* node_area{};
        void** point_process{};
        void** uniform_rng{};
        void** exp_rng{};
        void** vecRate{};
        void** vecTbins{};
        void** tqitem{};
        InhPoissonStim_Store* global{&InhPoissonStim_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"interval_min_InhPoissonStim", &InhPoissonStim_global.interval_min},
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
        return 1;
    }


    static inline int float_variables_size() {
        return 10;
    }


    static inline int int_variables_size() {
        return 7;
    }


    static inline int get_mech_type() {
        return InhPoissonStim_global.mech_type;
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
    static void nrn_private_constructor_InhPoissonStim(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new InhPoissonStim_Instance{};
        assert(inst->global == &InhPoissonStim_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(InhPoissonStim_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_InhPoissonStim(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<InhPoissonStim_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &InhPoissonStim_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(InhPoissonStim_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<InhPoissonStim_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &InhPoissonStim_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(InhPoissonStim_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->duration = ml->data+0*pnodecount;
        inst->rmax = ml->data+1*pnodecount;
        inst->index = ml->data+2*pnodecount;
        inst->curRate = ml->data+3*pnodecount;
        inst->start = ml->data+4*pnodecount;
        inst->event = ml->data+5*pnodecount;
        inst->usingR123 = ml->data+6*pnodecount;
        inst->activeFlag = ml->data+7*pnodecount;
        inst->v_unused = ml->data+8*pnodecount;
        inst->tsave = ml->data+9*pnodecount;
        inst->node_area = nt->_data;
        inst->point_process = nt->_vdata;
        inst->uniform_rng = nt->_vdata;
        inst->exp_rng = nt->_vdata;
        inst->vecRate = nt->_vdata;
        inst->vecTbins = nt->_vdata;
        inst->tqitem = nt->_vdata;
    }



    static void nrn_alloc_InhPoissonStim(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_InhPoissonStim(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<InhPoissonStim_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_InhPoissonStim(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<InhPoissonStim_Instance*>(ml->instance);

        #endif
    }


    inline double urand_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline double erand_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline double getPostRestoreFlag_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline double resumeEvent_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int generate_next_event_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int setRNGs_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int setTbins_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int setRate_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int update_time_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int restartEvent_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
}


using namespace coreneuron;


#if defined(NRN_VERSION_GTEQ)
#if NRN_VERSION_GTEQ(9,0,0)
#define NRN_VERSION_GTEQ_9_0_0
#endif
#endif
#ifndef NRN_VERSION_GTEQ_8_2_0
extern int ifarg(int iarg);
#ifndef CORENEURON_BUILD
extern double* vector_vec(void* vv);
extern void* vector_new1(int _i);
extern int vector_capacity(void* vv);
extern void* vector_arg(int iarg);
double nrn_random_pick(void* r);
#endif
void* nrn_random_arg(int argpos);
#define RANDCAST
#else
#define RANDCAST (Rand*)
#endif
#ifdef STIM_DEBUG
# define debug_printf(...) printf(__VA_ARGS__)
#else
# define debug_printf(...)
#endif
const int POST_RESTORE_RESTART_FLAG = -99;


#include "nrnran123.h"


static void bbcore_write(double* dArray, int* iArray, int* doffset, int* ioffset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
        uint32_t dsize = 0;
        if (nt->_vdata[indexes[4*pnodecount + id]])
        {
          dsize = (uint32_t)vector_capacity((IvocVect*)nt->_vdata[indexes[4*pnodecount + id]]);
        }
        if (iArray) {
                uint32_t* ia = ((uint32_t*)iArray) + *ioffset;
                nrnran123_State** pv = (nrnran123_State**)(&nt->_vdata[indexes[3*pnodecount + id]]);
                nrnran123_getids3(*pv, ia, ia+1, ia+2);
                char which;
                nrnran123_getseq(*pv, ia+3, &which);
                ia[4] = (int)which;
                ia = ia + 5;
                pv = (nrnran123_State**)(&nt->_vdata[indexes[2*pnodecount + id]]);
                nrnran123_getids3( *pv, ia, ia+1, ia+2);
                nrnran123_getseq(*pv, ia+3, &which);
                ia[4] = (int)which;
                ia = ia + 5;
                IvocVect* vec = (IvocVect*)nt->_vdata[indexes[4*pnodecount + id]];
                ia[0] = dsize;
                double *da = dArray + *doffset;
                double *dv;
                if(dsize)
                {
                  dv = vector_vec(vec);
                }
                int iInt;
                for (iInt = 0; iInt < dsize; ++iInt)
                {
                  da[iInt] = dv[iInt];
                }
                vec = (IvocVect*)nt->_vdata[indexes[5*pnodecount + id]];
                da = dArray + *doffset + dsize;
                if(dsize)
                {
                  dv = vector_vec(vec);
                }
                for (iInt = 0; iInt < dsize; ++iInt)
                {
                  da[iInt] = dv[iInt];
                }
        }
        *ioffset += 11;
        *doffset += 2*dsize;
}
static void bbcore_read(double* dArray, int* iArray, int* doffset, int* ioffset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
        uint32_t* ia = ((uint32_t*)iArray) + *ioffset;
        nrnran123_State** pv;
        if (ia[0] != 0 || ia[1] != 0)
        {
          pv = (nrnran123_State**)(&nt->_vdata[indexes[3*pnodecount + id]]);
#if !NRNBBCORE
          if(*pv) {
              nrnran123_deletestream(*pv);
          }
#endif
          *pv = nrnran123_newstream3(ia[0], ia[1], ia[2] );
          nrnran123_setseq(*pv, ia[3], (char)ia[4]);
        }
        ia = ia + 5;
        if (ia[0] != 0 || ia[1] != 0)
        {
          pv = (nrnran123_State**)(&nt->_vdata[indexes[2*pnodecount + id]]);
#if !NRNBBCORE
          if(*pv) {
            nrnran123_deletestream(*pv);
          }
#endif
          *pv = nrnran123_newstream3(ia[0], ia[1], ia[2] );
          nrnran123_setseq(*pv, ia[2], (char)ia[3]);
        }
        ia = ia + 5;
        int dsize = ia[0];
        *ioffset += 11;
        double *da = dArray + *doffset;
        if(!nt->_vdata[indexes[4*pnodecount + id]]) {
          nt->_vdata[indexes[4*pnodecount + id]] = (double*)vector_new1(dsize);  /* works for dsize=0 */
        }
        assert(dsize == vector_capacity((IvocVect*)nt->_vdata[indexes[4*pnodecount + id]]));
        double *dv = vector_vec((IvocVect*)nt->_vdata[indexes[4*pnodecount + id]]);
        int iInt;
        for (iInt = 0; iInt < dsize; ++iInt)
        {
          dv[iInt] = da[iInt];
        }
        *doffset += dsize;
        da = dArray + *doffset;
        if(!nt->_vdata[indexes[5*pnodecount + id]]) {
          nt->_vdata[indexes[5*pnodecount + id]] = (double*)vector_new1(dsize);
        }
        assert(dsize == vector_capacity((IvocVect*)nt->_vdata[indexes[5*pnodecount + id]]));
        dv = vector_vec((IvocVect*)nt->_vdata[indexes[5*pnodecount + id]]);
        for (iInt = 0; iInt < dsize; ++iInt)
        {
          dv[iInt] = da[iInt];
        }
        *doffset += dsize;
}


namespace coreneuron {


    inline int generate_next_event_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_generate_next_event = 0;
        double erand_in_1;
        {
            	if (inst->exp_rng[indexes[3*pnodecount + id]]) {
            		/*
            		:Supports separate independent but reproducible streams for
            		: each instance. However, the corresponding hoc Random
            		: distribution MUST be set to Random.negexp(1)
            		*/
                        if( inst->usingR123[id] ) {
            		erand_in_1 = nrnran123_negexp((nrnran123_State*)inst->exp_rng[indexes[3*pnodecount + id]]);
                        } else {
            #ifndef CORENEURON_BUILD
            		erand_in_1 = nrn_random_pick(RANDCAST inst->exp_rng[indexes[3*pnodecount + id]]);
            #endif
                        }
            	}else{
              	  hoc_execerror("multithread random in NetStim"," only via hoc Random");
            	}

        }
        inst->event[id] = 1000.0 / inst->rmax[id] * erand_in_1;
        if (inst->event[id] < 0.0) {
            inst->event[id] = 0.0;
        }
        return ret_generate_next_event;
    }


    inline int setRNGs_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_setRNGs = 0;
        {
        #ifndef CORENEURON_BUILD
            if( ifarg(1) && hoc_is_double_arg(1) ) {
                nrnran123_State** pv = (nrnran123_State**)(&inst->exp_rng[indexes[3*pnodecount + id]]);
                if (*pv) {
                    nrnran123_deletestream(*pv);
                    *pv = (nrnran123_State*)0;
                }
                *pv = nrnran123_newstream3((uint32_t)*getarg(1), (uint32_t)*getarg(2), (uint32_t)*getarg(3));
                pv = (nrnran123_State**)(&inst->uniform_rng[indexes[2*pnodecount + id]]);
                if (*pv) {
                    nrnran123_deletestream(*pv);
                    *pv = (nrnran123_State*)0;
                }
                *pv = nrnran123_newstream3((uint32_t)*getarg(4), (uint32_t)*getarg(5), (uint32_t)*getarg(6));
                inst->usingR123[id] = 1;
            } else if( ifarg(1) ) {
                void** pv = (void**)(&inst->exp_rng[indexes[3*pnodecount + id]]);
                *pv = nrn_random_arg(1);
                pv = (void**)(&inst->uniform_rng[indexes[2*pnodecount + id]]);
                *pv = nrn_random_arg(2);
                inst->usingR123[id] = 0;
            } else {
                if( inst->usingR123[id] ) {
                    nrnran123_State** pv = (nrnran123_State**)(&inst->exp_rng[indexes[3*pnodecount + id]]);
                    nrnran123_deletestream(*pv);
                    *pv = (nrnran123_State*)0;
                    pv = (nrnran123_State**)(&inst->uniform_rng[indexes[2*pnodecount + id]]);
                    nrnran123_deletestream(*pv);
                    *pv = (nrnran123_State*)0;
                }
            }
        #endif
        }

        return ret_setRNGs;
    }


    inline int setTbins_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_setTbins = 0;
          #ifndef CORENEURON_BUILD
          IvocVect** vv;
          vv = (IvocVect**)(&inst->vecTbins[indexes[5*pnodecount + id]]);
          *vv = (IvocVect*)0;
          if (ifarg(1)) {
            *vv = vector_arg(1);
            /*int size = vector_capacity(*vv);
            int i;
            double* px = vector_vec(*vv);
            for (i=0;i<size;i++) {
              printf("%f ", px[i]);
            }*/
          }
          #endif

        return ret_setTbins;
    }


    inline int setRate_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_setRate = 0;
          #ifndef CORENEURON_BUILD
          IvocVect** vv;
          vv = (IvocVect**)(&inst->vecRate[indexes[4*pnodecount + id]]);
          *vv = (IvocVect*)0;
          if (ifarg(1)) {
            *vv = vector_arg(1);
            int size = vector_capacity(*vv);
            int i;
            double max=0.0;
            double* px = vector_vec(*vv);
            for (i=0;i<size;i++) {
            	if (px[i]>max) max = px[i];
            }
            inst->curRate[id] = px[0];
            inst->rmax[id] = max;
            inst->activeFlag[id] = inst->activeFlag[id] + 1;
          }
          #endif

        return ret_setRate;
    }


    inline int update_time_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_update_time = 0;
          IvocVect* vv; int i, i_prev, size; double* px;
          i = (int)inst->index[id];
          i_prev = i;
          if (i >= 0) { 
            vv = *((IvocVect**)(&inst->vecTbins[indexes[5*pnodecount + id]]));
            if (vv) {
              size = vector_capacity(vv);
              px = vector_vec(vv);
              /* advance to current tbins without exceeding array bounds */
              while ((i+1 < size) && (nt->_t>=px[i+1])) {
        	inst->index[id] += 1.;
        	i += 1;
              }
              /* did the index change? */
              if (i!=i_prev) {
                /* advance curRate to next vecRate if possible */
                IvocVect *vvRate = *((IvocVect**)(&inst->vecRate[indexes[4*pnodecount + id]]));
                if (vvRate && vector_capacity(vvRate)>i) {
                  px = vector_vec(vvRate);
                  inst->curRate[id] = px[i];
                }
                else inst->curRate[id] = 1.0;
              }
              /* have we hit last bin? ... disable time advancing leaving curRate as it is*/
              if (i==size)
                inst->index[id] = -1.;
            } else { /* no vecTbins, use some defaults */
              inst->rmax[id] = 1.0;
              inst->curRate[id] = 1.0;
              inst->index[id] = -1.; /* no vecTbins ... disable time advancing & Poisson unit rate. */
            }
          }

        return ret_update_time;
    }


    inline int restartEvent_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_restartEvent = 0;
        #ifndef CORENEURON_BUILD
            double etime = resumeEvent_InhPoissonStim(id, pnodecount, inst, data, indexes, thread, nt, v);
            if (etime < inst->start[id]+inst->duration[id]) {
                debug_printf("InhPoisson: First event after resume at t = %6.3f\n", etime);
                #if defined(NRN_VERSION_GTEQ_9_0_0)
                artcell_net_send(&nt->_vdata[indexes[6*pnodecount + id]], (double*)0, indexes[1].get<Point_process*>(), etime, inst->activeFlag[id]);
                #else
                artcell_net_send(&nt->_vdata[indexes[6*pnodecount + id]], (double*)0, (Point_process*)indexes[1]._pvoid, etime, inst->activeFlag[id]);
                #endif
            }
        #endif

        return ret_restartEvent;
    }


    inline double urand_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_urand = 0.0;
        	if (inst->uniform_rng[indexes[2*pnodecount + id]]) {
        		/*
        		:Supports separate independent but reproducible streams for
        		: each instance. However, the corresponding hoc Random
        		: distribution MUST be set to Random.uniform(0,1)
        		*/
                    if( inst->usingR123[id] ) {
        		ret_urand = nrnran123_dblpick((nrnran123_State*)inst->uniform_rng[indexes[2*pnodecount + id]]);
                    } else {
        #ifndef CORENEURON_BUILD
        		ret_urand = nrn_random_pick(RANDCAST inst->uniform_rng[indexes[2*pnodecount + id]]);
        #endif
                    }
        	}else{
          	  hoc_execerror("multithread random in NetStim"," only via hoc Random");
        	}

        return ret_urand;
    }


    inline double erand_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_erand = 0.0;
        	if (inst->exp_rng[indexes[3*pnodecount + id]]) {
        		/*
        		:Supports separate independent but reproducible streams for
        		: each instance. However, the corresponding hoc Random
        		: distribution MUST be set to Random.negexp(1)
        		*/
                    if( inst->usingR123[id] ) {
        		ret_erand = nrnran123_negexp((nrnran123_State*)inst->exp_rng[indexes[3*pnodecount + id]]);
                    } else {
        #ifndef CORENEURON_BUILD
        		ret_erand = nrn_random_pick(RANDCAST inst->exp_rng[indexes[3*pnodecount + id]]);
        #endif
                    }
        	}else{
          	  hoc_execerror("multithread random in NetStim"," only via hoc Random");
        	}

        return ret_erand;
    }


    inline double getPostRestoreFlag_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_getPostRestoreFlag = 0.0;
            return POST_RESTORE_RESTART_FLAG;

        return ret_getPostRestoreFlag;
    }


    inline double resumeEvent_InhPoissonStim(int id, int pnodecount, InhPoissonStim_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_resumeEvent = 0.0;
        double elapsed_time;
        elapsed_time = inst->event[id];
        while (elapsed_time < nt->_t) {
            {
                  IvocVect* vv; int i, i_prev, size; double* px;
                  i = (int)inst->index[id];
                  i_prev = i;
                  if (i >= 0) { 
                    vv = *((IvocVect**)(&inst->vecTbins[indexes[5*pnodecount + id]]));
                    if (vv) {
                      size = vector_capacity(vv);
                      px = vector_vec(vv);
                      /* advance to current tbins without exceeding array bounds */
                      while ((i+1 < size) && (nt->_t>=px[i+1])) {
                	inst->index[id] += 1.;
                	i += 1;
                      }
                      /* did the index change? */
                      if (i!=i_prev) {
                        /* advance curRate to next vecRate if possible */
                        IvocVect *vvRate = *((IvocVect**)(&inst->vecRate[indexes[4*pnodecount + id]]));
                        if (vvRate && vector_capacity(vvRate)>i) {
                          px = vector_vec(vvRate);
                          inst->curRate[id] = px[i];
                        }
                        else inst->curRate[id] = 1.0;
                      }
                      /* have we hit last bin? ... disable time advancing leaving curRate as it is*/
                      if (i==size)
                        inst->index[id] = -1.;
                    } else { /* no vecTbins, use some defaults */
                      inst->rmax[id] = 1.0;
                      inst->curRate[id] = 1.0;
                      inst->index[id] = -1.; /* no vecTbins ... disable time advancing & Poisson unit rate. */
                    }
                  }

            }
            {
                double erand_in_1;
                {
                    	if (inst->exp_rng[indexes[3*pnodecount + id]]) {
                    		/*
                    		:Supports separate independent but reproducible streams for
                    		: each instance. However, the corresponding hoc Random
                    		: distribution MUST be set to Random.negexp(1)
                    		*/
                                if( inst->usingR123[id] ) {
                    		erand_in_1 = nrnran123_negexp((nrnran123_State*)inst->exp_rng[indexes[3*pnodecount + id]]);
                                } else {
                    #ifndef CORENEURON_BUILD
                    		erand_in_1 = nrn_random_pick(RANDCAST inst->exp_rng[indexes[3*pnodecount + id]]);
                    #endif
                                }
                    	}else{
                      	  hoc_execerror("multithread random in NetStim"," only via hoc Random");
                    	}

                }
                inst->event[id] = 1000.0 / inst->rmax[id] * erand_in_1;
                if (inst->event[id] < 0.0) {
                    inst->event[id] = 0.0;
                }
            }
            elapsed_time = elapsed_time + inst->event[id];
        }
        inst->event[id] = elapsed_time - nt->_t;
        ret_resumeEvent = elapsed_time;
        return ret_resumeEvent;
    }


    static inline void net_receive_InhPoissonStim(Point_process* pnt, int weight_index, double flag) {
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
        auto* const inst = static_cast<InhPoissonStim_Instance*>(ml->instance);

        double t = nt->_t;
        inst->tsave[id] = t;
        {
            if (flag == POST_RESTORE_RESTART_FLAG) {
                if (t + inst->event[id] < inst->start[id] + inst->duration[id]) {
                    artcell_net_send(&inst->tqitem[indexes[6*pnodecount + id]], weight_index, pnt, nt->_t+inst->event[id], inst->activeFlag[id]);
                }
            } else if (inst->activeFlag[id] == flag) {
                {
                      IvocVect* vv; int i, i_prev, size; double* px;
                      i = (int)inst->index[id];
                      i_prev = i;
                      if (i >= 0) { 
                        vv = *((IvocVect**)(&inst->vecTbins[indexes[5*pnodecount + id]]));
                        if (vv) {
                          size = vector_capacity(vv);
                          px = vector_vec(vv);
                          /* advance to current tbins without exceeding array bounds */
                          while ((i+1 < size) && (t>=px[i+1])) {
                    	inst->index[id] += 1.;
                    	i += 1;
                          }
                          /* did the index change? */
                          if (i!=i_prev) {
                            /* advance curRate to next vecRate if possible */
                            IvocVect *vvRate = *((IvocVect**)(&inst->vecRate[indexes[4*pnodecount + id]]));
                            if (vvRate && vector_capacity(vvRate)>i) {
                              px = vector_vec(vvRate);
                              inst->curRate[id] = px[i];
                            }
                            else inst->curRate[id] = 1.0;
                          }
                          /* have we hit last bin? ... disable time advancing leaving curRate as it is*/
                          if (i==size)
                            inst->index[id] = -1.;
                        } else { /* no vecTbins, use some defaults */
                          inst->rmax[id] = 1.0;
                          inst->curRate[id] = 1.0;
                          inst->index[id] = -1.; /* no vecTbins ... disable time advancing & Poisson unit rate. */
                        }
                      }

                }
                {
                    double erand_in_1;
                    {
                        	if (inst->exp_rng[indexes[3*pnodecount + id]]) {
                        		/*
                        		:Supports separate independent but reproducible streams for
                        		: each instance. However, the corresponding hoc Random
                        		: distribution MUST be set to Random.negexp(1)
                        		*/
                                    if( inst->usingR123[id] ) {
                        		erand_in_1 = nrnran123_negexp((nrnran123_State*)inst->exp_rng[indexes[3*pnodecount + id]]);
                                    } else {
                        #ifndef CORENEURON_BUILD
                        		erand_in_1 = nrn_random_pick(RANDCAST inst->exp_rng[indexes[3*pnodecount + id]]);
                        #endif
                                    }
                        	}else{
                          	  hoc_execerror("multithread random in NetStim"," only via hoc Random");
                        	}

                    }
                    inst->event[id] = 1000.0 / inst->rmax[id] * erand_in_1;
                    if (inst->event[id] < 0.0) {
                        inst->event[id] = 0.0;
                    }
                }
                if (t + inst->event[id] < inst->start[id] + inst->duration[id]) {
                    artcell_net_send(&inst->tqitem[indexes[6*pnodecount + id]], weight_index, pnt, nt->_t+inst->event[id], inst->activeFlag[id]);
                }
                        double u = (double)urand_InhPoissonStim(id, pnodecount, inst, data, indexes, thread, nt, v);
                        if (u<inst->curRate[id]/inst->rmax[id]) {
                            debug_printf("\nInhPoisson: Spike time t = %g [urand=%g curRate=%g, rmax=%g]\n",
                                         t, u, inst->curRate[id], inst->rmax[id]);

                net_event(pnt, t);
                        }

            }
        }
    }


    /** initialize channel */
    void nrn_init_InhPoissonStim(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<InhPoissonStim_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                inst->tsave[id] = -1e20;
                double v = 0.0;
                double erand_in_0;
                inst->index[id] = 0.0;
                inst->activeFlag[id] = 0.0;
                   IvocVect *vvTbins = *((IvocVect**)(&inst->vecTbins[indexes[5*pnodecount + id]]));
                   double* px;
                   if (vvTbins && vector_capacity(vvTbins)>=1) {
                     px = vector_vec(vvTbins);
                     inst->start[id] = px[0];
                     if (inst->start[id] < 0.0) inst->start[id]=0.0;
                   }
                   else inst->start[id] = 0.0;
                   /* first event is at the start
                   TODO: This should draw from a more appropriate dist
                   that has the surrogate process starting a t=-inf
                   */
                   inst->event[id] = inst->start[id];
                   /* set curRate */
                   IvocVect *vvRate = *((IvocVect**)(&inst->vecRate[indexes[4*pnodecount + id]]));
                   px = vector_vec(vvRate);
                   /* set rmax */
                   inst->rmax[id] = 0.0;
                   int i;
                   for (i=0;i<vector_capacity(vvRate);i++) {
                      if (px[i]>inst->rmax[id]) inst->rmax[id] = px[i];
                   }
                   if (vvRate && vector_capacity(vvRate)>0) {
                     inst->curRate[id] = px[0];
                   }
                   else {
                      inst->curRate[id] = 1.0;
                      inst->rmax[id] = 1.0;
                   }
                   /** after discussion with michael : rng streams should be set 0
                     * in initial block. this is to make sure if initial block is
                     * get called multiple times then the simulation should give the
                     * same results. Otherwise this is an issue in coreneuron because
                     * finitialized is get called twice in coreneuron (once from
                     * neurodamus and then in coreneuron. But in general, initial state
                     * should be callable multiple times.
                     */
                   if (inst->uniform_rng[indexes[2*pnodecount + id]] && inst->usingR123[id]) {
                     nrnran123_setseq((nrnran123_State*)inst->uniform_rng[indexes[2*pnodecount + id]], 0, 0);
                   }
                   if (inst->exp_rng[indexes[3*pnodecount + id]] && inst->usingR123[id]) {
                     nrnran123_setseq((nrnran123_State*)inst->exp_rng[indexes[3*pnodecount + id]], 0, 0);
                   }

                {
                      IvocVect* vv; int i, i_prev, size; double* px;
                      i = (int)inst->index[id];
                      i_prev = i;
                      if (i >= 0) { 
                        vv = *((IvocVect**)(&inst->vecTbins[indexes[5*pnodecount + id]]));
                        if (vv) {
                          size = vector_capacity(vv);
                          px = vector_vec(vv);
                          /* advance to current tbins without exceeding array bounds */
                          while ((i+1 < size) && (nt->_t>=px[i+1])) {
                    	inst->index[id] += 1.;
                    	i += 1;
                          }
                          /* did the index change? */
                          if (i!=i_prev) {
                            /* advance curRate to next vecRate if possible */
                            IvocVect *vvRate = *((IvocVect**)(&inst->vecRate[indexes[4*pnodecount + id]]));
                            if (vvRate && vector_capacity(vvRate)>i) {
                              px = vector_vec(vvRate);
                              inst->curRate[id] = px[i];
                            }
                            else inst->curRate[id] = 1.0;
                          }
                          /* have we hit last bin? ... disable time advancing leaving curRate as it is*/
                          if (i==size)
                            inst->index[id] = -1.;
                        } else { /* no vecTbins, use some defaults */
                          inst->rmax[id] = 1.0;
                          inst->curRate[id] = 1.0;
                          inst->index[id] = -1.; /* no vecTbins ... disable time advancing & Poisson unit rate. */
                        }
                      }

                }
                {
                    	if (inst->exp_rng[indexes[3*pnodecount + id]]) {
                    		/*
                    		:Supports separate independent but reproducible streams for
                    		: each instance. However, the corresponding hoc Random
                    		: distribution MUST be set to Random.negexp(1)
                    		*/
                                if( inst->usingR123[id] ) {
                    		erand_in_0 = nrnran123_negexp((nrnran123_State*)inst->exp_rng[indexes[3*pnodecount + id]]);
                                } else {
                    #ifndef CORENEURON_BUILD
                    		erand_in_0 = nrn_random_pick(RANDCAST inst->exp_rng[indexes[3*pnodecount + id]]);
                    #endif
                                }
                    	}else{
                      	  hoc_execerror("multithread random in NetStim"," only via hoc Random");
                    	}

                }
                {
                    double erand_in_1;
                    {
                        	if (inst->exp_rng[indexes[3*pnodecount + id]]) {
                        		/*
                        		:Supports separate independent but reproducible streams for
                        		: each instance. However, the corresponding hoc Random
                        		: distribution MUST be set to Random.negexp(1)
                        		*/
                                    if( inst->usingR123[id] ) {
                        		erand_in_1 = nrnran123_negexp((nrnran123_State*)inst->exp_rng[indexes[3*pnodecount + id]]);
                                    } else {
                        #ifndef CORENEURON_BUILD
                        		erand_in_1 = nrn_random_pick(RANDCAST inst->exp_rng[indexes[3*pnodecount + id]]);
                        #endif
                                    }
                        	}else{
                          	  hoc_execerror("multithread random in NetStim"," only via hoc Random");
                        	}

                    }
                    inst->event[id] = 1000.0 / inst->rmax[id] * erand_in_1;
                    if (inst->event[id] < 0.0) {
                        inst->event[id] = 0.0;
                    }
                }
                if (nt->_t + inst->event[id] < inst->start[id] + inst->duration[id]) {
                         debug_printf("InhPoisson: Initial event at t = %6.3f\n", nt->_t + inst->event[id]);

                    artcell_net_send(&inst->tqitem[indexes[6*pnodecount + id]], 0, (Point_process*)inst->point_process[indexes[1*pnodecount + id]], nt->_t+inst->event[id], inst->activeFlag[id]);
                }
            }
        }
    }


    /** register channel with the simulator */
    void _netstim_inhpoisson_reg() {

        int mech_type = nrn_get_mechtype("InhPoissonStim");
        InhPoissonStim_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        point_register_mech(mechanism_info, nrn_alloc_InhPoissonStim, nullptr, nullptr, nullptr, nrn_init_InhPoissonStim, nrn_private_constructor_InhPoissonStim, nrn_private_destructor_InhPoissonStim, first_pointer_var_index(), nullptr, nullptr, 1);

        hoc_reg_bbcore_read(mech_type, bbcore_read);
        hoc_reg_bbcore_write(mech_type, bbcore_write);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "area");
        hoc_register_dparam_semantics(mech_type, 1, "pntproc");
        hoc_register_dparam_semantics(mech_type, 2, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 3, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 4, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 5, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 6, "netsend");
        add_nrn_has_net_event(mech_type);
        add_nrn_artcell(mech_type, 6);
        set_pnt_receive(mech_type, net_receive_InhPoissonStim, nullptr, num_net_receive_args());
        hoc_register_net_send_buffering(mech_type);
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
