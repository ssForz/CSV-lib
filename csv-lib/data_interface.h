#pragma once

#include <iostream>

namespace NDataApi {

    class IDataStorage
    {
    public:

        virtual const std::string& GetCell(const uint64_t lineIndex, const uint64_t columnIndex) const = 0;

        virtual const std::vector<std::string>& GetColumn(const uint64_t fieldIndex) const = 0;

        virtual std::vector<std::string> GetLine(const uint64_t fieldIndex) const = 0;

        virtual void DeleteLine(const uint64_t fieldIndex) = 0;

        virtual void DeleteColumn(const uint64_t fieldIndex) = 0;

        virtual void SetCell(const uint64_t lineIndex, const uint64_t columnIndex, const std::string& value) = 0;

        virtual uint64_t ColumnCount() const = 0;

        virtual uint64_t Size() const = 0;

    };
}