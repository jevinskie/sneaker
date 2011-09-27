#include <sys/socket.h>
#include <dlfcn.h>
#include <stdio.h>
#include <Python.h>

#define PYCHECK(v)          \
    do {                    \
        if (v == NULL) {    \
            PyErr_Print();  \
            exit(-1);       \
        }                   \
    } while (0);

typedef int (*accept_ft)(int, struct sockaddr *, socklen_t *);
static accept_ft real_accept;

typedef int (*bind_ft)(int, const struct sockaddr *, socklen_t);
static bind_ft real_bind;

typedef int (*connect_ft)(int, const struct sockaddr *, socklen_t);
static connect_ft real_connect;

typedef int (*getpeername_ft)(int, struct sockaddr *, socklen_t *);
static getpeername_ft real_getpeername;

typedef int (*getsockname_ft)(int, struct sockaddr *, socklen_t *);
static getsockname_ft real_getsockname;

typedef int (*getsockopt_ft)(int, int, int, void *, socklen_t *);
static getsockopt_ft real_getsockopt;

typedef int (*listen_ft)(int, int);
static listen_ft real_listen;

typedef ssize_t (*recv_ft)(int, void *, size_t, int);
static recv_ft real_recv;

typedef ssize_t (*recvfrom_ft)(int, void *, size_t, int,
    struct sockaddr *, socklen_t *);
static recvfrom_ft real_recvfrom;

typedef ssize_t (*recvmsg_ft)(int, struct msghdr *, int);
static recvmsg_ft real_recvmsg;

typedef ssize_t (*send_ft)(int, const void *, size_t, int);
static send_ft real_send;

typedef ssize_t (*sendmsg_ft)(int, const struct msghdr *, int);
static sendmsg_ft real_sendmsg;

typedef ssize_t (*sendto_ft)(int, const void *, size_t, int,
    const struct sockaddr *, socklen_t);
static sendto_ft real_sendto;

typedef int (*setsockopt_ft)(int, int, int, const void *, socklen_t);
static setsockopt_ft real_setsockopt;

typedef int (*shutdown_ft)(int, int);
static shutdown_ft real_shutdown;

typedef int (*socket_ft)(int, int, int);
static socket_ft real_socket;

typedef int (*sockatmark_ft)(int);
static sockatmark_ft real_sockatmark;

typedef int (*socketpair_ft)(int, int, int, int[2]);
static socketpair_ft real_socketpair;


static void pysneak_init(void) __attribute__ ((constructor));
static void pysneak_fini(void) __attribute__ ((destructor));

static PyObject *pModule;
static PyObject *py_socket;

void fix_python_path(void) {
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");
    return;
}

#define POPULATE_REAL_FUNC(func)                    \
    do {                                            \
        real_##func = dlsym((void *) -1l, #func);   \
        if (real_##func == NULL) {                  \
            exit(-1);                               \
        }                                           \
    } while (0);

void populate_real_funcs(void) {
    POPULATE_REAL_FUNC(accept);
    POPULATE_REAL_FUNC(bind);
    POPULATE_REAL_FUNC(connect);
    POPULATE_REAL_FUNC(getpeername);
    POPULATE_REAL_FUNC(getsockname);
    POPULATE_REAL_FUNC(getsockopt);
    POPULATE_REAL_FUNC(listen);
    POPULATE_REAL_FUNC(recv);
    POPULATE_REAL_FUNC(recvfrom);
    POPULATE_REAL_FUNC(recvmsg);
    POPULATE_REAL_FUNC(send);
    POPULATE_REAL_FUNC(sendmsg);
    POPULATE_REAL_FUNC(sendto);
    POPULATE_REAL_FUNC(setsockopt);
    POPULATE_REAL_FUNC(shutdown);
    POPULATE_REAL_FUNC(socket);
    POPULATE_REAL_FUNC(sockatmark);
    POPULATE_REAL_FUNC(socketpair);
    return;
}

void pysneak_init(void) {
    PyObject *pName;

    printf("pysneak init\n");

    populate_real_funcs();

    Py_Initialize();
    
    fix_python_path();
    
    pName = PyString_FromString("pysneak_py");
    PYCHECK(pName);
    
    pModule = PyImport_Import(pName);
    PYCHECK(pModule);
    Py_DECREF(pName);
    
    py_socket = PyObject_GetAttrString(pModule, "socket");
    PYCHECK(py_socket);
    
    return;
}

void pysneak_fini(void) {
    Py_XDECREF(py_socket);
    Py_DECREF(pModule);
    
    Py_Finalize();
    
    printf("pysneak fini\n");
}

int socket(int domain, int type, int protocol) {
    static socket_ft real_socket = NULL;
    PyObject *pArgs, *pValue;
    int fd;

    if (real_socket == NULL) {
        real_socket = dlsym((void *) -1l, "socket");
        //real_socket = dlsym(RTLD_NEXT, "socket");
    }

    pArgs = PyTuple_New(3);
    PYCHECK(pArgs);

    pValue = PyInt_FromLong(domain);
    PYCHECK(pValue);
    PyTuple_SetItem(pArgs, 0, pValue);

    pValue = PyInt_FromLong(type);
    PYCHECK(pValue);
    PyTuple_SetItem(pArgs, 1, pValue);

    pValue = PyInt_FromLong(protocol);
    PYCHECK(pValue);
    PyTuple_SetItem(pArgs, 2, pValue);

    pValue = PyObject_CallObject(py_socket, pArgs);
    PYCHECK(pValue);
    Py_DECREF(pArgs);

    fd = PyInt_AsLong(pValue);
    Py_DECREF(pValue);

    return fd;
}

