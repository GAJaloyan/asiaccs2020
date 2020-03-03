#include <assert.h>
#include <vector>
#include <unordered_map>
#include <stack>
#include <set>
#include "riscv-disas.h"
#include "rvelf.h"

using namespace std;

typedef struct
{
    uint64_t addr;
    uint64_t inst;
    rv_decode dec;
    uint64_t len;
    bool jump;
    bool conditional_jump;
    bool indirect_jump;
    bool reachable;
    bool printed;
    bool call;
} node;

unordered_map <uint64_t, node> nodes; //each (valid) instruction is a node
unordered_map <uint64_t, set<uint64_t> > edges; //each edge is a possible succession
unordered_map <uint64_t, set<uint64_t> > redges; //reverse edges
set <uint64_t> indirect_jumps; //list of indirect jumps
stack<node> printage;


/* disassemble instruction */

void disasm_inst(char *buff, uint64_t pc, uint64_t base)
{
    int64_t* inst_loc = (int64_t*) (buff+pc-base);
    int64_t inst = *inst_loc;
    rv_decode dec = { 0 };
    dec.pc = pc;
    dec.inst = inst;
    decode_inst_opcode(&dec, rv64);
    decode_inst_operands(&dec);
    decompress_inst_rv64(&dec); 
    decode_inst_lift_pseudo(&dec);
    
    if(dec.op == rv_op_illegal)
      return;
  
    /* Filter out all of those useless instructions */
  
    node elem;
    elem.addr = pc;
    elem.inst = inst;
    elem.dec = dec;
    elem.len = inst_length(inst);
    elem.reachable = false;
    elem.jump = false;
    elem.indirect_jump = false;
    elem.conditional_jump = false;
    elem.printed = false;
    elem.call = false;
    
    if(dec.op == rv_op_jalr ||
       dec.op == rv_op_jr ||
       dec.op == rv_op_beq ||
       dec.op == rv_op_bne ||
       dec.op == rv_op_bnez ||
       dec.op == rv_op_blez ||
       dec.op == rv_op_bgez ||
       dec.op == rv_op_beqz ||
       dec.op == rv_op_bltz ||
       dec.op == rv_op_bgtz ||
       dec.op == rv_op_ble ||
       dec.op == rv_op_bleu ||
       dec.op == rv_op_bgt ||
       dec.op == rv_op_bgtu ||
       dec.op == rv_op_blt ||
       dec.op == rv_op_bge ||
       dec.op == rv_op_bltu ||
       dec.op == rv_op_bgeu ||
       dec.op == rv_op_c_beqz ||
       dec.op == rv_op_c_bnez ||
       dec.op == rv_op_c_jalr ||
       dec.op == rv_op_ret ||
       dec.op == rv_op_jal ||
       dec.op == rv_op_j ||
       dec.op == rv_op_c_jal ||
       dec.op == rv_op_ecall ||
       dec.op == rv_op_c_j)
    {
        elem.jump = true;

        if(dec.op == rv_op_beq ||
           dec.op == rv_op_bne ||
           dec.op == rv_op_bnez ||
           dec.op == rv_op_blez ||
           dec.op == rv_op_bgez ||
           dec.op == rv_op_beqz ||
           dec.op == rv_op_bltz ||
           dec.op == rv_op_bgtz ||
           dec.op == rv_op_ble ||
           dec.op == rv_op_bleu ||
           dec.op == rv_op_bgt ||
           dec.op == rv_op_bgtu ||
           dec.op == rv_op_blt ||
           dec.op == rv_op_bge ||
           dec.op == rv_op_bltu ||
           dec.op == rv_op_bgeu ||
           dec.op == rv_op_c_beqz ||
           dec.op == rv_op_c_bnez)
        {
            elem.conditional_jump = true;
        }
        else if(dec.op == rv_op_ret ||
                dec.op == rv_op_jr ||
                dec.op == rv_op_jalr ||
                dec.op == rv_op_c_jalr ||
                dec.op == rv_op_ecall)
        {
            elem.indirect_jump = true;
            indirect_jumps.insert(pc);
        }

	if(dec.op == rv_op_jal ||
	   dec.op == rv_op_c_jal)
        {
	    elem.call = true;
	}

    }
    set<uint64_t> next;
    nodes[pc] = elem;
    if(elem.jump)
    {
        if(elem.indirect_jump)
        {
            //do nothing
        }
        else if(elem.conditional_jump)
        {
            next.insert(pc+elem.len);
            next.insert(pc+elem.dec.imm);
        }
        else if(elem.call)
	{
	    next.insert(pc+elem.len);
	}
	else
        {
            next.insert(pc+elem.dec.imm);
        }
	
    }
    else
    {
        next.insert(pc+elem.len);
    }

    for(auto f: next)
    {
        edges[pc].insert(f);
        redges[f].insert(pc);
    }
}

void mark_reachable(uint64_t pc)
{
    if(nodes.find(pc) == nodes.end())
        return;
    if(nodes[pc].reachable == true)
        return;
    
    nodes[pc].reachable = true;
    for(auto e: redges[pc])
    {
        mark_reachable(e);
    }
}

void add_to_printage(uint64_t pc)
{
    if(nodes.find(pc) == nodes.end())
        return;
    if(nodes[pc].printed == true)
        return;    
    nodes[pc].printed = true;
    printage.push(nodes[pc]);
    for(auto e: redges[pc])
    {
        add_to_printage(e);
    }
}

void simplify_edges(void)
{
    for(auto s: indirect_jumps)
    {
        mark_reachable(s);
    }
    for(auto n: nodes)
    {
        if (n.second.reachable == false)
        {
            nodes.erase(n.first);
            edges.erase(n.first);
        }
    }
    for(auto e: redges)
    {
        if(nodes.find(e.first) != nodes.end())
        {
            for(auto f: e.second)
                if(nodes.find(f) == nodes.end())
                    redges[e.first].erase(f);
        }
        else
        {
            redges.erase(e.first);
        }
    }
    for(auto e: edges)
    {
        if(nodes.find(e.first) != nodes.end())
            for(auto f: e.second)
            {
                if(nodes.find(f) == nodes.end())
                    edges[e.first].erase(f);
            }
        else
        {
            edges.erase(e.first);
        }
    }
}

void print_graph(void)
{
    // PRINT THE OUTPUT
    printf("digraph {\n");
    while(!printage.empty())
    {
        node elem = printage.top();
        printage.pop();
        uint64_t nodeid = elem.addr;
        
        printf("%lu [fontname=\"monospace\", shape=box, label=\"",nodeid);

    label:
        char buf[128];
        size_t buflen = 128;
        format_inst(buf, buflen, 32, &elem.dec);
        printf("%lx: %s\\l", elem.addr, buf);

        if(edges[elem.addr].size() == 1)
        {
            for(auto nn: edges[elem.addr])
            {
                //one turn of loop only
                if(redges[nn].size() == 1)
                {
                    elem = printage.top();
                    printage.pop();
                    goto label;
                }
            }
        }

        printf("\"");
        int nbedges = edges[elem.addr].size();
        if(nbedges == 0)
            printf(",style=filled, fillcolor=lightyellow");
        printf("]; \n");
        for(auto e: edges[elem.addr])
        {
            printf("%lu -> %lu [color=\"",nodeid,e);
            switch(nbedges)
            {
            case 1:
                printf("blue"); break;
            case 2:
                printf("red");
                nbedges = -2;
                break;
            case -2:
                printf("green4"); break;
            default:
                printf("%d", nbedges);
                exit(-1);
            }
            printf("\"];\n");
        }
        
    }
    printf("}\n");
}

int main(int argc, char **argv)
{
    assert(argc == 2);
    FILE *fp = fopen(argv[1], "rb");
    assert(fp != NULL);

    // TODO : get the executable sections.
    Elf64_Ehdr eh;
    Elf64_Shdr sh;
    fread((void *) &eh, sizeof(char), sizeof(Elf64_Ehdr), fp);

    assert(eh.e_ident[0] == ELFMAG0 && eh.e_ident[1] == ELFMAG1 && eh.e_ident[2] == ELFMAG2 && eh.e_ident[3] == ELFMAG3);
    assert(sizeof(Elf64_Shdr) == eh.e_shentsize);
    
    fseek(fp, (size_t) eh.e_shoff, SEEK_SET);
    for(size_t i = 0; i < eh.e_shnum; i++)
    {
        fread((void*) &sh, sizeof(char), eh.e_shentsize, fp);
        //do the stuff here.
        if(sh.sh_flags & SHF_EXECINSTR)
        {            
            uint64_t current = ftell(fp);
            fseek(fp, sh.sh_offset, SEEK_SET);
            
            uint64_t sz = sh.sh_size;
            char *buff = (char*) calloc(sz+42, 1);
            fread(buff, sizeof(char), sz+2, fp);
                
            for (uint64_t i = 0; i < sz;)
            {
                disasm_inst(buff, i + sh.sh_offset, sh.sh_offset);
                i+=2;
            }
            fseek(fp, current, SEEK_SET);
        }
    }
    simplify_edges();

    for(auto s: indirect_jumps)
    {
        add_to_printage(s);
    }
    char filename[200];
    sprintf(filename,"graph.dot");
    freopen(filename, "w", stdout);
    print_graph();

    return 0;
}
