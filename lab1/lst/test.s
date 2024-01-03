Disassembly of section .text:
80000000 <_start>:
80000000:       00200a13    addi    x20,x0,2
80000004:       00000097    auipc   x1,0x0
80000008:       03c08093    addi    x1,x1,60 # 80000040 <_x>
8000000c <loop>:
8000000c:       0000a103    lw      x2,0(x1)
80000010:       002f8fb3    add     x31,x31,x2
80000014:       0040a103    lw      x2,4(x1)
80000018:       002f8fb3    add     x31,x31,x2
8000001c:       0080a103    lw      x2,8(x1)
80000020:       002f8fb3    add     x31,x31,x2
80000024:       00c0a103    lw      x2,12(x1)
80000028:       002f8fb3    add     x31,x31,x2
8000002c:       01008093    addi    x1,x1,16
80000030:       fffa0a13    addi    x20,x20,-1
80000034:       fc0a1ce3    bne     x20,x0,8000000c <loop>
80000038:       001f8f93    addi    x31,x31,1
8000003c <forever>:
8000003c:       0000006f    jal     x0,8000003c <forever>

Disassembly of section .data:
80000040 <_x>:
80000040:       0001        c.addi  x0,0
80000042:       0000        unimp
80000044:       0002        0x2
80000046:       0000        unimp
80000048:       00000003    lb      x0,0(x0) # 0 <elem_sz-0x4>
8000004c:       0004        c.addi4spn      x9,x2,0
8000004e:       0000        unimp
80000050:       0005        c.addi  x0,1
80000052:       0000        unimp
80000054:       0006        0x6
80000056:       0000        unimp
80000058:       00000007    0x7
8000005c:       0008        c.addi4spn      x10,x2,0
