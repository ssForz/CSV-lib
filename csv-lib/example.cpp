#include "csv_lib.h"

using namespace NDataApi;

int main()
{
    TCSVParser parser;
    TCSVStorage storage = parser.Deserialize("TestData/testing.csv", HAS_TITLE, {{0, "TIME"}, {4, "COST"}});
    std::cout<<storage.ColumnCount()<<std::endl;
    std::cout<<storage.Size()<<std::endl;

    std::cout<<storage.GetLine(0)[0]<<std::endl;

    storage.SetCell(0, 0, "1900");

    std::cout<<storage.GetLine(0)[0]<<std::endl;

    std::cout<<storage.GetCell(2, "TIME")<<std::endl;

    storage.DeleteColumn(0);
    storage.DeleteLine(2);
    std::cout<<storage.ColumnCount()<<std::endl;
    std::cout<<storage.Size()<<std::endl;

    std::cout<<storage.GetLine(0)[0]<<std::endl;

    parser.Serialize(storage, "TestData/testing_out.csv", '!', '=');
}