/*********************************************************
Model Name      : Kv7
Filename        : Kv7.mod
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
        "Kv7",
        "gKv7bar_Kv7",
        0,
        "ik_Kv7",
        "gKv7_Kv7",
        0,
        "n_Kv7",
        0,
        0
    };


    /** all global variables */
    struct Kv7_Store {
        int k_type{};
        double n0{};
        int reset{};
        int mech_type{};
        int slist1[1]{3};
        int dlist1[1]{9};
    };
    static_assert(std::is_trivially_copy_constructible_v<Kv7_Store>);
    static_assert(std::is_trivially_move_constructible_v<Kv7_Store>);
    static_assert(std::is_trivially_copy_assignable_v<Kv7_Store>);
    static_assert(std::is_trivially_move_assignable_v<Kv7_Store>);
    static_assert(std::is_trivially_destructible_v<Kv7_Store>);
    Kv7_Store Kv7_global;


    /** all mechanism instance variables and global variables */
    struct Kv7_Instance  {
        const double* gKv7bar{};
        double* ik{};
        double* gKv7{};
        double* n{};
        double* ek{};
        double* ninf{};
        double* ntau{};
        double* nalpha{};
        double* nbeta{};
        double* Dn{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ek{};
        double* ion_ik{};
        double* ion_dikdv{};
        Kv7_Store* global{&Kv7_global};
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
        return 12;
    }


    static inline int int_variables_size() {
        return 3;
    }


    static inline int get_mech_type() {
        return Kv7_global.mech_type;
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
    static void nrn_private_constructor_Kv7(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new Kv7_Instance{};
        assert(inst->global == &Kv7_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(Kv7_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_Kv7(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<Kv7_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &Kv7_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(Kv7_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<Kv7_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &Kv7_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(Kv7_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gKv7bar = ml->data+0*pnodecount;
        inst->ik = ml->data+1*pnodecount;
        inst->gKv7 = ml->data+2*pnodecount;
        inst->n = ml->data+3*pnodecount;
        inst->ek = ml->data+4*pnodecount;
        inst->ninf = ml->data+5*pnodecount;
        inst->ntau = ml->data+6*pnodecount;
        inst->nalpha = ml->data+7*pnodecount;
        inst->nbeta = ml->data+8*pnodecount;
        inst->Dn = ml->data+9*pnodecount;
        inst->v_unused = ml->data+10*pnodecount;
        inst->g_unused = ml->data+11*pnodecount;
        inst->ion_ek = nt->_data;
        inst->ion_ik = nt->_data;
        inst->ion_dikdv = nt->_data;
    }



    static void nrn_alloc_Kv7(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_Kv7(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Kv7_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_Kv7(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Kv7_Instance*>(ml->instance);

        #endif
    }


    inline int rates_Kv7(int id, int pnodecount, Kv7_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);


    inline int rates_Kv7(int id, int pnodecount, Kv7_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_rates = 0;
        inst->nalpha[id] = 0.036 * exp(0.909 * v / 26.55);
        inst->nbeta[id] = 0.002 * exp( -1.102 * v / 26.55);
        inst->ninf[id] = inst->nalpha[id] / (inst->nalpha[id] + inst->nbeta[id]);
        inst->ntau[id] = 1.0 / (inst->nalpha[id] + inst->nbeta[id]);
        return ret_rates;
    }


    /** initialize channel */
    void nrn_init_Kv7(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<Kv7_Instance*>(ml->instance);

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
                {
                    inst->nalpha[id] = 0.036 * exp(0.909 * v / 26.55);
                    inst->nbeta[id] = 0.002 * exp( -1.102 * v / 26.55);
                    inst->ninf[id] = inst->nalpha[id] / (inst->nalpha[id] + inst->nbeta[id]);
                    inst->ntau[id] = 1.0 / (inst->nalpha[id] + inst->nbeta[id]);
                }
                inst->n[id] = inst->ninf[id];
            }
        }
    }


    inline double nrn_current_Kv7(int id, int pnodecount, Kv7_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->gKv7[id] = inst->gKv7bar[id] * inst->n[id];
        inst->ik[id] = inst->gKv7[id] * (v - inst->ek[id]);
        current += inst->ik[id];
        return current;
    }


    /** update current */
    void nrn_cur_Kv7(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Kv7_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ek[id] = inst->ion_ek[indexes[0*pnodecount + id]];
            double g = nrn_current_Kv7(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double dik = inst->ik[id];
            double rhs = nrn_current_Kv7(id, pnodecount, inst, data, indexes, thread, nt, v);
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
    void nrn_state_Kv7(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<Kv7_Instance*>(ml->instance);

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
                inst->nalpha[id] = 0.036 * exp(0.909 * v / 26.55);
                inst->nbeta[id] = 0.002 * exp( -1.102 * v / 26.55);
                inst->ninf[id] = inst->nalpha[id] / (inst->nalpha[id] + inst->nbeta[id]);
                inst->ntau[id] = 1.0 / (inst->nalpha[id] + inst->nbeta[id]);
            }
            inst->n[id] = inst->n[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->ntau[id]))) * ( -(((inst->ninf[id])) / inst->ntau[id]) / (((( -1.0))) / inst->ntau[id]) - inst->n[id]);
        }
    }


    /** register channel with the simulator */
    void _Kv7_reg() {

        int mech_type = nrn_get_mechtype("Kv7");
        Kv7_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_Kv7, nrn_cur_Kv7, nullptr, nrn_state_Kv7, nrn_init_Kv7, nrn_private_constructor_Kv7, nrn_private_destructor_Kv7, first_pointer_var_index(), 1);
        Kv7_global.k_type = nrn_get_mechtype("k_ion");

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "k_ion");
        hoc_register_dparam_semantics(mech_type, 1, "k_ion");
        hoc_register_dparam_semantics(mech_type, 2, "k_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
