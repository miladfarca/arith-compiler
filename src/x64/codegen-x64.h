#define REG_COUNT 8

// machine resgiters in order
typedef enum a_register
{
    //x64 register from 0 to 7
    no_reg = -1,
    eax,
    ecx,
    edx,
    ebx,
    esp,
    ebp,
    esi,
    edi,
} a_register;

// register allocation
extern a_register first_reg;
extern a_register scratch_reg;
extern a_register reserved_registers[4];
extern char *register_order[REG_COUNT];

// opcodes
void load_int_to_register_x64(int imm, a_register reg_dst);
void negate_register_x64(a_register reg_dst);
void move_register_to_register_x64(a_register reg_dst, a_register reg_src);
void add_register_to_register_x64(a_register reg_dst, a_register reg_src);
void subtract_register_from_register_x64(a_register reg_dst, a_register reg_src);
void multiply_register_to_register_x64(a_register reg_dst, a_register reg_src);
void divide_register_by_register_x64(a_register reg_dst, a_register reg_src);
void prepare_return_x64();