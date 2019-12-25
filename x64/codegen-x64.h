// machine resgiters in order
typedef enum a_register
{
    //x64 register from 0 to 3
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
static const int REG_COUNT = 8;
static const a_register FIRST_REG = eax;
static const a_register SCRATCH_REG = edi;

// list of registers that should not be used directly
// static to avoid linker error with "Duplicate Symbols".
static a_register forbidden_registers[4] = {edx, esp, ebp, edi};

//opcodes
void load_int_to_register_x64(int imm, a_register reg);
void move_register_to_register_x64(a_register reg_0, a_register reg_1);
void add_register_to_register_x64(a_register reg_0, a_register reg_1);
void subtract_register_from_register_x64(a_register reg_0, a_register reg_1);
void multiply_register_to_register_x64(a_register reg_0, a_register reg_1);
void divide_register_by_register_x64(a_register reg_0, a_register reg_1);
void prepare_return_x64();