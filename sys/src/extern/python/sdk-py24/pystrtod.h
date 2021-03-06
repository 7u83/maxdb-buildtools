#ifndef Py_STRTOD_H
#define Py_STRTOD_H

#ifdef __cplusplus
extern "C" {
#endif


double PyOS_ascii_strtod(const char *str, char **ptr);
double PyOS_ascii_atof(const char *str);
char * PyOS_ascii_formatd(char *buffer, int buf_len,  const char *format, double d);


#ifdef __cplusplus
}
#endif

#endif /* !Py_STRTOD_H */
