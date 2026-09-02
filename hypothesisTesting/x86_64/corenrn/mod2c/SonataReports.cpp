/*********************************************************
Model Name      : SonataReport
Filename        : SonataReports.mod
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
        "SonataReport",
        "Dt",
        "tstart",
        "tstop",
        0,
        0,
        0,
        "ptr",
        0
    };


    /** all global variables */
    struct SonataReport_Store {
        int point_type{};
        int reset{};
        int mech_type{};
    };
    static_assert(std::is_trivially_copy_constructible_v<SonataReport_Store>);
    static_assert(std::is_trivially_move_constructible_v<SonataReport_Store>);
    static_assert(std::is_trivially_copy_assignable_v<SonataReport_Store>);
    static_assert(std::is_trivially_move_assignable_v<SonataReport_Store>);
    static_assert(std::is_trivially_destructible_v<SonataReport_Store>);
    SonataReport_Store SonataReport_global;


    /** all mechanism instance variables and global variables */
    struct SonataReport_Instance  {
        double* Dt{};
        double* tstart{};
        double* tstop{};
        double* v_unused{};
        const double* node_area{};
        const int* point_process{};
        void** ptr{};
        SonataReport_Store* global{&SonataReport_global};
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
        return 2;
    }


    static inline int first_random_var_index() {
        return -1;
    }


    static inline int float_variables_size() {
        return 4;
    }


    static inline int int_variables_size() {
        return 3;
    }


    static inline int get_mech_type() {
        return SonataReport_global.mech_type;
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
    static void nrn_private_constructor_SonataReport(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new SonataReport_Instance{};
        assert(inst->global == &SonataReport_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(SonataReport_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_SonataReport(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<SonataReport_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &SonataReport_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(SonataReport_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<SonataReport_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &SonataReport_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(SonataReport_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->Dt = ml->data+0*pnodecount;
        inst->tstart = ml->data+1*pnodecount;
        inst->tstop = ml->data+2*pnodecount;
        inst->v_unused = ml->data+3*pnodecount;
        inst->node_area = nt->_data;
        inst->point_process = ml->pdata;
        inst->ptr = nt->_vdata;
    }



    static void nrn_alloc_SonataReport(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_SonataReport(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SonataReport_Instance*>(ml->instance);

         {
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            Data** tempdata = (Data**)(&(inst->ptr[indexes[2*pnodecount + id]]));
            Data* data = 0;
            data = (Data*)hoc_Emalloc(sizeof(Data));
            hoc_malchk();
            if(ifarg(2) && hoc_is_str_arg(2) &&
                ifarg(3) && hoc_is_str_arg(3) &&
                ifarg(4) && ifarg(5) && ifarg(6) &&
                ifarg(7) && hoc_is_str_arg(7)
            )
            {
                sprintf(data->rptName_,"%s/%s",gargstr(3),gargstr(2));
                inst->tstart[id] = *getarg(4);
                inst->tstop[id] = *getarg(5);
                inst->Dt[id] = *getarg(6);
                sonata_create_report(data->rptName_, inst->tstart[id], inst->tstop[id], inst->Dt[id], gargstr(7), gargstr(8));
                *tempdata = data; 
            }
            else
            {
                int i = 1;
                while(ifarg(i))
                {
                    if(i==1)
                        printf("There is an error creating report\n");
                    printf("It has arg %d: ", i);
                    if(hoc_is_str_arg(i))
                        printf("%s\n",gargstr(i));
                    else
                        printf("%d\n",(int)*getarg(i));
                    i++;
                }
            }
        #else
                static int warning_shown = 0;
                if (ifarg(2) && hoc_is_str_arg(2))
                {
                    if (warning_shown == 0)
                    {
                        printf("WARNING: BinReports Constructor(): Trying to create and write report %s while the NEURODAMUS_DISABLE_REPORTINGLIB is set to ON, ignoring... \n", gargstr(2));
                        warning_shown++;
                    }
                }
        #endif
        #endif
        }

        #endif
    }


    void nrn_destructor_SonataReport(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<SonataReport_Instance*>(ml->instance);

        #endif
    }


    inline double AddNode_SonataReport(int id, int pnodecount, SonataReport_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline double AddVar_SonataReport(int id, int pnodecount, SonataReport_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
}


using namespace coreneuron;


#ifndef CORENEURON_BUILD
#ifndef DISABLE_REPORTINGLIB
#include <stdint.h>
#include <bbp/sonata/reports.h>
#ifndef NRN_VERSION_GTEQ_8_2_0
extern double* hoc_pgetarg(int iarg);
extern double* getarg(int iarg);
extern char* gargstr(int iarg);
extern int hoc_is_str_arg(int iarg);
extern int ifarg(int iarg);
extern double chkarg(int iarg, double low, double high);
#endif
typedef struct {
    char neuronName_[256];
    char rptName_[512];
} Data;
#endif
#endif


/** not executed in coreneuron and hence need empty stubs */
static void bbcore_write(double* x, int* d, int* xx, int* offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) { }
static void bbcore_read(double* x, int* d, int* xx, int* offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) { }


namespace coreneuron {


    inline double AddNode_SonataReport(int id, int pnodecount, SonataReport_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_AddNode = 0.0;
         {
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            Data** tempdata = (Data**)(&(inst->ptr[indexes[2*pnodecount + id]]));
            Data* data = *tempdata;
            if(ifarg(1))
            {
                char population_name[256] = "All";
                unsigned long population_offset = 0;
                if (ifarg(2)) {
                    sprintf(population_name,"%s", gargstr(2));
                }
                if (ifarg(3)) {
                    population_offset = (unsigned long) *getarg(3);
                }
                unsigned long node_id = (unsigned long) *getarg(1);
                sonata_add_node(data->rptName_, population_name, population_offset, node_id);
            }
        #endif
        #endif
        }

        return ret_AddNode;
    }


    inline double AddVar_SonataReport(int id, int pnodecount, SonataReport_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double ret_AddVar = 0.0;
         {
        #ifndef CORENEURON_BUILD
        #ifndef DISABLE_REPORTINGLIB
            Data** tempdata = (Data**)(&(inst->ptr[indexes[2*pnodecount + id]]));
            Data* data = *tempdata;
            if(ifarg(1))
            {
                int element_id = (int)*getarg(2);
                int node_id = (int) *getarg(3);
                char population_name[256] = "All";
                if (ifarg(4)) {
                    sprintf(population_name,"%s", gargstr(4));
                }
        #ifdef NRN_MECHANISM_DATA_IS_SOA
                sonata_add_element_handle(data->rptName_, population_name, node_id, element_id, [x=hoc_hgetarg<double>(1)]() { return *x; });
        #else
                sonata_add_element(data->rptName_, population_name, node_id, element_id, hoc_pgetarg(1));
        #endif
            }
        #endif
        #endif
        }

        return ret_AddVar;
    }


    /** initialize channel */
    void nrn_init_SonataReport(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<SonataReport_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
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
    }


    /** register channel with the simulator */
    void _SonataReports_reg() {

        int mech_type = nrn_get_mechtype("SonataReport");
        SonataReport_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        point_register_mech(mechanism_info, nrn_alloc_SonataReport, nullptr, nullptr, nullptr, nrn_init_SonataReport, nrn_private_constructor_SonataReport, nrn_private_destructor_SonataReport, first_pointer_var_index(), nrn_constructor_SonataReport, nullptr, 1);

        hoc_reg_bbcore_read(mech_type, bbcore_read);
        hoc_reg_bbcore_write(mech_type, bbcore_write);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "area");
        hoc_register_dparam_semantics(mech_type, 1, "pntproc");
        hoc_register_dparam_semantics(mech_type, 2, "bbcorepointer");
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
