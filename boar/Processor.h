/* Boar - Boar is a toolkit to modify text files.
* Copyright (C) 2017 Katsuya Iida. All rights reserved.
*/

#pragma once

namespace boar
{
    class Processor
    {
    protected:
        std::wstring _currentFilePath;

    public:
        Processor() {}
        virtual ~Processor() {}

        void ProcessFileList(const std::vector<std::wstring>& filePathList);
        void ProcessFile(const std::wstring& filePath);

    protected:
        virtual void BeginFile() {}
        virtual void ProcessBlock(_In_ const void* first, _In_ const void* last) = 0;
        virtual void EndFile() {}
        virtual void OutputBuffer(_In_ const void* first, _In_ const void* last);
    };
}
