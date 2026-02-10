/*********************************************************
Model Name      : Nap_Et2
Filename        : Nap_Et2.mod
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
        "Nap_Et2",
        "gNap_Et2bar_Nap_Et2",
        0,
        "ina_Nap_Et2",
        "gNap_Et2_Nap_Et2",
        0,
        "m_Nap_Et2",
        "h_Nap_Et2",
        0,
        0
    };


    /** all global variables */
    struct Nap_Et2_Store {
        int na_type{};
        int ttx_type{};
        double m0{};
        double h0{};
        int reset{};
        int mech_type{};
        int slist1[2]{3, 4};
        int dlist1[2]{16, 17};
    };
    static_assert(std::is_trivially_copy_constructible_v<Nap_Et2_Store>);
    static_assert(std::is_trivially_move_constructible_v<Nap_Et2_Store>);
    static_assert(std::is_trivially_copy_assignable_v<Nap_Et2_Store>);
    static_assert(std::is_trivially_move_assignable_v<Nap_Et2_Store>);
    static_assert(std::is_trivially_destructible_v<Nap_Et2_Store>);
    Nap_Et2_Store Nap_Et2_global;


    /** all mechanism instance variables and global variables */
    struct Nap_Et2_Instance  {
        const double* gNap_Et2bar{};
        double* ina{};
        double* gNap_Et2{};
        double* m{};
        double* h{};
        double* ttxo{};
        double* ttxi{};
        double* ena{};
        double* mInf{};
        double* mTau{};
        double* mAlpha{};
        double* mBeta{};
        double* hInf{};
        double* hTau{};
        double* hAlpha{};
        double* hBeta{};
        double* Dm{};
        double* Dh{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ena{};
        double* ion_ina{};
        double* ion_dinadv{};
        const double* ion_ttxo{};
        const double* ion_ttxi{};
        Nap_Et2_Store* global{&Nap_Et2_global};
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
        return 20;
    }


    static inline int int_variables_size() {
        return 5;
    }


    static inline int get_mech_type() {
        return Nap_Et2_global.mech_type;
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
    static void nrn_private_constructor_Nap_Et2(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new Nap_Et2_Instance{};
        assert(inst->global == &Nap_Et2_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(Nap_Et2_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_Nap_Et2(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<Nap_Et2_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &Nap_Et2_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(Nap_Et2_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<Nap_Et2_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &Nap_Et2_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(Nap_Et2_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gNap_Et2bar = ml->data+0*pnodecount;
        inst->ina = ml->data+1*pnodecount;
        inst->gNap_Et2 = ml->data+2*pnodecount;
        inst->m = ml->data+3*pnodecount;
        inst->h = ml->data+4*pnodecount;
        inst->ttxo = ml->data+5*pnodecount;
        inst->ttxi = ml->data+6*pnodecount;
        inst->ena = ml->data+7*pnodecount;
        inst->mInf = ml->data+8*pnodecount;
        inst->mTau = ml->data+9*pnodecount;
        inst->mAlpha = ml->data+10*pnodecount;
        inst->mBeta = ml->data+11*pnodecount;
        inst->hInf = ml->data+12*pnodecount;
        inst->hTau = ml->data+13*pnodecount;
        inst->hAlpha = ml->data+14*pnodecount;
        inst->hBeta = ml->data+15*pnodecount;
        inst->Dm = ml->data+16*pnodecount;
        inst->Dh = ml->data+17*pnodecount;
        inst->v_unused = ml->data+18*pnodecount;
        inst->g_unused = ml->data+19*pnodecount;
        inst->ion_ena = nt->_data;
        inst->ion_ina = nt->_data;
        inst->ion_dinadv = nt->_data;
        inst->ion_ttxo = nt->_data;
        inst->ion_ttxi = nt->_data;
    }



    static void nrn_alloc_Nap_Et2(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_Nap_Et2(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Nap_Et2_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_Nap_Et2(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Nap_Et2_Instance*>(ml->instance);

        #endif
    }


    inline int rates_Nap_Et2(int id, int pnodecount, Nap_Et2_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);


    inline int rates_Nap_Et2(int id, int pnodecount, Nap_Et2_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_rates = 0;
        double qt;
        qt = pow(2.3, ((34.0 - 21.0) / 10.0));
        inst->mInf[id] = 1.0 / (1.0 + exp((v -  -52.6) /  -4.6));
        if (v ==  -38.0) {
            v = v + 0.0001;
        }
        inst->mAlpha[id] = (0.182 * (v -  -38.0)) / (1.0 - (exp( -(v -  -38.0) / 6.0)));
        inst->mBeta[id] = (0.124 * ( -v - 38.0)) / (1.0 - (exp( -( -v - 38.0) / 6.0)));
        inst->mTau[id] = 6.0 * (1.0 / (inst->mAlpha[id] + inst->mBeta[id])) / qt;
        if (v ==  -17.0) {
            v = v + 0.0001;
        }
        if (v ==  -64.4) {
            v = v + 0.0001;
        }
        inst->hInf[id] = 1.0 / (1.0 + exp((v -  -48.8) / 10.0));
        inst->hAlpha[id] =  -2.88e-6 * (v + 17.0) / (1.0 - exp((v + 17.0) / 4.63));
        inst->hBeta[id] = 6.94e-6 * (v + 64.4) / (1.0 - exp( -(v + 64.4) / 2.63));
        inst->hTau[id] = (1.0 / (inst->hAlpha[id] + inst->hBeta[id])) / qt;
        return ret_rates;
    }


    /** initialize channel */
    void nrn_init_Nap_Et2(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<Nap_Et2_Instance*>(ml->instance);

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
                inst->h[id] = inst->global->h0;
                if (inst->ttxi[id] == 0.015625 && inst->ttxo[id] > 1e-12) {
                    inst->mInf[id] = 0.0;
                    inst->mTau[id] = 1e-12;
                    inst->hInf[id] = 1.0;
                    inst->hTau[id] = 1e-12;
                } else {
                    {
                        double qt;
                        qt = pow(2.3, ((34.0 - 21.0) / 10.0));
                        inst->mInf[id] = 1.0 / (1.0 + exp((v -  -52.6) /  -4.6));
                        if (v ==  -38.0) {
                            v = v + 0.0001;
                        }
                        inst->mAlpha[id] = (0.182 * (v -  -38.0)) / (1.0 - (exp( -(v -  -38.0) / 6.0)));
                        inst->mBeta[id] = (0.124 * ( -v - 38.0)) / (1.0 - (exp( -( -v - 38.0) / 6.0)));
                        inst->mTau[id] = 6.0 * (1.0 / (inst->mAlpha[id] + inst->mBeta[id])) / qt;
                        if (v ==  -17.0) {
                            v = v + 0.0001;
                        }
                        if (v ==  -64.4) {
                            v = v + 0.0001;
                        }
                        inst->hInf[id] = 1.0 / (1.0 + exp((v -  -48.8) / 10.0));
                        inst->hAlpha[id] =  -2.88e-6 * (v + 17.0) / (1.0 - exp((v + 17.0) / 4.63));
                        inst->hBeta[id] = 6.94e-6 * (v + 64.4) / (1.0 - exp( -(v + 64.4) / 2.63));
                        inst->hTau[id] = (1.0 / (inst->hAlpha[id] + inst->hBeta[id])) / qt;
                    }
                }
                inst->m[id] = inst->mInf[id];
                inst->h[id] = inst->hInf[id];
            }
        }
    }


    inline double nrn_current_Nap_Et2(int id, int pnodecount, Nap_Et2_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->gNap_Et2[id] = inst->gNap_Et2bar[id] * inst->m[id] * inst->m[id] * inst->m[id] * inst->h[id];
        inst->ina[id] = inst->gNap_Et2[id] * (v - inst->ena[id]);
        current += inst->ina[id];
        return current;
    }


    /** update current */
    void nrn_cur_Nap_Et2(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Nap_Et2_Instance*>(ml->instance);

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
            double g = nrn_current_Nap_Et2(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double dina = inst->ina[id];
            double rhs = nrn_current_Nap_Et2(id, pnodecount, inst, data, indexes, thread, nt, v);
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
    void nrn_state_Nap_Et2(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Nap_Et2_Instance*>(ml->instance);

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
                inst->hInf[id] = 1.0;
                inst->hTau[id] = 1e-12;
            } else {
                {
                    double qt;
                    qt = pow(2.3, ((34.0 - 21.0) / 10.0));
                    inst->mInf[id] = 1.0 / (1.0 + exp((v -  -52.6) /  -4.6));
                    if (v ==  -38.0) {
                        v = v + 0.0001;
                    }
                    inst->mAlpha[id] = (0.182 * (v -  -38.0)) / (1.0 - (exp( -(v -  -38.0) / 6.0)));
                    inst->mBeta[id] = (0.124 * ( -v - 38.0)) / (1.0 - (exp( -( -v - 38.0) / 6.0)));
                    inst->mTau[id] = 6.0 * (1.0 / (inst->mAlpha[id] + inst->mBeta[id])) / qt;
                    if (v ==  -17.0) {
                        v = v + 0.0001;
                    }
                    if (v ==  -64.4) {
                        v = v + 0.0001;
                    }
                    inst->hInf[id] = 1.0 / (1.0 + exp((v -  -48.8) / 10.0));
                    inst->hAlpha[id] =  -2.88e-6 * (v + 17.0) / (1.0 - exp((v + 17.0) / 4.63));
                    inst->hBeta[id] = 6.94e-6 * (v + 64.4) / (1.0 - exp( -(v + 64.4) / 2.63));
                    inst->hTau[id] = (1.0 / (inst->hAlpha[id] + inst->hBeta[id])) / qt;
                }
            }
            inst->m[id] = inst->m[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->mTau[id]))) * ( -(((inst->mInf[id])) / inst->mTau[id]) / (((( -1.0))) / inst->mTau[id]) - inst->m[id]);
            inst->h[id] = inst->h[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->hTau[id]))) * ( -(((inst->hInf[id])) / inst->hTau[id]) / (((( -1.0))) / inst->hTau[id]) - inst->h[id]);
        }
    }


    /** register channel with the simulator */
    void _Nap_Et2_reg() {

        int mech_type = nrn_get_mechtype("Nap_Et2");
        Nap_Et2_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_Nap_Et2, nrn_cur_Nap_Et2, nullptr, nrn_state_Nap_Et2, nrn_init_Nap_Et2, nrn_private_constructor_Nap_Et2, nrn_private_destructor_Nap_Et2, first_pointer_var_index(), 1);
        Nap_Et2_global.na_type = nrn_get_mechtype("na_ion");
        Nap_Et2_global.ttx_type = nrn_get_mechtype("ttx_ion");

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "na_ion");
        hoc_register_dparam_semantics(mech_type, 1, "na_ion");
        hoc_register_dparam_semantics(mech_type, 2, "na_ion");
        hoc_register_dparam_semantics(mech_type, 3, "ttx_ion");
        hoc_register_dparam_semantics(mech_type, 4, "ttx_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
