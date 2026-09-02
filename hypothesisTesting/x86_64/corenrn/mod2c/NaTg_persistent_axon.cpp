/*********************************************************
Model Name      : NaTg_persistent_axon
Filename        : NaTg_persistent_axon.mod
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
        "NaTg_persistent_axon",
        "gNaTgbar_NaTg_persistent_axon",
        "vshiftm_NaTg_persistent_axon",
        "slopem_NaTg_persistent_axon",
        0,
        "ina_NaTg_persistent_axon",
        "gNaTg_NaTg_persistent_axon",
        0,
        "m_NaTg_persistent_axon",
        0,
        0
    };


    /** all global variables */
    struct NaTg_persistent_axon_Store {
        int na_type{};
        int ttx_type{};
        double m0{};
        int reset{};
        int mech_type{};
        int slist1[1]{5};
        int dlist1[1]{13};
    };
    static_assert(std::is_trivially_copy_constructible_v<NaTg_persistent_axon_Store>);
    static_assert(std::is_trivially_move_constructible_v<NaTg_persistent_axon_Store>);
    static_assert(std::is_trivially_copy_assignable_v<NaTg_persistent_axon_Store>);
    static_assert(std::is_trivially_move_assignable_v<NaTg_persistent_axon_Store>);
    static_assert(std::is_trivially_destructible_v<NaTg_persistent_axon_Store>);
    NaTg_persistent_axon_Store NaTg_persistent_axon_global;


    /** all mechanism instance variables and global variables */
    struct NaTg_persistent_axon_Instance  {
        const double* gNaTgbar{};
        const double* vshiftm{};
        const double* slopem{};
        double* ina{};
        double* gNaTg{};
        double* m{};
        double* ttxo{};
        double* ttxi{};
        double* ena{};
        double* mInf{};
        double* mTau{};
        double* mAlpha{};
        double* mBeta{};
        double* Dm{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ena{};
        double* ion_ina{};
        double* ion_dinadv{};
        const double* ion_ttxo{};
        const double* ion_ttxi{};
        NaTg_persistent_axon_Store* global{&NaTg_persistent_axon_global};
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
        return 16;
    }


    static inline int int_variables_size() {
        return 5;
    }


    static inline int get_mech_type() {
        return NaTg_persistent_axon_global.mech_type;
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
    static void nrn_private_constructor_NaTg_persistent_axon(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new NaTg_persistent_axon_Instance{};
        assert(inst->global == &NaTg_persistent_axon_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(NaTg_persistent_axon_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_NaTg_persistent_axon(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<NaTg_persistent_axon_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &NaTg_persistent_axon_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(NaTg_persistent_axon_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<NaTg_persistent_axon_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &NaTg_persistent_axon_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(NaTg_persistent_axon_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gNaTgbar = ml->data+0*pnodecount;
        inst->vshiftm = ml->data+1*pnodecount;
        inst->slopem = ml->data+2*pnodecount;
        inst->ina = ml->data+3*pnodecount;
        inst->gNaTg = ml->data+4*pnodecount;
        inst->m = ml->data+5*pnodecount;
        inst->ttxo = ml->data+6*pnodecount;
        inst->ttxi = ml->data+7*pnodecount;
        inst->ena = ml->data+8*pnodecount;
        inst->mInf = ml->data+9*pnodecount;
        inst->mTau = ml->data+10*pnodecount;
        inst->mAlpha = ml->data+11*pnodecount;
        inst->mBeta = ml->data+12*pnodecount;
        inst->Dm = ml->data+13*pnodecount;
        inst->v_unused = ml->data+14*pnodecount;
        inst->g_unused = ml->data+15*pnodecount;
        inst->ion_ena = nt->_data;
        inst->ion_ina = nt->_data;
        inst->ion_dinadv = nt->_data;
        inst->ion_ttxo = nt->_data;
        inst->ion_ttxi = nt->_data;
    }



    static void nrn_alloc_NaTg_persistent_axon(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_NaTg_persistent_axon(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<NaTg_persistent_axon_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_NaTg_persistent_axon(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<NaTg_persistent_axon_Instance*>(ml->instance);

        #endif
    }


    inline int rates_NaTg_persistent_axon(int id, int pnodecount, NaTg_persistent_axon_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);


    inline int rates_NaTg_persistent_axon(int id, int pnodecount, NaTg_persistent_axon_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_rates = 0;
        double qt;
        qt = pow(2.3, ((34.0 - 21.0) / 10.0));
        if (v == ( -38.0 + inst->vshiftm[id])) {
            v = v + 0.0001;
        }
        inst->mAlpha[id] = (0.182 * (v - ( -38.0 + inst->vshiftm[id]))) / (1.0 - (exp( -(v - ( -38.0 + inst->vshiftm[id])) / inst->slopem[id])));
        inst->mBeta[id] = (0.124 * ( -v + ( -38.0 + inst->vshiftm[id]))) / (1.0 - (exp( -( -v + ( -38.0 + inst->vshiftm[id])) / inst->slopem[id])));
        inst->mTau[id] = (1.0 / (inst->mAlpha[id] + inst->mBeta[id])) / qt;
        inst->mInf[id] = inst->mAlpha[id] / (inst->mAlpha[id] + inst->mBeta[id]);
        return ret_rates;
    }


    /** initialize channel */
    void nrn_init_NaTg_persistent_axon(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<NaTg_persistent_axon_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
                inst->ttxo[id] = inst->ion_ttxo[indexes[3*pnodecount + id]];
                inst->ttxi[id] = inst->ion_ttxi[indexes[4*pnodecount + id]];
                inst->m[id] = inst->global->m0;
                if (inst->ttxi[id] == 0.015625 && inst->ttxo[id] > 1e-12) {
                    inst->mInf[id] = 0.0;
                    inst->mTau[id] = 1e-12;
                } else {
                    {
                        double qt;
                        qt = pow(2.3, ((34.0 - 21.0) / 10.0));
                        if (v == ( -38.0 + inst->vshiftm[id])) {
                            v = v + 0.0001;
                        }
                        inst->mAlpha[id] = (0.182 * (v - ( -38.0 + inst->vshiftm[id]))) / (1.0 - (exp( -(v - ( -38.0 + inst->vshiftm[id])) / inst->slopem[id])));
                        inst->mBeta[id] = (0.124 * ( -v + ( -38.0 + inst->vshiftm[id]))) / (1.0 - (exp( -( -v + ( -38.0 + inst->vshiftm[id])) / inst->slopem[id])));
                        inst->mTau[id] = (1.0 / (inst->mAlpha[id] + inst->mBeta[id])) / qt;
                        inst->mInf[id] = inst->mAlpha[id] / (inst->mAlpha[id] + inst->mBeta[id]);
                    }
                }
                inst->m[id] = inst->mInf[id];
            }
        }
    }


    inline double nrn_current_NaTg_persistent_axon(int id, int pnodecount, NaTg_persistent_axon_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->gNaTg[id] = inst->gNaTgbar[id] * inst->m[id] * inst->m[id] * inst->m[id];
        inst->ina[id] = inst->gNaTg[id] * (v - inst->ena[id]);
        current += inst->ina[id];
        return current;
    }


    /** update current */
    void nrn_cur_NaTg_persistent_axon(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<NaTg_persistent_axon_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
            inst->ttxo[id] = inst->ion_ttxo[indexes[3*pnodecount + id]];
            inst->ttxi[id] = inst->ion_ttxi[indexes[4*pnodecount + id]];
            double g = nrn_current_NaTg_persistent_axon(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double dina = inst->ina[id];
            double rhs = nrn_current_NaTg_persistent_axon(id, pnodecount, inst, data, indexes, thread, nt, v);
            g = (g-rhs)/0.001;
            inst->ion_dinadv[indexes[2*pnodecount + id]] += (dina-inst->ina[id])/0.001;
            inst->ion_ina[indexes[1*pnodecount + id]] += inst->ina[id];
            #if NRN_PRCELLSTATE
            inst->g_unused[id] = g;
            #endif
            vec_rhs[node_id] -= rhs;
            vec_d[node_id] += g;
        }
    }


    /** update state */
    void nrn_state_NaTg_persistent_axon(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<NaTg_persistent_axon_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
            inst->ttxo[id] = inst->ion_ttxo[indexes[3*pnodecount + id]];
            inst->ttxi[id] = inst->ion_ttxi[indexes[4*pnodecount + id]];
            if (inst->ttxi[id] == 0.015625 && inst->ttxo[id] > 1e-12) {
                inst->mInf[id] = 0.0;
                inst->mTau[id] = 1e-12;
            } else {
                {
                    double qt;
                    qt = pow(2.3, ((34.0 - 21.0) / 10.0));
                    if (v == ( -38.0 + inst->vshiftm[id])) {
                        v = v + 0.0001;
                    }
                    inst->mAlpha[id] = (0.182 * (v - ( -38.0 + inst->vshiftm[id]))) / (1.0 - (exp( -(v - ( -38.0 + inst->vshiftm[id])) / inst->slopem[id])));
                    inst->mBeta[id] = (0.124 * ( -v + ( -38.0 + inst->vshiftm[id]))) / (1.0 - (exp( -( -v + ( -38.0 + inst->vshiftm[id])) / inst->slopem[id])));
                    inst->mTau[id] = (1.0 / (inst->mAlpha[id] + inst->mBeta[id])) / qt;
                    inst->mInf[id] = inst->mAlpha[id] / (inst->mAlpha[id] + inst->mBeta[id]);
                }
            }
            inst->m[id] = inst->m[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->mTau[id]))) * ( -(((inst->mInf[id])) / inst->mTau[id]) / (((( -1.0))) / inst->mTau[id]) - inst->m[id]);
        }
    }


    /** register channel with the simulator */
    void _NaTg_persistent_axon_reg() {

        int mech_type = nrn_get_mechtype("NaTg_persistent_axon");
        NaTg_persistent_axon_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_NaTg_persistent_axon, nrn_cur_NaTg_persistent_axon, nullptr, nrn_state_NaTg_persistent_axon, nrn_init_NaTg_persistent_axon, nrn_private_constructor_NaTg_persistent_axon, nrn_private_destructor_NaTg_persistent_axon, first_pointer_var_index(), 1);
        NaTg_persistent_axon_global.na_type = nrn_get_mechtype("na_ion");
        NaTg_persistent_axon_global.ttx_type = nrn_get_mechtype("ttx_ion");

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "na_ion");
        hoc_register_dparam_semantics(mech_type, 1, "na_ion");
        hoc_register_dparam_semantics(mech_type, 2, "na_ion");
        hoc_register_dparam_semantics(mech_type, 3, "ttx_ion");
        hoc_register_dparam_semantics(mech_type, 4, "ttx_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
