#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>

class Buffer {
public:
    typedef union {
        long* l;
        unsigned long* ul;
        long long* ll;
        unsigned long long* ull;
        float* f;
        double* d;
        char* s;
    } BUFTYPE;
private:
    BUFTYPE buf;
    int type = 0;
    DWORD bufSize = 0;
public:
    Buffer(int type) {
        this->type = type;
        switch (type) {
        case 1:
            bufSize = sizeof(long);
            break;
        case 2:
            bufSize = sizeof(unsigned long);
            break;
        case 3:
            bufSize = sizeof(long long);
            break;
        case 4:
            bufSize = sizeof(unsigned long long);
            break;
        case 5:
            bufSize = sizeof(float);
            break;
        case 6:
            bufSize = sizeof(double);
            break;
        }
        this->buf.s = (PCHAR)malloc(bufSize);
    }
    Buffer(int type, DWORD bufSize) {
        this->type = 0;
        this->bufSize = bufSize;
        this->buf.s = (PCHAR)malloc(bufSize);
    }
    ~Buffer() {
        if (buf.s) {
            free(buf.s);
        }
    }
    void* getPointer() {
        return buf.s;
    }
    BUFTYPE getBuffer() {
        return buf;
    }
    DWORD getSize() {
        return bufSize;
    }
    static Buffer* createFromStdin() {
        int _type = -1;
        DWORD _size;
        do {
            std::cout << "Please enter the type of memory to be accessed (0 = string, 1 = long, 2 = unsigned long, 3 = long long, 4 = unsigned long long, 5 = float, 6 = double): ";
            scanf("%d", &_type);
            if (_type == 0) {
                std::cout << "Please enter the size of string: ";
                scanf("%lu", &_size);
            } else if (_type < 0 || _type > 6) {
                std::cout << "Invalid type\n";
                _type = -1;
            }
        } while (_type == -1);
        Buffer* buf;
        if (_type == 0)
            buf = new Buffer(0, _size);
        else
            buf = new Buffer(_type);
        return buf;
    }
    void input() {
        bool isHex;
        bool sign = type == 2 || type == 4;
        if (type > 0 && type < 5) { // 1, 2, 3, 4 integer
            int valType = -1;
            do {
                std::cout << "What kind of value did you want to enter? (0 = dec / base 10, 1 = hex): ";
                scanf("%d", &valType);
                switch (valType) {
                case 0:
                case 1:
                    isHex = valType;
                    break;
                default:
                    std::cout << "Invalid value type\n"; valType = -1;
                    break;
                }
            } while (valType == -1);
            if (isHex) {
                std::cout << "Please enter the value to be stored (in hex): ";
            } else {
                if (sign) {
                    std::cout << "Please enter the value to be stored (in integer): ";
                } else {
                    std::cout << "Please enter the value to be stored (in positive integer / >= 0): ";
                }
            }
        } else if (type >= 5) { // 5 and 6, decimal number
            std::cout << "Please enter the value to be stored (in decimal, e.g. 0.21): ";
        } else { // 0, string
            std::cout << "Please enter the value to be stored (in string): ";
        }
        switch(type) {
        case 1:
        case 2:
            if (isHex) scanf("%lx", buf.l);
            else if (sign) scanf("%ld", buf.l);
            else scanf("%lu", buf.l);
            break;
        case 3:
        case 4:
            if (isHex) scanf("%llx", buf.ll);
            else if (sign) scanf("%lld", buf.ll);
            else scanf("%llu", buf.ll);
            break;
        case 5:
            scanf("%f", buf.f);
            break;
        case 6:
            scanf("%lf", buf.d);
            break;
        default:
            int c;
            scanf("%*[\n]");
            fgets(buf.s, bufSize, stdin);
            while ((c = getchar()) != EOF && c != '\n'); // discard up to and including next newline
            buf.s[strcspn(buf.s, "\r\n")] = '\0'; // discard any trailing EOL seq
            break;
        }
    }
    void output(bool isHex=true) {
        if (isHex && type > 0 && type < 5) {
            if (type > 0 && type < 3) printf("0x%lx", *buf.l);
            else printf("0x%llx", *buf.ll);
        } else {
            switch(type) {
            case 1:
                printf("%ld", *buf.l);
                break;
            case 2:
                printf("%lu", *buf.ul);
                break;
            case 3:
                printf("%lld", *buf.ll);
                break;
            case 4:
                printf("%llu", *buf.ull);
                break;
            case 5:
                printf("%f", *buf.f);
                break;
            case 6:
                printf("%lf", *buf.d);
                break;
            default:
                printf("%.*s", bufSize, buf.s);
                break;
            }
        }
    }
};

class Process {
private:\
    DWORD pid;
    ULONG_PTR addressMem;
public:
    Process(DWORD pid) {
        this->pid = pid;
    }
    DWORD getPid() {
        return pid;
    }
    void setPid(DWORD pid) {
        this->pid = pid;
    }
    void inputMemAddr() {
        std::cout << "Please enter the memory address to access (in hex): ";
        scanf("%x", &addressMem);
    }
    void readMem() {
        HANDLE phandle = OpenProcess(PROCESS_VM_READ, FALSE, pid);
        if (!phandle) {
            std::cout << "Error opening process" << std::endl;
            return;
        }
        inputMemAddr();
        Buffer* buf = Buffer::createFromStdin();
        SIZE_T readSize;
        if (
            !ReadProcessMemory(
                phandle,(PVOID)addressMem, buf->getPointer(), buf->getSize(), &readSize
            ) || readSize != buf->getSize()
        ) {
            printf(
                "Error reading %lu bytes memory at address 0x%p in process PID %lu: 0x%x\n",
                buf->getSize(), (PVOID)addressMem, pid, GetLastError()
            );
            delete buf;
            CloseHandle(phandle);
            return;
        }
        printf("Memory at address 0x%p in process PID %lu is '", (PVOID)addressMem, pid);
        buf->output(true);
        printf("' (hex) '");
        buf->output(false);
        printf("' (dec) '%.*s' (string) [%lu bytes]\n", buf->getSize(), buf->getBuffer().s, buf->getSize());
        delete buf;
        CloseHandle(phandle);
    }
    void writeMem() {
        HANDLE phandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid);
        if (!phandle) {
            std::cout << "Error opening process" << std::endl;
            return;
        }
        inputMemAddr();
        Buffer* buf = Buffer::createFromStdin();
        buf->input();
        SIZE_T writeSize;
        if (
            !WriteProcessMemory(
                phandle, (PVOID)addressMem, buf->getPointer(), buf->getSize(), &writeSize
            ) || writeSize != buf->getSize()
        ) {
            printf("Error writing %lu bytes memory at address 0x%p in process PID %lu: 0x%x\n",
                buf->getSize(), (PVOID)addressMem, pid, GetLastError());
            delete buf;
            CloseHandle(phandle);
            return;
        }
        printf("Successfully writing %lu bytes memory at address 0x%p in process PID %lu\n",
            buf->getSize(), (PVOID)addressMem, pid);
        delete buf;
        CloseHandle(phandle);
    }
    void printName() {
        TCHAR pname[MAX_PATH] = TEXT("<unknown>");
        HANDLE phandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (phandle != NULL) {
            HMODULE pmod;
            DWORD temp;
            if (EnumProcessModules(phandle, &pmod, sizeof(pmod), &temp))
                GetModuleBaseName(phandle, pmod, pname, sizeof(pname) / sizeof(TCHAR));
        }
        _tprintf(TEXT("%s (PID %u)\n"), pname, pid);
        CloseHandle(phandle);
    }
    static void printAllProcess() {
        std::cout << "Listing processess\n";
        DWORD ps_list[1024], ps_memctr, ps_ctr;
        if (!EnumProcesses(ps_list, sizeof(ps_list), &ps_memctr))
            return;
        ps_ctr = ps_memctr / sizeof(DWORD);
        for (int i = 0; i < ps_ctr; i++)
            if (ps_list[i] != 0)
                Process(ps_list[i]).printName();
    }
};

Process proc(0);
void printMenu() {
    std::cout << "Memory Editor Menu" << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "1. Print process list" << std::endl;
    std::cout << "2. Set process to edit" << std::endl;
    std::cout << "3. Read memory" << std::endl;
    std::cout << "4. Write memory" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "------------------" << std::endl;
    if (proc.getPid() != 0) {
        std::cout << "You are now editing ";
        proc.printName();
        std::cout << "------------------" << std::endl;
    }
    std::cout << "Please enter your mode: ";
}

int main() {
    int mode = 0;
    int pid;
    do {
        printMenu();
        scanf("%d", &mode);
        switch (mode) {
            case 0:
                break;
            case 1:
                Process::printAllProcess();
                break;
            case 2:
                std::cout << "\nPlease enter your PID: ";
                scanf("%d", &pid);
                proc.setPid(pid);
                break;
            case 3:
                proc.readMem();
                break;
            case 4:
                proc.writeMem();
                break;
            default:
                std::cout << "Unrecognized mode!\n";
                break;
        }
    } while (mode != 0);
    std::cout << "Goodbye!" << std::endl;
    return 0;
}