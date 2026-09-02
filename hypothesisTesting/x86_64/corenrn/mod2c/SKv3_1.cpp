/*********************************************************
Model Name      : SKv3_1
Filename        : SKv3_1.mod
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
        "SKv3_1",
        "gSKv3_1bar_SKv3_1",
        0,
        "ik_SKv3_1",
        "gSKv3_1_SKv3_1",
        0,
        "m_SKv3_1",
        0,
        0
    };


    /** all global variables */
    struct SKv3_1_Store {
        int k_type{};
        double m0{};
        int reset{};
        int mech_type{};
        int slist1[1]{3};
        int dlist1[1]{7};
    };
    static_assert(std::is_trivially_copy_constructible_v<SKv3_1_Store>);
    static_assert(std::is_trivially_move_constructible_v<SKv3_1_Store>);
    static_assert(std::is_trivially_copy_assignable_v<SKv3_1_Store>);
    static_assert(std::is_trivially_move_assignable_v<SKv3_1_Store>);
    static_assert(std::is_trivially_destructible_v<SKv3_1_Store>);
    SKv3_1_Store SKv3_1_global;


    /** all mechanism instance variables and global variables */
    struct SKv3_1_Instance  {
        const double* gSKv3_1bar{};
        double* ik{};
        double* gSKv3_1{};
        double* m{};
        double* ek{};
        double* mInf{};
        double* mTau{};
        double* Dm{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ek{};
        double* ion_ik{};
        double* ion_dikdv{};
        SKv3_1_Store* global{&SKv3_1_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {nullptr, nullptr}
    };


    /** connect global (array) variables to hoc -- */
    static DoubVec hoc_vector_double[] = {
        {nullptr, nullptr, 0}
    };


    static inline int first_pointer_var_index() {
        return -1;
    }


    static inline int first_random_var_index() {
        return -1;
    }


    static inline int float_variables_size() {
        return 10;
    }


    static inline int int_variables_size() {
        return 3;
    }


    static inline int get_mech_type() {
        return SKv3_1_global.mech_type;
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
    static void nrn_private_constructor_SKv3_1(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new SKv3_1_Instance{};
        assert(inst->global == &SKv3_1_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(SKv3_1_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_SKv3_1(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<SKv3_1_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &SKv3_1_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(SKv3_1_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<SKv3_1_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &SKv3_1_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(SKv3_1_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gSKv3_1bar = ml->data+0*pnodecount;
        inst->ik = ml->data+1*pnodecount;
        inst->gSKv3_1 = ml->data+2*pnodecount;
        inst->m = ml->data+3*pnodecount;
        inst->ek = ml->data+4*pnodecount;
        inst->mInf = ml->data+5*pnodecount;
        inst->mTau = ml->data+6*pnodecount;
        inst->Dm = ml->data+7*pnodecount;
        inst->v_unused = ml->data+8*pnodecount;
        inst->g_unused = ml->data+9*pnodecount;
        inst->ion_ek = nt->_data;
        inst->ion_ik = nt->_data;
        inst->ion_dikdv = nt->_data;
    }



    static void nrn_alloc_SKv3_1(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_SKv3_1(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SKv3_1_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_SKv3_1(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SKv3_1_Instance*>(ml->instance);

        #endif
    }


    inline int rates_SKv3_1(int id, int pnodecount, SKv3_1_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);


    inline int rates_SKv3_1(int id, int pnodecount, SKv3_1_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_rates = 0;
        inst->mInf[id] = 1.0 / (1.0 + exp(((v - (18.700)) / ( -9.700))));
        inst->mTau[id] = 0.2 * 20.0 / (1.0 + exp(((v - ( -46.560)) / ( -44.140))));
        return ret_rates;
    }


    /** initialize channel */
    void nrn_init_SKv3_1(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<SKv3_1_Instance*>(ml->instance);

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
                inst->m[id] = inst->global->m0;
                {
                    inst->mInf[id] = 1.0 / (1.0 + exp(((v - (18.700)) / ( -9.700))));
                    inst->mTau[id] = 0.2 * 20.0 / (1.0 + exp(((v - ( -46.560)) / ( -44.140))));
                }
                inst->m[id] = inst->mInf[id];
            }
        }
    }


    inline double nrn_current_SKv3_1(int id, int pnodecount, SKv3_1_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->gSKv3_1[id] = inst->gSKv3_1bar[id] * inst->m[id];
        inst->ik[id] = inst->gSKv3_1[id] * (v - inst->ek[id]);
        current += inst->ik[id];
        return current;
    }


    /** update current */
    void nrn_cur_SKv3_1(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SKv3_1_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ek[id] = inst->ion_ek[indexes[0*pnodecount + id]];
            double g = nrn_current_SKv3_1(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double dik = inst->ik[id];
            double rhs = nrn_current_SKv3_1(id, pnodecount, inst, data, indexes, thread, nt, v);
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
    void nrn_state_SKv3_1(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SKv3_1_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ek[id] = inst->ion_ek[indexes[0*pnodecount + id]];
            {
                inst->mInf[id] = 1.0 / (1.0 + exp(((v - (18.700)) / ( -9.700))));
                inst->mTau[id] = 0.2 * 20.0 / (1.0 + exp(((v - ( -46.560)) / ( -44.140))));
            }
            inst->m[id] = inst->m[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->mTau[id]))) * ( -(((inst->mInf[id])) / inst->mTau[id]) / (((( -1.0))) / inst->mTau[id]) - inst->m[id]);
        }
    }


    /** register channel with the simulator */
    void _SKv3_1_reg() {

        int mech_type = nrn_get_mechtype("SKv3_1");
        SKv3_1_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_SKv3_1, nrn_cur_SKv3_1, nullptr, nrn_state_SKv3_1, nrn_init_SKv3_1, nrn_private_constructor_SKv3_1, nrn_private_destructor_SKv3_1, first_pointer_var_index(), 1);
        SKv3_1_global.k_type = nrn_get_mechtype("k_ion");

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "k_ion");
        hoc_register_dparam_semantics(mech_type, 1, "k_ion");
        hoc_register_dparam_semantics(mech_type, 2, "k_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
