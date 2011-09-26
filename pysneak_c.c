#include <sys/socket.h>
#include <dlfcn.h>
#include <stdio.h>
#include <Python.h>

#define PYCHECK(v) do {     \
    if (v == NULL) {        \
        PyErr_Print();      \
        exit(-1);           \
    } } while (0);


typedef int (*socket_ft)(int domain, int type, int protocol);

static void pysneak_init(void) __attribute__ ((constructor));
static void pysneak_fini(void) __attribute__ ((destructor));

static PyObject *pModule;
static PyObject *py_socket;

void fix_python_path(void) {
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");
    return;
}

void pysneak_init(void) {
    PyObject *pName;

    printf("pysneak init\n");

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

