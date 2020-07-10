#ifndef _RP_SER_GEN_INTERN_H
#define _RP_SER_GEN_INTERN_H

#define _SERIALIZE_ "__SERIALIZE__:USE"
#define _SERIALIZE_OUT(x) "__SERIALIZE__:OUT=" #x
#define _SERIALIZE_HOST_FILE(file) "__SERIALIZE__:HOST_FILE=" file

#define SERIALIZE(...) _SERIALIZE_ ## __VA_ARGS__

#endif
