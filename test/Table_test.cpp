//
// Created by ervin on 11/9/25.
//
#include "../Table.h"

#include <assert.h>

void should_add_row() {
    Table *studenti = new Table(5, "studenti");
    studenti->setColumn(0, "CNP");
    studenti->setColumn(1, "Nume");
    studenti->setColumn(2, "Varsta");
    studenti->setColumn(3, "Adresa");
    studenti->setColumn(4, "An");
    std::string student1[] = {"CNP1", "Popescu Ion", "21", "Str. Libertății 10", "3"};
    std::string student2[] = {"CNP2", "Claudia", "22", "Str. Libertății 5", "2"};
    studenti->addRow(student1);
    studenti->addRow(student2);
    assert(studenti->getNoOfRows() == 2);
    assert(studenti->getNoOfColumns() == 5);
    studenti->print_table();
    delete studenti;
}

int main() {
    should_add_row();
}
