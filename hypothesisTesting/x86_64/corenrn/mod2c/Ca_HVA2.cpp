/*********************************************************
Model Name      : Ca_HVA2
Filename        : Ca_HVA2.mod
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
        "Ca_HVA2",
        "gCa_HVAbar_Ca_HVA2",
        0,
        "ica_Ca_HVA2",
        0,
        "m_Ca_HVA2",
        "h_Ca_HVA2",
        0,
        0
    };


    /** all global variables */
    struct Ca_HVA2_Store {
        int ca_type{};
        double m0{};
        double h0{};
        int reset{};
        int mech_type{};
        int slist1[2]{2, 3};
        int dlist1[2]{14, 15};
    };
    static_assert(std::is_trivially_copy_constructible_v<Ca_HVA2_Store>);
    static_assert(std::is_trivially_move_constructible_v<Ca_HVA2_Store>);
    static_assert(std::is_trivially_copy_assignable_v<Ca_HVA2_Store>);
    static_assert(std::is_trivially_move_assignable_v<Ca_HVA2_Store>);
    static_assert(std::is_trivially_destructible_v<Ca_HVA2_Store>);
    Ca_HVA2_Store Ca_HVA2_global;


    /** all mechanism instance variables and global variables */
    struct Ca_HVA2_Instance  {
        const double* gCa_HVAbar{};
        double* ica{};
        double* m{};
        double* h{};
        double* eca{};
        double* gCa{};
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
        const double* ion_eca{};
        double* ion_ica{};
        double* ion_dicadv{};
        Ca_HVA2_Store* global{&Ca_HVA2_global};
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
        return 18;
    }


    static inline int int_variables_size() {
        return 3;
    }


    static inline int get_mech_type() {
        return Ca_HVA2_global.mech_type;
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
    static void nrn_private_constructor_Ca_HVA2(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new Ca_HVA2_Instance{};
        assert(inst->global == &Ca_HVA2_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(Ca_HVA2_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_Ca_HVA2(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<Ca_HVA2_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &Ca_HVA2_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(Ca_HVA2_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<Ca_HVA2_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &Ca_HVA2_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(Ca_HVA2_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gCa_HVAbar = ml->data+0*pnodecount;
        inst->ica = ml->data+1*pnodecount;
        inst->m = ml->data+2*pnodecount;
        inst->h = ml->data+3*pnodecount;
        inst->eca = ml->data+4*pnodecount;
        inst->gCa = ml->data+5*pnodecount;
        inst->mInf = ml->data+6*pnodecount;
        inst->mTau = ml->data+7*pnodecount;
        inst->mAlpha = ml->data+8*pnodecount;
        inst->mBeta = ml->data+9*pnodecount;
        inst->hInf = ml->data+10*pnodecount;
        inst->hTau = ml->data+11*pnodecount;
        inst->hAlpha = ml->data+12*pnodecount;
        inst->hBeta = ml->data+13*pnodecount;
        inst->Dm = ml->data+14*pnodecount;
        inst->Dh = ml->data+15*pnodecount;
        inst->v_unused = ml->data+16*pnodecount;
        inst->g_unused = ml->data+17*pnodecount;
        inst->ion_eca = nt->_data;
        inst->ion_ica = nt->_data;
        inst->ion_dicadv = nt->_data;
    }



    static void nrn_alloc_Ca_HVA2(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_Ca_HVA2(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Ca_HVA2_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_Ca_HVA2(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Ca_HVA2_Instance*>(ml->instance);

        #endif
    }


    inline int rates_Ca_HVA2(int id, int pnodecount, Ca_HVA2_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);


    inline int rates_Ca_HVA2(int id, int pnodecount, Ca_HVA2_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_rates = 0;
        if ((v ==  -27.0)) {
            v = v + 0.0001;
        }
        inst->mAlpha[id] = (0.055 * ( -27.0 - v)) / (exp(( -27.0 - v) / 3.8) - 1.0);
        inst->mBeta[id] = (0.94 * exp(( -75.0 - v) / 17.0));
        inst->mInf[id] = inst->mAlpha[id] / (inst->mAlpha[id] + inst->mBeta[id]);
        inst->mTau[id] = 1.0 / (inst->mAlpha[id] + inst->mBeta[id]);
        v = v + 12.3;
        inst->hAlpha[id] = (0.000457 * exp(( -13.0 - v) / 50.0));
        inst->hBeta[id] = (0.0065 / (exp(( -v - 15.0) / 28.0) + 1.0));
        inst->hInf[id] = inst->hAlpha[id] / (inst->hAlpha[id] + inst->hBeta[id]);
        inst->hTau[id] = 1.0 / (inst->hAlpha[id] + inst->hBeta[id]);
        v = v - 12.3;
        return ret_rates;
    }


    /** initialize channel */
    void nrn_init_Ca_HVA2(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<Ca_HVA2_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->eca[id] = inst->ion_eca[indexes[0*pnodecount + id]];
                inst->m[id] = inst->global->m0;
                inst->h[id] = inst->global->h0;
                {
                    if ((v ==  -27.0)) {
                        v = v + 0.0001;
                    }
                    inst->mAlpha[id] = (0.055 * ( -27.0 - v)) / (exp(( -27.0 - v) / 3.8) - 1.0);
                    inst->mBeta[id] = (0.94 * exp(( -75.0 - v) / 17.0));
                    inst->mInf[id] = inst->mAlpha[id] / (inst->mAlpha[id] + inst->mBeta[id]);
                    inst->mTau[id] = 1.0 / (inst->mAlpha[id] + inst->mBeta[id]);
                    v = v + 12.3;
                    inst->hAlpha[id] = (0.000457 * exp(( -13.0 - v) / 50.0));
                    inst->hBeta[id] = (0.0065 / (exp(( -v - 15.0) / 28.0) + 1.0));
                    inst->hInf[id] = inst->hAlpha[id] / (inst->hAlpha[id] + inst->hBeta[id]);
                    inst->hTau[id] = 1.0 / (inst->hAlpha[id] + inst->hBeta[id]);
                    v = v - 12.3;
                }
                inst->m[id] = inst->mInf[id];
                inst->h[id] = inst->hInf[id];
            }
        }
    }


    inline double nrn_current_Ca_HVA2(int id, int pnodecount, Ca_HVA2_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->gCa[id] = inst->gCa_HVAbar[id] * inst->m[id] * inst->m[id] * inst->h[id];
        inst->ica[id] = inst->gCa[id] * (v - inst->eca[id]);
        current += inst->ica[id];
        return current;
    }


    /** update current */
    void nrn_cur_Ca_HVA2(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Ca_HVA2_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->eca[id] = inst->ion_eca[indexes[0*pnodecount + id]];
            double g = nrn_current_Ca_HVA2(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double dica = inst->ica[id];
            double rhs = nrn_current_Ca_HVA2(id, pnodecount, inst, data, indexes, thread, nt, v);
            g = (g-rhs)/0.001;
            inst->ion_dicadv[indexes[2*pnodecount + id]] += (dica-inst->ica[id])/0.001;
            inst->ion_ica[indexes[1*pnodecount + id]] += inst->ica[id];
            #if NRN_PRCELLSTATE
            inst->g_unused[id] = g;
            #endif
            vec_rhs[node_id] -= rhs;
            vec_d[node_id] += g;
        }
    }


    /** update state */
    void nrn_state_Ca_HVA2(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Ca_HVA2_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->eca[id] = inst->ion_eca[indexes[0*pnodecount + id]];
            {
                if ((v ==  -27.0)) {
                    v = v + 0.0001;
                }
                inst->mAlpha[id] = (0.055 * ( -27.0 - v)) / (exp(( -27.0 - v) / 3.8) - 1.0);
                inst->mBeta[id] = (0.94 * exp(( -75.0 - v) / 17.0));
                inst->mInf[id] = inst->mAlpha[id] / (inst->mAlpha[id] + inst->mBeta[id]);
                inst->mTau[id] = 1.0 / (inst->mAlpha[id] + inst->mBeta[id]);
                v = v + 12.3;
                inst->hAlpha[id] = (0.000457 * exp(( -13.0 - v) / 50.0));
                inst->hBeta[id] = (0.0065 / (exp(( -v - 15.0) / 28.0) + 1.0));
                inst->hInf[id] = inst->hAlpha[id] / (inst->hAlpha[id] + inst->hBeta[id]);
                inst->hTau[id] = 1.0 / (inst->hAlpha[id] + inst->hBeta[id]);
                v = v - 12.3;
            }
            inst->m[id] = inst->m[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->mTau[id]))) * ( -(((inst->mInf[id])) / inst->mTau[id]) / (((( -1.0))) / inst->mTau[id]) - inst->m[id]);
            inst->h[id] = inst->h[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->hTau[id]))) * ( -(((inst->hInf[id])) / inst->hTau[id]) / (((( -1.0))) / inst->hTau[id]) - inst->h[id]);
        }
    }


    /** register channel with the simulator */
    void _Ca_HVA2_reg() {

        int mech_type = nrn_get_mechtype("Ca_HVA2");
        Ca_HVA2_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_Ca_HVA2, nrn_cur_Ca_HVA2, nullptr, nrn_state_Ca_HVA2, nrn_init_Ca_HVA2, nrn_private_constructor_Ca_HVA2, nrn_private_destructor_Ca_HVA2, first_pointer_var_index(), 1);
        Ca_HVA2_global.ca_type = nrn_get_mechtype("ca_ion");

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "ca_ion");
        hoc_register_dparam_semantics(mech_type, 1, "ca_ion");
        hoc_register_dparam_semantics(mech_type, 2, "ca_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
