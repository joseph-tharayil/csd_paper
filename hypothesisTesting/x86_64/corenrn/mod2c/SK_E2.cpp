/*********************************************************
Model Name      : SK_E2
Filename        : SK_E2.mod
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
        "SK_E2",
        "gSK_E2bar_SK_E2",
        "zTau_SK_E2",
        0,
        "ik_SK_E2",
        "gSK_E2_SK_E2",
        0,
        "z_SK_E2",
        0,
        0
    };


    /** all global variables */
    struct SK_E2_Store {
        int k_type{};
        int ca_type{};
        double z0{};
        int reset{};
        int mech_type{};
        int slist1[1]{4};
        int dlist1[1]{8};
    };
    static_assert(std::is_trivially_copy_constructible_v<SK_E2_Store>);
    static_assert(std::is_trivially_move_constructible_v<SK_E2_Store>);
    static_assert(std::is_trivially_copy_assignable_v<SK_E2_Store>);
    static_assert(std::is_trivially_move_assignable_v<SK_E2_Store>);
    static_assert(std::is_trivially_destructible_v<SK_E2_Store>);
    SK_E2_Store SK_E2_global;


    /** all mechanism instance variables and global variables */
    struct SK_E2_Instance  {
        const double* gSK_E2bar{};
        const double* zTau{};
        double* ik{};
        double* gSK_E2{};
        double* z{};
        double* ek{};
        double* cai{};
        double* zInf{};
        double* Dz{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ek{};
        double* ion_ik{};
        double* ion_dikdv{};
        const double* ion_cai{};
        const double* ion_cao{};
        SK_E2_Store* global{&SK_E2_global};
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
        return 11;
    }


    static inline int int_variables_size() {
        return 5;
    }


    static inline int get_mech_type() {
        return SK_E2_global.mech_type;
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
    static void nrn_private_constructor_SK_E2(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new SK_E2_Instance{};
        assert(inst->global == &SK_E2_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(SK_E2_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_SK_E2(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<SK_E2_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &SK_E2_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(SK_E2_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<SK_E2_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &SK_E2_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(SK_E2_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gSK_E2bar = ml->data+0*pnodecount;
        inst->zTau = ml->data+1*pnodecount;
        inst->ik = ml->data+2*pnodecount;
        inst->gSK_E2 = ml->data+3*pnodecount;
        inst->z = ml->data+4*pnodecount;
        inst->ek = ml->data+5*pnodecount;
        inst->cai = ml->data+6*pnodecount;
        inst->zInf = ml->data+7*pnodecount;
        inst->Dz = ml->data+8*pnodecount;
        inst->v_unused = ml->data+9*pnodecount;
        inst->g_unused = ml->data+10*pnodecount;
        inst->ion_ek = nt->_data;
        inst->ion_ik = nt->_data;
        inst->ion_dikdv = nt->_data;
        inst->ion_cai = nt->_data;
        inst->ion_cao = nt->_data;
    }



    static void nrn_alloc_SK_E2(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_SK_E2(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SK_E2_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_SK_E2(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SK_E2_Instance*>(ml->instance);

        #endif
    }


    inline int rates_SK_E2(int id, int pnodecount, SK_E2_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double ca);


    inline int rates_SK_E2(int id, int pnodecount, SK_E2_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double ca) {
        int ret_rates = 0;
        if (ca < 1e-7) {
            ca = ca + 1e-07;
        }
        inst->zInf[id] = 1.0 / (1.0 + pow((0.00043 / ca), 4.8));
        return ret_rates;
    }


    /** initialize channel */
    void nrn_init_SK_E2(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<SK_E2_Instance*>(ml->instance);

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
                inst->cai[id] = inst->ion_cai[indexes[3*pnodecount + id]];
                inst->z[id] = inst->global->z0;
                {
                    double ca_in_1;
                    ca_in_1 = inst->cai[id];
                    if (ca_in_1 < 1e-7) {
                        ca_in_1 = ca_in_1 + 1e-07;
                    }
                    inst->zInf[id] = 1.0 / (1.0 + pow((0.00043 / ca_in_1), 4.8));
                }
                inst->z[id] = inst->zInf[id];
            }
        }
    }


    inline double nrn_current_SK_E2(int id, int pnodecount, SK_E2_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->gSK_E2[id] = inst->gSK_E2bar[id] * inst->z[id];
        inst->ik[id] = inst->gSK_E2[id] * (v - inst->ek[id]);
        current += inst->ik[id];
        return current;
    }


    /** update current */
    void nrn_cur_SK_E2(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SK_E2_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ek[id] = inst->ion_ek[indexes[0*pnodecount + id]];
            inst->cai[id] = inst->ion_cai[indexes[3*pnodecount + id]];
            double g = nrn_current_SK_E2(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double dik = inst->ik[id];
            double rhs = nrn_current_SK_E2(id, pnodecount, inst, data, indexes, thread, nt, v);
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
    void nrn_state_SK_E2(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SK_E2_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ek[id] = inst->ion_ek[indexes[0*pnodecount + id]];
            inst->cai[id] = inst->ion_cai[indexes[3*pnodecount + id]];
            {
                double ca_in_0;
                ca_in_0 = inst->cai[id];
                if (ca_in_0 < 1e-7) {
                    ca_in_0 = ca_in_0 + 1e-07;
                }
                inst->zInf[id] = 1.0 / (1.0 + pow((0.00043 / ca_in_0), 4.8));
            }
            inst->z[id] = inst->z[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->zTau[id]))) * ( -(((inst->zInf[id])) / inst->zTau[id]) / (((( -1.0))) / inst->zTau[id]) - inst->z[id]);
        }
    }


    /** register channel with the simulator */
    void _SK_E2_reg() {

        int mech_type = nrn_get_mechtype("SK_E2");
        SK_E2_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_SK_E2, nrn_cur_SK_E2, nullptr, nrn_state_SK_E2, nrn_init_SK_E2, nrn_private_constructor_SK_E2, nrn_private_destructor_SK_E2, first_pointer_var_index(), 1);
        SK_E2_global.k_type = nrn_get_mechtype("k_ion");
        SK_E2_global.ca_type = nrn_get_mechtype("ca_ion");

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "k_ion");
        hoc_register_dparam_semantics(mech_type, 1, "k_ion");
        hoc_register_dparam_semantics(mech_type, 2, "k_ion");
        hoc_register_dparam_semantics(mech_type, 3, "ca_ion");
        hoc_register_dparam_semantics(mech_type, 4, "ca_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
