#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <iterator>

/**
 * This function opens a file specified by the filename argument,
 * and counts the number of lines in said file
 */
int count_lines(const std::string &filename)
{
    std::ifstream in(filename);
    in.unsetf(std::ios_base::skipws);

    // We are creating an iterator over the input stream and
    // passing it to the count algorithm to count the number
    // of newline characters
    return std::count(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        '\n');
}

/**
 * Given a list of files, this function returns a list of
 * line counts for each of them
 */
std::vector<std::pair<std::string, int>>
count_lines_in_files(const std::vector<std::string> &files)
{
    std::vector<std::pair<std::string, int>> results(files.size());
    std::transform(files.cbegin(), files.cend(), results.begin(), [](const auto& file) {
        return std::make_pair(file, count_lines(file));
    });
    return results;
}

int main(int argc, char *argv[])
{
    auto results = count_lines_in_files({"trans_and_filter.cpp", "count_lines.cpp"});

    for (auto result: results) {
        std::cout << result.first << " : " << result.second << " line(s)\n";
    }

    return 0;
}
