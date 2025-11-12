#include "../Catalog.h"

#include "gtest/gtest.h"
//
// Created by ervin on 11/9/25.
//
TEST(CatalogTests, shouldAddTableToCatalog) {
    Catalog *catalog = new Catalog();
    Table *studenti = new Table(5, "STUDENTI");
    studenti->setColumn(0, "CNP");
    studenti->setColumn(1, "Nume");
    studenti->setColumn(2, "Varsta");
    studenti->setColumn(3, "Adresa");
    studenti->setColumn(4, "An");
    std::string student1[] = {"CNP1", "Popescu Ion", "21", "Str. Libertatii 1", "3"};
    std::string student2[] = {"CNP2", "Dana", "22", "Str. Libertatii 5", "2"};
    std::string student3[] = {"CNP3", "Claudia Ioana Maria", "23", "Str. Independentei 5", "1"};
    studenti->addRow(student1);
    studenti->addRow(student2);
    studenti->addRow(student3);
    studenti->addRow(student3);
    catalog->add_table(*studenti);
    catalog->add_table(*studenti);
    EXPECT_EQ(catalog->getNoOfTables(), 2);
    delete catalog;
    delete studenti;
}
