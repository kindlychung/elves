#include <bsd/vis.h>
#include <err.h>
#include <fcntl.h>  // file_control, O_RDONLY
#include <gelf.h>
#include <libelf.h>
#include <unistd.h>  // close
#include <cstdlib>
#include <iostream>

int main(int argc, char const* argv[]) {
    if (argc != 2) {
        errx(EXIT_FAILURE, "usage: %s <filename>", argv[0]);
    }
    if (elf_version(EV_CURRENT) == EV_NONE) {
        errx(EXIT_FAILURE, "libelf initialization failed: %s", elf_errmsg(-1));
    }
    int fd;
    if ((fd = open(argv[1], O_RDONLY, 0)) < 0) {
        errx(EXIT_FAILURE, "failed to open %s", argv[1]);
    }
    Elf* elf_pointer;
    char* elf_kind_string;
    Elf_Kind elf_k;
    if ((elf_pointer = elf_begin(fd, ELF_C_READ, NULL)) == NULL) {
        errx(EXIT_FAILURE, "failed to point to the beginnin of file. %s",
             elf_errmsg(-1));
    }
    elf_k = elf_kind(elf_pointer);
    if (elf_k == ELF_K_AR) {
        elf_kind_string = "archive";
    } else if (elf_k == ELF_K_ELF) {
        elf_kind_string = "object";
    } else if (elf_k == ELF_K_NONE) {
        elf_kind_string = "data";
    } else {
        elf_kind_string = "unrecognized";
    }
    std::cout << argv[1] << ": "
              << "elf " << elf_kind_string << "\n";
    GElf_Ehdr gelf_header;
    if (gelf_getehdr(elf_pointer, &gelf_header) == NULL) {
        errx(EXIT_FAILURE, "failed to get elf header: %s", elf_errmsg(-1));
    }

    int elf_class;
    if ((elf_class = gelf_getclass(elf_pointer)) == ELFCLASSNONE) {
        errx(EXIT_FAILURE, "failed to get class of elf: %s", elf_errmsg(-1));
    }
    std::cout << argv[1] << ": " << (elf_class == ELFCLASS32 ? 32 : 64)
              << "bit\n";
    char* elf_id;
    if ((elf_id = elf_getident(elf_pointer, NULL)) == NULL) {
        errx(EXIT_FAILURE, "failed to get elf id: %s", elf_errmsg(-1));
    }
    std::cout << argv[1] << " abi: " << EI_ABIVERSION << "\n";
    printf("%3s e_ident[0..%1d] %7s", " ", EI_ABIVERSION, " ");
    char bytes[5];
    for (int i = 0; i < EI_ABIVERSION; i++) {
        vis(bytes, elf_id[i], VIS_WHITE, 0);
        printf(" ['%s' %X]", bytes, elf_id[i]);
    }
    printf("\n");

#define PrintFormat "    %-20s 0x%x\n"
#define PrintField(N)                                      \
    do {                                                   \
        printf(PrintFormat, #N, (uintmax_t)gelf_header.N); \
    } while (0);

    PrintField(e_type);
    PrintField(e_machine);
    PrintField(e_version);
    PrintField(e_entry);
    PrintField(e_phoff);
    PrintField(e_shoff);
    PrintField(e_flags);
    PrintField(e_ehsize);
    PrintField(e_phentsize);
    PrintField(e_shentsize);
    elf_end(elf_pointer);
    close(fd);
    return 0;
}
