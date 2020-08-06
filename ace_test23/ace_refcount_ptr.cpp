#include <stdio.h>
#include <string>

#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/Refcounted_Auto_Ptr.h"

class Printer {
 public:
    Printer(std::string name);
    ~Printer();

    void print(std::string str);

 private:
    std::string m_name;
};

Printer::Printer(std::string name) :
    m_name(name) {
    printf("Printer::Printer %s\n", m_name.c_str());
}

Printer::~Printer() {
    printf("Printer::~Printer()\n");
}

void Printer::print(std::string str) {
    printf("Printer::print %s\n", str.c_str());
}

typedef ACE_Refcounted_Auto_Ptr <Printer, ACE_Thread_Mutex> PrinterRef;

void test_printer(const PrinterRef& p_ref, std::string str);
void test_printer1(const PrinterRef p_ref, std::string str);

void test_printer(const PrinterRef& p_ref, std::string str) {
    // reference count still 0 after reference
    printf("printer_ref count:1 %d\n", p_ref.count());

    PrinterRef p_ref1(p_ref);
    // reference count should be 1 (+1) after reference
    printf("printer_ref count:2 %d\n", p_ref1.count());

    // "->" will redirect to printer->print(std::string str)
    p_ref1->print(str);

    // p_ref and p_ref1 will be freed here and count - 2 = 0
    return;
}

void test_printer1(PrinterRef p_ref, std::string str) {
    // reference count should be 1 (+1) after let
    printf("printer_ref count:4 %d\n", p_ref.count());

    PrinterRef p_ref1 = p_ref;
    // reference count should be 2 (+1) after "="
    printf("printer_ref count:5 %d\n", p_ref1.count());

    // "->" will redirect to printer->print(std::string str)
    p_ref1->print(str);

    // p_ref and p_ref1 will be freed here and count - 2 = 0
    return;
}

int main() {
    Printer* printer = new Printer(std::string("MyPrinter"));

    // wrap with ref count pointer
    PrinterRef printer_ref(printer);
    // print the raw pointer
    printf("printer ptr value 0x%x\n", printer);

    // printer_ref pointer should be equal to printer
    printf("printer_ref value 0x%x\n", printer_ref.get());
    // the count should be 0 now when initialize with raw pointer
    printf("printer_ref count:0 %d\n", printer_ref.count());

    test_printer(printer_ref, std::string("test printer"));
    // the count should be 0 now
    printf("printer_ref count:3 %d\n", printer_ref.count());

    test_printer1(printer_ref, std::string("test printer1"));

    // don't need to call delete and the printer_ref will delete the printer by self because the count value is 0
    printf("printer_ref count:6 %d\n", printer_ref.count());
}

/*
 * root@wihartgw:~# ./ace_refcount_ptr.out
 * Printer::Printer MyPrinter
 * printer ptr value 0x2b3d0
 * printer_ref value 0x2b3d0
 * printer_ref count:0 0
 * printer_ref count:1 0
 * printer_ref count:2 1
 * Printer::print test printer
 * printer_ref count:3 0
 * printer_ref count:4 1
 * printer_ref count:5 2
 * Printer::print test printer1
 * printer_ref count:6 0
 * Printer::~Printer()
*/
