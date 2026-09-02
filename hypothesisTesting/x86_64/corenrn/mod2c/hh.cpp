/*********************************************************
Model Name      : hh
Filename        : hh.mod
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
        "hh",
        "gnabar_hh",
        "gkbar_hh",
        "gl_hh",
        "el_hh",
        0,
        "gna_hh",
        "gk_hh",
        "il_hh",
        "minf_hh",
        "hinf_hh",
        "ninf_hh",
        "mtau_hh",
        "htau_hh",
        "ntau_hh",
        0,
        "m_hh",
        "h_hh",
        "n_hh",
        0,
        0
    };


    /** all global variables */
    struct hh_Store {
        int na_type{};
        int k_type{};
        double m0{};
        double h0{};
        double n0{};
        int reset{};
        int mech_type{};
        int slist1[3]{13, 14, 15};
        int dlist1[3]{16, 17, 18};
    };
    static_assert(std::is_trivially_copy_constructible_v<hh_Store>);
    static_assert(std::is_trivially_move_constructible_v<hh_Store>);
    static_assert(std::is_trivially_copy_assignable_v<hh_Store>);
    static_assert(std::is_trivially_move_assignable_v<hh_Store>);
    static_assert(std::is_trivially_destructible_v<hh_Store>);
    hh_Store hh_global;


    /** all mechanism instance variables and global variables */
    struct hh_Instance  {
        double* celsius{&coreneuron::celsius};
        const double* gnabar{};
        const double* gkbar{};
        const double* gl{};
        const double* el{};
        double* gna{};
        double* gk{};
        double* il{};
        double* minf{};
        double* hinf{};
        double* ninf{};
        double* mtau{};
        double* htau{};
        double* ntau{};
        double* m{};
        double* h{};
        double* n{};
        double* Dm{};
        double* Dh{};
        double* Dn{};
        double* ena{};
        double* ek{};
        double* ina{};
        double* ik{};
        double* v_unused{};
        double* g_unused{};
        const double* ion_ena{};
        double* ion_ina{};
        double* ion_dinadv{};
        const double* ion_ek{};
        double* ion_ik{};
        double* ion_dikdv{};
        hh_Store* global{&hh_global};
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
        return 25;
    }


    static inline int int_variables_size() {
        return 6;
    }


    static inline int get_mech_type() {
        return hh_global.mech_type;
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
    static void nrn_private_constructor_hh(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new hh_Instance{};
        assert(inst->global == &hh_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(hh_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_hh(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<hh_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &hh_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(hh_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<hh_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &hh_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(hh_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->gnabar = ml->data+0*pnodecount;
        inst->gkbar = ml->data+1*pnodecount;
        inst->gl = ml->data+2*pnodecount;
        inst->el = ml->data+3*pnodecount;
        inst->gna = ml->data+4*pnodecount;
        inst->gk = ml->data+5*pnodecount;
        inst->il = ml->data+6*pnodecount;
        inst->minf = ml->data+7*pnodecount;
        inst->hinf = ml->data+8*pnodecount;
        inst->ninf = ml->data+9*pnodecount;
        inst->mtau = ml->data+10*pnodecount;
        inst->htau = ml->data+11*pnodecount;
        inst->ntau = ml->data+12*pnodecount;
        inst->m = ml->data+13*pnodecount;
        inst->h = ml->data+14*pnodecount;
        inst->n = ml->data+15*pnodecount;
        inst->Dm = ml->data+16*pnodecount;
        inst->Dh = ml->data+17*pnodecount;
        inst->Dn = ml->data+18*pnodecount;
        inst->ena = ml->data+19*pnodecount;
        inst->ek = ml->data+20*pnodecount;
        inst->ina = ml->data+21*pnodecount;
        inst->ik = ml->data+22*pnodecount;
        inst->v_unused = ml->data+23*pnodecount;
        inst->g_unused = ml->data+24*pnodecount;
        inst->ion_ena = nt->_data;
        inst->ion_ina = nt->_data;
        inst->ion_dinadv = nt->_data;
        inst->ion_ek = nt->_data;
        inst->ion_ik = nt->_data;
        inst->ion_dikdv = nt->_data;
    }



    static void nrn_alloc_hh(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_hh(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<hh_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_hh(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<hh_Instance*>(ml->instance);

        #endif
    }


    inline double vtrap_hh(int id, int pnodecount, hh_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double x, double y);
    inline int rates_hh(int id, int pnodecount, hh_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double arg_v);


    inline int rates_hh(int id, int pnodecount, hh_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double arg_v) {
        int ret_rates = 0;
        double alpha, beta, sum, q10, vtrap_in_0, vtrap_in_1;
        q10 = pow(3.0, ((*(inst->celsius) - 6.3) / 10.0));
        {
            double x_in_0, y_in_0;
            x_in_0 =  -(arg_v + 40.0);
            y_in_0 = 10.0;
            if (fabs(x_in_0 / y_in_0) < 1e-6) {
                vtrap_in_0 = y_in_0 * (1.0 - x_in_0 / y_in_0 / 2.0);
            } else {
                vtrap_in_0 = x_in_0 / (exp(x_in_0 / y_in_0) - 1.0);
            }
        }
        alpha = .1 * vtrap_in_0;
        beta = 4.0 * exp( -(arg_v + 65.0) / 18.0);
        sum = alpha + beta;
        inst->mtau[id] = 1.0 / (q10 * sum);
        inst->minf[id] = alpha / sum;
        alpha = .07 * exp( -(arg_v + 65.0) / 20.0);
        beta = 1.0 / (exp( -(arg_v + 35.0) / 10.0) + 1.0);
        sum = alpha + beta;
        inst->htau[id] = 1.0 / (q10 * sum);
        inst->hinf[id] = alpha / sum;
        {
            double x_in_1, y_in_1;
            x_in_1 =  -(arg_v + 55.0);
            y_in_1 = 10.0;
            if (fabs(x_in_1 / y_in_1) < 1e-6) {
                vtrap_in_1 = y_in_1 * (1.0 - x_in_1 / y_in_1 / 2.0);
            } else {
                vtrap_in_1 = x_in_1 / (exp(x_in_1 / y_in_1) - 1.0);
            }
        }
        alpha = .01 * vtrap_in_1;
        beta = .125 * exp( -(arg_v + 65.0) / 80.0);
        sum = alpha + beta;
        inst->ntau[id] = 1.0 / (q10 * sum);
        inst->ninf[id] = alpha / sum;
        return ret_rates;
    }


    inline double vtrap_hh(int id, int pnodecount, hh_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v, double x, double y) {
        double ret_vtrap = 0.0;
        if (fabs(x / y) < 1e-6) {
            ret_vtrap = y * (1.0 - x / y / 2.0);
        } else {
            ret_vtrap = x / (exp(x / y) - 1.0);
        }
        return ret_vtrap;
    }


    /** initialize channel */
    void nrn_init_hh(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<hh_Instance*>(ml->instance);

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
                inst->ek[id] = inst->ion_ek[indexes[3*pnodecount + id]];
                inst->m[id] = inst->global->m0;
                inst->h[id] = inst->global->h0;
                inst->n[id] = inst->global->n0;
                {
                    double alpha, beta, sum, q10, vtrap_in_0, vtrap_in_1, v_in_0;
                    v_in_0 = v;
                    q10 = pow(3.0, ((*(inst->celsius) - 6.3) / 10.0));
                    {
                        double x_in_0, y_in_0;
                        x_in_0 =  -(v_in_0 + 40.0);
                        y_in_0 = 10.0;
                        if (fabs(x_in_0 / y_in_0) < 1e-6) {
                            vtrap_in_0 = y_in_0 * (1.0 - x_in_0 / y_in_0 / 2.0);
                        } else {
                            vtrap_in_0 = x_in_0 / (exp(x_in_0 / y_in_0) - 1.0);
                        }
                    }
                    alpha = .1 * vtrap_in_0;
                    beta = 4.0 * exp( -(v_in_0 + 65.0) / 18.0);
                    sum = alpha + beta;
                    inst->mtau[id] = 1.0 / (q10 * sum);
                    inst->minf[id] = alpha / sum;
                    alpha = .07 * exp( -(v_in_0 + 65.0) / 20.0);
                    beta = 1.0 / (exp( -(v_in_0 + 35.0) / 10.0) + 1.0);
                    sum = alpha + beta;
                    inst->htau[id] = 1.0 / (q10 * sum);
                    inst->hinf[id] = alpha / sum;
                    {
                        double x_in_1, y_in_1;
                        x_in_1 =  -(v_in_0 + 55.0);
                        y_in_1 = 10.0;
                        if (fabs(x_in_1 / y_in_1) < 1e-6) {
                            vtrap_in_1 = y_in_1 * (1.0 - x_in_1 / y_in_1 / 2.0);
                        } else {
                            vtrap_in_1 = x_in_1 / (exp(x_in_1 / y_in_1) - 1.0);
                        }
                    }
                    alpha = .01 * vtrap_in_1;
                    beta = .125 * exp( -(v_in_0 + 65.0) / 80.0);
                    sum = alpha + beta;
                    inst->ntau[id] = 1.0 / (q10 * sum);
                    inst->ninf[id] = alpha / sum;
                }
                inst->m[id] = inst->minf[id];
                inst->h[id] = inst->hinf[id];
                inst->n[id] = inst->ninf[id];
            }
        }
    }


    inline double nrn_current_hh(int id, int pnodecount, hh_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->gna[id] = inst->gnabar[id] * inst->m[id] * inst->m[id] * inst->m[id] * inst->h[id];
        inst->ina[id] = inst->gna[id] * (v - inst->ena[id]);
        inst->gk[id] = inst->gkbar[id] * inst->n[id] * inst->n[id] * inst->n[id] * inst->n[id];
        inst->ik[id] = inst->gk[id] * (v - inst->ek[id]);
        inst->il[id] = inst->gl[id] * (v - inst->el[id]);
        current += inst->il[id];
        current += inst->ina[id];
        current += inst->ik[id];
        return current;
    }


    /** update current */
    void nrn_cur_hh(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        double* vec_rhs = nt->_actual_rhs;
        double* vec_d = nt->_actual_d;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<hh_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
            inst->ek[id] = inst->ion_ek[indexes[3*pnodecount + id]];
            double g = nrn_current_hh(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double dina = inst->ina[id];
            double dik = inst->ik[id];
            double rhs = nrn_current_hh(id, pnodecount, inst, data, indexes, thread, nt, v);
            g = (g-rhs)/0.001;
            inst->ion_dinadv[indexes[2*pnodecount + id]] += (dina-inst->ina[id])/0.001;
            inst->ion_dikdv[indexes[5*pnodecount + id]] += (dik-inst->ik[id])/0.001;
            inst->ion_ina[indexes[1*pnodecount + id]] += inst->ina[id];
            inst->ion_ik[indexes[4*pnodecount + id]] += inst->ik[id];
            #if NRN_PRCELLSTATE
            inst->g_unused[id] = g;
            #endif
            vec_rhs[node_id] -= rhs;
            vec_d[node_id] += g;
        }
    }


    /** update state */
    void nrn_state_hh(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<hh_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->ena[id] = inst->ion_ena[indexes[0*pnodecount + id]];
            inst->ek[id] = inst->ion_ek[indexes[3*pnodecount + id]];
            {
                double alpha, beta, sum, q10, vtrap_in_0, vtrap_in_1, v_in_1;
                v_in_1 = v;
                q10 = pow(3.0, ((*(inst->celsius) - 6.3) / 10.0));
                {
                    double x_in_0, y_in_0;
                    x_in_0 =  -(v_in_1 + 40.0);
                    y_in_0 = 10.0;
                    if (fabs(x_in_0 / y_in_0) < 1e-6) {
                        vtrap_in_0 = y_in_0 * (1.0 - x_in_0 / y_in_0 / 2.0);
                    } else {
                        vtrap_in_0 = x_in_0 / (exp(x_in_0 / y_in_0) - 1.0);
                    }
                }
                alpha = .1 * vtrap_in_0;
                beta = 4.0 * exp( -(v_in_1 + 65.0) / 18.0);
                sum = alpha + beta;
                inst->mtau[id] = 1.0 / (q10 * sum);
                inst->minf[id] = alpha / sum;
                alpha = .07 * exp( -(v_in_1 + 65.0) / 20.0);
                beta = 1.0 / (exp( -(v_in_1 + 35.0) / 10.0) + 1.0);
                sum = alpha + beta;
                inst->htau[id] = 1.0 / (q10 * sum);
                inst->hinf[id] = alpha / sum;
                {
                    double x_in_1, y_in_1;
                    x_in_1 =  -(v_in_1 + 55.0);
                    y_in_1 = 10.0;
                    if (fabs(x_in_1 / y_in_1) < 1e-6) {
                        vtrap_in_1 = y_in_1 * (1.0 - x_in_1 / y_in_1 / 2.0);
                    } else {
                        vtrap_in_1 = x_in_1 / (exp(x_in_1 / y_in_1) - 1.0);
                    }
                }
                alpha = .01 * vtrap_in_1;
                beta = .125 * exp( -(v_in_1 + 65.0) / 80.0);
                sum = alpha + beta;
                inst->ntau[id] = 1.0 / (q10 * sum);
                inst->ninf[id] = alpha / sum;
            }
            inst->m[id] = inst->m[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->mtau[id]))) * ( -(((inst->minf[id])) / inst->mtau[id]) / (((( -1.0))) / inst->mtau[id]) - inst->m[id]);
            inst->h[id] = inst->h[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->htau[id]))) * ( -(((inst->hinf[id])) / inst->htau[id]) / (((( -1.0))) / inst->htau[id]) - inst->h[id]);
            inst->n[id] = inst->n[id] + (1.0 - exp(nt->_dt * (((( -1.0))) / inst->ntau[id]))) * ( -(((inst->ninf[id])) / inst->ntau[id]) / (((( -1.0))) / inst->ntau[id]) - inst->n[id]);
        }
    }


    /** register channel with the simulator */
    void _hh_reg() {

        int mech_type = nrn_get_mechtype("hh");
        hh_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        register_mech(mechanism_info, nrn_alloc_hh, nrn_cur_hh, nullptr, nrn_state_hh, nrn_init_hh, nrn_private_constructor_hh, nrn_private_destructor_hh, first_pointer_var_index(), 1);
        hh_global.na_type = nrn_get_mechtype("na_ion");
        hh_global.k_type = nrn_get_mechtype("k_ion");

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "na_ion");
        hoc_register_dparam_semantics(mech_type, 1, "na_ion");
        hoc_register_dparam_semantics(mech_type, 2, "na_ion");
        hoc_register_dparam_semantics(mech_type, 3, "k_ion");
        hoc_register_dparam_semantics(mech_type, 4, "k_ion");
        hoc_register_dparam_semantics(mech_type, 5, "k_ion");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
