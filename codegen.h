#ifndef CODEGEN
#define CODEGEN
void *codegen_mem;
int codegen_mem_offset;

//arch specific
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

//helpers
void init_codegen();
void emit(unsigned char byte);
int run_codegen_and_return();
void set_final_destination(a_register reg);
a_register final_destination;

// register allocation
a_register get_free_register();
void dealocate_reg(a_register reg);

//opcodes
void load_int_to_register(int imm, a_register reg);
void move_register_to_register(a_register reg_0, a_register reg_1);
void add_register_to_register(a_register reg_0, a_register reg_1);
void subtract_register_from_register(a_register reg_0, a_register reg_1);
void multiply_register_to_register(a_register reg_0, a_register reg_1);
void divide_register_by_register(a_register reg_0, a_register reg_1);
void prepare_return();
#endif