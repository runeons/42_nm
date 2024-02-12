
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
