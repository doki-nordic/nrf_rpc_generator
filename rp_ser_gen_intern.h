#ifndef _RP_SER_GEN_INTERN_H
#define _RP_SER_GEN_INTERN_H

#define _SERIALIZE_ "SERIALIZE:USE"
#define _SERIALIZE_OUT(x) "SERIALIZE:OUT=" #x

#define SERIALIZE(...) _SERIALIZE_ ## __VA_ARGS__

#endif
