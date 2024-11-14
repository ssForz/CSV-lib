#pragma once

#include <cassert>
#include <iostream>
#include <vector>
#include <variant>

#include "data_interface.h"

namespace NDataApi {

    enum EDataHasTitle {
        NO_TITLE = 0,
        HAS_TITLE = 1
    };

    class TCSVParser;

    class TCSVStorage : IDataStorage
    {
        friend TCSVParser;

    public:
        TCSVStorage() = default;

        const std::string& GetCell(const uint64_t lineIndex, const std::string& columnName) const;

        const std::string& GetCell(const uint64_t lineIndex, const uint64_t columnIndex) const override;

        const std::vector<std::string>& GetColumn(const std::string& fieldName) const;

        const std::vector<std::string>& GetColumn(const uint64_t fieldIndex) const override;

        const std::string& GetColumnName(const uint64_t fieldIndex) const;

        std::vector<std::string> GetLine(const uint64_t fieldIndex) const override;

        void DeleteLine(const uint64_t fieldIndex) override;

        void DeleteColumn(const std::string& fieldName);

        void DeleteColumn(const uint64_t fieldIndex) override;

        void SetCell(const uint64_t lineIndex, const uint64_t columnIndex, const std::string& value) override;

        void SetCell(const uint64_t lineIndex, const std::string& columnName, const std::string& value);

        uint64_t ColumnCount() const override;

        uint64_t Size() const override;

        void SetColumnNames(const std::vector<std::pair<uint64_t, std::string>>& specialColumnNames);


    private:

        struct TColumn {

            TColumn() = default;
            explicit TColumn(const std::string& columnName);

            std::string ColumnName;
            std::vector<std::string> ColumnInfo;
        };

        std::vector<TColumn> DataFrames;
    };


    class TCSVParser
    {
    public:
        TCSVParser() = default;

        [[nodiscard]] TCSVStorage Deserialize(const std::string& fileName) const;

        [[nodiscard]] TCSVStorage Deserialize(
            const std::string& fileName,
            const EDataHasTitle titleMode,
            const std::vector<std::pair<uint64_t, std::string>>& specialColumnNames
        ) const;

        [[nodiscard]] TCSVStorage Deserialize(
            const std::string& fileName,
            const char symbolSeparator = ',',
            const char symbolQuote = '"',
            const EDataHasTitle titleMode = NO_TITLE,
            const std::vector<std::pair<uint64_t, std::string>>& specialColumnNames = {}
        ) const;

        void Serialize(
            TCSVStorage& currentStorage,
            const std::string& fileName,
            const char symbolSeparator = ',',
            const char symbolQuote = '"'
        ) const;

    };
}