#include "hornet/translator.hh"

#include "hornet/java.hh"
#include "hornet/vm.hh"

#include <classfile_constants.h>
#include <jni.h>

#include <cstdio>

using namespace std;

namespace hornet {

void translator::translate()
{
    prologue();

    uint16_t pc = 0;

next_insn:
    assert(pc < _method->code_length);

    uint8_t opc = _method->code[pc];

    switch (opc) {
    case JVM_OPC_iconst_m1:
    case JVM_OPC_iconst_0:
    case JVM_OPC_iconst_1:
    case JVM_OPC_iconst_2:
    case JVM_OPC_iconst_3:
    case JVM_OPC_iconst_4:
    case JVM_OPC_iconst_5: {
        jint value = opc - JVM_OPC_iconst_0;
        op_const(type::t_int, value);
        break;
    }
    case JVM_OPC_iload_0:
    case JVM_OPC_iload_1:
    case JVM_OPC_iload_2:
    case JVM_OPC_iload_3: {
        uint16_t idx = opc - JVM_OPC_iload_0;
        op_load(type::t_int, idx);
        break;
    }
    case JVM_OPC_istore_0:
    case JVM_OPC_istore_1:
    case JVM_OPC_istore_2:
    case JVM_OPC_istore_3: {
        uint16_t idx = opc - JVM_OPC_istore_0;
        op_store(type::t_int, idx);
        break;
    }
    case JVM_OPC_iadd: {
        op_binary(type::t_int, binop::op_add);
        break;
    }
    case JVM_OPC_isub: {
        op_binary(type::t_int, binop::op_sub);
        break;
    }
    case JVM_OPC_imul: {
        op_binary(type::t_int, binop::op_mul);
        break;
    }
    case JVM_OPC_idiv: {
        op_binary(type::t_int, binop::op_div);
        break;
    }
    case JVM_OPC_irem: {
        op_binary(type::t_int, binop::op_rem);
        break;
    }
    case JVM_OPC_iand: {
        op_binary(type::t_int, binop::op_and);
        break;
    }
    case JVM_OPC_ior: {
        op_binary(type::t_int, binop::op_or);
        break;
    }
    case JVM_OPC_ixor: {
        op_binary(type::t_int, binop::op_xor);
        break;
    }
    case JVM_OPC_return:
        op_returnvoid();
        return;
    default:
        fprintf(stderr, "error: unsupported bytecode: %u\n", opc);
        abort();
    }

    pc += opcode_length[opc];

    goto next_insn;
}

}