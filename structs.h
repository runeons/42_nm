
Basic types
    ElfN_Addr       Unsigned program address, uintN_t
    ElfN_Off        Unsigned file offset, uintN_t
    ElfN_Section    Unsigned section index, uint16_t
    ElfN_Versym     Unsigned version symbol information, uint16_t
    Elf_Byte        unsigned char
    ElfN_Half       uint16_t
    ElfN_Sword      int32_t
    ElfN_Word       uint32_t
    ElfN_Sxword     int64_t
    ElfN_Xword      uint64_t



#define EI_NIDENT 16

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    ElfN_Addr     e_entry;
    ElfN_Off      e_phoff;
    ElfN_Off      e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    uint16_t      e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
} ElfN_Ehdr;

#define PRIu8 "hhu"
#define PRId8 "hhd"
#define PRIx8 "hhx"
#define PRIu16 "hu"
#define PRId16 "hd"
#define PRIx16 "hx"
#define PRIu32 "u"
#define PRId32 "d"
#define PRIx32 "x"
#define PRIu64 "llu" // or possibly "lu"
#define PRId64 "lld" // or possibly "ld"
#define PRIx64 "llx" // or possibly "lx"

typedef struct {
    uint32_t   sh_name;
    uint32_t   sh_type;
    uint32_t   sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off  sh_offset;
    uint32_t   sh_size;
    uint32_t   sh_link;
    uint32_t   sh_info;
    uint32_t   sh_addralign;
    uint32_t   sh_entsize;
} Elf32_Shdr;

typedef struct {
    uint32_t   sh_name;
    uint32_t   sh_type;
    uint64_t   sh_flags;
    Elf64_Addr sh_addr;
    Elf64_Off  sh_offset;
    uint64_t   sh_size;
    uint32_t   sh_link;
    uint32_t   sh_info;
    uint64_t   sh_addralign;
    uint64_t   sh_entsize;
} Elf64_Shdr;

SYMTABS - String and symbol tables

String table sections hold null-terminated character sequences,
commonly called strings.  The object file uses these strings to
represent symbol and section names.  One references a string as
an index into the string table section.  The first byte, which is
index zero, is defined to hold a null byte ('\0').  Similarly, a
string table's last byte is defined to hold a null byte, ensuring
null termination for all strings.

An object file's symbol table holds information needed to locate
and relocate a programs symbolic definitions and references.  A
symbol table index is a subscript into this array.

    typedef struct {
        uint32_t      st_name;
        Elf32_Addr    st_value;
        uint32_t      st_size;
        unsigned char st_info;
        unsigned char st_other;
        uint16_t      st_shndx;
    } Elf32_Sym;

    typedef struct {
        uint32_t      st_name;
        unsigned char st_info;
        unsigned char st_other;
        uint16_t      st_shndx;
        Elf64_Addr    st_value;
        uint64_t      st_size;
    } Elf64_Sym;



    glibc/elf/elf.h:570-579
    570 /* How to extract and insert information held in the st_info field.  */
    572 #define ELF32_ST_BIND(val)\
                    (((unsigned char) (val)) >> 4)
    573 #define ELF32_ST_TYPE(val)\
                    ((val) & 0xf)
    574 #define ELF32_ST_INFO(bind, type) \
                     (((bind) << 4) + ((type) & 0xf))
    
    576 /* Both Elf32_Sym and Elf64_Sym use the same one-byte st_info field.  */

    577 #define ELF64_ST_BIND(val) ELF32_ST_BIND (val)
    578 #define ELF64_ST_TYPE(val) ELF32_ST_TYPE (val)
    579 #define ELF64_ST_INFO(bind, type) ELF32_ST_INFO ((bind), (type)) 



/usr/include/elf.h

ST_BIND ET ST_TYPE

/* Legal values for ST_BIND subfield of st_info (symbol binding).  */

#define STB_LOCAL       0               /* Local symbol */
#define STB_GLOBAL      1               /* Global symbol */
#define STB_WEAK        2               /* Weak symbol */
#define STB_NUM         3               /* Number of defined types.  */
#define STB_LOOS        10              /* Start of OS-specific */
#define STB_GNU_UNIQUE  10              /* Unique symbol.  */
#define STB_HIOS        12              /* End of OS-specific */
#define STB_LOPROC      13              /* Start of processor-specific */
#define STB_HIPROC      15              /* End of processor-specific */

/* Legal values for ST_TYPE subfield of st_info (symbol type).  */

#define STT_NOTYPE      0               /* Symbol type is unspecified */
#define STT_OBJECT      1               /* Symbol is a data object */
#define STT_FUNC        2               /* Symbol is a code object */
#define STT_SECTION     3               /* Symbol associated with a section */
#define STT_FILE        4               /* Symbol's name is file name */
#define STT_COMMON      5               /* Symbol is a common data object */
#define STT_TLS         6               /* Symbol is thread-local data object*/
#define STT_NUM         7               /* Number of defined types.  */
#define STT_LOOS        10              /* Start of OS-specific */
#define STT_GNU_IFUNC   10              /* Symbol is indirect code object */
#define STT_HIOS        12              /* End of OS-specific */
#define STT_LOPROC      13              /* Start of processor-specific */
#define STT_HIPROC      15              /* End of processor-specific */

E_TYPE

#define ET_NONE         0               /* No file type */
#define ET_REL          1               /* Relocatable file */     =====> .o
#define ET_EXEC         2               /* Executable file */
#define ET_DYN          3               /* Shared object file */   =====> .so   a.out
#define ET_CORE         4               /* Core file */
#define ET_NUM          5               /* Number of defined types */
#define ET_LOOS         0xfe00          /* OS-specific range start */
#define ET_HIOS         0xfeff          /* OS-specific range end */
#define ET_LOPROC       0xff00          /* Processor-specific range start */
#define ET_HIPROC       0xffff          /* Processor-specific range end */

exit_error = STT_FUNC 2 et STB_GLOBAL 1         .text
exit_corrupted = STT_FUNC 2 et STB_GLOBAL 1     .text


/* Special section indices.  */

#define SHN_UNDEF       0               /* Undefined section */
#define SHN_LORESERVE   0xff00          /* Start of reserved indices */
#define SHN_LOPROC      0xff00          /* Start of processor-specific */
#define SHN_BEFORE      0xff00          /* Order section before all others
                                           (Solaris).  */
#define SHN_AFTER       0xff01          /* Order section after all others
                                           (Solaris).  */
#define SHN_HIPROC      0xff1f          /* End of processor-specific */
#define SHN_LOOS        0xff20          /* Start of OS-specific */
#define SHN_HIOS        0xff3f          /* End of OS-specific */
#define SHN_ABS         0xfff1          /* Associated symbol is absolute */
#define SHN_COMMON      0xfff2          /* Associated symbol is common */
#define SHN_XINDEX      0xffff          /* Index is in extra table.  */
#define SHN_HIRESERVE   0xffff          /* End of reserved indices */


// t_type all_types[] =
// {
//     {'b', 1, ".bss",             },//  -1, -1// bss uninitializes
//     {'d', 1, ".data",            },//  -1, -1// initialized
//     {'g', 1, "",                 },//  -1, -1// initialized for small objects
//     {'p', 0, "",                 },//  -1, -1// stack unwind
//     {'r', 1, ".rodata",          },//  -1, -1// read only 
//     {'s', 1, "",                 },//  -1, -1// uninitialized or zero-initialized data section for small objects
//     {'t', 1, ".text",            },//  -1, -1// executable text (code) section
//     {'a', -1, "",                 },//  -1, -1// ? capitalise type - STT_FILE
//     {'A', -1, "",                 },//  -1, -1// ? capitalise absolute
//     {'C', 0, "",                 },//  -1, -1// common (inc. uninitialized)
//     {'i', 0, "",                 },//  -1, -1// indirect functions
//     {'N', 0, ".debug",           },//  -1, -1// debug
//     {'u', 0, "",                 },//  -1, -1// unique global
//     {'U', 0, "",                 },//  -1, -1// undefined
//     {'v', 1, "",                 },//  -1, -1// weak object
//     {'w', 1, "",                 },//  -1, -1// weak symbol not weak object
//     {'-', 0, "",                 },//  -1, -1// stabs symbol
//     {'?', 0, "",                 },//  -1, -1// unknown
// };


// // not used PREDEFINED USER sections = 
// .comment    // comment
// .init       // runtime initialization instructions
// .line       // line # info for symbolic debugging
// .note       // note information

