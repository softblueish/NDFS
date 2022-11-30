#include <iostream>
#include <fstream>
#include <vector>

class NDFS{
    public:
    
        NDFS(std::string databaseDirectory){ // Create or open a database
            storedDatabaseDirectory = databaseDirectory; // Store database directory to variable
            lockfileDirectory = storedDatabaseDirectory + ".lock"; // Makes a lockfile directory for locking the database

            std::ifstream inputDatabaseFilestream(databaseDirectory); // Check if database is empty, if not, initialize one
            lock();
            std::string buffer;
            inputDatabaseFilestream >> buffer;
            inputDatabaseFilestream.close();
            if(buffer.size()<1){
                std::ofstream outputDatabaseFilestream(databaseDirectory);
                outputDatabaseFilestream << "$";
                outputDatabaseFilestream.close();
            }
            unlock();
        }
        bool isLocked(){ // Checks if the database is locked and returns the value
            std::ifstream(lockfileDirectory);
            return lockfileDirectory.good();
        }
        int createColumn(std::string columnName){ // Creates a column in the database
            if(!isLocked()) return 1; // Checks if database is locked
            lock();

            std::string readBuffer; // Stores the read string literal
            std::vector<std::string> writeBuffer; // Stores what should be written IN ORDER
            std::ifstream inputDatabaseFilestream(storedDatabaseDirectory);
            inputDatabaseFilestream >> readBuffer; // Read entire file
            inputDatabaseFilestream.close();
            std::string blockBuffer; // Stores blocks of data temporarily
            int cursor=0; // Selects the block in the string literal
            int section=0; // Selects the section
            for(int i = 0; i<readBuffer.size(); i++){ // Rereads the database an injects the new column
                if(readBuffer.at(i)=='$'&&section==0){ // If new section
                    writeBuffer.push_back(columnName);
                    writeBuffer.push_back(";");
                    section++;
                    writeBuffer.push_back("$");
                }
                if(readBuffer.at(i)==';'){ // If new block
                    cursor++;
                    writeBuffer.push_back(blockBuffer);
                    blockBuffer = "";
                    writeBuffer.push_back(";");
                }else if(readBuffer.at(i)==':'&&section==1){ // If new row
                    writeBuffer.push_back(":");
                }else{ // Write to blockbuffer
                    blockBuffer.append(readBuffer.substr(i, 1));
                }
            }

            std::string toFileBuffer;
            for(int i = 0; i<writeBuffer.size(); i++){ // Compile the file
                toFileBuffer.append(writeBuffer.at(i));
            }
            std::ofstream outputDatebaseFileStream(storedDatabaseDirectory);
            outputDatebaseFileStream << toFileBuffer; // Write to file
            outputDatebaseFileStream.close();

            unlock();
            return 0; // Success
        }
        int getColumnPosition(std::string columnName){ // Get position of column
            lock();


            unlock();
        }
    private:
        std::string storedDatabaseDirectory;
        std::string lockfileDirectory;

        void lock(){ // Locks the database
            std::ofstream lockfile(lockfileDirectory);
            lockfile << " ";
            lockfile.close();
        }
        void unlock(){ // Unlocks the database
            remove(lockfileDirectory.c_str());
        }
};