/*********************************************************
Model Name      : KdShu2007
Filename        : KdShu2007.mod
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
        "KdShu2007",
        "gkbar_KdShu2007",
        "ek_KdShu2007",
        0,
        "ik_KdShu2007",
        "minf_KdShu2007",
        "mtau_KdShu2007",
        "hinf_KdShu2007",
        "htau_KdShu2007",
        0,
        "m_KdShu2007",
        "h_KdShu2007",
        0,
        0
    };


    /** all global variables */
    struct KdShu2007_Store {
        int k_type{};
        double m0{};
        double h0{};
        int reset{};
        int mech_type{};
        double vhalfm{-43};
        double km{8};
        double vhalfh{-67};
        double kh{7.3};
        double q10{2.3};
        int slist1[2]{7, 8};
        int dlist1[2]{9, 10};
    };
    static_assert(std::is_trivially_copy_constructible_v<KdShu2007_Store>);
    static_assert(std::is_trivially_move_constructible_v<KdShu2007_Store>);
    static_assert(std::is_trivially_copy_assignable_v<KdShu2007_Store>);
    static_assert(std::is_trivially_move_assignable_v<KdShu2007_Store>);
    static_assert(std::is_trivially_destructible_v<KdShu2007_Store>);
    KdShu2007_Store KdShu2007_global;


    /** all mechanism instance variables and global variables */
    struct KdShu2007_Instance  {
        double* celsius{&coreneuron::celsius};
        const double* gkbar{};
        const double* ek{};
        double* ik{};
        double* minf{};
        double* mtau{};
        double* hinf{};
        double* htau{};
        double* m{};
        double* h{};
        double* Dm{};
        double* Dh{};
        double* v_unused{};
        double* g_unused{};
        double* ion_ik{};
        double* ion_dikdv{};
        KdShu2007_Store* global{&KdShu2007_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"vhalfm_KdShu2007", &KdShu2007_global.vhalfm},
        {"km_KdShu2007", &KdShu2007_global.km},
        {"vhalfh_KdShu2007", &KdShu2007_global.vhalfh},
        {"kh_KdShu2007", &KdShu2007_global.kh},
        {"q10_KdShu2007", &KdShu2007_global.q10},
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
        return 13;
    }


    static inline int int_variables_size() {
        return 2;
    }


    static inline int get_mech_type() {
        return KdShu2007_global.mech_type;
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
    static void nrn_private_constructor_KdShu2007(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new KdShu2007_Instance{};
        assert(inst->global == &KdShu2007_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(KdShu2007_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_KdShu2007(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<KdShu2007_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &KdShu2007_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(KdShu2007_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<KdShu2007_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &KdShu2007_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(KdShu2007_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gkbar = ml->data+0*pnodecount;
        inst->ek = ml->data+1*pnodecount;
        inst->ik = ml->data+2*pnodecount;
        inst->minf = ml->data+3*pnodecount;
        inst->mtau = ml->data+4*pnodecount;
        inst->hinf = ml->data+5*pnodecount;
        inst->htau = ml->data+6*pnodecount;
        inst->m = ml->data+7*pnodecount;
        inst->h = ml->data+8*pnodecount;
        inst->Dm = ml->data+9*pnodecount;
        inst->Dh = ml->data+10*pnodecount;
        inst->v_unused = ml->data+11*pnodecount;
        inst->g_unused = ml->data+12*pnodecount;
        inst->ion_ik = nt->_data;
        inst->ion_dikdv = nt->_data;
    }



    static void nrn_alloc_KdShu2007(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_KdShu2007(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<KdShu2007_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_KdShu2007(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<KdShu2007_Instance*>(ml->instance);

        #endif
    }


    inline int trates_KdShu2007(int id, int pnodecount, KdShu2007_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double arg_v);


    inline int trates_KdShu2007(int id, int pnodecount, KdShu2007_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double arg_v) {
        int ret_trates = 0;
        double qt;
        qt = pow(inst->global->q10, ((*(inst->celsius) - 22.0) / 10.0));
        inst->minf[id] = 1.0 - 1.0 / (1.0 + exp((arg_v - inst->global->vhalfm) / inst->global->km));
        inst->hinf[id] = 1.0 / (1.0 + exp((arg_v - inst->global->vhalfh) / inst->global->kh));
        inst->mtau[id] = 0.6;
        inst->htau[id] = 1500.0;
        return ret_trates;
    }


    /** initialize channel */
    void nrn_init_KdShu2007(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<KdShu2007_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                inst->m[id] = inst->global->m0;
                inst->h[id] = inst->global->h0;
                {
                    double qt, v_in_0;
                    v_in_0 = v;
                    qt = pow(inst->global->q10, ((*(inst->celsius) - 22.0) / 10.0));
                    inst->minf[id] = 1.0 - 1.0 / (1.0 + exp((v_in_0 - inst->global->vhalfm) / inst->global->km));
                    inst->hinf[id] = 1.0 / (1.0 + exp((v_in_0 - inst->global->vhalfh) / inst->global->kh));
                    inst->mtau[id] = 0.6;
                    inst->htau[id] = 1500.0;
                }
                inst->m[id] = inst->minf[id];
                inst->h[id] = inst->hinf[id];
            }
        }
    }


    inline double nrn_current_KdShu2007(int id, int pnodecount, KdShu2007_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->ik[id] = inst->gkbar[id] * inst->m[id] * inst->h[id] * (v - inst->ek[id]);
        current += inst->ik[id];
        return current;
    }


    /** update current */
    void nrn_cur_KdShu2007(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<KdShu2007_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            double g = nrn_current_KdShu2007(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double dik = inst->ik[id];
            double rhs = nrn_current_KdShu2007(id, pnodecount, inst, data, indexes, thread, nt, v);
            g = (g-rhs)/0.001;
            inst->ion_dikdv[indexes[1*pnodecount + id]] += (dik-inst->ik[id])/0.001;
            inst->ion_ik[indexes[0*pnodecount + id]] += inst->ik[id];
            #if NRN_PRCELLSTATE
            inst->g_unused[id] = g;
            #endif
            vec_rhs[node_id] -= rhs;
            vec_d[node_id] += g;
        }
    }


    /** update state */
    void nrn_state_KdShu2007(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<KdShu2007_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            {
                double qt, v_in_1;
                v_in_1 = v;
                qt = pow(inst->global->q10, ((*(inst->celsius) - 22.0) / 10.0));
                inst->minf[id] = 1.0 - 1.0 / (1.0 + exp((v_in_1 - inst->global->vhalfm) / inst->global->km));
                inst->hinf[id] = 1.0 / (1.0 + exp((v_in_1 - inst->global->vhalfh) / inst->global->kh));
                inst->mtau[id] = 0.6;
                inst->htau[id] = 1500.0;
            }
            inst->m[id] = inst->m[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->mtau[id]))) * ( -(((inst->minf[id])) / inst->mtau[id]) / (((( -1.0))) / inst->mtau[id]) - inst->m[id]);
            inst->h[id] = inst->h[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->htau[id]))) * ( -(((inst->hinf[id])) / inst->htau[id]) / (((( -1.0))) / inst->htau[id]) - inst->h[id]);
        }
    }


    /** register channel with the simulator */
    void _KdShu2007_reg() {

        int mech_type = nrn_get_mechtype("KdShu2007");
        KdShu2007_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_KdShu2007, nrn_cur_KdShu2007, nullptr, nrn_state_KdShu2007, nrn_init_KdShu2007, nrn_private_constructor_KdShu2007, nrn_private_destructor_KdShu2007, first_pointer_var_index(), 1);
        KdShu2007_global.k_type = nrn_get_mechtype("k_ion");

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "k_ion");
        hoc_register_dparam_semantics(mech_type, 1, "k_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
