#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 10000

typedef struct {
    int **faceData;
    int *slash;
} Face;

typedef struct {
    char *name;
    float **vertexData;
    float **textureData;
    float **normalData;
    Face *faces;
} Object;

int vertexCtr;
int textureCtr;
int normalCtr;
int faceCtr;
int **RGB;

int vIndex;
int nIndex;
int tIndex;
PyObject *whd;
Object *newObj;

int fileSize(FILE *fp);
void endFile(FILE *fp);
char skipSpace(FILE *fp);
char *getToken(FILE *fp);
void initializeObject(Object *obj);
void parseName(FILE *fp, Object *obj, char *token);
void parseVertex(FILE *fp, Object *obj, char *token);
void parseTexture(FILE *fp, Object *obj, char *token);
void parseNormal(FILE *fp, Object *obj, char *token);
void parseFaces(FILE *fp, Face *faces, char *token);
void parseObject(FILE *fp, Object *obj);
void printPPM(PyObject *whd, int **RGB);
int **makeRGB(PyObject *whd, PyObject *rgb);
void writeJS(int **RGB, PyObject *whd);
void reorganizeData(float **vertices, float **normals, float **textCoords, int *indices);
void appendVertex(int vIndex, int fIndex, int i, float **vertices, int *indices);
void appendTexture(int tIndex, int fIndex, int i, float **textCoords);
void appendNormal(int nIndex, int fIndex, int i, float **normals);
void writeOBJ(FILE *fp);
void writeOtherdata(FILE *fp);
void writeVertices(FILE *fp, float **vertices);
void writeNormals(FILE *fp, float **normals);
void writeTextCoords(FILE *fp, float **textCoords);
void writeIndices(FILE *fp, int *indices);
int writeWHD(FILE *fp, PyObject *whd);
void writeTexture(FILE *fp, int **RGB, int area);
void freeObject(Object *obj);
void freeArr(float **arr, int counter);
void freeFaceData(int **faceData);
void freeFaces(Face *face);