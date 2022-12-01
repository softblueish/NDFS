#include "libNDFS.cpp"

int main(){
    NDFS database("../exampleDatabases/testing.ndfs");
    database.renameColumn("Name", "Country");
}