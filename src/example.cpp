#include "libNDFS.cpp"

int main(){
    NDFS database("../exampleDatabases/testing.ndfs");
    database.createColumn("Age");
    int result = database.getColumnPosition("Age");
    std::cout << result;
}