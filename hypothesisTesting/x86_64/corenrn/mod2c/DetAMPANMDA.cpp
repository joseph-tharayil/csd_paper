/*********************************************************
Model Name      : DetAMPANMDA
Filename        : DetAMPANMDA.mod
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
        "DetAMPANMDA",
        "tau_r_AMPA",
        "tau_d_AMPA",
        "tau_r_NMDA",
        "tau_d_NMDA",
        "Use",
        "Dep",
        "Fac",
        "e",
        "mg",
        "u0",
        "NMDA_ratio",
        "synapseID",
        "verboseLevel",
        "conductance",
        0,
        "i",
        "i_AMPA",
        "i_NMDA",
        "g_AMPA",
        "g_NMDA",
        "g",
        "next_delay",
        0,
        "A_AMPA",
        "B_AMPA",
        "A_NMDA",
        "B_NMDA",
        0,
        "delay_times",
        "delay_weights",
        0
    };


    /** all global variables */
    struct DetAMPANMDA_Store {
        int point_type{};
        double A_AMPA0{};
        double B_AMPA0{};
        double A_NMDA0{};
        double B_NMDA0{};
        int reset{};
        int mech_type{};
        double nc_type_param{7};
        double gmax{0.001};
        int slist1[4]{21, 22, 23, 24};
        int dlist1[4]{28, 29, 30, 31};
    };
    static_assert(std::is_trivially_copy_constructible_v<DetAMPANMDA_Store>);
    static_assert(std::is_trivially_move_constructible_v<DetAMPANMDA_Store>);
    static_assert(std::is_trivially_copy_assignable_v<DetAMPANMDA_Store>);
    static_assert(std::is_trivially_move_assignable_v<DetAMPANMDA_Store>);
    static_assert(std::is_trivially_destructible_v<DetAMPANMDA_Store>);
    DetAMPANMDA_Store DetAMPANMDA_global;


    /** all mechanism instance variables and global variables */
    struct DetAMPANMDA_Instance  {
        const double* tau_r_AMPA{};
        const double* tau_d_AMPA{};
        const double* tau_r_NMDA{};
        const double* tau_d_NMDA{};
        const double* Use{};
        const double* Dep{};
        const double* Fac{};
        const double* e{};
        const double* mg{};
        const double* u0{};
        const double* NMDA_ratio{};
        const double* synapseID{};
        double* verboseLevel{};
        const double* conductance{};
        double* i{};
        double* i_AMPA{};
        double* i_NMDA{};
        double* g_AMPA{};
        double* g_NMDA{};
        double* g{};
        double* next_delay{};
        double* A_AMPA{};
        double* B_AMPA{};
        double* A_NMDA{};
        double* B_NMDA{};
        double* factor_AMPA{};
        double* factor_NMDA{};
        double* mggate{};
        double* DA_AMPA{};
        double* DB_AMPA{};
        double* DA_NMDA{};
        double* DB_NMDA{};
        double* v_unused{};
        double* g_unused{};
        double* tsave{};
        const double* node_area{};
        const int* point_process{};
        void** delay_times{};
        void** delay_weights{};
        const int* tqitem{};
        DetAMPANMDA_Store* global{&DetAMPANMDA_global};
    };


    /** connect global (scalar) variables to hoc -- */
    static DoubScal hoc_scalar_double[] = {
        {"nc_type_param_DetAMPANMDA", &DetAMPANMDA_global.nc_type_param},
        {"gmax_DetAMPANMDA", &DetAMPANMDA_global.gmax},
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
        return 8;
    }


    static inline int float_variables_size() {
        return 35;
    }


    static inline int int_variables_size() {
        return 5;
    }


    static inline int get_mech_type() {
        return DetAMPANMDA_global.mech_type;
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
    static void nrn_private_constructor_DetAMPANMDA(NrnThread* nt, Memb_list* ml, int type) {
        assert(!ml->instance);
        assert(!ml->global_variables);
        assert(ml->global_variables_size == 0);
        auto* const inst = new DetAMPANMDA_Instance{};
        assert(inst->global == &DetAMPANMDA_global);
        ml->instance = inst;
        ml->global_variables = inst->global;
        ml->global_variables_size = sizeof(DetAMPANMDA_Store);
    }

    // Deallocate the instance structure
    static void nrn_private_destructor_DetAMPANMDA(NrnThread* nt, Memb_list* ml, int type) {
        auto* const inst = static_cast<DetAMPANMDA_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &DetAMPANMDA_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(DetAMPANMDA_Store));
        delete inst;
        ml->instance = nullptr;
        ml->global_variables = nullptr;
        ml->global_variables_size = 0;
    }

    /** initialize mechanism instance variables */
    static inline void setup_instance(NrnThread* nt, Memb_list* ml) {
        auto* const inst = static_cast<DetAMPANMDA_Instance*>(ml->instance);
        assert(inst);
        assert(inst->global);
        assert(inst->global == &DetAMPANMDA_global);
        assert(inst->global == ml->global_variables);
        assert(ml->global_variables_size == sizeof(DetAMPANMDA_Store));
        int pnodecount = ml->_nodecount_padded;
        Datum* indexes = ml->pdata;
        inst->tau_r_AMPA = ml->data+0*pnodecount;
        inst->tau_d_AMPA = ml->data+1*pnodecount;
        inst->tau_r_NMDA = ml->data+2*pnodecount;
        inst->tau_d_NMDA = ml->data+3*pnodecount;
        inst->Use = ml->data+4*pnodecount;
        inst->Dep = ml->data+5*pnodecount;
        inst->Fac = ml->data+6*pnodecount;
        inst->e = ml->data+7*pnodecount;
        inst->mg = ml->data+8*pnodecount;
        inst->u0 = ml->data+9*pnodecount;
        inst->NMDA_ratio = ml->data+10*pnodecount;
        inst->synapseID = ml->data+11*pnodecount;
        inst->verboseLevel = ml->data+12*pnodecount;
        inst->conductance = ml->data+13*pnodecount;
        inst->i = ml->data+14*pnodecount;
        inst->i_AMPA = ml->data+15*pnodecount;
        inst->i_NMDA = ml->data+16*pnodecount;
        inst->g_AMPA = ml->data+17*pnodecount;
        inst->g_NMDA = ml->data+18*pnodecount;
        inst->g = ml->data+19*pnodecount;
        inst->next_delay = ml->data+20*pnodecount;
        inst->A_AMPA = ml->data+21*pnodecount;
        inst->B_AMPA = ml->data+22*pnodecount;
        inst->A_NMDA = ml->data+23*pnodecount;
        inst->B_NMDA = ml->data+24*pnodecount;
        inst->factor_AMPA = ml->data+25*pnodecount;
        inst->factor_NMDA = ml->data+26*pnodecount;
        inst->mggate = ml->data+27*pnodecount;
        inst->DA_AMPA = ml->data+28*pnodecount;
        inst->DB_AMPA = ml->data+29*pnodecount;
        inst->DA_NMDA = ml->data+30*pnodecount;
        inst->DB_NMDA = ml->data+31*pnodecount;
        inst->v_unused = ml->data+32*pnodecount;
        inst->g_unused = ml->data+33*pnodecount;
        inst->tsave = ml->data+34*pnodecount;
        inst->node_area = nt->_data;
        inst->point_process = ml->pdata;
        inst->delay_times = nt->_vdata;
        inst->delay_weights = nt->_vdata;
        inst->tqitem = ml->pdata;
    }



    static void nrn_alloc_DetAMPANMDA(double* data, Datum* indexes, int type) {
        // do nothing
    }


    void nrn_constructor_DetAMPANMDA(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<DetAMPANMDA_Instance*>(ml->instance);

        #endif
    }


    void nrn_destructor_DetAMPANMDA(NrnThread* nt, Memb_list* ml, int type) {
        #ifndef CORENEURON_BUILD
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<DetAMPANMDA_Instance*>(ml->instance);

        #endif
    }


    inline int setup_delay_vecs_DetAMPANMDA(int id, int pnodecount, DetAMPANMDA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
    inline int toggleVerbose_DetAMPANMDA(int id, int pnodecount, DetAMPANMDA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v);
}


using namespace coreneuron;


#ifndef CORENEURON_BUILD
extern int ifarg(int iarg);
#ifndef NRN_VERSION_GTEQ_8_2_0
extern void* vector_arg(int iarg);
extern double* vector_vec(void* vv);
extern int vector_capacity(void* vv);
#endif
#endif


static void bbcore_write(double* x, int* d, int* x_offset, int* d_offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
  IvocVect *vv_delay_times = *((IvocVect**)(&nt->_vdata[indexes[2*pnodecount + id]]));
  IvocVect *vv_delay_weights = *((IvocVect**)(&nt->_vdata[indexes[3*pnodecount + id]]));
  if (vv_delay_times && vv_delay_weights &&
     (vector_capacity(vv_delay_times) >= 1) && (vector_capacity(vv_delay_weights) >= 1)) {
    if (d && x) {
      int* d_i = d + *d_offset;
      d_i[0] = vector_capacity(vv_delay_times);
      d_i[1] = vector_capacity(vv_delay_weights);
      double* delay_times_el = vector_vec(vv_delay_times);
      double* delay_weights_el = vector_vec(vv_delay_weights);
      double* x_i = x + *x_offset;
      int delay_vecs_idx;
      int x_idx = 0;
      for(delay_vecs_idx = 0; delay_vecs_idx < vector_capacity(vv_delay_times); ++delay_vecs_idx) {
         x_i[x_idx++] = delay_times_el[delay_vecs_idx];
         x_i[x_idx++] = delay_weights_el[delay_vecs_idx];
      }
    }
  } else {
    if (d) {
      int* d_i = d + *d_offset;
      d_i[0] = 0;
      d_i[1] = 0;
    }
  }
  *d_offset += 2;
  if (vv_delay_times && vv_delay_weights) {
    *x_offset += vector_capacity(vv_delay_times) + vector_capacity(vv_delay_weights);
  }
}
static void bbcore_read(double* x, int* d, int* x_offset, int* d_offset, int id, int pnodecount, double* data, Datum* indexes, ThreadDatum* thread, NrnThread* nt, Memb_list* ml, double v) {
  assert(!nt->_vdata[indexes[2*pnodecount + id]] && !nt->_vdata[indexes[3*pnodecount + id]]);
  int* d_i = d + *d_offset;
  int delay_times_sz = d_i[0];
  int delay_weights_sz = d_i[1];
  *d_offset += 2;
  if ((delay_times_sz > 0) && (delay_weights_sz > 0)) {
    double* x_i = x + *x_offset;
    nt->_vdata[indexes[2*pnodecount + id]] = (double*)vector_new1(delay_times_sz);
    nt->_vdata[indexes[3*pnodecount + id]] = (double*)vector_new1(delay_weights_sz);
    double* delay_times_el = vector_vec((IvocVect*)nt->_vdata[indexes[2*pnodecount + id]]);
    double* delay_weights_el = vector_vec((IvocVect*)nt->_vdata[indexes[3*pnodecount + id]]);
    int x_idx;
    int vec_idx = 0;
    for(x_idx = 0; x_idx < delay_times_sz + delay_weights_sz; x_idx += 2) {
      delay_times_el[vec_idx] = x_i[x_idx];
      delay_weights_el[vec_idx++] = x_i[x_idx+1];
    }
    *x_offset += delay_times_sz + delay_weights_sz;
  }
}


namespace coreneuron {


    inline int setup_delay_vecs_DetAMPANMDA(int id, int pnodecount, DetAMPANMDA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_setup_delay_vecs = 0;
        #ifndef CORENEURON_BUILD
            void** vv_delay_times = (void**)(&inst->delay_times[indexes[2*pnodecount + id]]);
            void** vv_delay_weights = (void**)(&inst->delay_weights[indexes[3*pnodecount + id]]);
            *vv_delay_times = (void*)NULL;
            *vv_delay_weights = (void*)NULL;
            if (ifarg(1)) {
                *vv_delay_times = vector_arg(1);
            }
            if (ifarg(2)) {
                *vv_delay_weights = vector_arg(2);
            }
        #endif

        return ret_setup_delay_vecs;
    }


    inline int toggleVerbose_DetAMPANMDA(int id, int pnodecount, DetAMPANMDA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        int ret_toggleVerbose = 0;
        inst->verboseLevel[id] = 1.0 - inst->verboseLevel[id];
        return ret_toggleVerbose;
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


    /** initialize block for net receive */
    static void net_init(Point_process* pnt, int weight_index, double flag) {
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
        auto* const inst = static_cast<DetAMPANMDA_Instance*>(ml->instance);

        double* R = weights + weight_index + 3;
        double* u = weights + weight_index + 5;
        double* tsyn = weights + weight_index + 6;
        double* nc_type = weights + weight_index + 7;
        (*R) = 1.0;
        (*u) = inst->u0[id];
        (*tsyn) = nt->_t;
        if ((*nc_type) == 0.0) {
                        IvocVect *vv_delay_times = *((IvocVect**)(&inst->delay_times[indexes[2*pnodecount + id]]));
                        IvocVect *vv_delay_weights = *((IvocVect**)(&inst->delay_weights[indexes[3*pnodecount + id]]));
                        if (vv_delay_times && vector_capacity(vv_delay_times)>=1) {
                          double* deltm_el = vector_vec(vv_delay_times);
                          int delay_times_idx;
                          inst->next_delay[id] = 0;
                          for(delay_times_idx = 0; delay_times_idx < vector_capacity(vv_delay_times); ++delay_times_idx) {
                            double next_delay_t = deltm_el[delay_times_idx];

            net_send_buffering(nt, ml->_net_send_buffer, 0, inst->tqitem[4*pnodecount+id], weight_index, inst->point_process[1*pnodecount+id], nt->_t+next_delay_t, 1.0);
                          }
                        }

        }
        auto& nsb = ml->_net_send_buffer;
    }


    static inline void net_receive_kernel_DetAMPANMDA(double t, Point_process* pnt, DetAMPANMDA_Instance* inst, NrnThread* nt, Memb_list* ml, int weight_index, double flag) {
        int tid = pnt->_tid;
        int id = pnt->_i_instance;
        double v = 0;
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        double* data = ml->data;
        double* weights = nt->weights;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        double* weight = weights + weight_index + 0;
        double* weight_AMPA = weights + weight_index + 1;
        double* weight_NMDA = weights + weight_index + 2;
        double* R = weights + weight_index + 3;
        double* Pr = weights + weight_index + 4;
        double* u = weights + weight_index + 5;
        double* tsyn = weights + weight_index + 6;
        double* nc_type = weights + weight_index + 7;
        inst->tsave[id] = t;
        {
            double result;
            (*weight_AMPA) = (*weight);
            (*weight_NMDA) = (*weight) * inst->NMDA_ratio[id];
            if (t < 0.0) {
                        return;

            }
            if (flag == 1.0) {
                        IvocVect *vv_delay_weights = *((IvocVect**)(&inst->delay_weights[indexes[3*pnodecount + id]]));
                        if (vv_delay_weights && vector_capacity(vv_delay_weights)>=inst->next_delay[id]) {
                          double* weights_v = vector_vec(vv_delay_weights);
                          double next_delay_weight = weights_v[(int)inst->next_delay[id]];

                (*weight) = inst->conductance[id] * next_delay_weight;
                inst->next_delay[id] = inst->next_delay[id] + 1.0;
                        }
                        return;

            }
            if (inst->Fac[id] > 0.0) {
                (*u) = (*u) * exp( -(t - (*tsyn)) / inst->Fac[id]);
            } else {
                (*u) = inst->Use[id];
            }
            if (inst->Fac[id] > 0.0) {
                (*u) = (*u) + inst->Use[id] * (1.0 - (*u));
            }
            (*R) = 1.0 - (1.0 - (*R)) * exp( -(t - (*tsyn)) / inst->Dep[id]);
            (*Pr) = (*u) * (*R);
            (*R) = (*R) - (*u) * (*R);
            if (inst->verboseLevel[id] > 0.0) {
                printf("Synapse %f at time %g: R = %g Pr = %g erand = %g\n", inst->synapseID[id], t, (*R), (*Pr), result);
            }
            (*tsyn) = t;
            inst->A_AMPA[id] = inst->A_AMPA[id] + (*Pr) * (*weight_AMPA) * inst->factor_AMPA[id];
            inst->B_AMPA[id] = inst->B_AMPA[id] + (*Pr) * (*weight_AMPA) * inst->factor_AMPA[id];
            inst->A_NMDA[id] = inst->A_NMDA[id] + (*Pr) * (*weight_NMDA) * inst->factor_NMDA[id];
            inst->B_NMDA[id] = inst->B_NMDA[id] + (*Pr) * (*weight_NMDA) * inst->factor_NMDA[id];
            if (inst->verboseLevel[id] > 0.0) {
                printf(" vals %g %g %g %g\n", inst->A_AMPA[id], (*weight_AMPA), inst->factor_AMPA[id], (*weight));
            }
        }
    }


    static void net_receive_DetAMPANMDA(Point_process* pnt, int weight_index, double flag) {
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


    void net_buf_receive_DetAMPANMDA(NrnThread* nt) {
        Memb_list* ml = get_memb_list(nt);
        if (!ml) {
            return;
        }

        NetReceiveBuffer_t* nrb = ml->_net_receive_buffer;
        auto* const inst = static_cast<DetAMPANMDA_Instance*>(ml->instance);
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
                net_receive_kernel_DetAMPANMDA(t, point_process, inst, nt, ml, weight_index, flag);
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
    void nrn_init_DetAMPANMDA(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;

        setup_instance(nt, ml);
        auto* const inst = static_cast<DetAMPANMDA_Instance*>(ml->instance);

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
                inst->A_AMPA[id] = inst->global->A_AMPA0;
                inst->B_AMPA[id] = inst->global->B_AMPA0;
                inst->A_NMDA[id] = inst->global->A_NMDA0;
                inst->B_NMDA[id] = inst->global->B_NMDA0;
                double tp_AMPA, tp_NMDA;
                inst->A_AMPA[id] = 0.0;
                inst->B_AMPA[id] = 0.0;
                inst->A_NMDA[id] = 0.0;
                inst->B_NMDA[id] = 0.0;
                tp_AMPA = (inst->tau_r_AMPA[id] * inst->tau_d_AMPA[id]) / (inst->tau_d_AMPA[id] - inst->tau_r_AMPA[id]) * log(inst->tau_d_AMPA[id] / inst->tau_r_AMPA[id]);
                tp_NMDA = (inst->tau_r_NMDA[id] * inst->tau_d_NMDA[id]) / (inst->tau_d_NMDA[id] - inst->tau_r_NMDA[id]) * log(inst->tau_d_NMDA[id] / inst->tau_r_NMDA[id]);
                inst->factor_AMPA[id] =  -exp( -tp_AMPA / inst->tau_r_AMPA[id]) + exp( -tp_AMPA / inst->tau_d_AMPA[id]);
                inst->factor_AMPA[id] = 1.0 / inst->factor_AMPA[id];
                inst->factor_NMDA[id] =  -exp( -tp_NMDA / inst->tau_r_NMDA[id]) + exp( -tp_NMDA / inst->tau_d_NMDA[id]);
                inst->factor_NMDA[id] = 1.0 / inst->factor_NMDA[id];
                inst->next_delay[id] =  -1.0;
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


    inline double nrn_current_DetAMPANMDA(int id, int pnodecount, DetAMPANMDA_Instance* inst, double* data, const Datum* indexes, ThreadDatum* thread, NrnThread* nt, double v) {
        double current = 0.0;
        inst->mggate[id] = 1.0 / (1.0 + exp(0.062 *  -(v)) * (inst->mg[id] / 3.57));
        inst->g_AMPA[id] = inst->global->gmax * (inst->B_AMPA[id] - inst->A_AMPA[id]);
        inst->g_NMDA[id] = inst->global->gmax * (inst->B_NMDA[id] - inst->A_NMDA[id]) * inst->mggate[id];
        inst->g[id] = inst->g_AMPA[id] + inst->g_NMDA[id];
        inst->i_AMPA[id] = inst->g_AMPA[id] * (v - inst->e[id]);
        inst->i_NMDA[id] = inst->g_NMDA[id] * (v - inst->e[id]);
        inst->i[id] = inst->i_AMPA[id] + inst->i_NMDA[id];
        current += inst->i[id];
        return current;
    }


    /** update current */
    void nrn_cur_DetAMPANMDA(NrnThread* nt, Memb_list* ml, int type) {
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
        auto* const inst = static_cast<DetAMPANMDA_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            double g = nrn_current_DetAMPANMDA(id, pnodecount, inst, data, indexes, thread, nt, v+0.001);
            double rhs = nrn_current_DetAMPANMDA(id, pnodecount, inst, data, indexes, thread, nt, v);
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
    void nrn_state_DetAMPANMDA(NrnThread* nt, Memb_list* ml, int type) {
        int nodecount = ml->nodecount;
        int pnodecount = ml->_nodecount_padded;
        const int* node_index = ml->nodeindices;
        double* data = ml->data;
        const double* voltage = nt->_actual_v;
        Datum* indexes = ml->pdata;
        ThreadDatum* thread = ml->_thread;
        auto* const inst = static_cast<DetAMPANMDA_Instance*>(ml->instance);

        #pragma omp simd
        #pragma ivdep
        for (int id = 0; id < nodecount; id++) {
            int node_id = node_index[id];
            double v = voltage[node_id];
            #if NRN_PRCELLSTATE
            inst->v_unused[id] = v;
            #endif
            inst->A_AMPA[id] = inst->A_AMPA[id] + (1.0 - exp(nt->_dt * (( -1.0) / inst->tau_r_AMPA[id]))) * ( -(0.0) / (( -1.0) / inst->tau_r_AMPA[id]) - inst->A_AMPA[id]);
            inst->B_AMPA[id] = inst->B_AMPA[id] + (1.0 - exp(nt->_dt * (( -1.0) / inst->tau_d_AMPA[id]))) * ( -(0.0) / (( -1.0) / inst->tau_d_AMPA[id]) - inst->B_AMPA[id]);
            inst->A_NMDA[id] = inst->A_NMDA[id] + (1.0 - exp(nt->_dt * (( -1.0) / inst->tau_r_NMDA[id]))) * ( -(0.0) / (( -1.0) / inst->tau_r_NMDA[id]) - inst->A_NMDA[id]);
            inst->B_NMDA[id] = inst->B_NMDA[id] + (1.0 - exp(nt->_dt * (( -1.0) / inst->tau_d_NMDA[id]))) * ( -(0.0) / (( -1.0) / inst->tau_d_NMDA[id]) - inst->B_NMDA[id]);
        }
    }


    /** register channel with the simulator */
    void _DetAMPANMDA_reg() {

        int mech_type = nrn_get_mechtype("DetAMPANMDA");
        DetAMPANMDA_global.mech_type = mech_type;
        if (mech_type == -1) {
            return;
        }

        _nrn_layout_reg(mech_type, 0);
        point_register_mech(mechanism_info, nrn_alloc_DetAMPANMDA, nrn_cur_DetAMPANMDA, nullptr, nrn_state_DetAMPANMDA, nrn_init_DetAMPANMDA, nrn_private_constructor_DetAMPANMDA, nrn_private_destructor_DetAMPANMDA, first_pointer_var_index(), nullptr, nullptr, 1);

        hoc_reg_bbcore_read(mech_type, bbcore_read);
        hoc_reg_bbcore_write(mech_type, bbcore_write);
        hoc_register_prop_size(mech_type, float_variables_size(), int_variables_size());
        hoc_register_dparam_semantics(mech_type, 0, "area");
        hoc_register_dparam_semantics(mech_type, 1, "pntproc");
        hoc_register_dparam_semantics(mech_type, 2, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 3, "bbcorepointer");
        hoc_register_dparam_semantics(mech_type, 4, "netsend");
        hoc_register_net_receive_buffering(net_buf_receive_DetAMPANMDA, mech_type);
        set_pnt_receive(mech_type, net_receive_DetAMPANMDA, net_init, num_net_receive_args());
        hoc_register_net_send_buffering(mech_type);
        hoc_register_var(hoc_scalar_double, hoc_vector_double, NULL);
    }
}
