import * as fs from "fs";
import * as path from 'path';

type TableEntry = {
    [key: string]: number;
};

enum CmdType {
    EMPTY,
    A_COMMAND,
    C_COMMAND,
    L_COMMAND
}

const programName = "Assembler.js"

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

if (!process.argv[2]) {
    printUsage();
    process.exit(1);
}

const filePath: string = process.argv[2];
let outputFile: string;

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
    srcFile:  string;
    lines:    string[] = [];
    currLine: number   = 0;
    cmdType:  CmdType  = CmdType.EMPTY;
    symbol:   string   = "";
    dest:     string   = "";
    comp:     string   = "";
    jump:     string   = "";

    constructor(sourceFile: string) {
        this.srcFile = sourceFile;
        const data = fs.readFileSync(filePath, 'utf8');
        this.lines = data.split("\n");
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

    advance(): void {
        const l = this.lines[this.currLine]
                  .split("//", 2)[0]
                  .replace(/\s+/g, "");
        this.currLine++;
        const hasAt: boolean = l.startsWith("@");
        const hasEq: boolean = l.includes("=");
        const hasSc: boolean = l.includes(";");
        if (!l) {
            this.symbol  = "";
            this.dest    = "";
            this.comp    = "";
            this.jump    = "";
            this.cmdType = CmdType.EMPTY;
        } else if (l.startsWith("(") && l.endsWith(")")) {
            this.symbol  = l.slice(1, -1);
            this.dest    = "";
            this.comp    = "";
            this.jump    = "";
            this.cmdType = CmdType.L_COMMAND;
        } else if (hasAt) {
            this.symbol  = l.split("@", 2)[1];
            this.dest    = "";
            this.comp    = "";
            this.jump    = "";
            this.cmdType = CmdType.A_COMMAND;
        } else if (hasEq && hasSc) {
            const destComp = l.split("=", 2);
            if (!destComp[1].includes(";")) {
                throw Error(`invalid command \"${l}\"`);
            }
            this.symbol  = "";
            this.dest    = destComp[0];
            this.comp    = destComp[1].split(";", 2)[0];
            this.jump    = destComp[1].split(";", 2)[1];
            this.cmdType = CmdType.C_COMMAND;
        } else if (hasEq) {
            this.symbol  = "";
            this.dest    = l.split("=", 2)[0];
            this.comp    = l.split("=", 2)[1];
            this.jump    = "";
            this.cmdType = CmdType.C_COMMAND;
        } else if (hasSc) {
            this.symbol  = "";
            this.dest    = "";
            this.comp    = l.split(";", 2)[0];
            this.jump    = l.split(";", 2)[1];
            this.cmdType = CmdType.C_COMMAND;
        } else {
            this.symbol  = "";
            this.dest    = "";
            this.comp    = l;
            this.jump    = "";
            this.cmdType = CmdType.C_COMMAND;
        }
    }
}

try {
    const p: Parser = new Parser(filePath);
    const s: SymbolTable = new SymbolTable();
    outputFile = path.basename(changeExtension(filePath, ".hack"));
    let nextAddress = 16;
    let cmdCount = 0;
    let commands: number[] = [];

    while (p.hasMoreCommands()) {
        try {
            p.advance();
        } catch (e: any) {
            err(p.currLine, "Parse", e.message)
        }
        switch (p.cmdType) {
            case CmdType.EMPTY:
                break;
            case CmdType.L_COMMAND: {
                if (Parser.isValidSymbol(p.symbol)) {
                    if (s.contains(p.symbol)) {
                        err(p.currLine, "Symbol", `symbol redefinition \"${p.symbol}\"`);
                    }
                    s.addEntry(p.symbol, cmdCount);
                } else {
                    err(p.currLine, "Symbol", `invalid symbol \"${p.symbol}\"`);
                }
                break;
            }
            default:
                cmdCount++;
                break;
        }
    }

    cmdCount = 0;
    p.currLine = 0;
    while (p.hasMoreCommands()) {
        try {
            p.advance();
        } catch (e: any) {
            err(p.currLine, "Parse", e.message)
        }
        switch (p.cmdType) {
            case CmdType.EMPTY:
                break;
            case CmdType.A_COMMAND: {
                let constant: number;
                if (Parser.isValidConstant(p.symbol)) {
                    constant = Number(p.symbol);
                } else if (Parser.isValidSymbol(p.symbol)) {
                    if (s.contains(p.symbol)) {
                        constant = s.getAddress(p.symbol);
                    } else {
                        constant = nextAddress;
                        s.addEntry(p.symbol, nextAddress++);
                    }
                } else {
                    err(p.currLine, "Command A", `invalid symbol/constant \"${p.symbol}\"`);
                }

                constant = constant & 0x7FFF;
                commands.push(constant);
                cmdCount++;
                break;
            }
            case CmdType.C_COMMAND: {
                try {
                    const dest = Code.dest(p.dest) << 3;
                    const comp = Code.comp(p.comp) << 6;
                    const jump = Code.jump(p.jump);
                    const cmd  = ((0b111 << 13) | dest | comp | jump) & 0xFFFF;
                    commands.push(cmd);
                } catch (e: any) {
                    err(p.currLine, "Command C", e.message)
                }
                cmdCount++;
                break;
            }
            default:
                break;
        }
    }

    const lines = commands.map(num => {
        return num.toString(2).padStart(16, '0');
    });

    fs.writeFileSync(outputFile, lines.join('\n'), 'utf8');
} catch (e: unknown) {
    if (e instanceof Error) {
        console.error(e.message);
    } else {
        console.error("Non-Error thrown:", e);
    }
    process.exit(1);
}
