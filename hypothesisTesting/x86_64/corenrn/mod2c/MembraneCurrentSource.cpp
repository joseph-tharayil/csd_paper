/*********************************************************
Model Name      : MembraneCurrentSource
Filename        : MembraneCurrentSource.mod
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
        "MembraneCurrentSource",
        "del",
        "dur",
        "amp",
        0,
        "i",
        0,
        0,
        0
    };


    /** all global variables */
    struct MembraneCurrentSource_Store {
        int point_type{};
        int reset{};
        int mech_type{};
    };
    static_assert(std::is_trivially_copy_constructible_v<MembraneCurrentSource_Store>);
    static_assert(std::is_trivially_move_constructible_v<MembraneCurrentSource_Store>);
    static_assert(std::is_trivially_copy_assignable_v<MembraneCurrentSource_Store>);
    static_assert(std::is_trivially_move_assignable_v<MembraneCurrentSource_Store>);
    static_assert(std::is_trivially_destructible_v<MembraneCurrentSource_Store>);
    MembraneCurrentSource_Store MembraneCurrentSource_global;


    /** all mechanism instance variables and global variables */
    struct MembraneCurrentSource_Instance  {
        const double* del{};
        const double* dur{};
        const double* amp{};
        double* i{};
        double* v_unused{};
        double* g_unused{};
        const double* node_area{};
        const int* point_process{};
        MembraneCurrentSource_Store* global{&MembraneCurrentSource_global};
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
        return 6;
    }


    static inline int int_variables_size() {
        return 2;
    }


    static inline int get_mech_type() {
        return MembraneCurrentSource_global.mech_type;
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
    static void nrn_private_constructor_MembraneCurrentSource(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new MembraneCurrentSource_Instance{};
        assert(inst->global == &MembraneCurrentSource_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(MembraneCurrentSource_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_MembraneCurrentSource(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<MembraneCurrentSource_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &MembraneCurrentSource_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(MembraneCurrentSource_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<MembraneCurrentSource_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &MembraneCurrentSource_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(MembraneCurrentSource_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->del = ml->data+0*pnodecount;
        inst->dur = ml->data+1*pnodecount;
        inst->amp = ml->data+2*pnodecount;
        inst->i = ml->data+3*pnodecount;
        inst->v_unused = ml->data+4*pnodecount;
        inst->g_unused = ml->data+5*pnodecount;
        inst->node_area = nt->_data;
        inst->point_process = ml->pdata;
    }



    static void nrn_alloc_MembraneCurrentSource(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_MembraneCurrentSource(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<MembraneCurrentSource_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_MembraneCurrentSource(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<MembraneCurrentSource_Instance*>(ml->instance);

        #endif
    }


    /** initialize channel */
    void nrn_init_MembraneCurrentSource(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<MembraneCurrentSource_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->i[id] = 0.0;
            }
        }
    }


    inline double nrn_current_MembraneCurrentSource(int id, int pnodecount, MembraneCurrentSource_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        at_time(nt, inst->del[id]);
        at_time(nt, inst->del[id] + inst->dur[id]);
        if (nt->_t < inst->del[id] + inst->dur[id] && nt->_t >= inst->del[id]) {
            inst->i[id] =  -1.0 * inst->amp[id];
        } else {
            inst->i[id] = 0.0;
        }
        current += inst->i[id];
        return current;
    }


    /** update current */
    void nrn_cur_MembraneCurrentSource(NrnThread* nt, Memb_list* ml, int type) {
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
        auto* const inst = static_cast<MembraneCurrentSource_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            double g = nrn_current_MembraneCurrentSource(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double rhs = nrn_current_MembraneCurrentSource(id, pnodecount, inst, data, indexes, thread, nt, v);
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
    void nrn_state_MembraneCurrentSource(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<MembraneCurrentSource_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
        }
    }


    /** register channel with the simulator */
    void _MembraneCurrentSource_reg() {

        int mech_type = nrn_get_mechtype("MembraneCurrentSource");
        MembraneCurrentSource_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        point_register_mech(mechanism_info, nrn_alloc_MembraneCurrentSource, nrn_cur_MembraneCurrentSource, nullptr, nrn_state_MembraneCurrentSource, nrn_init_MembraneCurrentSource, nrn_private_constructor_MembraneCurrentSource, nrn_private_destructor_MembraneCurrentSource, first_pointer_var_index(), nullptr, nullptr, 1);

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "area");
        hoc_register_dparam_semantics(mech_type, 1, "pntproc");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
