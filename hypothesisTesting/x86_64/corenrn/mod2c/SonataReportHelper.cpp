/*********************************************************
Model Name      : SonataReportHelper
Filename        : SonataReportHelper.mod
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
        "SonataReportHelper",
        "activeStep",
        "initialStep",
        0,
        0,
        0,
        0
    };


    /** all global variables */
    struct SonataReportHelper_Store {
        int point_type{};
        int reset{};
        int mech_type{};
        double Dt{0.1};
    };
    static_assert(std::is_trivially_copy_constructible_v<SonataReportHelper_Store>);
    static_assert(std::is_trivially_move_constructible_v<SonataReportHelper_Store>);
    static_assert(std::is_trivially_copy_assignable_v<SonataReportHelper_Store>);
    static_assert(std::is_trivially_move_assignable_v<SonataReportHelper_Store>);
    static_assert(std::is_trivially_destructible_v<SonataReportHelper_Store>);
    SonataReportHelper_Store SonataReportHelper_global;


    /** all mechanism instance variables and global variables */
    struct SonataReportHelper_Instance  {
        double* activeStep{};
        double* initialStep{};
        double* v_unused{};
        double* tsave{};
        const double* node_area{};
        void** point_process{};
        void** tqitem{};
        SonataReportHelper_Store* global{&SonataReportHelper_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"Dt_SonataReportHelper", &SonataReportHelper_global.Dt},
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


    static inline int num_net_receive_args() {
        return 1;
    }


    static inline int float_variables_size() {
        return 4;
    }


    static inline int int_variables_size() {
        return 3;
    }


    static inline int get_mech_type() {
        return SonataReportHelper_global.mech_type;
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
    static void nrn_private_constructor_SonataReportHelper(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new SonataReportHelper_Instance{};
        assert(inst->global == &SonataReportHelper_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(SonataReportHelper_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_SonataReportHelper(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<SonataReportHelper_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &SonataReportHelper_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(SonataReportHelper_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<SonataReportHelper_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &SonataReportHelper_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(SonataReportHelper_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->activeStep = ml->data+0*pnodecount;
        inst->initialStep = ml->data+1*pnodecount;
        inst->v_unused = ml->data+2*pnodecount;
        inst->tsave = ml->data+3*pnodecount;
        inst->node_area = nt->_data;
        inst->point_process = nt->_vdata;
        inst->tqitem = nt->_vdata;
    }



    static void nrn_alloc_SonataReportHelper(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_SonataReportHelper(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SonataReportHelper_Instance*>(ml->instance);

         {
        /**
        * \param 1: Dt (double, optional). If not given no initializaton is performed
        * \param 2: register_recalc_ptr (double, optional). By default will invoke
        *    nrn_register_recalc_ptr_callback, which can be disabled by passing 0
        */
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            if( !ifarg(1) ) {
                return;
            }
            inst->global->Dt = *getarg(1);
            sonata_set_atomic_step(inst->global->Dt);
        #ifndef NRN_MECHANISM_DATA_IS_SOA
            int register_recalc_ptr = 1;
            if( ifarg(2) ) {
                register_recalc_ptr = (int)*getarg(2);
            }
            if( register_recalc_ptr ) {
                nrn_register_recalc_ptr_callback( sonataRefreshPointers );
            }
        #endif
        #endif
        #endif
        }

        #endif
    }


    void nrn_destructor_SonataReportHelper(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SonataReportHelper_Instance*>(ml->instance);

        #endif
    }


    inline double redirect_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int make_comm_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int prepare_datasets_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int disable_auto_flush_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int set_steps_to_buffer_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int set_max_buffer_size_hint_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int flush_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int pre_savestate_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int savestate_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int restoretime_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int restorestate_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int clear_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int create_spikefile_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int write_spike_populations_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int close_spikefile_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int write_spikes_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int add_spikes_population_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
}


using namespace coreneuron;


#ifndef CORENEURON_BUILD
#ifndef DISABLE_REPORTINGLIB
#include <stdint.h>
#include <bbp/sonata/reports.h>
#include <mpi.h>
#ifndef NRN_VERSION_GTEQ_8_2_0
    extern int ifarg(int iarg);
    extern double* getarg(int iarg);
    extern double* vector_vec();
    extern int vector_capacity();
    extern void* vector_arg(int);
    extern void nrn_register_recalc_ptr_callback(void (*f)(void));
    extern double* nrn_recalc_ptr(double*);
#endif
#ifndef NRN_MECHANISM_DATA_IS_SOA
    void sonataRefreshPointers() { 
        sonata_refresh_pointers(nrn_recalc_ptr); 
    }
#endif
#endif
#endif


namespace coreneuron {


    inline int make_comm_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_make_comm = 0;
        {
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            sonata_setup_communicators();
        #endif
        #endif
        }

        return ret_make_comm;
    }


    inline int prepare_datasets_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_prepare_datasets = 0;
        {
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            sonata_prepare_datasets();
        #endif
        #endif
        }

        return ret_prepare_datasets;
    }


    inline int disable_auto_flush_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_disable_auto_flush = 0;
        return ret_disable_auto_flush;
    }


    inline int set_steps_to_buffer_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_set_steps_to_buffer = 0;
        return ret_set_steps_to_buffer;
    }


    inline int set_max_buffer_size_hint_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_set_max_buffer_size_hint = 0;
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            int buffer_size = (int) *getarg(1);
            sonata_set_max_buffer_size_hint(buffer_size);
        #endif
        #endif

        return ret_set_max_buffer_size_hint;
    }


    inline int flush_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_flush = 0;
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            sonata_flush( nt->_t );
        #endif
        #endif

        return ret_flush;
    }


    inline int pre_savestate_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_pre_savestate = 0;
        return ret_pre_savestate;
    }


    inline int savestate_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_savestate = 0;
        return ret_savestate;
    }


    inline int restoretime_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_restoretime = 0;
        inst->initialStep[id] = nt->_t / inst->global->Dt;
        return ret_restoretime;
    }


    inline int restorestate_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_restorestate = 0;
        inst->activeStep[id] = nt->_t / inst->global->Dt;
        return ret_restorestate;
    }


    inline int clear_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_clear = 0;
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            sonata_clear();
        #endif
        #endif

        return ret_clear;
    }


    inline int create_spikefile_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_create_spikefile = 0;
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            char output_dir[256] = ".";
            if (ifarg(1)) {
                sprintf(output_dir,"%s", gargstr(1));
            }
            char file_name[256] = "out";
            if (ifarg(2)) {
                sprintf(file_name,"%s", gargstr(2));
            }
            sonata_create_spikefile(output_dir, file_name);
        #endif
        #endif

        return ret_create_spikefile;
    }


    inline int write_spike_populations_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_write_spike_populations = 0;
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            sonata_write_spike_populations();
        #endif
        #endif

        return ret_write_spike_populations;
    }


    inline int close_spikefile_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_close_spikefile = 0;
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            sonata_close_spikefile();
        #endif
        #endif

        return ret_close_spikefile;
    }


    inline int write_spikes_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_write_spikes = 0;
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            char output_dir[256] = ".";
            char population_name[256] = "All";
            char file_name[256] = "out";
            double *time = NULL, *gid = NULL;
            int num_spikes = 0;
            int num_gids = 0;
            IvocVect* v1;
            IvocVect* v2;
            if (ifarg(1)) {
                v1 = vector_arg(1);
                time = vector_vec(v1);
                num_spikes = vector_capacity(v1);
            }
            if (ifarg(2)) {
                v2 = vector_arg(2);
                gid = vector_vec(v2);
                num_gids = vector_capacity(v2);
            }
            if (ifarg(3)) {
                sprintf(output_dir,"%s", gargstr(3));
            }
            if (ifarg(4)) {
                sprintf(population_name,"%s", gargstr(4));
            }
            int* int_gid = (int*)malloc(num_gids * sizeof(int));
            int i;
            for(i=0; i<num_spikes; ++i) {
                int_gid[i] = (int)gid[i];
            }
            sonata_create_spikefile(output_dir, file_name);
            sonata_add_spikes_population(population_name, 0, time, num_spikes, int_gid, num_gids);
            sonata_write_spike_populations();
            sonata_close_spikefile();
            free(int_gid);
        #endif
        #endif

        return ret_write_spikes;
    }


    inline int add_spikes_population_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_add_spikes_population = 0;
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            char population_name[256] = "All";
            int population_offset = 0;
            double *time = NULL, *gid = NULL;
            int num_spikes = 0;
            int num_gids = 0;
            IvocVect* v1;
            IvocVect* v2;
            if (ifarg(1)) {
                v1 = vector_arg(1);
                time = vector_vec(v1);
                num_spikes = vector_capacity(v1);
            }
            if (ifarg(2)) {
                v2 = vector_arg(2);
                gid = vector_vec(v2);
                num_gids = vector_capacity(v2);
            }
            if (ifarg(3)) {
                sprintf(population_name,"%s", gargstr(3));
            }
            if (ifarg(4)) {
                population_offset = (int) *getarg(4);
            }
            int* int_gid = (int*)malloc(num_gids * sizeof(int));
            int i;
            for(i=0; i<num_spikes; ++i) {
                int_gid[i] = (int)gid[i];
            }
            sonata_add_spikes_population(population_name, population_offset, time, num_spikes, int_gid, num_gids);
            free(int_gid);
        #endif
        #endif

        return ret_add_spikes_population;
    }


    inline double redirect_SonataReportHelper(int id, int pnodecount, SonataReportHelper_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_redirect = 0.0;
        return ret_redirect;
    }


    static inline void net_receive_SonataReportHelper(Point_process* pnt, int weight_index, double flag) {
        int tid = pnt->_tid;
        int id = pnt->_i_instance;
        double v = 0;
        NrnThread* nt = nrn_threads + tid;
        Memb_list* ml = nt->_ml_list[pnt->_type];
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        double* data = ml->data;
        double* weights = nt->weights;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SonataReportHelper_Instance*>(ml->instance);

        double t = nt->_t;
        inst->tsave[id] = t;
        {
            #ifndef CORENEURON_BUILD
            #ifndef DISABLE_REPORTINGLIB
                sonata_record_data(inst->activeStep[id]);
                inst->activeStep[id]++;
            #endif
            #endif

            artcell_net_send(&inst->tqitem[indexes[2*pnodecount + id]], weight_index, pnt, nt->_t+inst->global->Dt, 1.0);
        }
    }


    /** initialize channel */
    void nrn_init_SonataReportHelper(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<SonataReportHelper_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                inst->tsave[id] = -1e20;
                double v = 0.0;
                inst->activeStep[id] = inst->initialStep[id];
                artcell_net_send(&inst->tqitem[indexes[2*pnodecount + id]], 0, (Point_process*)inst->point_process[indexes[1*pnodecount + id]], nt->_t+inst->initialStep[id] * inst->global->Dt, 1.0);
            }
        }
    }


    /** register channel with the simulator */
    void _SonataReportHelper_reg() {

        int mech_type = nrn_get_mechtype("SonataReportHelper");
        SonataReportHelper_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        point_register_mech(mechanism_info, nrn_alloc_SonataReportHelper, nullptr, nullptr, nullptr, nrn_init_SonataReportHelper, nrn_private_constructor_SonataReportHelper, nrn_private_destructor_SonataReportHelper, first_pointer_var_index(), nrn_constructor_SonataReportHelper, nullptr, 1);

        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "area");
        hoc_register_dparam_semantics(mech_type, 1, "pntproc");
        hoc_register_dparam_semantics(mech_type, 2, "netsend");
        add_nrn_artcell(mech_type, 2);
        set_pnt_receive(mech_type, net_receive_SonataReportHelper, nullptr, num_net_receive_args());
        hoc_register_net_send_buffering(mech_type);
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
