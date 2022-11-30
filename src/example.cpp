#include "libNDFS.cpp"

int main(){
    NDFS database("../exampleDatabases/testing.ndfs");
    database.createColumn("Name");
    database.createColumn("Surname");
    database.createColumn("Age");
    database.createColumn("City");
}