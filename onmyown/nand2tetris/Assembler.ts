import * as fs from "fs";
import * as path from "path";

const programName = "Assembler.js"
const DEBUG = false;

type TableEntry = {
    [key: string]: number;
};

type Instruction =
  | { type: "A"; line: number, symbol: string }
  | { type: "C"; line: number, dest: string; comp: string; jump: string }
  | { type: "L"; line: number, label: string };

function printUsage(): void {
    console.log(`Usage: node ${programName} file`)
}

function err(line: number, type: string, msg: string): never {
    throw Error(`Line ${line} ${type} Error: ${msg}`);
}

function changeExtension(filePath: string, newExt: string): string {
    const dir = path.dirname(filePath);
    const base = path.basename(filePath, path.extname(filePath));
    return path.join(dir, base + newExt);
}

class Code {
    static dest_tbl: TableEntry = {
        "":    0b000,
        "M":   0b001,
        "D":   0b010,
        "MD":  0b011,
        "A":   0b100,
        "AM":  0b101,
        "AD":  0b110,
        "AMD": 0b111,
    }
    static jump_tbl: TableEntry = {
        "":    0b000,
        "JGT": 0b001,
        "JEQ": 0b010,
        "JGE": 0b011,
        "JLT": 0b100,
        "JNE": 0b101,
        "JLE": 0b110,
        "JMP": 0b111,
    }
    static comp_tbl: TableEntry = {
        "0":   0b0101010,
        "1":   0b0111111,
        "-1":  0b0111010,
        "D":   0b0001100,
        "A":   0b0110000,
        "!D":  0b0001101,
        "!A":  0b0110001,
        "-D":  0b0001111,
        "-A":  0b0110011,
        "D+1": 0b0011111,
        "A+1": 0b0110111,
        "D-1": 0b0001110,
        "A-1": 0b0110010,
        "D+A": 0b0000010,
        "D-A": 0b0010011,
        "A-D": 0b0000111,
        "D&A": 0b0000000,
        "D|A": 0b0010101,
        "M":   0b1110000,
        "!M":  0b1110001,
        "-M":  0b1110011,
        "M+1": 0b1110111,
        "M-1": 0b1110010,
        "D+M": 0b1000010,
        "D-M": 0b1010011,
        "M-D": 0b1000111,
        "D&M": 0b1000000,
        "D|M": 0b1010101
    }
    static dest(str: string): number {
        if (!(str in this.dest_tbl)) {
            throw Error(`invalid destination \"${str}\"`);
        } else {
            return this.dest_tbl[str];
        }
    }
    static jump(str: string): number {
        if (!(str in this.jump_tbl)) {
            throw Error(`invalid jump \"${str}\"`);
        } else {
            return this.jump_tbl[str];
        }
    }
    static comp(str: string): number {
        if (!(str in this.comp_tbl)) {
            throw Error(`invalid computation \"${str}\"`);
        } else {
            return this.comp_tbl[str];
        }
    }
}

class SymbolTable {
    table: TableEntry = {
        R0: 0,  R1: 1,  R2: 2,  R3: 3,  R4: 4,
        R5: 5,  R6: 6,  R7: 7,  R8: 8,  R9: 9,
        R10: 10, R11: 11, R12: 12, R13: 13, R14: 14, R15: 15,
        SP: 0, LCL: 1, ARG: 2, THIS: 3, THAT: 4,
        SCREEN: 16384, KBD: 24576
    };
    addEntry(symbol: string, address: number): void {
        this.table[symbol] = address;
    }
    contains(symbol: string): boolean {
        return symbol in this.table;
    }
    getAddress(symbol:string): number {
        return this.table[symbol];
    }
}

class Parser {
    lines:    string[] = [];
    instructions: Instruction[] = [];
    currLine: number   = 0;
    currInst: Instruction = {type: "A", line: 0, symbol: ""};

    constructor(lines: string[]) {
        this.lines = lines;
    }

    static isValidSymbol(str: string): boolean {
        return /^[A-Za-z_.$:][A-Za-z0-9_.$:]*$/.test(str);
    }

    static isValidConstant(str: string): boolean {
        return /^[0-9]+$/.test(str);
    }

    hasMoreCommands(): boolean {
        return this.currLine < this.lines.length;
    }

    static normalizeLine(l: string): string {
        return l.split("//", 2)[0].replace(/\s+/g, "");
    }

    advance(): void {
        while (!this.lines[this.currLine]) {
            this.currLine++;
            if (!this.hasMoreCommands()) return;
        }
        const l = this.lines[this.currLine];
        let instr: Instruction;
        this.currLine++;
        const hasAt: boolean = l.startsWith("@");
        const hasEq: boolean = l.includes("=");
        const hasSc: boolean = l.includes(";");

        if (l.startsWith("(") && l.endsWith(")")) {
            instr = {
                type: "L",
                line: this.currLine,
                label: l.slice(1, -1)
            }
        } else if (hasAt) {
            instr = {
                type: "A",
                line: this.currLine,
                symbol: l.split("@", 2)[1]
            }
        } else if (hasEq && hasSc) {
            const destComp = l.split("=", 2);
            if (!destComp[1].includes(";")) {
                throw Error(`invalid command \"${l}\"`);
            }
            const [comp, jump] = destComp[1].split(";", 2);
            instr = {
                type: "C",
                line: this.currLine,
                dest: destComp[0],
                comp: comp,
                jump: jump
            }
        } else if (hasEq) {
            const [dest, comp] = l.split("=", 2);
            instr = {
                type: "C",
                line: this.currLine,
                dest: dest,
                comp: comp,
                jump: ""
            }
        } else if (hasSc) {
            const [comp, jump] = l.split(";", 2);
            instr = {
                type: "C",
                line: this.currLine,
                dest: "",
                comp: comp,
                jump: jump
            }
        } else {
            instr = {
                type: "C",
                line: this.currLine,
                dest: "",
                comp: l,
                jump: ""
            }
        }

        if (DEBUG) {
            console.log(`${this.currLine} line Command: ${l}`);
            console.log(`   type: ${instr.type}, instr:`, instr);
            console.log(`   hasAt: ${hasAt}, hasEq: ${hasEq}, hasSc: ${hasSc}`);
        }

        if (instr.type === "A" && !instr.symbol) {
            throw Error(`missing symbol/constant \"${l}\"`);
        } else if (instr.type === "C") {
            if (!instr.comp) {
                throw Error(`missing computation \"${l}\"`);
            } else if (hasEq && !instr.dest) {
                throw Error(`missing destination \"${l}\"`);
            } else if (hasSc && !instr.jump) {
                throw Error(`missing jump\"${l}\"`);
            }
        } else if (instr.type === "L" && !instr.label) {
            throw Error(`missing label \"${l}\"`);
        }

        this.instructions.push(instr);
    }
}

function assemble(lines: string[]): string[] {
    try {
        lines = lines.map(l => Parser.normalizeLine(l));
        const p: Parser = new Parser(lines);
        const s: SymbolTable = new SymbolTable();
        let variableAddress = 16;
        let commandAddress = 0;
        let commands: number[] = [];

        while (p.hasMoreCommands()) {
            try {
                p.advance();
            } catch (e: any) {
                err(p.currLine, "Parse", e.message)
            }
        }

        for (const i of p.instructions) {
            if (i.type === "L") {
                if (Parser.isValidSymbol(i.label)) {
                    if (s.contains(i.label)) {
                        err(i.line, "Label", `label redefinition \"${i.label}\"`);
                    }
                    s.addEntry(i.label, commandAddress);
                } else {
                    err(i.line, "Label", `invalid label \"${i.label}\"`);
                }
            } else commandAddress++;
        }

        commandAddress = 0;
        for (const i of p.instructions) {
            if (i.type === "A") {
                let constant: number;
                if (Parser.isValidConstant(i.symbol)) {
                    constant = Number(i.symbol);
                } else if (Parser.isValidSymbol(i.symbol)) {
                    if (s.contains(i.symbol)) {
                        constant = s.getAddress(i.symbol);
                    } else {
                        constant = variableAddress;
                        s.addEntry(i.symbol, variableAddress++);
                    }
                } else {
                    err(i.line, "Command A",
                        `invalid symbol/constant \"${i.symbol}\"`);
                }
                if (constant > 0x7FFF) {
                    err(i.line, "Command A",
                        `value out of range \"${i.symbol}\" - \"${constant}\"`);
                }
                commands.push(constant);
                commandAddress++;
            } else if (i.type === "C") {
                try {
                    const dest = Code.dest(i.dest);
                    const comp = Code.comp(i.comp);
                    const jump = Code.jump(i.jump);
                    const cmd =
                        (0b111 << 13) |
                        (comp << 6)   |
                        (dest << 3)   |
                        jump;
                    commands.push(cmd & 0xFFFF);
                } catch (e: any) {
                    err(i.line, "Command C", e.message)
                }
                commandAddress++;
            }
        }

        const instructions = commands.map(num => {
            return num.toString(2).padStart(16, '0');
        });

        return instructions;
    } catch (e: unknown) {
        if (e instanceof Error) {
            console.error(e.message);
        } else {
            console.error("Non-Error thrown:", e);
        }
        process.exit(1);
    }
}

if (!process.argv[2]) {
    printUsage();
    process.exit(0);
}

const filePath: string = process.argv[2];
const outputFile = path.basename(changeExtension(filePath, ".hack"));

const lines = fs.readFileSync(filePath, "utf8").split("\n");
const instructions = assemble(lines);
fs.writeFileSync(outputFile, instructions.join('\n'), 'utf8');
