/*********************************************************
Model Name      : CaDynamics_DC0
Filename        : CaDynamics_DC0.mod
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
    #ifndef NRN_PRCELLSTATE
    #define NRN_PRCELLSTATE 0
    #endif


    /** channel information */
    static const char *mechanism_info[] = {
        "7.7.0",
        "CaDynamics_DC0",
        "gamma_CaDynamics_DC0",
        "decay_CaDynamics_DC0",
        0,
        0,
        0,
        0
    };


    /** all global variables */
    struct CaDynamics_DC0_Store {
        int ca_type{};
        double cai0{};
        int reset{};
        int mech_type{};
        double minCai{6.5e-05};
        int slist1[1]{4};
        int dlist1[1]{5};
    };
    static_assert(std::is_trivially_copy_constructible_v<CaDynamics_DC0_Store>);
    static_assert(std::is_trivially_move_constructible_v<CaDynamics_DC0_Store>);
    static_assert(std::is_trivially_copy_assignable_v<CaDynamics_DC0_Store>);
    static_assert(std::is_trivially_move_assignable_v<CaDynamics_DC0_Store>);
    static_assert(std::is_trivially_destructible_v<CaDynamics_DC0_Store>);
    CaDynamics_DC0_Store CaDynamics_DC0_global;


    /** all mechanism instance variables and global variables */
    struct CaDynamics_DC0_Instance  {
        const double* gamma{};
        const double* decay{};
        double* ica{};
        double* surftovol{};
        double* cai{};
        double* Dcai{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ica{};
        const double* ion_cao{};
        double* ion_cai{};
        double* ion_ca_erev{};
        const int* style_ca{};
        double* diam{};
        CaDynamics_DC0_Store* global{&CaDynamics_DC0_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"minCai_CaDynamics_DC0", &CaDynamics_DC0_global.minCai},
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
        return 8;
    }


    static inline int int_variables_size() {
        return 6;
    }


    static inline int get_mech_type() {
        return CaDynamics_DC0_global.mech_type;
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
    static void nrn_private_constructor_CaDynamics_DC0(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new CaDynamics_DC0_Instance{};
        assert(inst->global == &CaDynamics_DC0_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(CaDynamics_DC0_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_CaDynamics_DC0(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<CaDynamics_DC0_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &CaDynamics_DC0_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(CaDynamics_DC0_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<CaDynamics_DC0_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &CaDynamics_DC0_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(CaDynamics_DC0_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gamma = ml->data+0*pnodecount;
        inst->decay = ml->data+1*pnodecount;
        inst->ica = ml->data+2*pnodecount;
        inst->surftovol = ml->data+3*pnodecount;
        inst->cai = ml->data+4*pnodecount;
        inst->Dcai = ml->data+5*pnodecount;
        inst->v_unused = ml->data+6*pnodecount;
        inst->g_unused = ml->data+7*pnodecount;
        inst->ion_ica = nt->_data;
        inst->ion_cao = nt->_data;
        inst->ion_cai = nt->_data;
        inst->ion_ca_erev = nt->_data;
        inst->style_ca = ml->pdata;
        inst->diam = nt->_data;
    }



    static void nrn_alloc_CaDynamics_DC0(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_CaDynamics_DC0(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<CaDynamics_DC0_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_CaDynamics_DC0(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<CaDynamics_DC0_Instance*>(ml->instance);

        #endif
    }


    /** initialize channel */
    void nrn_init_CaDynamics_DC0(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<CaDynamics_DC0_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->ica[id] = inst->ion_ica[indexes[0*pnodecount + id]];
                inst->cai[id] = inst->ion_cai[indexes[2*pnodecount + id]];
                inst->surftovol[id] = 4.0 / inst->diam[indexes[5*pnodecount + id]];
                inst->cai[id] = inst->global->minCai;
                inst->ion_cai[indexes[2*pnodecount + id]] = inst->cai[id];
                int ca_type = inst->global->ca_type;
                nrn_wrote_conc(ca_type, &(inst->ion_cai[indexes[2*pnodecount + id]]), 1, inst->style_ca[4], nrn_ion_global_map, celsius, nt->_ml_list[ca_type]->_nodecount_padded);
            }
        }
    }


    /** update state */
    void nrn_state_CaDynamics_DC0(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<CaDynamics_DC0_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ica[id] = inst->ion_ica[indexes[0*pnodecount + id]];
            inst->cai[id] = inst->ion_cai[indexes[2*pnodecount + id]];
            inst->cai[id] = inst->cai[id] + (1.0 - exp(nt->_dt * (( -((1.0)) / inst->decay[id])))) * ( -((((( -(10000.0)) * (inst->ica[id])) * (inst->surftovol[id])) * (inst->gamma[id])) / (2.0 * FARADAY) - ((( -inst->global->minCai))) / inst->decay[id]) / (( -((1.0)) / inst->decay[id])) - inst->cai[id]);
            inst->ion_cai[indexes[2*pnodecount + id]] = inst->cai[id];
        }
    }


    /** register channel with the simulator */
    void _CaDynamics_DC0_reg() {

        int mech_type = nrn_get_mechtype("CaDynamics_DC0");
        CaDynamics_DC0_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_CaDynamics_DC0, nullptr, nullptr, nrn_state_CaDynamics_DC0, nrn_init_CaDynamics_DC0, nrn_private_constructor_CaDynamics_DC0, nrn_private_destructor_CaDynamics_DC0, first_pointer_var_index(), 1);
        CaDynamics_DC0_global.ca_type = nrn_get_mechtype("ca_ion");

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "ca_ion");
        hoc_register_dparam_semantics(mech_type, 1, "ca_ion");
        hoc_register_dparam_semantics(mech_type, 2, "ca_ion");
        hoc_register_dparam_semantics(mech_type, 3, "ca_ion");
        hoc_register_dparam_semantics(mech_type, 4, "#ca_ion");
        hoc_register_dparam_semantics(mech_type, 5, "diam");
        nrn_writes_conc(mech_type, 0);
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
