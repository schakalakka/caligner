from cffi import FFI

from pyinterface import alignconfig

ffi = FFI()

lib = ffi.dlopen('../lib/libaligner.so')
print('Loaded lib {0}'.format(lib))

ffi.cdef('''
typedef _Bool bool;

typedef struct {
    int match;
    int mismatch;
    int gap_open;
    int gap_extend;
    bool top;
    bool left;
    bool right;
    bool bottom;
    int lower_diagonal;
    int upper_diagonal;
    bool local;
} alignconfig;

typedef alignconfig* alignconfig_t;
''')

ffi.cdef('int naive_overlap(char* s, unsigned int s_len, char* t, unsigned int t_len);')
ffi.cdef('int align(char* a, unsigned int a_len, char* b, unsigned int b_len, const alignconfig_t);')


def naive_overlap(a: str, b: str) -> int:
    return lib.naive_overlap(bytes(a, 'ascii'), len(a), bytes(b, 'ascii'), len(b))


def align(a, b, conf=alignconfig.AlignConfig()):
    c_conf = ffi.new('alignconfig_t', (
        conf.match, conf.mismatch, conf.gap_open, conf.gap_extend, conf.top, 1 if conf.left else 0,
        1 if conf.right else 0, 1 if conf.bottom else 0,
        conf.lower_diagonal, conf.upper_diagonal, conf.local))
    lib.align(bytes(a, 'ascii'), len(a), bytes(b, 'ascii'), len(b), c_conf)
    print(lib.align(bytes(a, 'ascii'), len(a), bytes(b, 'ascii'), len(b), c_conf))
