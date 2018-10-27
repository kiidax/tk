/* bigtext - bigtext is a collection of tools to process large text files.
 * Copyright (C) 2017 Katsuya Iida. All rights reserved.
 */

#include "stdafx.h"

#include "bigtext.h"

namespace bigtext
{
    static int MainUsage()
    {
        std::wcout <<
            L"usage: bigtext COMMAND [ARGS]\n"
            "\n"
            "bigtext is a collection of tools to process large text files.\n"
            "\n"
            "List of commands:\n"
            "\n"
            "   count      Count the number of lines in the files.\n"
            "   sample     Sample lines from the files.\n"
            "   vocab      Count the words in the files.\n"
            "   version    Show the version info.\n";
        return 0;
    }

    const wchar_t *PROGRAM_NAME = L"bigtext";
    const int MAJOR_VERSION = 0;
    const int MINOR_VERSION = 9;
    const int BUILD_VERSION = 1;
    const int REVISION_VERSION = 0;

    std::wstring GetVersionString()
    {
        return std::to_wstring(MAJOR_VERSION) + L"."
            + std::to_wstring(MINOR_VERSION) + L"."
            + std::to_wstring(BUILD_VERSION) + L"."
            + std::to_wstring(REVISION_VERSION);
    }

    static int VersionCommand(int argc, wchar_t *argv[])
    {
        std::wcout
            << PROGRAM_NAME << " " << GetVersionString() << std::endl
            << L"Copyright (C) 2017 Katsuya Iida. All rights reserved." << std::endl;
        return 0;
    }

    int Main(int argc, wchar_t *argv[])
    {
        if (argc == 1)
        {
            return MainUsage();
        }
        else if (argc >= 2)
        {
            const std::wstring commandName(argv[1]);
            if (commandName == L"count")
            {
                return CountCommand(argc - 1, argv + 1);
            }
            else if (commandName == L"sample")
            {
                return SampleCommand(argc - 1, argv + 1);
            }
            else if (commandName == L"vocab")
            {
                return VocabCommand(argc - 1, argv + 1);
            }
            else if (commandName == L"version")
            {
                return VersionCommand(argc - 1, argv + 1);
            }
            else
            {
                std::wcerr << L"Unknown command `" << commandName << L"'." << std::endl;
                exit(1);
            }
        }
        else
        {
            return MainUsage();
        }
        return 0;
    }

    bool CheckInputFiles(const std::vector<fs::path> &inputFileNameList)
    {
        for (auto& fileName : inputFileNameList)
        {
            if (!fs::is_regular_file(fileName))
            {
                std::wcerr << "`" << fileName.wstring() << "' doesn't exist." << std::endl;
                return false;
            }
        }
        return true;
    }

    bool CheckOutputFiles(const std::vector<fs::path>& outputFileNameList)
    {
        for (auto &it = outputFileNameList.cbegin(); it != outputFileNameList.cend(); ++it)
        {
            auto &fileName = *it;
            if (fs::exists(fileName))
            {
                std::wcerr << "`" << fileName.native() << "' already exists." << std::endl;
                return false;
            }

            for (auto &it2 = outputFileNameList.cbegin(); it2 != it; ++it2)
            {
                auto &fileName2 = *it2;
                if (fileName == fileName2)
                {
                    std::wcerr << "Duplicated output file `" << fileName.native() << "'." << std::endl;
                    return false;
                }
            }
        }
        return true;
    }

    bool TryParseRate(const std::wstring &s, double &rate)
    {
        try
        {
            if (s.empty())
            {
                return false;
            }

            size_t idx;
            double v = std::stod(s, &idx);
            if (idx == s.size() - 1 && s[idx] == '%')
            {
                if (v <= 0 || v > 100)
                {
                    return false;
                }
                rate = v / 100.0;
                return true;
            }
            else if (idx != s.size())
            {
                return false;
            }
            if (v <= 0 || v > 1.0)
            {
                return false;
            }
            rate = v;
            return true;
        }
        catch (std::invalid_argument)
        {
            return false;
        }
        catch (std::out_of_range)
        {
            return false;
        }
    }

    bool TryParseNumber(const std::wstring &s, uintmax_t &numberOfLines)
    {
        try
        {
            if (s.empty())
            {
                return false;
            }

            size_t idx = 0;
            unsigned long long v = std::stoull(s, &idx);
            if (idx != s.size())
            {
                return false;
            }
            if (v <= 0)
            {
                return false;
            }
            numberOfLines = v;
            return true;
        }
        catch (std::invalid_argument)
        {
            return false;
        }
        catch (std::out_of_range)
        {
            return false;
        }
    }
}