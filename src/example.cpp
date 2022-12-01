#include "libNDFS.cpp"

int main(){
    NDFS database("../exampleDatabases/example.ndfs");
    while(database.faillock){
        NDFS database("../exampleDatabases/example.ndfs");
    }
    database.renameColumn("AgeNew", "Age");
}