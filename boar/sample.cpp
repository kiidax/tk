/* Boar - Boar is a toolkit to modify text files.
* Copyright (C) 2017 Katsuya Iida. All rights reserved.
*/

#include "stdafx.h"

#include "boar.h"
#include "filesource.h"
#include "LineCountProcessor.h"
#include "LineSampleProcessor.h"

namespace boar
{
    namespace po = boost::program_options;

    static int usage(po::options_description &options)
    {
        std::wcout << "usage: boar sample [--rate rate] inputfiles outputfile" << std::endl;
        std::wcout << std::endl;
        std::cout << options << std::endl;
        return 1;
    }

    int SampleCommand(int argc, wchar_t *argv[])
    {
        LineSampleProcessor2<char> proc;
        proc.Run();
    }

    int sample_command(int argc, wchar_t *argv[])
    {
        po::options_description options("options:");
        options.add_options()
            ("help,h", "Show help message")
            ("force,f", "Force overwrite")
            ("rate,r", po::wvalue<double>()->default_value(0.25), "Rate at which the lines are sampled");
        po::options_description hidden("hidden options");
        hidden.add_options()
            ("file", po::wvalue<std::vector<std::wstring>>()->required(), "files");
        po::options_description desc;
        desc.add(options).add(hidden);
        po::positional_options_description p;
        p.add("file", -1);
        double rate;
        std::vector<std::wstring> inputPathList;
        try
        {
            po::variables_map vm;
            po::store(po::wcommand_line_parser(argc, argv).options(desc).positional(p).run(), vm);
            po::notify(vm);
            if (vm.count("help") > 0)
            {
                return usage(options);
            }
            rate = vm["rate"].as<double>();
            inputPathList = vm["file"].as<std::vector<std::wstring>>();
        }
        catch (const po::error_with_option_name& e)
        {
            std::wcerr << e.what() << std::endl;
            return 1;
        }
        if (inputPathList.size() < 2)
        {
            std::wcerr << "The input files are missing" << std::endl;
            return 1;
        }

        std::wstring outputPath(inputPathList.back());
        inputPathList.pop_back();

        std::wcout << "Output file: " << outputPath << std::endl;
        for (auto it = inputPathList.begin(); it != inputPathList.end(); ++it)
        {
            std::wcout << "Input files: " << *it << std::endl;
        }

        std::srand(static_cast<int>(std::time(nullptr)));
        std::auto_ptr<Processor> proc(new LineSampleProcessor<char>(rate));
        proc->SetOutputFilePath(outputPath);
        proc->ProcessFileList(inputPathList);

        return 0;
    }
}