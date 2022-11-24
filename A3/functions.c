#include "functions.h"

void endFile(FILE *fp){
    if (feof(fp)){
        writeJS(RGB, whd);
        freeObject(newObj);
        fclose(fp);
        exit(1);
    }
}

char skipSpace(FILE *fp){
    char tmp = getc(fp);
    endFile(fp);
    if (tmp == 32 || tmp == 9){
        while (tmp == 32 || tmp == 9){
            tmp = getc(fp);
            endFile(fp);
        }
    }
    return(tmp);
}

char *getToken(FILE *fp){
    char *token = (char *)malloc(sizeof(char) * 50);
    char tmp = skipSpace(fp);

    memset(token, '\0', sizeof(char) * 50);
    while (tmp != 32 && tmp != 9){
        if (tmp == 10 && strlen(token) != 0){
            ungetc(tmp, fp);
            break;
        }
        strncat(token, &tmp, 1);
        token[strlen(token)] = '\0';
        if (tmp == 10){
            break;
        }
        tmp = getc(fp);
    }
    return(token);
}

void initializeObject(Object *obj){
    obj->name = (char*)malloc(sizeof(char) * MAX);
    obj->vertexData = (float**)malloc(sizeof(float *) * MAX);
    obj->normalData = (float**)malloc(sizeof(float *) * MAX);
    obj->textureData = (float**)malloc(sizeof(float *) * MAX);
    obj->faces = (Face *)malloc(sizeof(Face) * MAX);
    obj->faces->faceData = (int**)malloc(sizeof(int *) * MAX);
    obj->faces->slash = malloc(sizeof(int) * MAX);       
}

void parseObject(FILE *fp, Object *obj){
    char *token = getToken(fp);
    parseName(fp, obj, token);
    parseVertex(fp, obj, token);
    parseNormal(fp, obj, token);
    parseTexture(fp, obj, token);
    parseFaces(fp, obj->faces, token);
    free(token);
    parseObject(fp, obj);
}

void parseName(FILE *fp, Object *obj, char *token){
    if (!strcmp(token, "o") || !strcmp(token, "g")){
        char *name = getToken(fp);
        memcpy(obj->name, name, strlen(name) + 1);
    }
}

void parseVertex(FILE *fp, Object *obj, char *token){
    if(token != NULL && !strcmp(token, "v")){
        obj->vertexData[vertexCtr] = (float*)malloc(sizeof(float) * 3);
        token = getToken(fp);
        int i = 0;
        while (strcmp(token, "\n")){
            obj->vertexData[vertexCtr][i] = atof(token);
            free(token);
            token = NULL;
            token = getToken(fp);
            i++;
        }
        vertexCtr++;
        if (i < 2 || i > 3){
            printf("Error: Vertex limit exceeded\n");
            exit(2);
        }
    }
}

void parseNormal(FILE *fp, Object *obj, char *token){
    if(token != NULL && !strcmp(token, "vn")){
        obj->normalData[normalCtr] = (float*)malloc(sizeof(float) * 3);
        token = getToken(fp);
        int i = 0;
        while(strcmp(token, "\n")){
            obj->normalData[normalCtr][i] = atof(token);
            free(token);
            token = NULL;
            token = getToken(fp);
            i++;
        }
        normalCtr++;
        if (i < 2 || i > 3){
            printf("Error: Normal limit exceeded\n");
            exit(3);
        }
    }
}

void parseTexture(FILE *fp, Object *obj, char *token){
    if(token != NULL && !strcmp(token, "vt")){
        obj->textureData[textureCtr] = (float*)malloc(sizeof(float) * 2);
        token = getToken(fp);
        int i = 0;
        while(strcmp(token, "\n")){
            obj->textureData[textureCtr][i] = atof(token);
            free(token);
            token = NULL;
            token = getToken(fp);
            i++;
        }
        textureCtr++;
        if (i < 1 || i > 2){
            printf("Error: Texture limit exceeded\n");
            exit(4);
        }
    }
}

void parseFaces(FILE *fp, Face *faces, char *token){
    if(token != NULL && !strcmp(token, "f")){
        faces->faceData[faceCtr] = (int*)malloc(sizeof(int) * 13);
        for (int i = 0; i < 13; i++){
            faces->faceData[faceCtr][i] = '\0';
        }
        token = getToken(fp);
        int i = 0;
        
        char *p = strstr(token, "//");
        if(p){
            faces->slash[faceCtr] = 1;
        } else {
            faces->slash[faceCtr] = 0;
        }
        while(strcmp(token, "\n")){
            char *tmp = strtok(token, "/");
            while(tmp != NULL){
                faces->faceData[faceCtr][i] = atoi(tmp);
                tmp = strtok(NULL, "/");
                i++;
            }

            free(token);
            token = getToken(fp);
        }
        
        faceCtr++;
        if (i < 1 || i > 12){
            printf("Error: Face limit exceeded\n");
            exit(5);
        }
    }
}

void printPPM(PyObject *whd, int **RGB){
    PyObject *w = PySequence_GetItem(whd, 0);
    PyObject *h = PySequence_GetItem(whd, 1);
    PyObject *d = PySequence_GetItem(whd, 2);
    int width = PyLong_AsLong(w);
    int height = PyLong_AsLong(h);
    int depth = PyLong_AsLong(d);

    printf("%d %d %d\n", width, height, depth);
    for (int i = 0; i < height * width; i++){
        for (int j = 0; j < 4; j++){
            printf("%d ", RGB[i][j]);
        }
        printf("\n");
    }
}

int **makeRGB(PyObject *whd, PyObject *rgb){
    PyObject *w = PySequence_GetItem(whd, 0);
    PyObject *h = PySequence_GetItem(whd, 1);
    int width = PyLong_AsLong(w);
    int height = PyLong_AsLong(h);

    int area = width * height;
    int **RGB = malloc(sizeof(int *) * area);
    for(int i = 0; i < area; i++){
        RGB[i] = malloc(sizeof(int) * 4);
    }

    int index = 0;
    for (int i = 0; i < area; i++){
        for (int j = 0; j < 3; j++){
            PyObject *val = PySequence_GetItem(rgb, index);
            int num = PyLong_AsLong(val);
            RGB[i][j] = num;
            index++;
        }
        RGB[i][3] = 225;
    }
    return(RGB);
}

void writeJS(int **RGB, PyObject *whd){
    FILE *fp = fopen("html/js/loaddata.js", "w");
    writeOBJ(fp);
    int area = writeWHD(fp, whd);
    writeTexture(fp, RGB, area);
    fclose(fp);
}

void writeOBJ(FILE *fp){
    float **vertices = (float **)malloc(sizeof(float *) * MAX);
    float **normals = (float **)malloc(sizeof(float *) * MAX);
    float **textCoords = (float **)malloc(sizeof(float *) * MAX);
    int *indices = (int *)malloc(sizeof(int) * MAX);
    reorganizeData(vertices, normals, textCoords, indices);
    writeOtherdata(fp);
    writeVertices(fp, vertices);
    writeNormals(fp, normals);
    writeTextCoords(fp, textCoords);
    writeIndices(fp, indices);
}

void reorganizeData(float **vertices, float **normals, float **textCoords, int *indices){
    vIndex = 0;
    nIndex = 0;
    tIndex = 0;
    for (int i = 0; i < faceCtr; i++){
        appendVertex(vIndex, 0, i, vertices, indices);
        vIndex++;
        appendVertex(vIndex, 3, i, vertices, indices);
        vIndex++;
        appendVertex(vIndex, 6, i, vertices, indices);
        vIndex++;

        appendTexture(tIndex, 1, i, textCoords);
        tIndex++;
        appendTexture(tIndex, 4, i, textCoords);
        tIndex++;
        appendTexture(tIndex, 7, i, textCoords);
        tIndex++;

        appendNormal(nIndex, 2, i, normals);
        nIndex++;
        appendNormal(nIndex, 5, i, normals);
        nIndex++;
        appendNormal(nIndex, 8, i, normals);
        nIndex++;
    }
}

void appendVertex(int vIndex, int fIndex, int i, float **vertices, int *indices){
    vertices[vIndex] = (float *)malloc(sizeof(float) * 3);
    for(int k = 0; k < 3; k++){
        vertices[vIndex][k] = newObj->vertexData[newObj->faces->faceData[i][fIndex] - 1][k];
    }
    indices[vIndex] = newObj->faces->faceData[i][fIndex] - 1;
}

void appendNormal(int nIndex, int fIndex, int i, float **normals){
    normals[nIndex] = (float *)malloc(sizeof(float) * 3);
    for(int k = 0; k < 3; k++){
        normals[nIndex][k] = newObj->normalData[newObj->faces->faceData[i][fIndex] - 1][k];
    }
}

void appendTexture(int tIndex, int fIndex, int i, float **textCoords){
    textCoords[tIndex] = (float *)malloc(sizeof(float) * 2);
    for(int k = 0; k < 2; k++){
        textCoords[tIndex][k] = newObj->textureData[newObj->faces->faceData[i][fIndex] - 1][k];
    }

}

void writeOtherdata(FILE *fp){
    fprintf(fp, "%s", "function getVertexCount(){");
    fprintf(fp, "%s", "return(");
    fprintf(fp, "%d", vertexCtr);
    fprintf(fp, "%s", ");}\n\n");

    fprintf(fp, "%s", "function getdistance(){");
    fprintf(fp, "%s", "return(");
    fprintf(fp, "%d", -3);
    fprintf(fp, "%s", ");}\n\n");
}

void writeVertices(FILE *fp, float **vertices){
    fprintf(fp, "%s", "function loadvertices(){\n");
    fprintf(fp, "%s", "\treturn[\n");
    for(int i = 0; i < vIndex; i++){
        fprintf(fp, "%s", "\t\t");
        for(int j = 0; j < 3; j++){
            fprintf(fp, "%f", vertices[i][j]);
            fprintf(fp, "%s", ",");
        }
        fprintf(fp, "%s", "\n");
    }
    fprintf(fp, "%s", "\t];\n}\n\n");
}

void writeNormals(FILE *fp, float **normals){   
    fprintf(fp, "%s", "function loadnormals(){\n");
    fprintf(fp, "%s", "\treturn[\n");
    for(int i = 0; i < nIndex; i++){
        fprintf(fp, "%s", "\t\t");
        for(int j = 0; j < 3; j++){
            fprintf(fp, "%f", normals[i][j]);
            fprintf(fp, "%s", ",");
        }
        fprintf(fp, "%s", "\n");
    }
    fprintf(fp, "%s", "\t];\n}\n\n");
}

void writeTextCoords(FILE *fp, float **textCoords){
    fprintf(fp, "%s", "function loadtextcoords(){\n");
    fprintf(fp, "%s", "\treturn[\n");
    for(int i = 0; i < tIndex; i++){
        fprintf(fp, "%s", "\t\t");
        for(int j = 0; j < 2; j++){
            fprintf(fp, "%f", textCoords[i][j]);
            fprintf(fp, "%s", ",");
        }
        fprintf(fp, "%s", "\n");
    }
    fprintf(fp, "%s", "\t];\n}\n\n");
}

void writeIndices(FILE *fp, int *indices){
    fprintf(fp, "%s", "function loadvertexindices(){\n");
    fprintf(fp, "%s", "\treturn[\n");
    for(int i = 0; i < vIndex; i++){
        fprintf(fp, "%s", "\t\t");
        fprintf(fp, "%d", indices[i]);
        fprintf(fp, "%s", ",");
        fprintf(fp, "%s", "\n");
    }
    fprintf(fp, "%s", "\t];\n}\n\n");
} 

int writeWHD(FILE *fp, PyObject *whd){
    PyObject *w = PySequence_GetItem(whd, 0);
    PyObject *h = PySequence_GetItem(whd, 1);
    int width = PyLong_AsLong(w);
    int height = PyLong_AsLong(h);

    fprintf(fp, "%s", "function loadwidth(){");
    fprintf(fp, "%s", "return(");
    fprintf(fp, "%d", width);
    fprintf(fp, "%s", ");}\n\n");

    fprintf(fp, "%s", "function loadheight(){");
    fprintf(fp, "%s", "return(");
    fprintf(fp, "%d", height);
    fprintf(fp, "%s", ");}\n\n");

    return(width * height);
}

void writeTexture(FILE *fp, int **RGB, int area){
    fprintf(fp, "%s", "function loadtexture(){\n");
    fprintf(fp, "%s", "\treturn(new Uint8Array([\n");
    
    for(int i = 0; i < area; i++){
        fprintf(fp, "%s", "\t\t");
        for (int j = 0; j < 4; j++){
            fprintf(fp, "%d", RGB[i][j]);
            if (i + 1 == area && j == 3){
                break;
            }
            fprintf(fp, "%s", ",");
        }
        fprintf(fp, "%s", "\n");
    }
    fprintf(fp, "%s", "\t]));\n}\n");

}

void freeObject(Object *obj){
    free(obj->name);
    freeArr(obj->vertexData, vertexCtr);
    free(obj->vertexData);
    freeArr(obj->normalData, normalCtr);
    free(obj->normalData);
    freeArr(obj->textureData, textureCtr);
    free(obj->textureData);
    freeFaceData(obj->faces->faceData);
    free(obj->faces->faceData);
    free(obj->faces->slash);
    free(obj->faces);
    free(obj);
}

void freeArr(float **arr, int counter){
    for (int i = 0; i < counter; i++){
        free(arr[i]);
    }
}

void freeFaceData(int **arr){
    for (int i = 0; i < faceCtr; i++){
        free(arr[i]);
    }
}