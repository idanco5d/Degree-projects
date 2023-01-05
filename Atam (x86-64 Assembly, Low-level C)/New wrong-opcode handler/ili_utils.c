#include <asm/desc.h>

// TO_DO: store the offset of the IDT at the parameter given
void my_store_idt(struct desc_ptr *idtr) {
// <STUDENT FILL>
    asm volatile("sidt %0;" : "=m"(*idtr) );
    return;
// </STUDENT FILL>
}////

// TO_DO: load the offset of the IDT from the parameter given
void my_load_idt(struct desc_ptr *idtr) {
// <STUDENT FILL>
    asm volatile("lidt %0;": : "m"(*idtr) );
    return;
// <STUDENT FILL>
}

// TO_DO: set the offset in the right place at the parameter gate
// try to remember - how this information is stored?
void my_set_gate_offset(gate_desc *gate, unsigned long addr) {
// <STUDENT FILL>
    gate->offset_low = (uint16_t)(addr & 0xFFFF);
    gate->offset_middle = (uint16_t)((addr>>16) & 0xFFFF);
    gate->offset_high = (uint32_t)((addr>>32) & 0xFFFFFFFF);
    
// </STUDENT FILL>
}

// TO_DO: return val is the offset stored at the parameter gate_desc gate
// try to remember - how this information is stored?
unsigned long my_get_gate_offset(gate_desc *gate) {
// <STUDENT FILL>

    return gate->offset_low | ((unsigned long)gate->offset_middle << 16) |
        ((unsigned long) gate->offset_high << 32);

// </STUDENT FILL>
}
