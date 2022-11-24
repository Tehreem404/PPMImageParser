#include "functions.h"

int main(int argc, char **argv){
    if(argv[1] == NULL){
        printf("Usage: ./a1 [filename]\n");
        exit(0);
    }
    
    Py_Initialize();
    PyObject *pName, *pModule;
    PyObject *pArgs = NULL;
    PyObject *pValue = NULL;
    PyObject *pFunc = NULL;

    pName = PyUnicode_DecodeFSDefault("readppm");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "ppm_parser");
    }
    pArgs = PyTuple_New(1);
    pValue = PyUnicode_DecodeFSDefault(argv[1]);
    PyTuple_SetItem(pArgs, 0, pValue);
    
    pValue = PyObject_CallObject(pFunc, pArgs);
    PyObject *r, *g, *b, *rgb;
    PyArg_ParseTuple(pValue, "OOOOO", &whd, &r, &g, &b, &rgb);

    RGB = makeRGB(whd, rgb);
    
    Py_Finalize();
    if (Py_FinalizeEx() < 0) {
        return 120;
    }

    char *objName = strcat(argv[1], ".obj");

    FILE *fp = fopen(objName, "r");
    if (fp == NULL){
        printf("Error: C File not found\n");
        exit(1);
    }
    newObj = malloc(sizeof(Object));
    initializeObject(newObj);
    parseObject(fp, newObj);

    return(0);
}