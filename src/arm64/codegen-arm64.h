#define GPR_COUNT 8
#define FPR_COUNT 16

// machine resgiters in order
typedef enum gpr
{
    // arm64 general register from 0 to 7
    no_gpr = -1,
    w0,
    w1,
    w2,
    w3,
    w4,
    w5,
    w6,
    w7,
} gpr;

typedef enum fpr
{
    // arm64 fp registers from 0 to 15
    no_fpr = -1,
    s0,
    s1,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    s12,
    s13,
    s14,
    s15
} fpr;

// debuging
char *get_gpr_symbol(gpr reg);
char *get_fpr_symbol(fpr reg);
void print_inst(char *instr_symbol, char *instr_symbol_intermediate, int imm, char *reg_dst, char *reg_src1, char *reg_src2, char *comments);

// code emitters
void emit(unsigned int instruction);

// fp register allocation
extern gpr scratch_gpr;
extern fpr scratch_fpr;
extern fpr reserved_fprs[1];
extern char *gpr_order[GPR_COUNT];
extern char *fpr_order[FPR_COUNT];

// opcodes
void load_int_to_gpr_arm64(int imm, gpr reg_dst);
void load_int_to_fpr_arm64(int imm, fpr reg_dst);
void convert_gpr_to_fpr_arm64(fpr reg_dst, gpr reg_src);
void convert_fpr_to_gpr_arm64(gpr reg_dst, fpr reg_src);
void negate_fpr_arm64(fpr reg_dst);
void add_fpr_to_fpr_arm64(fpr reg_dst, fpr reg_src);
void subtract_fpr_from_fpr_arm64(fpr reg_dst, fpr reg_src);
void multiply_fpr_to_fpr_arm64(fpr reg_dst, fpr reg_src);
void divide_fpr_by_fpr_arm64(fpr reg_dst, fpr reg_src);
void prepare_return_arm64();
