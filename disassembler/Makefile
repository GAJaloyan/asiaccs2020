all: disasm

disasm: disasm.cpp
	g++ disasm.cpp riscv-disas.cpp -std=gnu++14 -o disasm
	./disasm ../experiments/fedora_hifiveu/fedora.out

clean:
	rm -f disasm
