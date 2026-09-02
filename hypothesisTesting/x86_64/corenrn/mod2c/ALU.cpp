/*********************************************************
Model Name      : ALU
Filename        : ALU.mod
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
        "ALU",
        "Dt",
        "output",
        0,
        0,
        0,
        "ptr",
        0
    };


    /** all global variables */
    struct ALU_Store {
        int point_type{};
        int reset{};
        int mech_type{};
    };
    static_assert(std::is_trivially_copy_constructible_v<ALU_Store>);
    static_assert(std::is_trivially_move_constructible_v<ALU_Store>);
    static_assert(std::is_trivially_copy_assignable_v<ALU_Store>);
    static_assert(std::is_trivially_move_assignable_v<ALU_Store>);
    static_assert(std::is_trivially_destructible_v<ALU_Store>);
    ALU_Store ALU_global;


    /** all mechanism instance variables and global variables */
    struct ALU_Instance  {
        double* Dt{};
        double* output{};
        double* v_unused{};
        double* tsave{};
        const double* node_area{};
        const int* point_process{};
        void** ptr{};
        const int* tqitem{};
        ALU_Store* global{&ALU_global};
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


    static inline int num_net_receive_args() {
        return 1;
    }


    static inline int float_variables_size() {
        return 4;
    }


    static inline int int_variables_size() {
        return 4;
    }


    static inline int get_mech_type() {
        return ALU_global.mech_type;
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
    static void nrn_private_constructor_ALU(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new ALU_Instance{};
        assert(inst->global == &ALU_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(ALU_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_ALU(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<ALU_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &ALU_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(ALU_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<ALU_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &ALU_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(ALU_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->Dt = ml->data+0*pnodecount;
        inst->output = ml->data+1*pnodecount;
        inst->v_unused = ml->data+2*pnodecount;
        inst->tsave = ml->data+3*pnodecount;
        inst->node_area = nt->_data;
        inst->point_process = ml->pdata;
        inst->ptr = nt->_vdata;
        inst->tqitem = ml->pdata;
    }



    static void nrn_alloc_ALU(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_ALU(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<ALU_Instance*>(ml->instance);

         {
        #ifndef CORENEURON_BUILD
        #ifndef NRN_MECHANISM_DATA_IS_SOA
            static int first = 1;
            if (first) {
                first = 0;
                nrn_register_recalc_ptr_callback(recalc_ptr_callback);
            }
        #endif
            INFOCAST;
            Info* info = (Info*)hoc_Emalloc(sizeof(Info)); hoc_malchk();
            info->psize_ = 10;
        #ifdef NRN_VERSION_GTEQ_9_0_0
            info->ptrs_ = new handle_to_double[info->psize_]; hoc_malchk();
        #else
            info->ptrs_ = (handle_to_double*)hoc_Ecalloc(info->psize_, sizeof(handle_to_double)); hoc_malchk();
        #endif
            info->scalars_ = (double*)hoc_Ecalloc(info->psize_, sizeof(double)); hoc_malchk();
            info->np_ = 0;
            *ip = info;
            if (ifarg(2)) {
                inst->Dt[id] = *getarg(2);
            }
            info->process = &average_ALU;
        #endif
        }

        #endif
    }


    void nrn_destructor_ALU(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<ALU_Instance*>(ml->instance);

         {
        #ifndef CORENEURON_BUILD
            INFOCAST;
            Info* info = *ip;
        #ifdef NRN_VERSION_GTEQ_9_0_0
            delete[] info->ptrs_;
        #else
            free(info->ptrs_);
        #endif
            free(info);
        #endif
        }

        #endif
    }


    inline int restartEvent_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int addvar_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int constant_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int average_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int summation_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int setop_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
}


using namespace coreneuron;


#ifndef CORENEURON_BUILD
#if defined(NRN_VERSION_GTEQ)
#if NRN_VERSION_GTEQ(9,0,0)
#define NRN_VERSION_GTEQ_9_0_0
#endif
#endif
#ifndef NRN_VERSION_GTEQ_8_2_0
extern double* hoc_pgetarg(int iarg);
extern double* getarg(int iarg);
extern int ifarg(int iarg);
extern void nrn_register_recalc_ptr_callback(void (*f)());
extern Point_process* ob2pntproc(Object*);
extern double* nrn_recalc_ptr(double*);
#endif
#ifdef NRN_MECHANISM_DATA_IS_SOA
using handle_to_double = decltype(hoc_hgetarg<double>(42));
#else
typedef double* handle_to_double;
#endif
typedef struct {
    handle_to_double* ptrs_;
    /*! list of scalars to apply to corresponding variables; useful for making units of variables
     * from different sources consistent (e.g. i current sources may be distributed, mA/cm^2, or point processes, nA)
     */
    double * scalars_;
    int np_;
    int psize_;
#ifdef NRN_MECHANISM_DATA_IS_SOA
    int (*process)(_internalthreadargsproto_);
#else
    int (*process)(int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v);
#endif
} Info;
#define INFOCAST Info** ip = (Info**)(&(nt->_vdata[indexes[2*pnodecount + id]]))
#define dp double*
#ifndef NRN_MECHANISM_DATA_IS_SOA
static void recalcptr(Info* info, int cnt, double** old_vp, double* new_v) {
    int i;
    /*printf("recalcptr np_=%d %s\n", info->np_, info->path_);*/
}
static void recalc_ptr_callback() {
    Symbol* sym;
    int i;
    hoc_List* instances;
    hoc_Item* q;
    /*printf("ASCIIrecord.mod recalc_ptr_callback\n");*/
    /* hoc has a list of the ASCIIRecord instances */
    sym = hoc_lookup("ALU");
    #ifndef NRN_VERSION_GTEQ_8_2_0
    instances = sym->u.template->olist;
    #else
    instances = sym->u.ctemplate->olist;
    #endif
    ITERATE(q, instances) {
        Info* InfoPtr;
        Point_process* pnt;
        Object* o = OBJ(q);
        /*printf("callback for %s\n", hoc_object_name(o));*/
        pnt = ob2pntproc(o);
        Datum* indexes = pnt->_prop->dparam;
        INFOCAST;
        InfoPtr = *ip;
        for (i=0; i < InfoPtr->np_; ++i)
            InfoPtr->ptrs_[i] =  nrn_recalc_ptr(InfoPtr->ptrs_[i]);
    }
}
#endif
#endif


/** not executed in coreneuron and hence need empty stubs only */
static void bbcore_write(double* x, int* d, int* xx, int* offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
}
static void bbcore_read(double* x, int* d, int* xx, int* offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
}


namespace coreneuron {


    inline int restartEvent_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_restartEvent = 0;
        #ifndef CORENEURON_BUILD
            const double etime = *getarg(1);
            #if defined(NRN_VERSION_GTEQ_9_0_0)
            net_send(&indexes[3*pnodecount+id], (double*)0, indexes[1].get<Point_process*>(), etime, 1.0);
            #else
            net_send(&indexes[3*pnodecount+id], (double*)0, (Point_process*)indexes[1]._pvoid, etime, 1.0);
            #endif    
        #endif

        return ret_restartEvent;
    }


    inline int addvar_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_addvar = 0;
         {
        #ifndef CORENEURON_BUILD
            INFOCAST;
            Info* info = *ip;
            if (info->np_ >= info->psize_) {
                info->psize_ += 10;
        #ifdef NRN_VERSION_GTEQ_9_0_0
                auto old_ptrs = info->ptrs_;
                info->ptrs_ = new handle_to_double[info->psize_]; hoc_malchk();
                std::copy(old_ptrs, old_ptrs+info->np_, info->ptrs_);
                delete[] old_ptrs;
        #else
                info->ptrs_ = (handle_to_double*)hoc_Erealloc(info->ptrs_, info->psize_*sizeof(handle_to_double)); hoc_malchk();
        #endif
                info->scalars_ = (double*) hoc_Erealloc(info->scalars_, info->psize_*sizeof(double)); hoc_malchk();
            }
        #ifdef NRN_MECHANISM_DATA_IS_SOA
            handle_to_double var = hoc_hgetarg<double>(1);
        #else
            handle_to_double var = hoc_pgetarg(1);
        #endif
            info->ptrs_[info->np_] = var;
            if( ifarg(2)) {
                info->scalars_[info->np_] = *getarg(2);
            } else {
                info->scalars_[info->np_] = 1;
            }
            ++info->np_;
        #endif
        }

        return ret_addvar;
    }


    inline int constant_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_constant = 0;
         {
        #ifndef CORENEURON_BUILD
            INFOCAST;
            Info* info = *ip;
            if( info->np_ > 0 ) {
                inst->output[id] = info->scalars_[0];
            } else {
                inst->output[id] = 0;
            }
        #endif
        }

        return ret_constant;
    }


    inline int average_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_average = 0;
         {
        #ifndef CORENEURON_BUILD
            INFOCAST;
            Info* info = *ip;
            int i;
            double n = 0;
            for (i=0; i < info->np_; ++i) {
                n += (*info->ptrs_[i] * info->scalars_[i]);
            }
            if (info->np_ > 0)
              inst->output[id] = n/info->np_;
            else inst->output[id] = 0;
        #endif
        }

        return ret_average;
    }


    inline int summation_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_summation = 0;
         {
        #ifndef CORENEURON_BUILD
            INFOCAST; Info* info = *ip;
            int i;
            double n = 0;
            for (i=0; i < info->np_; ++i) {
                n += (*info->ptrs_[i] * info->scalars_[i]);
            }
            inst->output[id] = n;
        #endif
        }

        return ret_summation;
    }


    inline int setop_ALU(int id, int pnodecount, ALU_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_setop = 0;
         {
        #ifndef CORENEURON_BUILD
            INFOCAST; Info* info = *ip;
            char *opname = NULL;
            if (!hoc_is_str_arg(1)) {
                exit(0);
            }
            opname = gargstr(1);
            if( strcmp( opname, "summation" ) == 0 ) {
                info->process = &summation_ALU;
            } else if ( strcmp( opname, "average" ) == 0 ) {
                info->process = &average_ALU;
            } else if ( strcmp( opname, "constant" ) == 0 ) {
                info->process = &constant_ALU;
            } else {
                fprintf( stderr, "Error: unknown operation '%s' for ALU object.  Terminating.\n", opname );
                exit(0);
            }
        #endif
        }

        return ret_setop;
    }


    static inline void net_send_buffering(const NrnThread* nt, NetSendBuffer_t* nsb, int type, int vdata_index, int weight_index, int point_index, double t, double flag) {
        int i = 0;
        i = nsb->_cnt++;
        if (i >= nsb->_size) {
            nsb->grow();
        }
        if (i < nsb->_size) {
            nsb->_sendtype[i] = type;
            nsb->_vdata_index[i] = vdata_index;
            nsb->_weight_index[i] = weight_index;
            nsb->_pnt_index[i] = point_index;
            nsb->_nsb_t[i] = t;
            nsb->_nsb_flag[i] = flag;
        }
    }


    static inline void net_receive_kernel_ALU(double t, Point_process* pnt, ALU_Instance* inst, NrnThread* nt, Memb_list* ml, int weight_index, double flag) {
        int tid = pnt->_tid;
        int id = pnt->_i_instance;
        double v = 0;
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        double* data = ml->data;
        double* weights = nt->weights;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        inst->tsave[id] = t;
        {
             {
            #ifndef CORENEURON_BUILD
                INFOCAST;
                Info* info = *ip;
                info->process(id, pnodecount, data, indexes, thread, nt, ml, v);
            #endif
            }

            net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[3*pnodecount+id], weight_index, inst->point_process[1*pnodecount+id], t+inst->Dt[id] - 1e-5, 1.0);
        }
    }


    static void net_receive_ALU(Point_process* pnt, int weight_index, double flag) {
        NrnThread* nt = nrn_threads + pnt->_tid;
        Memb_list* ml = get_memb_list(nt);
        NetReceiveBuffer_t* nrb = ml->_net_receive_buffer;
        if (nrb->_cnt >= nrb->_size) {
            realloc_net_receive_buffer(nt, ml);
        }
        int id = nrb->_cnt;
        nrb->_pnt_index[id] = pnt-nt->pntprocs;
        nrb->_weight_index[id] = weight_index;
        nrb->_nrb_t[id] = nt->_t;
        nrb->_nrb_flag[id] = flag;
        nrb->_cnt++;
    }


    void net_buf_receive_ALU(NrnThread* nt) {
        Memb_list* ml = get_memb_list(nt);
        if (!ml) {
            return;
        }

        NetReceiveBuffer_t* nrb = ml->_net_receive_buffer;
        auto* const inst = static_cast<ALU_Instance*>(ml->instance);
        int count = nrb->_displ_cnt;
        #pragma omp simd
        #pragma ivdep
        for (int i = 0; i < count; i++) {
            int start = nrb->_displ[i];
            int end = nrb->_displ[i+1];
            for (int j = start; j < end; j++) {
                int index = nrb->_nrb_index[j];
                int offset = nrb->_pnt_index[index];
                double t = nrb->_nrb_t[index];
                int weight_index = nrb->_weight_index[index];
                double flag = nrb->_nrb_flag[index];
                Point_process* point_process = nt->pntprocs + offset;
                net_receive_kernel_ALU(t, point_process, inst, nt, ml, weight_index, flag);
            }
        }
        nrb->_displ_cnt = 0;
        nrb->_cnt = 0;

        NetSendBuffer_t* nsb = ml->_net_send_buffer;
        for (int i=0; i < nsb->_cnt; i++) {
            int type = nsb->_sendtype[i];
            int tid = nt->id;
            double t = nsb->_nsb_t[i];
            double flag = nsb->_nsb_flag[i];
            int vdata_index = nsb->_vdata_index[i];
            int weight_index = nsb->_weight_index[i];
            int point_index = nsb->_pnt_index[i];
            net_sem_from_gpu(type, vdata_index, weight_index, tid, point_index, t, flag);
        }
        nsb->_cnt = 0;
    }


    /** initialize channel */
    void nrn_init_ALU(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<ALU_Instance*>(ml->instance);

        if (_nrn_skip_initmodel == 0) {
            #pragma omp simd
            #pragma ivdep
            for (int id = 0; id < nodecount; id++) {
                inst->tsave[id] = -1e20;
                int node_id = node_index[id];
                double v = voltage[node_id];
                #if NRN_PRCELLSTATE
                inst->v_unused[id] = v;
                #endif
                net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[3*pnodecount+id], 0, inst->point_process[1*pnodecount+id], nt->_t+0.0, 1.0);
            }
        }

        NetSendBuffer_t* nsb = ml->_net_send_buffer;
        for (int i=0; i < nsb->_cnt; i++) {
            int type = nsb->_sendtype[i];
            int tid = nt->id;
            double t = nsb->_nsb_t[i];
            double flag = nsb->_nsb_flag[i];
            int vdata_index = nsb->_vdata_index[i];
            int weight_index = nsb->_weight_index[i];
            int point_index = nsb->_pnt_index[i];
            net_sem_from_gpu(type, vdata_index, weight_index, tid, point_index, t, flag);
        }
        nsb->_cnt = 0;
    }


    /** register channel with the simulator */
    void _ALU_reg() {

        int mech_type = nrn_get_mechtype("ALU");
        ALU_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        point_register_mech(mechanism_info, nrn_alloc_ALU, nullptr, nullptr, nullptr, nrn_init_ALU, nrn_private_constructor_ALU, nrn_private_destructor_ALU, first_pointer_var_index(), nrn_constructor_ALU, nrn_destructor_ALU, 1);

        hoc_reg_bbcore_read(mech_type, bbcore_read);
        hoc_reg_bbcore_write(mech_type, bbcore_write);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "area");
        hoc_register_dparam_semantics(mech_type, 1, "pntproc");
        hoc_register_dparam_semantics(mech_type, 2, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 3, "netsend");
        hoc_register_net_receive_buffering(net_buf_receive_ALU, mech_type);
        set_pnt_receive(mech_type, net_receive_ALU, nullptr, num_net_receive_args());
        hoc_register_net_send_buffering(mech_type);
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
