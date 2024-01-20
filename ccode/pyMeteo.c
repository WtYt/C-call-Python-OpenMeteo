/*****************************************
* Auther : Yuta Watanabe
* Name   : pyMeteo.c
* Date   : 2024-01-07
* About  : invoke python module
*****************************************/
#include <Python.h>
#include <stdlib.h>
#include <string.h>
#include "pyMeteo.h"

MeteoWeather *pyMeteo(const char *name, int year, int month, int day){
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue, *pValue2;

    MeteoWeather *weather = (MeteoWeather*)malloc(sizeof(MeteoWeather));
    weather->fullname = (char*)malloc(sizeof(char)*PY_STR_BUFFSIZE*6);
    weather->weather_msg = (char*)malloc(sizeof(char)*PY_STR_BUFFSIZE*6);

    //Py_Initialize();
    PyObject *sys      = PyImport_ImportModule("sys");
    PyObject *sys_path = PyObject_GetAttrString(sys, "path");
    PyList_Append(sys_path, PyUnicode_DecodeFSDefault("../pycode"));
    pName = PyUnicode_FromString("Meteo");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    //printf("reached\n");

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "getWeather");
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(4);
            pValue = PyUnicode_FromString(name);
            PyTuple_SetItem(pArgs, 0, pValue);
            pValue = PyLong_FromLong(year);
            PyTuple_SetItem(pArgs, 1, pValue);
            pValue = PyLong_FromLong(month);
            PyTuple_SetItem(pArgs, 2, pValue);
            pValue = PyLong_FromLong(day);
            PyTuple_SetItem(pArgs, 3, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);

            Py_DECREF(pArgs);
            if (pValue != NULL) {
                Py_ssize_t size = PY_STR_BUFFSIZE;
                pValue2                  = PyTuple_GetItem(pValue, 0);
                strncpy(weather->fullname, PyUnicode_AsUTF8AndSize(pValue2, &size), PY_STR_BUFFSIZE*6);
                pValue2                  = PyTuple_GetItem(pValue, 1);
                strncpy(weather->weather_msg, PyUnicode_AsUTF8AndSize(pValue2, &size), PY_STR_BUFFSIZE*6);
                pValue2                  = PyTuple_GetItem(pValue, 2);
                weather->weather_code    = (int)PyLong_AsLong(pValue2);
                pValue2                  = PyTuple_GetItem(pValue, 3);
                weather->max_temperature = PyFloat_AsDouble(pValue2);
                pValue2                  = PyTuple_GetItem(pValue, 4);
                weather->min_temperature = PyFloat_AsDouble(pValue2);
                Py_DECREF(pValue);
                Py_DECREF(pValue2);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return NULL;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"getWeather\"\n");
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"Meteo\"\n");
        return NULL;
    }
    //Py_Finalize();
    return weather;
}

/*
int main(int argc, char const *argv[]) {
  char name[100];
  int year, month, day;
  printf("どこの天気を調べますか\n> ");
  scanf("%s", name);
  printf("いつの天気ですか（1940年から二週間先まで　半角）\ny m d 年 月 日\n> ");
  scanf("%d %d %d", &year, &month, &day);
  MeteoWeather* weather = pyMeteo(name, year, month, day);
  printf("%04d-%02d-%02d\n", year, month, day);
  printf("%sの天候をお知らせします\n", weather->fullname);
  printf("天気は%sです\n", weather->weather_msg);
  printf("最高気温は%.1lf℃です\n", weather->max_temperature);
  printf("最高気温は%.1lf℃です\n", weather->min_temperature);
  free(weather->fullname);
  free(weather->weather_msg);
  free(weather);
  return 0;
}
*/
