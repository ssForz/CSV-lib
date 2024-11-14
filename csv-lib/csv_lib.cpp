#include "csv_lib.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace NDataApi {

    const std::string& TCSVStorage::GetCell(const uint64_t lineIndex, const std::string& columnName) const {
        for (const auto& column : DataFrames) {
            if (column.ColumnName == columnName) {
                if (lineIndex + 1 > column.ColumnInfo.size()) {
                    throw std::runtime_error(
                        "Index for line given("
                        + std::to_string(lineIndex) 
                        + ") is out of range\n"
                    );
                }

                return column.ColumnInfo[lineIndex];
            }
        }

        throw std::runtime_error("No name " + columnName + " in stored data\n");
    };

    const std::string& TCSVStorage::GetCell(const uint64_t lineIndex, const uint64_t columnIndex) const {
        if (DataFrames.empty() || columnIndex > DataFrames.size() || lineIndex > DataFrames[0].ColumnInfo.size()) {
            throw std::runtime_error(
                "Indexes given("
                + std::to_string(lineIndex)
                + " ," 
                + std::to_string(columnIndex)
                + ") is out of range\n"
            );
        }

        return DataFrames[columnIndex].ColumnInfo[lineIndex];

    };

    const std::vector<std::string>& TCSVStorage::GetColumn(const std::string& fieldName) const {
        for (const auto& column : DataFrames) {
            if (column.ColumnName == fieldName) {
                return column.ColumnInfo;
            }
        }

        throw std::runtime_error("No name " + fieldName + " in stored data\n");
    };

    const std::vector<std::string>& TCSVStorage::GetColumn(const uint64_t fieldIndex) const {
        if (fieldIndex + 1 > DataFrames.size()) {
            throw std::runtime_error(
                "Index for column given("
                + std::to_string(fieldIndex) 
                + ") is out of range\n"
            );
        }

        return DataFrames[fieldIndex].ColumnInfo;
    }; 

    const std::string& TCSVStorage::GetColumnName(const uint64_t fieldIndex) const {
        if (fieldIndex + 1 > DataFrames.size()) {
            throw std::runtime_error(
                "Index for column given("
                + std::to_string(fieldIndex) 
                + ") is out of range\n"
            );
        }

        return DataFrames[fieldIndex].ColumnName;
    }

    std::vector<std::string> TCSVStorage::GetLine(const uint64_t fieldIndex) const {
        if (DataFrames.empty() || fieldIndex + 1 > DataFrames[0].ColumnInfo.size()) {
            throw std::runtime_error(
                "Index for line given("
                + std::to_string(fieldIndex) 
                + ") is out of range\n"
            );
        }

        std::vector<std::string> result;

        for (size_t i = 0; i < ColumnCount(); ++i) {
            result.push_back(DataFrames[i].ColumnInfo[fieldIndex]);
        }

        return result;
    };

    void TCSVStorage::DeleteLine(const uint64_t fieldIndex) {
        if (DataFrames.empty() || fieldIndex + 1 > DataFrames[0].ColumnInfo.size()) {
            throw std::runtime_error(
                "Index for line given("
                + std::to_string(fieldIndex) 
                + ") is out of range\n"
            );
        }

        for (auto& column : DataFrames) {
            column.ColumnInfo.erase(column.ColumnInfo.begin() + fieldIndex);
            std::vector<std::string>(column.ColumnInfo).swap(column.ColumnInfo);
        }
    };

    void TCSVStorage::DeleteColumn(const std::string& fieldName) {
        for (size_t i = 0; i < DataFrames.size(); ++i) {
            if (DataFrames[i].ColumnName == fieldName) {
                DataFrames.erase(DataFrames.begin() + i);
                std::vector<TColumn>(DataFrames).swap(DataFrames);
            }
        }

        throw std::runtime_error("No name " + fieldName + " in stored data\n");
    };

    void TCSVStorage::DeleteColumn(const uint64_t fieldIndex) {
        if (fieldIndex + 1 > DataFrames.size()) {
            throw std::runtime_error(
                "Index for column given("
                + std::to_string(fieldIndex) 
                + ") is out of range\n"
            );
        }

        DataFrames.erase(DataFrames.begin() + fieldIndex);
        std::vector<TColumn>(DataFrames).swap(DataFrames);
    };

    void TCSVStorage::SetCell(const uint64_t lineIndex, const uint64_t columnIndex, const std::string& value) {
        if (DataFrames.empty() || columnIndex > DataFrames.size() || lineIndex > DataFrames[0].ColumnInfo.size()) {
            throw std::runtime_error(
                "Indexes given("
                + std::to_string(lineIndex)
                + " ," 
                + std::to_string(columnIndex)
                + ") is out of range\n"
            );
        }

        DataFrames[columnIndex].ColumnInfo[lineIndex] = value;
    };

    void TCSVStorage::SetCell(const uint64_t lineIndex, const std::string& columnName, const std::string& value) {
        for (auto& column : DataFrames) {
            if (column.ColumnName == columnName) {
                if (lineIndex + 1 > column.ColumnInfo.size()) {
                    throw std::runtime_error(
                        "Index for line given("
                        + std::to_string(lineIndex) 
                        + ") is out of range\n"
                    );
                }

                column.ColumnInfo[lineIndex] = value;
            }
        }

        throw std::runtime_error("No name " + columnName + " in stored data\n");
    };

    uint64_t TCSVStorage::ColumnCount() const {
        return DataFrames.size();
    };
        
    uint64_t TCSVStorage::Size() const {
        return DataFrames.empty() ? 0 : DataFrames[0].ColumnInfo.size();
    };

    void TCSVStorage::SetColumnNames(const std::vector<std::pair<uint64_t, std::string>>& specialColumnNames) {
        if (DataFrames.size() == 0) {
            throw std::runtime_error("Data is not initialized\n");
        }

        for (const auto& [pos, name] : specialColumnNames) {
            if (pos > DataFrames.size() - 1) {
                throw std::runtime_error(
                    "Index for column name given("
                    + std::to_string(specialColumnNames.size()) 
                    + ") is out of range\n"
                );
            }

            DataFrames[pos].ColumnName = name;
        }
    };


    TCSVStorage::TColumn::TColumn(const std::string& columnName)
    :   ColumnName(columnName)
    { }


    TCSVStorage TCSVParser::Deserialize(const std::string& fileName) const {
        return Deserialize(fileName, ',', '"');
    }

    TCSVStorage TCSVParser::Deserialize(
        const std::string& fileName,
        const EDataHasTitle titleMode,
        const std::vector<std::pair<uint64_t, std::string>>& specialColumnNames
    ) const {
        return Deserialize(fileName, ',', '"', titleMode, specialColumnNames);
    };

    TCSVStorage TCSVParser::Deserialize(
        const std::string& fileName,
        const char symbolSeparator,
        const char symbolQuote,
        const EDataHasTitle titleMode,
        const std::vector<std::pair<uint64_t, std::string>>& specialColumnNames
    ) const {

        auto extraQuotesEraser = [symbolQuote](const std::string& line) {
            std::string clearLine = "";

            size_t start = 0;
            size_t end = line.size();

            if (line[0] == symbolQuote) start = 1;
            if (line[line.size() - 2] == symbolQuote && line[line.size() - 1] != symbolQuote) end = line.size() - 2;
            if (line[line.size() - 1] == symbolQuote) end = line.size() - 1;

            int prevWasQuote = 0;

            for (size_t i = start; i < end; ++i) {
                if (line[i] == symbolQuote) {
                    if (!prevWasQuote) {
                        clearLine += symbolQuote;
                    }

                    prevWasQuote = !prevWasQuote;
                    continue;
                }

                clearLine += line[i];
            }

            return clearLine;
        };

        TCSVStorage currentStorage;


        std::ifstream dataFile(fileName);

        if (!dataFile.is_open()) {
            throw std::runtime_error("Could not open file " + fileName + "\n");
        }

        if (!dataFile.good()) {
            throw std::runtime_error("Problem with file " + fileName + "\n");
        }
        

        if (titleMode == HAS_TITLE) { //working with title
            std::string titleLine;

            std::getline(dataFile, titleLine);

            std::stringstream lineParse(titleLine);

            std::string columnName = "";

            while (std::getline(lineParse, columnName, symbolSeparator)) {
                uint64_t totalQuotes = std::count(columnName.begin(), columnName.end(), symbolQuote); 

                if (totalQuotes == 0) {
                    currentStorage.DataFrames.push_back(TCSVStorage::TColumn(columnName));
                    continue;
                }

                while (totalQuotes % 2 == 1) {
                    std::string addPart;
                    columnName += ",";
                    if (!std::getline(lineParse, addPart, symbolSeparator)) {
                        throw std::runtime_error("Invalid format with " + std::to_string(symbolQuote) + " separator\n");
                    }

                    columnName += addPart;
                    totalQuotes += std::count(addPart.begin(), addPart.end(), symbolQuote);
                }

                columnName = extraQuotesEraser(columnName);
                currentStorage.DataFrames.push_back(TCSVStorage::TColumn(columnName));
            }
        }


        std::string currentFrame;
        while(std::getline(dataFile, currentFrame)) {

            std::stringstream lineParse(currentFrame);
            
            uint64_t columndId = 0;

            while(std::getline(lineParse, currentFrame, symbolSeparator)) {
                uint64_t totalQuotes = std::count(currentFrame.begin(), currentFrame.end(), symbolQuote); 

                if (totalQuotes == 0) {
                    if (
                        (titleMode == NO_TITLE) && 
                        (columndId + 1 > currentStorage.DataFrames.size())
                    ) {
                        currentStorage.DataFrames.push_back(TCSVStorage::TColumn("NO_TITLE"));
                    }

                    currentStorage.DataFrames[columndId].ColumnInfo.push_back(currentFrame);

                    ++columndId;
                    continue;
                }

                while (totalQuotes % 2 == 1) {
                    std::string addPart;
                    currentFrame += ",";
                    if (!std::getline(lineParse, addPart, symbolSeparator)) {
                        throw std::runtime_error("Invalid format with " + std::to_string(symbolQuote) + " separator\n");
                    }

                    currentFrame += addPart;
                    totalQuotes += std::count(addPart.begin(), addPart.end(), symbolQuote);
                }

                currentFrame = extraQuotesEraser(currentFrame);

                if (
                    (titleMode == NO_TITLE) && 
                    (columndId + 1 > currentStorage.DataFrames.size())
                ) {
                    currentStorage.DataFrames.push_back(TCSVStorage::TColumn("NO_TITLE"));
                }
                
                currentStorage.DataFrames[columndId].ColumnInfo.push_back(currentFrame);

                ++columndId;
            }
        }

        currentStorage.SetColumnNames(specialColumnNames);
        dataFile.close();

        return currentStorage;
    };

    void TCSVParser::Serialize(TCSVStorage& currentStorage, const std::string& fileName, const char symbolSeparator, const char symbolQuote) const {
        
        auto hasSpecialSymbols = [symbolSeparator, symbolQuote](const std::string& line) {
            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i] == symbolSeparator || line[i] == symbolQuote || line[i] == '\n') {
                    return true;
                }
            }
            return false;
        };

        auto copyWithSpecialSymbolFormatting = [symbolQuote](const std::string& line) {
            std::string result = "";

            result += symbolQuote;
            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i] == symbolQuote) {
                    result += symbolQuote;
                    result += symbolQuote;
                    continue;
                }
                result += line[i];
            }

            result += symbolQuote;

            return result;
        };

        std::ofstream outFile(fileName);

        if (currentStorage.DataFrames.empty()) {
            return;
        }

        for (size_t column = 0; column < currentStorage.ColumnCount(); ++column) {
            if (hasSpecialSymbols(currentStorage.DataFrames[column].ColumnName)) {
                outFile<<copyWithSpecialSymbolFormatting(currentStorage.DataFrames[column].ColumnName);
            } else {
                outFile<<currentStorage.DataFrames[column].ColumnName;
            }

            if (column != currentStorage.DataFrames.size() - 1) {
                outFile<<symbolSeparator;
            }
        }

        outFile<<"\n";
        
        for (size_t line = 0; line < currentStorage.Size(); ++line) {
            for (size_t column = 0; column < currentStorage.ColumnCount(); ++column) {
                if (hasSpecialSymbols(currentStorage.DataFrames[column].ColumnInfo[line])) {
                    outFile<<copyWithSpecialSymbolFormatting(currentStorage.DataFrames[column].ColumnInfo[line]);
                } else {
                    outFile<<currentStorage.DataFrames[column].ColumnInfo[line];
                }

                if (column != currentStorage.DataFrames.size() - 1) {
                    outFile<<symbolSeparator;
                }
            }

            outFile<<"\n";
        }

        outFile.close();
        
    };


}