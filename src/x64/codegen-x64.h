#define GPR_COUNT 8
#define FPR_COUNT 16

// machine resgiters in order
typedef enum gpr
{
    // x64 general register from 0 to 7
    no_gpr = -1,
    eax,
    ecx,
    edx,
    ebx,
    esp,
    ebp,
    esi,
    edi,
} gpr;

typedef enum fpr
{
    // x64 fp registers from 0 to 15
    no_fpr = -1,
    xmm0,
    xmm1,
    xmm2,
    xmm3,
    xmm4,
    xmm5,
    xmm6,
    xmm7,
    xmm8,
    xmm9,
    xmm10,
    xmm11,
    xmm12,
    xmm13,
    xmm14,
    xmm15
} fpr;

// debuging
char *get_gpr_symbol(gpr reg);
char *get_fpr_symbol(fpr reg);
void print_inst(char *instr_symbol, int imm, char *reg_dst, char *reg_src, char *comments);

// code emitters
void emit(unsigned char byte);
void emit_imm(int immediate);

// fp register allocation
extern gpr scratch_gpr;
extern fpr scratch_fpr;
extern fpr reserved_fprs[1];
extern char *gpr_order[GPR_COUNT];
extern char *fpr_order[FPR_COUNT];

// opcodes
void load_int_to_gpr_x64(int imm, gpr reg_dst);
void load_int_to_fpr_x64(int imm, fpr reg_dst);
void convert_gpr_to_fpr_x64(fpr reg_dst, gpr reg_src);
void convert_fpr_to_gpr_x64(gpr reg_dst, fpr reg_src);
void negate_fpr_x64(fpr reg_dst);
void add_fpr_to_fpr_x64(fpr reg_dst, fpr reg_src);
void subtract_fpr_from_fpr_x64(fpr reg_dst, fpr reg_src);
void multiply_fpr_to_fpr_x64(fpr reg_dst, fpr reg_src);
void divide_fpr_by_fpr_x64(fpr reg_dst, fpr reg_src);
void prepare_return_x64();
