#include <iostream>
#include <fstream>

class NDFS{
    public:
    
        NDFS(std::string databaseDirectory){
            storedDatabaseDirectory = databaseDirectory; // Store database directory to variable

            std::ifstream inputDatabaseFilestream(databaseDirectory); // Check if database is empty, if not, initialize one
            std::string buffer;
            inputDatabaseFilestream >> buffer;
            inputDatabaseFilestream.close();
            if(buffer.size()<1){
                std::ofstream outputDatabaseFilestream(databaseDirectory);
                outputDatabaseFilestream << "§";
            }

        }



    private:
        std::string storedDatabaseDirectory;

};